// eFisDlg.h : header file
//

#if !defined(AFX_EFISDLG_H__8589BD18_C0F3_4E44_9269_EBE6B91257C4__INCLUDED_)
#define AFX_EFISDLG_H__8589BD18_C0F3_4E44_9269_EBE6B91257C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Com.h"


/////////////////////////////////////////////////////////////////////////////
// CEFisDlg dialog

class CEFisDlg : public CDialog, CVideoOwner
{
// Construction
public:
	CEFisDlg(CWnd* pParent = NULL);	// standard constructor

	LRESULT OnFrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);

	void ShowCaptureTag(CStatic *pPic);
	void ProcessTrigger();
	void DisplayThreshold(int nThres);

	void LoadAllTemplates();
	void GetAllMatchingResult(CStringArray &ResultArray);
	void ExtractFeature();
	void Enroll();
	void Verify();
	void Recognize();
	void PhotoCheck();
	void Tracking();
	void Tunning();
	
	void AddDebugText(char *fmt, ... );
	void SendTTS(CString msg);

	CStringArray* ReadPersonList(CString strFileName);
	void WritePersonList(CStringArray *PersonListArray, CString strFileName);
	
	//------------------------------------
	//	Variables & Instances
	//------------------------------------
	CDib *m_pDib;
	CBlobArray m_BlobArray;
	CBlobArray m_TemplateArray;

	int m_nProcessID;
	int m_nNextProcessID;

	int m_nMaxTryCnt;
	int m_nMaxBlobCnt;

	CVideoTool m_VideoTool;
	CStringArray *m_pPersonListArray;

	CCom m_Com;
	char m_chMsg[2];

	CPoint m_ptLeft, m_ptRight;
	CString m_strTrackingID;

// Dialog Data
	//{{AFX_DATA(CEFisDlg)
	enum { IDD = IDD_EFIS_DIALOG };
	CEdit	m_dbgEdit;
	CWinXPButtonST	m_btRecognize;
	CStatic	m_picSmall4;
	CStatic	m_picSmall5;
	CStatic	m_picSmall3;
	CStatic	m_picSmall2;
	CStatic	m_picSmall1;
	CLabel	m_static2;
	CLabel	m_static1;
	CWinXPButtonST	m_btSaveStillImage;
	CWinXPButtonST	m_btEnrollment;
	CWinXPButtonST	m_btDetectFace;
	CListBox	m_lbImageTitle;
	CWinXPButtonST	m_btVerify;
	CGradProgress	m_progress;
	CStatic	m_picVerifyFace;
	CStatic	m_picMain;
	CListBox	m_lbPerson;
	CWinXPButtonST	m_btInitialize;
	CString	m_etOutput;
	int		m_nPersonListCnt;
	CString	m_strID;
	BOOL	m_bIsTracking;
	BOOL	m_bIsPhotoCheck;
	BOOL	m_bIsRealTimeRecogn;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEFisDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEFisDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnInitialize();
	afx_msg void OnVerify();
	afx_msg void OnSelchangeLbPerson();
	afx_msg void OnSelchangeLbImageTitle();
	afx_msg void OnDetectFace();
	afx_msg void OnEnrollment();
	afx_msg void OnSaveStillImage();
	afx_msg void OnDblclkLbPerson();
	afx_msg void OnEtriLogo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTrackingEyes();
	afx_msg void OnRecognize();
	afx_msg void OnRealTimeRecogn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFISDLG_H__8589BD18_C0F3_4E44_9269_EBE6B91257C4__INCLUDED_)
