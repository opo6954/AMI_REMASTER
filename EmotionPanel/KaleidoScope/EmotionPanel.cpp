// EmotionPanel.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "EmotionPanel.h"

#include "OpenGLWnd.h"
#include "MainFrm.h"

#include <mmsystem.h>

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmotionPanelApp

BEGIN_MESSAGE_MAP(CEmotionPanelApp, CWinApp)
	//{{AFX_MSG_MAP(CEmotionPanelApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmotionPanelApp construction

CEmotionPanelApp::CEmotionPanelApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEmotionPanelApp object

CEmotionPanelApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEmotionPanelApp initialization

BOOL CEmotionPanelApp::InitInstance()
{
	m_pMainWnd = new CMainFrame;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


BOOL CEmotionPanelApp::OnIdle(LONG lCount) 
{
	static bool flag = false;

	static clock_t  currtime, lasttime;
	double   elapsed_time;

	if(flag == false)
	{
		flag = true;
		lasttime = timeGetTime();
	}
	else
		lasttime = currtime;

	currtime = timeGetTime();
	elapsed_time = ((double)(currtime - lasttime))*0.001;

	currtime -= (clock_t)(NextFrame( elapsed_time )*1000.0);

	return 1;
//	return CWinApp::OnIdle(lCount);
}

#define VK_A 0x41 //a key  
#define VK_B 0x42 //b key  
#define VK_C 0x43 //c key  
#define VK_D 0x44 //d key  
#define VK_E 0x45 //e key  
#define VK_F 0x46 //f key  
#define VK_G 0x47 //g key  
#define VK_H 0x48 //h key  
#define VK_I 0x49 //i key  
#define VK_J 0x4A //j key  
#define VK_K 0x4B //k key  
#define VK_L 0x4C //l key  
#define VK_M 0x4D //m key  
#define VK_N 0x4E //n key  
#define VK_O 0x4F //o key  
#define VK_P 0x50 //p key  
#define VK_Q 0x51 //q key  
#define VK_R 0x52 //r key  
#define VK_S 0x53 //s key  
#define VK_T 0x54 //t key  
#define VK_U 0x55 //u key  
#define VK_V 0x56 //v key  
#define VK_W 0x57 //w key  
#define VK_X 0x58 //x key  
#define VK_Y 0x59 //y key  
#define VK_Z 0x5A 



void CEmotionPanelApp::Process()
{
	if(GetAsyncKeyState(VK_UP))
	{
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->IncA();
	}
	if(GetAsyncKeyState(VK_DOWN))
	{
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->DecA();
	}
	if(GetAsyncKeyState(VK_LEFT))
	{
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->DecSJ();
	}
	if(GetAsyncKeyState(VK_RIGHT))
	{
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->IncSJ();
	}
	if(GetAsyncKeyState(VK_ADD))
	{
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->HeartBeatRate--;
		if(((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->HeartBeatRate<7)
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->HeartBeatRate=7;
	}
	if(GetAsyncKeyState(VK_SUBTRACT))
	{
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->HeartBeatRate++;
		if(((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->HeartBeatRate>45)
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->HeartBeatRate=45;
	}

	if(((CMainFrame*)m_pMainWnd)->OpenGLWnd)
	{
		if(GetAsyncKeyState(VK_Q))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->theta+=0.2f;
		}
		if(GetAsyncKeyState(VK_A))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->theta-=0.2f;
		}
		if(GetAsyncKeyState(VK_W))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->scalex+=0.2f;
		}
		if(GetAsyncKeyState(VK_S))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->scalex-=0.2f;
		}
		if(GetAsyncKeyState(VK_E))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->scaley+=0.2f;
		}
		if(GetAsyncKeyState(VK_D))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->scaley-=0.2f;
		}
		if(GetAsyncKeyState(VK_H))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->transx+=0.02f;
		}
		if(GetAsyncKeyState(VK_F))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->transx-=0.02f;
		}
		if(GetAsyncKeyState(VK_T))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->transy+=0.02f;
		}
		if(GetAsyncKeyState(VK_G))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->transy-=0.02f;
		}
		if(GetAsyncKeyState(VK_Z))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->map_alpha-=0.04f;
		}
		if(GetAsyncKeyState(VK_X))
		{
			((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->map_alpha+=0.04f;
		}

	}
	
	if(GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->ProcessHeart();

	float *sorrow_joy, *sorrow_joy_tobe;
	float *anger, *anger_tobe;

	sorrow_joy = &((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->sorrow_joy;
	sorrow_joy_tobe = &((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->sorrow_joy_tobe;
	anger = &((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->anger;
	anger_tobe = &((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->anger_tobe;

	*sorrow_joy = *sorrow_joy*0.91f + *sorrow_joy_tobe*0.09f;
	*anger = *anger*0.91f + *anger_tobe*0.09f;

	((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->duration-=30;

	if(((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->duration<31 && ((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->duration!=0)
	{
		*sorrow_joy_tobe = 0.5f;
		*anger_tobe = 0.0f;
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->duration=0;
	}

	((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->theta+=0.22f;

	{
		static float th = 0.0f;

		th+=0.008f;

		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->transx+=sin(th)/1000.0f;
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->transy+=cos(th)/1000.0f;

		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->scalex-=sin(th)/100.0f;
		((CMainFrame*)m_pMainWnd)->OpenGLWnd.hud->scaley-=cos(th)/100.0f;
	}

}

float CEmotionPanelApp::NextFrame(float dt)
{
	int i;

	if(dt>0.03f)
	{
		for(i=1;i<=(int)(dt/0.030f);i++)
		{
			Process();
//			SimulationEngine->NextFrame(0.020f);
		}

		((CMainFrame*)AfxGetMainWnd())->OpenGLWnd.RedrawWindow();
		return (dt - 0.030f*(i-1));
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->OpenGLWnd.RedrawWindow();
		return dt;
	}
}
