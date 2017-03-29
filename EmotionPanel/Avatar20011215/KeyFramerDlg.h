#if !defined(AFX_KEYFRAMERDLG_H__DED848A2_03A1_11D2_AF36_00A0C98A1BF2__INCLUDED_)
#define AFX_KEYFRAMERDLG_H__DED848A2_03A1_11D2_AF36_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KeyFramerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyFramerDlg dialog

class CKeyFramerDlg : public CDialog
{
// Construction
public:
	void RefreshSlider(bool flag=FALSE);
	CKeyFramerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyFramerDlg)
	enum { IDD = IDD_PROPPAGE_SMALL };
	CButton	m_STOP;
	CButton	m_START;
	CButton	m_RECORD;
	CButton	m_PLUS;
	CButton	m_PLAY;
	CButton	m_MINUS;
	CButton	m_END;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyFramerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyFramerDlg)
	afx_msg void OnEnd();
	afx_msg void OnMinus();
	afx_msg void OnPlay();
	afx_msg void OnPlus();
	afx_msg void OnRecord();
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYFRAMERDLG_H__DED848A2_03A1_11D2_AF36_00A0C98A1BF2__INCLUDED_)
