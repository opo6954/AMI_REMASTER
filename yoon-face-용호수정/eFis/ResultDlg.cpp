// ResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eFis.h"
#include "ResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultDlg dialog


CResultDlg::CResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResultDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
	//{{AFX_MSG_MAP(CResultDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultDlg message handlers

BOOL CResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowPos(NULL, 600, 200, 0, 0, SWP_NOSIZE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CResultDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	BITMAP bm;

	if(m_bAccept)	
	{
		bitmap.LoadBitmap(IDB_ACCEPT);
		SetWindowText("Accept");
	}
	else
	{
		bitmap.LoadBitmap(IDB_REJECT);
		SetWindowText("Reject");
	}

	bitmap.GetBitmap(&bm);
	CBitmap* pOld = MemDC.SelectObject(&bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOld);
	MemDC.DeleteDC();

	// Do not call CDialog::OnPaint() for painting messages
}

void CResultDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
}

void CResultDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	OnOK();
	
	CDialog::OnLButtonUp(nFlags, point);
}


