// Avatar.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyDIB.h"
#include "PDef.h"
#include "Eyes.h"
#include "Avatar.h"

#include "parser.h"
#include "OpenGLView.h"

#include "MainFrm.h"
#include "AvatarDoc.h"
#include "AvatarView.h"

#include "ExpSliders.h"
#include "MorphObj.h"
#include "ToothObj.h"
#include "TongueObj.h"
#include "KeyFramerDlg.h"
#include "KeyFramer.h"
#include "Face.h"

#include "ShapeArray.h"
#include "MakeSpchSeq.h"

#include "DIBitmap.h"

#include <mmsystem.h>
#include <math.h>

extern COpenGLView m_wndViewPort;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAvatarApp

BEGIN_MESSAGE_MAP(CAvatarApp, CWinApp)
	//{{AFX_MSG_MAP(CAvatarApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAvatarApp construction

CAvatarApp::CAvatarApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAvatarApp object

CAvatarApp theApp;

CExpSliders ExpSliders;
CMorphObj *Obj;
CToothObj *ToothObj;
CTongueObj *TongueObj;

//CDIBitmap *m_pBitmap;
CParser *pParser0;
CParser *pParser1;
CParser *pParser2;
CParser *pParser3;
CParser *pParser4;
CParser *pParser5;
CParser *pParser6;
CParser *pParser7;
CParser *pParser8;
CParser *pParser9;
CParser *pParser10;
CParser *pParser11;
CParser *pParser12;

CParser *pParserToothDefault;
CParser *pParserToothMap;
CParser *pParserToothOpen;
CParser *pParserTongueDefault;
CParser *pParserTongueOpen;
CParser *pParserTongueMap;

CSliderCtrl m_ctrlSlider;
CKeyFramerDlg KeyFramerDlg;
CKeyFramer KeyFramer(0,FALSE);
CKeyFramer KeyFramerWink(1,FALSE);
CKeyFramer KeyFramerSmile(2,FALSE);
CKeyFramer KeyFramerBrowUp(3,FALSE);
CKeyFramer KeyFramerRealtime(4,FALSE);

COpenGLView m_wndViewPort;
SliderStatus m_SliderStatus;
SliderStatus CurrentStatus;
SliderStatus NextStatus;

CFace Face;

CEyes Eyes;

CShapeArray ShapeArray;

CMakeSpchSeq MakeSpchSeq;

/////////////////////////////////////////////////////////////////////////////
// CAvatarApp initialization

BOOL CAvatarApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAvatarDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CAvatarView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAvatarApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAvatarApp commands


BOOL CAvatarApp::OnIdle(LONG lCount) 
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



void CAvatarApp::Process()
{

	if(GetAsyncKeyState(VK_UP))
	{
		m_wndViewPort.hud->IncA();
	}
	if(GetAsyncKeyState(VK_DOWN))
	{
		m_wndViewPort.hud->DecA();
	}
	if(GetAsyncKeyState(VK_LEFT))
	{
		m_wndViewPort.hud->DecSJ();
	}
	if(GetAsyncKeyState(VK_RIGHT))
	{
		m_wndViewPort.hud->IncSJ();
	}

	if(GetAsyncKeyState(VK_Q) && !GetAsyncKeyState(VK_SHIFT))
	{
		m_wndViewPort.hud->photoa[0][1] = 1.0f;
		m_wndViewPort.hud->heartflag = false;
	}
	if(GetAsyncKeyState(VK_Q) && GetAsyncKeyState(VK_SHIFT))
	{
		m_wndViewPort.hud->photoa[0][1] = 1.0f;
		m_wndViewPort.hud->heartflag = true;
	}
	if(GetAsyncKeyState(VK_A))
	{
		m_wndViewPort.hud->photoa[0][1] = 0.0f;
	}
	if(GetAsyncKeyState(VK_W))
	{
		m_wndViewPort.hud->photoa[1][1] = 1.0f;
	}
	if(GetAsyncKeyState(VK_S))
	{
		m_wndViewPort.hud->photoa[1][1] = 0.0f;
	}
	if(GetAsyncKeyState(VK_E))
	{
		m_wndViewPort.hud->photoa[2][1] = 1.0f;
	}
	if(GetAsyncKeyState(VK_D))
	{
		m_wndViewPort.hud->photoa[2][1] = 0.0f;
	}
	if(GetAsyncKeyState(VK_R))
	{
		m_wndViewPort.hud->photoa[3][1] = 1.0f;
	}
	if(GetAsyncKeyState(VK_F))
	{
		m_wndViewPort.hud->photoa[3][1] = 0.0f;
	}

	if(GetAsyncKeyState(VK_T))
	{
		m_wndViewPort.hud->PhotoNext();
	}
	if(GetAsyncKeyState(VK_G))
	{
		m_wndViewPort.hud->PhotoPrev();
	}
	
/*
	if(m_wndViewPort)
	{
		if(GetAsyncKeyState(VK_Q))
		{
			m_wndViewPort.hud->theta+=0.2f;
		}
		if(GetAsyncKeyState(VK_A))
		{
			m_wndViewPort.hud->theta-=0.2f;
		}
		if(GetAsyncKeyState(VK_W))
		{
			m_wndViewPort.hud->scalex+=0.2f;
		}
		if(GetAsyncKeyState(VK_S))
		{
			m_wndViewPort.hud->scalex-=0.2f;
		}
		if(GetAsyncKeyState(VK_E))
		{
			m_wndViewPort.hud->scaley+=0.2f;
		}
		if(GetAsyncKeyState(VK_D))
		{
			m_wndViewPort.hud->scaley-=0.2f;
		}
		if(GetAsyncKeyState(VK_H))
		{
			m_wndViewPort.hud->transx+=0.02f;
		}
		if(GetAsyncKeyState(VK_F))
		{
			m_wndViewPort.hud->transx-=0.02f;
		}
		if(GetAsyncKeyState(VK_T))
		{
			m_wndViewPort.hud->transy+=0.02f;
		}
		if(GetAsyncKeyState(VK_G))
		{
			m_wndViewPort.hud->transy-=0.02f;
		}
		if(GetAsyncKeyState(VK_Z))
		{
			m_wndViewPort.hud->map_alpha-=0.04f;
		}
		if(GetAsyncKeyState(VK_X))
		{
			m_wndViewPort.hud->map_alpha+=0.04f;
		}

	}
*/	

	if(GetAsyncKeyState(VK_RETURN))
	{
		m_wndViewPort.hud->state = !m_wndViewPort.hud->state;
		Sleep(100);
	}

	if(GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	m_wndViewPort.hud->ProcessHeart();

	float *sorrow_joy, *sorrow_joy_tobe;
	float *anger, *anger_tobe;

	sorrow_joy = &m_wndViewPort.hud->sorrow_joy;
	sorrow_joy_tobe = &m_wndViewPort.hud->sorrow_joy_tobe;
	anger = &m_wndViewPort.hud->anger;
	anger_tobe = &m_wndViewPort.hud->anger_tobe;

	*sorrow_joy = *sorrow_joy*0.91f + *sorrow_joy_tobe*0.09f;
	*anger = *anger*0.91f + *anger_tobe*0.09f;

	m_wndViewPort.hud->duration-=30;

	if(m_wndViewPort.hud->duration<31 && m_wndViewPort.hud->duration!=0)
	{
		*sorrow_joy_tobe = 0.5f;
		*anger_tobe = 0.0f;
		m_wndViewPort.hud->duration=0;
	}

	m_wndViewPort.hud->theta+=0.25f;

	{
		static float th = 0.0f;

		th+=0.003f;

		m_wndViewPort.hud->transx+=sin(th)/100.0f;
		m_wndViewPort.hud->transy+=cos(th)/100.0f;

		m_wndViewPort.hud->scalex-=sin(th)/100.0f;
		m_wndViewPort.hud->scaley-=cos(th)/100.0f;

		m_wndViewPort.m_spinx = sin(th*0.64)*10.0f+cos(th*0.24)*10.0f;
		m_wndViewPort.m_spiny = sin(th*0.44)*6.0f;

		Face.OnTimer();

		float puresorrow;
		float purejoy;

		puresorrow = (0.5f-*sorrow_joy)*3.5f;
		purejoy = (*sorrow_joy-0.5f)*3.5f;
		if(puresorrow<0.0f)
			puresorrow = 0.0f;
		if(purejoy<0.0f)
			purejoy = 0.0f;

		Obj->CalculateMorph(pParser2->GetVertexList(), purejoy);
		Obj->CalculateMorph(pParser3->GetVertexList(), puresorrow);

		Obj->CalculateMorph(pParser10->GetVertexList(), purejoy);
		Obj->CalculateMorph(pParser11->GetVertexList(), puresorrow);

		Obj->CalculateMorph(pParser8->GetVertexList(), (*anger-puresorrow*0.3f)*1.4f);
		Obj->CalculateMorph(pParser7->GetVertexList(), (*anger-puresorrow*0.1f)*2.0f);
	}

}

float CAvatarApp::NextFrame(float dt)
{
	int i;

	if(dt>0.03f)
	{
		for(i=1;i<=(int)(dt/0.030f);i++)
		{
			Process();
//			SimulationEngine->NextFrame(0.020f);
		}

		m_wndViewPort.RedrawWindow();
		return (dt - 0.030f*(i-1));
	}
	else
	{
		m_wndViewPort.RedrawWindow();
		return dt;
	}
}
