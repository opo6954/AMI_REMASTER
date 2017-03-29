// HRView.h : interface of the CHRView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HRVIEW_H__C15047CD_67C1_11D4_9BEE_00010263B61C__INCLUDED_)
#define AFX_HRVIEW_H__C15047CD_67C1_11D4_9BEE_00010263B61C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <mmsystem.h>

#include "comm.h"
#include "yesnodlg.h"

#define MAX_BUFFER		200
#define MAX_COMMAND		100

typedef struct {
	double	v,  w;	/*	linear and angular velocites					*/
	int		v1, v2;	/*	velocities of right and left wheel (pulse unit)	*/
} Speed;

typedef struct {
	double	x, y, q;
} Posture;

class CHRView : public CView
{
protected: // create from serialization only
	CHRView();
	DECLARE_DYNCREATE(CHRView)

// Attributes
public:
///////////////////////// daniel 2001.4.26.
	UINT MsgEmotion;
////////////////////////////////////////////

	CHRDoc* GetDocument();
	CYesNoDlg yesNoDlg;
	
	CComm*	m_pComm;

	/* for mobile */
	CComm   m_comm1;
	/* for head */
	CComm   m_comm2;	
	/* for right arm */
	CComm   m_comm3;	
	/* for left arm */
	CComm   m_comm4;	

	char	m_abIn[MAX_BUFFER+1];
	char	m_abIn2[MAX_BUFFER+1];
	char	m_abIn3[MAX_BUFFER+1];
	char	m_abIn4[MAX_BUFFER+1];

	BYTE	m_command[MAX_COMMAND+1];	

	CString	m_file;
	char msg[80];

// Operations
public:
//by Joyfully

	void OnSeeHere();
	void SeeHere();

	void InitPos();
	void Speak(int t);
	bool MoveArm(int whichArm);
	void ResetArm();
	void SendArm();
	void Ready();
	bool isMoveArm(int i);
	void Lift();
	void Drop();

	void OnArmReady();
	void OnArmGrab();
	void OnArmMoveLeft();
	void OnArmMoveRight();
	void OnResetArm();
// ----------

	void Alert(CString msg);	
	void printText(int x,int y,char *fmt, ...);

	void Delay(long delay);
	void OnComboChange();


	void OnInit();
	void DownLoad();

	void InitMobileParameters();
	void PredictMobilePose(Posture *robot,Speed *speed);
	double toBound2PI(double fAngle);
	double calAngle(double x1,double y1,double x2,double y2);
	double GetAngleFromObjectPos(Posture  Object);

	BOOL GoMobile(BYTE dir);
	void SetMobileSpeed(BYTE max_acc,BYTE max_rotation_acc);
	BOOL ReadMobileEncoder();

	void SensorEnable(BYTE flag);
	BOOL ReadSensor();

	void WallyVision();
	void Wally();
	BOOL IsSafe(BYTE want_action);

	void Navigation();
	int SafetyCount(BYTE want_action);
	BOOL FindObstacle(BYTE want_action);

	void HeadStop();
	void SendHead();
	void ReadHead();
	void headLeftScan();
	void headRightScan();
	void headStop();

	void SendFace();

	void SendRightHand();
	void SendLeftHand();


	void SendRightArm();
	void SendLeftArm();
	BOOL ReadRightArm();
	BOOL ReadLeftArm();


	void SendTTS(CString msg); 

	void OnSensor();
	void OnWally();
	
	void OnUp();
	void OnDown();
	void OnLeft();
	void OnRight();
	void OnStop();
	void OnDemo();

	void OnTest();
	void OnTest2();
	void OnTest4();
	void OnHandReset();

	void OnLove();
	void ReturnRead(int r);

	void OnJoy();
	void OnAllStop();

	BOOL LoadCfg();
	BOOL SaveCfg();
	LONG OnReceiveData(UINT WParam,LONG LParam);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHRView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHRView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnHandstp();
	afx_msg void OnTest3();
	afx_msg void OnHandClose();
	afx_msg void OnWalking();
	afx_msg void OnStopwalk();
	afx_msg void OnIdleEnable();
	afx_msg void OnIdleDisable();
	afx_msg void OnHandInit();
	afx_msg void OnClean();
	afx_msg void OnNiceMotion1();
	afx_msg void OnNiceMotion2();
	afx_msg void OnNiceMotion3();
	afx_msg void OnPlayalone();
	afx_msg void OnReadReady();
	afx_msg void OnReading();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HRView.cpp
inline CHRDoc* CHRView::GetDocument()
   { return (CHRDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HRVIEW_H__C15047CD_67C1_11D4_9BEE_00010263B61C__INCLUDED_)
