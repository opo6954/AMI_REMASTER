// KeyFramerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Avatar.h"
#include "pdef.h"
#include "mainfrm.h"
#include "AvatarDoc.h"
#include "AvatarView.h"
#include "KeyFramerDlg.h"
#include "keyframer.h"
#include "ExpSliders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CExpSliders ExpSliders;
extern CKeyFramer KeyFramer;

/////////////////////////////////////////////////////////////////////////////
// CKeyFramerDlg dialog


CKeyFramerDlg::CKeyFramerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyFramerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyFramerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKeyFramerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyFramerDlg)
	DDX_Control(pDX, IDC_STOP, m_STOP);
	DDX_Control(pDX, IDC_START, m_START);
	DDX_Control(pDX, IDC_RECORD, m_RECORD);
	DDX_Control(pDX, IDC_PLUS, m_PLUS);
	DDX_Control(pDX, IDC_PLAY, m_PLAY);
	DDX_Control(pDX, IDC_MINUS, m_MINUS);
	DDX_Control(pDX, IDC_END, m_END);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyFramerDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyFramerDlg)
	ON_BN_CLICKED(IDC_END, OnEnd)
	ON_BN_CLICKED(IDC_MINUS, OnMinus)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_PLUS, OnPlus)
	ON_BN_CLICKED(IDC_RECORD, OnRecord)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyFramerDlg message handlers
/*
void CKeyFramerDlg::OnButton1() 
{
	KeyFramer.PreviousFrame();
}

void CKeyFramerDlg::OnButton2() 
{
	KeyFramer.play();
}

void CKeyFramerDlg::OnButton3() 
{
	KeyFramer.stop();
}

void CKeyFramerDlg::OnButton4() 
{
	KeyFramer.RecordFrame();
	
}

void CKeyFramerDlg::OnButton5() 
{
	KeyFramer.NextFrame();
}
*/
void CKeyFramerDlg::OnEnd() 
{
	RefreshSlider();
	
}

void CKeyFramerDlg::OnMinus() 
{
	KeyFramer.PreviousFrame();
	
//	RefreshSlider();
}

void CKeyFramerDlg::OnPlay() 
{
	KeyFramer.play();
	
//	RefreshSlider();
}

void CKeyFramerDlg::OnPlus() 
{
	KeyFramer.NextFrame();
}

void CKeyFramerDlg::OnRecord() 
{
	KeyFramer.RecordFrame();
	
//	RefreshSlider();
}

void CKeyFramerDlg::OnStart() 
{
	KeyFramer.SetCurrentFrame(0);

//	RefreshSlider();
}

void CKeyFramerDlg::OnStop() 
{
	KeyFramer.stop();

//	RefreshSlider();
}

void CKeyFramerDlg::RefreshSlider(bool flag)
{
	char temp[100];
	
	sprintf(temp,"Time Slider (%dth frame)",KeyFramer.CurrentFrame);

	SetWindowText(temp);

	CClientDC DC(this);
	if(flag)
	{

		int i;
		for(i=0;i<100;i++)
		{
			DC.SetPixel(i*3+5,50,RGB(150,150,150));
			DC.SetPixel(i*3+5,51,RGB(150,150,150));
			DC.SetPixel(i*3+5,52,RGB(150,150,150));
		}
	}

	DC.SelectStockObject(WHITE_PEN);

	DC.MoveTo(KeyFramer.CurrentFrame*3+4,53);
	DC.LineTo(KeyFramer.CurrentFrame*3+4,90);
	DC.LineTo(KeyFramer.CurrentFrame*3+6,90);
	DC.LineTo(KeyFramer.CurrentFrame*3+6,53);
	DC.LineTo(KeyFramer.CurrentFrame*3+4,53);

	DC.SelectStockObject(BLACK_PEN);
	DC.MoveTo(KeyFramer.PrevFrame*3+4,53);
	DC.LineTo(KeyFramer.PrevFrame*3+4,90);
	DC.LineTo(KeyFramer.PrevFrame*3+6,90);
	DC.LineTo(KeyFramer.PrevFrame*3+6,53);
	DC.LineTo(KeyFramer.PrevFrame*3+4,53);
//	DC.LineTo(0,15);
}

void CKeyFramerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(point.y>50&&KeyFramer.CurrentFrame!=KeyFramer.PrevFrame&&(point.x-5)/3>=0)
		KeyFramer.SetCurrentFrame((point.x-5)/3);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CKeyFramerDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(point.y>50&&(nFlags&MK_LBUTTON)&&((point.x-5)/3)!=KeyFramer.PrevFrame&&(point.x-5)/3>=0)
		KeyFramer.SetCurrentFrame((point.x-5)/3);
	
	CDialog::OnMouseMove(nFlags, point);
}
