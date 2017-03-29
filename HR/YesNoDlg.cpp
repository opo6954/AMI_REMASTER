// YesNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HR.h"
#include "YesNoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYesNoDlg dialog


CYesNoDlg::CYesNoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYesNoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYesNoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CYesNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYesNoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYesNoDlg, CDialog)
	//{{AFX_MSG_MAP(CYesNoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYesNoDlg message handlers
