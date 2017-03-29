// Grab.cpp : implementation file
//

#include "stdafx.h"
#include "HR.h"
#include "Grab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Grab dialog


Grab::Grab(CWnd* pParent /*=NULL*/)
	: CDialog(Grab::IDD, pParent)
{
	//{{AFX_DATA_INIT(Grab)
	m_objx = 0.0f;
	m_objy = 0.0f;
	m_objz = 0.0f;
	//}}AFX_DATA_INIT
}


void Grab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Grab)
	DDX_Text(pDX, IDC_OBJX, m_objx);
	DDX_Text(pDX, IDC_OBJY, m_objy);
	DDX_Text(pDX, IDC_OBJZ, m_objz);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Grab, CDialog)
	//{{AFX_MSG_MAP(Grab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Grab message handlers
