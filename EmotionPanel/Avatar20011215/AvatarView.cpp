// AvatarView.cpp : implementation of the CAvatarView class
//

#include "stdafx.h"
#include "MyDIB.h"
#include "PDef.h"
#include "Eyes.h"
#include "Avatar.h"

#include "MorphObj.h"

#include "MainFrm.h"

#include "Parser.h"

#include "AvatarDoc.h"
#include "OpenGLVIew.h"
#include "AvatarView.h"

#include "ExpSliders.h"
#include "Keyframerdlg.h"
#include "Keyframer.h"

#include"gl\gl.h"
#include"gl\glu.h"
#include"gl\glaux.h"

#include "math.h"

#include "face.h"
#include "dibitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define RAND_MAX 2000

extern CExpSliders ExpSliders;
extern SliderStatus CurrentStatus;
extern SliderStatus NextStatus;
extern CMorphObj *Obj;
extern CParser *pParser0;
extern CParser *pParser1;
extern CParser *pParser2;
extern CParser *pParser3;
extern CParser *pParser4;
extern CParser *pParser5;
extern CParser *pParser6;
extern CParser *pParser7;
extern CParser *pParser8;
extern CParser *pParser9;
extern CParser *pParser10;
extern CParser *pParser11;

extern CSliderCtrl m_ctrlSlider;
extern CKeyFramerDlg KeyFramerDlg;
extern CKeyFramer KeyFramer;
extern CKeyFramer KeyFramerWink;
extern CKeyFramer KeyFramerSmile;
extern CKeyFramer KeyFramerBrowUp;

extern COpenGLView m_wndViewPort;

extern CFace Face;

extern CDIBitmap *m_pBitmap;

/////////////////////////////////////////////////////////////////////////////
// CAvatarView

IMPLEMENT_DYNCREATE(CAvatarView, CView)

BEGIN_MESSAGE_MAP(CAvatarView, CView)
	//{{AFX_MSG_MAP(CAvatarView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAvatarView construction/destruction

CAvatarView::CAvatarView()
{
	m_fLoad=FALSE;
}

CAvatarView::~CAvatarView()
{
	CleanHeap();
}

BOOL CAvatarView::PreCreateWindow(CREATESTRUCT& cs)
{

	cs.style|=CS_OWNDC;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAvatarView drawing

void CAvatarView::OnDraw(CDC* pDC)
{
	CAvatarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CAvatarView diagnostics

#ifdef _DEBUG
void CAvatarView::AssertValid() const
{
	CView::AssertValid();
}

void CAvatarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAvatarDoc* CAvatarView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAvatarDoc)));
	return (CAvatarDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAvatarView message handlers

int CAvatarView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndViewPort.Create( NULL, "OpenGL Viewport", WS_CHILD | WS_VISIBLE, CRect(0,0,10,10), this, 1001);

//	m_ctrlSlider.Create( WS_CHILD | WS_VISIBLE | TBS_HORZ, CRect(0,0,10,10), this, 1000);

	ExpSliders.Create( IDD_PROPPAGE_MEDIUM );
	KeyFramerDlg.Create( IDD_PROPPAGE_SMALL );

//	SetTimer( 9999, 33, NULL );

	return 0;
}

void CAvatarView::OnDestroy() 
{
//	KillTimer( 9999 );
	CView::OnDestroy();
}

void CAvatarView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	glViewport(0, 0, cx, cy);

	m_WinSize.x = cx;
	m_WinSize.y = cy;

//	m_ctrlSlider.MoveWindow(0, cy-32, cx-170, 32);
//	KeyFramerDlg.MoveWindow(cx-170, cy-32, 170, 32);
	m_wndViewPort.MoveWindow(0, 0, cx, cy);
}

BOOL CAvatarView::OnEraseBkgnd(CDC* pDC) 
{
	if(m_fLoad)
		return TRUE;
	
	return CView::OnEraseBkgnd(pDC);
}

void CAvatarView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CMainFrame* pFrame=(CMainFrame*) AfxGetMainWnd();
	char temp[20];

	m_SliderPos = m_ctrlSlider.GetPos();

	sprintf(temp, "%d frame   ", m_SliderPos);

	pFrame->m_wndStatusBar.SetWindowText( temp );

	KeyFramer.SetCurrentFrame( m_SliderPos );
}

void CAvatarView::MakeSampleData()
{
}

void CAvatarView::CleanHeap()
{
}

void CAvatarView::RefreshExp(SliderStatus * status)
{
/*	Obj->ResetInitialVertexList();

	Obj->CalculateMorph(pParser1->GetVertexList(), status->mouth_open);
	Obj->CalculateMorph(pParser2->GetVertexList(), status->liptail_up);
	Obj->CalculateMorph(pParser3->GetVertexList(), status->liptail_down);
	Obj->CalculateMorph(pParser4->GetVertexList(), status->lip_longer);
	Obj->CalculateMorph(pParser5->GetVertexList(), status->lip_tighten_o);
	Obj->CalculateMorph(pParser6->GetVertexList(), status->lip_tighten_u);
	Obj->CalculateMorph(pParser7->GetVertexList(), status->lip_open);
	Obj->CalculateMorph(pParser8->GetVertexList(), status->eyetail_up);
	Obj->CalculateMorph(pParser9->GetVertexList(), status->eye_close);
	Obj->CalculateMorph(pParser10->GetVertexList(), status->eyebrow_up);
	Obj->CalculateMorph(pParser11->GetVertexList(), status->eyebrow_glower);
*/
/*
	NextStatus.mouth_open = status->mouth_open;
	NextStatus.liptail_up = status->liptail_up;
	NextStatus.liptail_down = status->liptail_down;
	NextStatus.lip_longer = status->lip_longer;
	NextStatus.lip_tighten_o = status->lip_tighten_o;
	NextStatus.lip_tighten_u = status->lip_tighten_u;
	NextStatus.lip_open = status->lip_open;
	NextStatus.eyetail_up = status->eyetail_up;
	NextStatus.eye_close = status->eye_close;
	NextStatus.eyebrow_up = status->eyebrow_up;
	NextStatus.eyebrow_glower = status->eyebrow_glower;
*/
	CurrentStatus.mouth_open = status->mouth_open;
	CurrentStatus.liptail_up = status->liptail_up;
	CurrentStatus.liptail_down = status->liptail_down;
	CurrentStatus.lip_longer = status->lip_longer;
	CurrentStatus.lip_tighten_o = status->lip_tighten_o;
	CurrentStatus.lip_tighten_u = status->lip_tighten_u;
	CurrentStatus.lip_open = status->lip_open;
	CurrentStatus.eyetail_up = status->eyetail_up;
	CurrentStatus.eye_close = status->eye_close;
	CurrentStatus.eyebrow_up = status->eyebrow_up;
	CurrentStatus.eyebrow_glower = status->eyebrow_glower;

	Face.FaceCalculate();
//	Face.MakeSequence();

//	Face.PlaySequence();
	
//	m_wndViewPort.InvalidateRect(NULL,FALSE);

}

void CAvatarView::OnFileOpen() 
{
	CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();
	
	CFileDialog FileOpenDlg(TRUE, "key", "*.key", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, pFrame);
	char temp[200];

	FileOpenDlg.DoModal();
	sprintf(temp,"%s",(LPCTSTR)FileOpenDlg.GetPathName());

	KeyFramer.FileLoad( temp );
}

void CAvatarView::OnFileSave() 
{
	CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();
	
	CFileDialog FileSaveDlg(FALSE, "key", "*.key", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, pFrame);
	char temp[200];

	FileSaveDlg.DoModal();
	sprintf(temp,"%s",(LPCTSTR)FileSaveDlg.GetPathName());

	KeyFramer.FileSave( temp );
}

void CAvatarView::OnTimer(UINT nIDEvent) 
{
	static bool flag=FALSE;
	if(!flag)
	{
		CMainFrame* pFrame=(CMainFrame*) AfxGetMainWnd();

		pFrame->SetWindowText( "Facial Animation");

		flag=TRUE;
	}
	Face.OnTimer();
}
