// eFis.h : main header file for the EFIS application
//

#if !defined(AFX_EFIS_H__D37A3B80_44DF_4C1D_B705_0905887B18B5__INCLUDED_)
#define AFX_EFIS_H__D37A3B80_44DF_4C1D_B705_0905887B18B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEFisApp:
// See eFis.cpp for the implementation of this class
//

class CEFisApp : public CWinApp
{
public:
	CEFisApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEFisApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEFisApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFIS_H__D37A3B80_44DF_4C1D_B705_0905887B18B5__INCLUDED_)
