// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "PDef.h"
#include "Avatar.h"

#include "MainFrm.h"
#include "ExpSliders.h"
#include "KeyFramerDlg.h"
#include "OpenGlView.h"

extern CExpSliders ExpSliders;
extern CKeyFramerDlg KeyFramerDlg;
extern COpenGLView m_wndViewPort;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/*	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
/*
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
*/
/*
	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
//	MsgNewFace = RegisterWindowMessage("MSG_DR_NEWFACE");
	MsgEmotion = RegisterWindowMessage("Wm_Center2Emotion");
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	cs.cx=800;
	cs.cy=600;
	cs.hMenu=NULL;
	cs.style = WS_POPUP|WS_VISIBLE;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	
	if(message==MsgEmotion)
	{
		
		switch(lParam)
		{
		case 0://±â»Û Ç¥Á¤
			m_wndViewPort.hud->anger = 1.0f;
			m_wndViewPort.hud->sorrow_joy = 0.0f;
			break;
		case 1://½½ÇÂ Ç¥Á¤
			m_wndViewPort.hud->sorrow_joy = 1.0f;
			m_wndViewPort.hud->anger = 0.0f;
			break;
		default:
			break;
		}

		
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);

	RECT rect;

	GetWindowRect(&rect);
	ExpSliders.SetWindowPos(&wndTop, rect.right, rect.top, 0, 0, SWP_NOSIZE);
	KeyFramerDlg.SetWindowPos(&wndTop, rect.left, rect.bottom,0,0, SWP_NOSIZE);
	KeyFramerDlg.ShowWindow(SW_HIDE);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	RECT rect;

	GetWindowRect(&rect);
	ExpSliders.SetWindowPos(&wndTop, rect.right, rect.top, 0, 0, SWP_NOSIZE);
	KeyFramerDlg.SetWindowPos(&wndTop, rect.left, rect.bottom,0,0, SWP_NOSIZE);

}

