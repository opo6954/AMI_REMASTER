#if !defined(AFX_GRAB_H__C992D761_AAEA_11D4_9BF4_00C026A05154__INCLUDED_)
#define AFX_GRAB_H__C992D761_AAEA_11D4_9BF4_00C026A05154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Grab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Grab dialog

class Grab : public CDialog
{
// Construction
public:
	Grab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Grab)
	enum { IDD = IDD_GRAB };
	float	m_objx;
	float	m_objy;
	float	m_objz;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Grab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Grab)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAB_H__C992D761_AAEA_11D4_9BF4_00C026A05154__INCLUDED_)
