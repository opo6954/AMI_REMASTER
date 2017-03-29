// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C18B961F_E2EF_4E4B_947E_E4B6A0BAD14C__INCLUDED_)
#define AFX_STDAFX_H__C18B961F_E2EF_4E4B_947E_E4B6A0BAD14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C18B961F_E2EF_4E4B_947E_E4B6A0BAD14C__INCLUDED_)


/*------------------------------------------
			Library
  ------------------------------------------*/
#include "../include/VfwK.h"
#include "../include/DibK.h"
#include "../include/FaceApi.h"

#include "../include/FaceK.h"

#ifdef _DEBUG
	#pragma comment(lib, "../lib/VfwKd.lib")
	#pragma comment(lib, "../lib/DibKd.lib")
	//#pragma comment(lib, "../lib/FaceKd.lib")

#else
	#pragma comment(lib, "../lib/VfwK.lib")
	#pragma comment(lib, "../lib/DibK.lib")
	//#pragma comment(lib, "../lib/FaceK.lib")

#endif

#pragma comment(lib, "../Exe/eFace100.lib")


/*------------------------------------------
			Tools (Interface)
  ------------------------------------------*/
#include "../ToolBox/MacButtons.h"
#include "../ToolBox/ListCtrlEx.h"
#include "../ToolBox/DirTreeCtrl.h"
#include "../ToolBox/XTabCtrl.h"
#include "../ToolBox/GradProgress.h"
#include "../ToolBox/MyToolTipCtrl.h"
#include "../ToolBox/BtnST.h"
#include "../ToolBox/WinXPButtonST.h"
#include "../ToolBox/Label.h"


