// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "EmotionPanel.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

//IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	Create(NULL, "EmotionPanel OpenGL application");	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/*
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
*/
	MsgEmotion = RegisterWindowMessage("MSG_AIMM_EMOTION");

	RECT rect;
	GetClientRect(&rect);
	OpenGLWnd.Create( NULL, "OpenGL View", WS_CHILD | WS_VISIBLE, rect, this, 1);

	ShowCursor(FALSE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_VISIBLE | WS_POPUP;

	cs.x = 0;
	cs.y = 0;
	cs.cx = 800;
	cs.cy = 600;

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

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

//	RECT rect;
//	m_wndStatusBar.GetWindowRect(&rect);

	OpenGLWnd.OnChangeSize(cx, cy);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==MsgEmotion)
	{
/*
		switch(wParam)
		{
			case 1:
				OpenGLWnd.hud->state = 1;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				break;
			case 2:
				OpenGLWnd.hud->state = 2;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				break;
			case 3:
				OpenGLWnd.hud->state = 3;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 1.0f;
				break;
			case 4:
				OpenGLWnd.hud->state = 4;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 1.0f;
				break;
			case 5:
				OpenGLWnd.hud->state = 5;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 1.0f;
				break;
			case 6:
				OpenGLWnd.hud->state = 6;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 0.5f;
				break;
			case 7:
				OpenGLWnd.hud->state = 7;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.5f;
				break;
			case 8:
				OpenGLWnd.hud->state = 8;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 0.5f;
				break;
			case 9:
				OpenGLWnd.hud->state = 9;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.25f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				break;
			case 10:
				OpenGLWnd.hud->state = 10;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.75f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				break;
			case 11:
				OpenGLWnd.hud->state = 11;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				break;
			case 12:
				OpenGLWnd.hud->state = 12;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.3f;
				break;
			case 13:
				OpenGLWnd.hud->state = 13;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 0.3f;
				break;
			case 14:
				OpenGLWnd.hud->state = 14;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.7f;
				break;
			case 15:
				OpenGLWnd.hud->state = 15;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.25f;
				OpenGLWnd.hud->anger_tobe = 0.7f;
				break;
			case 16:
				OpenGLWnd.hud->state = 16;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 0.7f;
				break;
			case 17:
				OpenGLWnd.hud->state = 17;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.75f;
				OpenGLWnd.hud->anger_tobe = 0.7f;
				break;
			case 18:
				OpenGLWnd.hud->state = 18;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.1f;
				break;
			case 19:
				OpenGLWnd.hud->state = 19;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.25f;
				OpenGLWnd.hud->anger_tobe = 0.3f;
				break;
			case 20:
				OpenGLWnd.hud->state = 20;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 0.6f;
				break;
			case 21:
				OpenGLWnd.hud->state = 21;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.75f;
				OpenGLWnd.hud->anger_tobe = 0.2f;
				break;
			case 22:
				OpenGLWnd.hud->state = 22;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				break;
		}
*/

		if(wParam==50000)
			PostQuitMessage(0);			


		OpenGLWnd.hud->sorrow_joy_tobe = (float)(wParam%100)/100.0f;
		OpenGLWnd.hud->anger_tobe = (float)(wParam/100)/100.0f;
		OpenGLWnd.hud->duration = lParam;

		return 0;
	}

	switch(message)
	{
	case WM_CHAR:
		{
			switch(wParam)
			{
			case '1':
				OpenGLWnd.hud->state = 1;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				OpenGLWnd.hud->duration = 5000;
				break;
			case '2':
				OpenGLWnd.hud->state = 2;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				OpenGLWnd.hud->duration = 3000;
				break;
			case '3':
				OpenGLWnd.hud->state = 3;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 1.0f;
				OpenGLWnd.hud->duration = 2000;
				break;
			case '4':
				OpenGLWnd.hud->state = 4;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 1.0f;
				OpenGLWnd.hud->duration = 1000;
				break;
			case '5':
				OpenGLWnd.hud->state = 5;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 1.0f;
				OpenGLWnd.hud->duration = 6000;
				break;
			case '6':
				OpenGLWnd.hud->state = 6;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 0.5f;
				OpenGLWnd.hud->duration = 7000;
				break;
			case '7':
				OpenGLWnd.hud->state = 7;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.5f;
				OpenGLWnd.hud->duration = 8000;
				break;
			case '8':
				OpenGLWnd.hud->state = 8;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 0.5f;
				OpenGLWnd.hud->duration = 9000;
				break;
			case '9':
				OpenGLWnd.hud->state = 9;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.25f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				OpenGLWnd.hud->duration = 10000;
				break;
			case '10':
				OpenGLWnd.hud->state = 10;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.75f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				OpenGLWnd.hud->duration = 12000;
				break;
			case '11':
				OpenGLWnd.hud->state = 11;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				break;
			case '12':
				OpenGLWnd.hud->state = 12;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.3f;
				break;
			case '13':
				OpenGLWnd.hud->state = 13;
				OpenGLWnd.hud->sorrow_joy_tobe = 1.0f;
				OpenGLWnd.hud->anger_tobe = 0.3f;
				break;
			case '14':
				OpenGLWnd.hud->state = 14;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.7f;
				break;
			case '15':
				OpenGLWnd.hud->state = 15;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.25f;
				OpenGLWnd.hud->anger_tobe = 0.7f;
				break;
			case '16':
				OpenGLWnd.hud->state = 16;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 0.7f;
				break;
			case '17':
				OpenGLWnd.hud->state = 17;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.75f;
				OpenGLWnd.hud->anger_tobe = 0.7f;
				break;
			case '18':
				OpenGLWnd.hud->state = 18;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.0f;
				OpenGLWnd.hud->anger_tobe = 0.1f;
				break;
			case '19':
				OpenGLWnd.hud->state = 19;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.25f;
				OpenGLWnd.hud->anger_tobe = 0.3f;
				break;
			case '20':
				OpenGLWnd.hud->state = 20;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 0.6f;
				break;
			case '21':
				OpenGLWnd.hud->state = 21;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.75f;
				OpenGLWnd.hud->anger_tobe = 0.2f;
				break;
			case '22':
				OpenGLWnd.hud->state = 22;
				OpenGLWnd.hud->sorrow_joy_tobe = 0.5f;
				OpenGLWnd.hud->anger_tobe = 0.0f;
				break;
			}
		}
		break;
	default:
		break;
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}
