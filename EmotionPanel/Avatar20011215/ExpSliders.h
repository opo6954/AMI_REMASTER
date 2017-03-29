#if !defined(AFX_EXPSLIDERS_H__CDB4DB83_FD98_11D1_BDBB_00A0C98A1BF2__INCLUDED_)
#define AFX_EXPSLIDERS_H__CDB4DB83_FD98_11D1_BDBB_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExpSliders.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpSliders dialog

class CExpSliders : public CDialog
{
// Construction
public:
	LPCTSTR GetText();
	void UpdateSliders(SliderStatus *status);
	CExpSliders(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExpSliders)
	enum { IDD = IDD_PROPPAGE_MEDIUM };
	CSliderCtrl	m_eyebrow_up;
	CSliderCtrl	m_eyebrow_glower;
	CSliderCtrl	m_eye_close;
	CSliderCtrl	m_eyetail_up;
	CSliderCtrl	m_lip_open;
	CSliderCtrl	m_lip_tighten_u;
	CSliderCtrl	m_lip_tighten_o;
	CSliderCtrl	m_lip_longer;
	CSliderCtrl	m_liptail_down;
	CSliderCtrl	m_liptail_up;
	CSliderCtrl	m_mouth_open;
	CString	m_ctrlEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpSliders)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateText();

	// Generated message map functions
	//{{AFX_MSG(CExpSliders)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPSLIDERS_H__CDB4DB83_FD98_11D1_BDBB_00A0C98A1BF2__INCLUDED_)
