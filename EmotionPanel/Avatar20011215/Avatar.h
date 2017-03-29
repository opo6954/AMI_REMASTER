// Avatar.h : main header file for the AVATAR application
//

#if !defined(AFX_AVATAR_H__A80E7826_F30D_11D1_9A10_444553540000__INCLUDED_)
#define AFX_AVATAR_H__A80E7826_F30D_11D1_9A10_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAvatarApp:
// See Avatar.cpp for the implementation of this class
//

class CAvatarApp : public CWinApp
{
public:
	CAvatarApp();

	float NextFrame(float dt);
	void Process();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAvatarApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAvatarApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVATAR_H__A80E7826_F30D_11D1_9A10_444553540000__INCLUDED_)
