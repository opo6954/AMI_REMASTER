#if !defined(AFX_YESNODLG_H__4FA12EA1_AB00_11D4_9BF4_00C026A05154__INCLUDED_)
#define AFX_YESNODLG_H__4FA12EA1_AB00_11D4_9BF4_00C026A05154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YesNoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CYesNoDlg dialog

class CYesNoDlg : public CDialog
{
// Construction
public:
	CYesNoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CYesNoDlg)
	enum { IDD = IDD_YESNO_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYesNoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYesNoDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YESNODLG_H__4FA12EA1_AB00_11D4_9BF4_00C026A05154__INCLUDED_)
