// EmotionPanel.h : main header file for the OPENGLWIZARD application
//

#if !defined(AFX_SOL_H__9DAC6A90_D186_11D1_B7DD_00A0C98B8E35__INCLUDED_)
#define AFX_SOL_H__9DAC6A90_D186_11D1_B7DD_00A0C98B8E35__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEmotionPanelApp:
// See EmotionPanel.cpp for the implementation of this class
//

class CEmotionPanelApp : public CWinApp
{
public:
	float NextFrame(float dt);
	void Process();
	CEmotionPanelApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmotionPanelApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEmotionPanelApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOL_H__9DAC6A90_D186_11D1_B7DD_00A0C98B8E35__INCLUDED_)
