// HR.h : main header file for the HR application
//

#if !defined(AFX_HR_H__C15047C5_67C1_11D4_9BEE_00010263B61C__INCLUDED_)
#define AFX_HR_H__C15047C5_67C1_11D4_9BEE_00010263B61C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHRApp:
// See HR.cpp for the implementation of this class
//

class CHRApp : public CWinApp
{
public:
	CHRApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHRApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHRApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HR_H__C15047C5_67C1_11D4_9BEE_00010263B61C__INCLUDED_)
