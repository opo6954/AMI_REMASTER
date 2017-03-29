// ExpSliders.cpp : implementation file
//

#include "stdafx.h"
#include "PDef.h"
#include "MyDIB.h"
#include "Eyes.h"
#include "Avatar.h"
#include "ExpSliders.h"
#include "MainFrm.h"
#include "Parser.h"
#include "OpenGLView.h"
#include "AvatarDoc.h"
#include "AvatarView.h"
#include "MakeSpchSeq.h"
#include "KeyFramer.h"
#include "face.h"

extern SliderStatus m_SliderStatus;
extern CMakeSpchSeq MakeSpchSeq;
extern CKeyFramer KeyFramerRealtime;
extern CFace Face;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpSliders dialog


CExpSliders::CExpSliders(CWnd* pParent /*=NULL*/)
	: CDialog(CExpSliders::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpSliders)
	m_ctrlEdit = _T("");
	//}}AFX_DATA_INIT
}


void CExpSliders::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpSliders)
	DDX_Control(pDX, IDC_SLIDER12, m_eyebrow_up);
	DDX_Control(pDX, IDC_SLIDER11, m_eyebrow_glower);
	DDX_Control(pDX, IDC_SLIDER10, m_eye_close);
	DDX_Control(pDX, IDC_SLIDER9, m_eyetail_up);
	DDX_Control(pDX, IDC_SLIDER8, m_lip_open);
	DDX_Control(pDX, IDC_SLIDER7, m_lip_tighten_u);
	DDX_Control(pDX, IDC_SLIDER6, m_lip_tighten_o);
	DDX_Control(pDX, IDC_SLIDER5, m_lip_longer);
	DDX_Control(pDX, IDC_SLIDER3, m_liptail_down);
	DDX_Control(pDX, IDC_SLIDER4, m_liptail_up);
	DDX_Control(pDX, IDC_SLIDER1, m_mouth_open);
	DDX_Text(pDX, IDC_EDIT1, m_ctrlEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpSliders, CDialog)
	//{{AFX_MSG_MAP(CExpSliders)
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpSliders message handlers

void CExpSliders::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CMainFrame* pFrame=(CMainFrame*) AfxGetMainWnd();
	CAvatarView* pView=(CAvatarView*) pFrame->GetActiveView();
	
	m_SliderStatus.mouth_open		= (float)m_mouth_open.GetPos()/100.0f;
	m_SliderStatus.liptail_up		= (float)m_liptail_up.GetPos()/100.0f;
	m_SliderStatus.liptail_down		= (float)m_liptail_down.GetPos()/100.0f;
	m_SliderStatus.lip_longer		= (float)m_lip_longer.GetPos()/100.0f;
	m_SliderStatus.lip_tighten_o	= (float)m_lip_tighten_o.GetPos()/100.0f;
	m_SliderStatus.lip_tighten_u	= (float)m_lip_tighten_u.GetPos()/100.0f;
	m_SliderStatus.lip_open			= (float)m_lip_open.GetPos()/100.0f;
	m_SliderStatus.eyetail_up		= (float)m_eyetail_up.GetPos()/100.0f;
	m_SliderStatus.eye_close		= (float)m_eye_close.GetPos()/100.0f;
	m_SliderStatus.eyebrow_up		= (float)m_eyebrow_up.GetPos()/100.0f;
	m_SliderStatus.eyebrow_glower	= (float)m_eyebrow_glower.GetPos()/100.0f;

//	char temp[50];
//	sprintf(temp,"%f",m_SliderStatus.mouth_open);
//	AfxMessageBox(temp);

	pView->RefreshExp(&m_SliderStatus);
//	AfxMessageBox("^^^^^^^^");

//	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CExpSliders::UpdateSliders(SliderStatus * status)
{
	m_mouth_open.SetPos((int)(status->mouth_open*100));
	m_liptail_up.SetPos((int)(status->liptail_up*100));
	m_liptail_down.SetPos((int)(status->liptail_down*100));
	m_lip_longer.SetPos((int)(status->lip_longer*100));
	m_lip_tighten_o.SetPos((int)(status->lip_tighten_o*100));
	m_lip_tighten_u.SetPos((int)(status->lip_tighten_u*100));
	m_lip_open.SetPos((int)(status->lip_open*100));
	m_eyetail_up.SetPos((int)(status->eyetail_up*100));
	m_eye_close.SetPos((int)(status->eye_close*100));
	m_eyebrow_up.SetPos((int)(status->eyebrow_up*100));
	m_eyebrow_glower.SetPos((int)(status->eyebrow_glower*100));
}

BOOL CExpSliders::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==13)
	{
		UpdateText();
		return TRUE;
	}
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void CExpSliders::UpdateText()
{
	UpdateData(TRUE);

	MakeSpchSeq.MakeSeqAs((char*)(LPCTSTR)m_ctrlEdit);

//	KeyFramerRealtime.play();
	Face.PlaySequence();

//	AfxMessageBox((LPCTSTR)m_ctrlEdit);
}

LPCTSTR CExpSliders::GetText()
{
	return (LPCTSTR)m_ctrlEdit;
}

BOOL CExpSliders::OnInitDialog() 
{
	CDialog::OnInitDialog();


	m_mouth_open.SetRange(0,200,true);
	m_liptail_up.SetRange(0,200,true);
	m_liptail_down.SetRange(0,200,true);
	m_lip_longer.SetRange(0,200,true);
	m_lip_tighten_o.SetRange(0,200,true);
	m_lip_tighten_u.SetRange(0,200,true);
	m_lip_open.SetRange(0,200,true);
	m_eyetail_up.SetRange(0,200,true);
	m_eye_close.SetRange(0,200,true);
	m_eyebrow_up.SetRange(0,200,true);
	m_eyebrow_glower.SetRange(0,200,true);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
