/***************************************************************
 	Copyright (C) 2000 AI&MM LAB, EECS, KAIST

	Author	: yhseo
	Section : Main Robot Control Part  
		
	Description
		Human Robot main program
*****************************************************************/
// HRView.cpp : implementation of the CHRView class
//

#include "stdafx.h"
#include "HR.h"

#include "HRDoc.h"
#include "HRView.h"

#include "matrix.h"

#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "MainFrm.h" 
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define WALLY_NAVIGATION

long motionX[6] = {2000, 0,-1000, 1000,0,0};
long motionY[6] = {-3000,-4000 ,-5000,-4000,-4000,-4000};

//BYTE mousePos[6] = {0x15,0x13,0x13,0x15,0x15,0x15};
BYTE mousePos[6] = {0x15,0x15,0x15,0x15,0x15,0x15};

#define AMI1 1
#define AMI2 2
#define ACTION1 3 // 인사해봐
#define ACTION2 4 // 악수하자
#define ACTION3 5 // 내가 누구니?
#define ACTION4 6 // 빨간공 들어
#define ACTION5 7 // 공들기 준비
#define ACTION6 8 // 오른쪽에 놔
#define ACTION7 9 // 가운데 놔
#define ACTION8 10 // 왼쪽에 놔
#define ACTION9 11  // 바로
#define ACTION10 12 // 같이놀자
#define ACTION11 13 // 청소준비
#define ACTION12 14 // 청소시작
#define ACTION13 15 // 이제그만
#define ACTION14 16 // 잘했어
#define ACTION15 17 // 못했어
#define ACTION16 18 // 멋진모습
#define ACTION17 19 // 앞으로 가
#define ACTION18 20 // 뒤로가
#define ACTION19 21 // 오른쪽으로 돌아
#define ACTION20 22 // 왼쪽으로 돌아
#define ACTION21 23 // 정지
#define ACTION22 24 // 쉬어
#define ACTION23 25 // 여길봐
#define ACTION24 26 // 혼자놀아
#define ACTION25 27 // 기분이 어때?
#define ACTION26 28 // 나 어떠니?
#define ACTION27 29 // 읽기 준비
#define ACTION28 30 // 읽어봐

#define ThreshDist 60  // Wally() 함수 안..



int headMotionCount;
int armMotionCount;
int shakeCount;
int mouthCount;
int maxMouthCount=0;
bool idleMode = false;
bool speechMode = false;
int speechCount = 0;

bool cleanMode = false;
bool playAloneMode = false;

int nicemotion = 0;
bool bSeeHere = false;
bool mouthOpen = false;
int randnum;

int i,j,count;
/*--------------------------*/
/* 기타 조정을 위한 변수	*/
/*--------------------------*/
CMainFrame	*pFrame;

#define	MAX_MOVE	90000

BYTE	moveTable[MAX_MOVE];

int		recordCount=0;
int		playCount=0;
bool	walking;
bool	walkingMode = false;
bool	readMode = false;
int		emotionCount = 0;
/*
typedef struct { 
    UINT wXpos; 
    UINT wYpos; 
    UINT wZpos; 
    UINT wButtons; 
} JOYINFO; 
 
<dwButtons>

JOY_BUTTON1 : First joystick button is pressed. 
JOY_BUTTON2 : Second joystick button is pressed. 
JOY_BUTTON3 : Third joystick button is pressed. 
JOY_BUTTON4 : Fourth joystick button is pressed. 
*/
JOYCAPS		joycaps;
JOYINFOEX	joyinfoEx;
JOYINFO		joyinfo;

BOOL	bInit;
BOOL	bCommandMode;

BOOL	bJoy;
BOOL	bNavigation;
BOOL	bWally;
BOOL	bIsTable;
BOOL	bCatchBallReady;
BOOL	bGo;
BOOL	bSpeechGo;
BOOL	bPlay;

// for shared memory
int dwSize=3000;
HANDLE hVTB;

PUCHAR lpSharedMemory;
PUCHAR lpVisionCommand;
PUCHAR lpVisionData;

UINT	MSG_MOUTH_OPEN,MSG_MOUTH_CLOSE;
/////////////////////////////////////////////////////////////////
// CHRView

IMPLEMENT_DYNCREATE(CHRView, CView)

BEGIN_MESSAGE_MAP(CHRView, CView)
	//{{AFX_MSG_MAP(CHRView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CHAR()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_BN_CLICKED(IDC_IDLEENABLE, OnIdleEnable)
	ON_BN_CLICKED(IDC_IDLEDISABLE, OnIdleDisable)
	ON_BN_CLICKED(IDC_CLEAN, OnClean)



	//}}AFX_MSG_MAP

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_MESSAGE(WM_RECEIVEDATA,OnReceiveData)

	ON_CBN_SELCHANGE(IDC_COMBO1,OnComboChange)


	ON_COMMAND(IDC_INIT, OnInit)






END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHRView construction/destruction

CHRView::CHRView()
{
	bInit=FALSE;
	bCommandMode=MOBILE_MODE;
	
	bJoy=FALSE;
	bPlay=FALSE;
	bNavigation=FALSE;
	bWally=FALSE;

	bCatchBallReady=FALSE;
	bIsTable=FALSE;
	bGo=FALSE;
	bSpeechGo=FALSE;

	m_file=".\\hr.cfg";
	globalTimer=0;
}

CHRView::~CHRView()
{
	CloseHandle(hVTB);
}

BOOL CHRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHRView drawing

void CHRView::OnDraw(CDC* pDC)
{
	CHRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
	CComboBox	*pWnd;
	pFrame=(CMainFrame *)AfxGetMainWnd();
	pWnd=(CComboBox *)pFrame->m_wndDlg.GetDlgItem(IDC_COMBO1);
	pWnd->SetCurSel(bCommandMode);

}

/////////////////////////////////////////////////////////////////////////////
// CHRView printing

BOOL CHRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CHRView diagnostics

#ifdef _DEBUG
void CHRView::AssertValid() const
{
	CView::AssertValid();
}

void CHRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHRDoc* CHRView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHRDoc)));
	return (CHRDoc*)m_pDocument;
}
#endif //_DEBUG

void CHRView::Alert(CString msg)
{
	AfxMessageBox(msg);
}

int CHRView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
///////////////////////// daniel 2001.4.26.
	MsgEmotion = RegisterWindowMessage("MSG_AIMM_EMOTION");
////////////////////////////////////////////

	srand( (unsigned)time( NULL ) );

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!LoadCfg())
	{
		SaveCfg();
	}
	
	MSG_MOUTH_OPEN=RegisterWindowMessage("MSG_AIMM_MOUTHOPEN");
	MSG_MOUTH_CLOSE=RegisterWindowMessage("MSG_AIMM_MOUTHCLOSE");

	// create memory-mapped file for shared memory
	hVTB=CreateFileMapping((HANDLE)0xffffffff,0,PAGE_READWRITE,0,
		dwSize,(const char*)"VTB_DATA");
	
	if(hVTB==0)
	{
		Alert("Error Creat MM file!\n");
		ExitProcess(0); 
	}

	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		//TRACE("Alreay Exists!\n");
		//return;
	}

	lpSharedMemory=(PUCHAR)MapViewOfFile(hVTB,FILE_MAP_WRITE,0,0,0);

	lpVisionCommand=lpSharedMemory+1;
	lpVisionData=lpSharedMemory+2;

	if(lpSharedMemory==0)
	{
		Alert("Error Creat View of MM file!\n");
		ExitProcess(0); 
	}	

	// vision
	SetTimer(77, 150, NULL);
	
	// for global planner
	SetTimer(0, 100, NULL);

	// for navigation & mobile
	SetTimer(1, 333, NULL);

#ifdef JOYSTICK	
	SetTimer(15, 1000, NULL);
#endif

	return 0;
}


void CHRView::OnComboChange(void)
{
	CComboBox	*pWnd;

	pWnd=(CComboBox *)pFrame->m_wndDlg.GetDlgItem(IDC_COMBO1);
	int num=pWnd->GetCurSel();
	
	if(num==0)	bCommandMode=MOBILE_MODE;
	else	bCommandMode=CAMERA_MODE;
/*	
	CString msg;
	msg.Format("COMBO [%d]",num);
	AfxMessageBox(msg);
*/
}


// download hex-files to 196processors
void CHRView::OnInit()
{

}

void CHRView::Delay(long delay)
{
	while(delay--);
	
}


int visionCount=0;
int scanCount=0;
int findTarget=0;
int retryCount=0;

// 얼굴 검출 부분으로 바뀜
void CHRView::WallyVision() 
{
	//Vision Initialize
	if(visionCount==0)
	{
		scanCount=0;
		

		TRACE("\nINIT VIS");
	}

	memcpy(&Vstereo[0],lpVisionData,sizeof(VISION_DATA));

	//만약 얼굴이 있으면,
	if(!Vstereo[0].empty)
	{
		scanCount=0;
		retryCount=0;
		
//		findTarget=TRUE;

		//카메라 좌표계에서 중간에 오도록 함	
		if(Vstereo[0].MC.x>55 && Vstereo[0].MC.x<105
			&& Vstereo[0].MC.y>45 && Vstereo[0].MC.y<80)
		{
			if(findTarget==0)
			{
	
				findTarget=1;

				randnum = rand()%5;

				::SendMessage(HWND_BROADCAST,MsgEmotion,5099,3000);
			}

			return;
		}
		
		if(Vstereo[0].MC.x<75)
		{
//			headPosX=-3000;	
			headPosX=R_headPosX-200;	
		}
		if(Vstereo[0].MC.x>85)
		{
//			headPosX=3000;	
			headPosX=R_headPosX+200;	
		}

		if(Vstereo[0].MC.y<50)
		{
			headPosY=R_headPosY-200;	

		}
		if(Vstereo[0].MC.y>80)
		{
			headPosY=R_headPosY+200;
		}

		printText(50,350,"===============0");
		//printText(50,200,"%d x0 %d y0 %d, x1 %d y1 %d, rx %f ry %f ",Vstereo[1].empty,Vstereo[1].MC.x,Vstereo[1].MC.y,Vstereo[1].MC.x,Vstereo[1].MC.y,sp.rx,sp.ry);
		
	}

	visionCount++;
}

#define WANDERING	0
#define FIND_TARGET	1
#define GOTO_TARGET	2

int uMask[30]={0,};
int uMask2[30]={0,};

void CHRView::SendTTS(CString msg) 
{
#ifdef TTS	
	HWND cvwHandle;
	COPYDATASTRUCT cds;

	//CString msg;
	//msg.Format("안녕 하세요?  싸이언 토피아에 오신 것을 환영합니다. 저는 카이스트   인공 지능 연구실에    휴먼 로봇 입니다.");
	//msg.Format("Hellow, I am humanrobot. Nice meet you");
	cds.dwData=1;
	cds.cbData=msg.GetLength();  
	cds.lpData=(void *)LPCTSTR(msg); 

	cvwHandle=::FindWindow(NULL,_T("거원TTS"));


	if(cvwHandle==0)
		AfxMessageBox("error!");
	else
	::SendMessage(cvwHandle, WM_COPYDATA, (WPARAM)(this->m_hWnd), LPARAM(&cds));
#endif
}



void CHRView::printText(int x,int y,char *fmt, ...)
{
	CDC	*pDC=GetDC();
	va_list ap;
	va_start( ap, fmt );


	vsprintf(msg, fmt, ap );
	pDC->TextOut(x,y,msg);			

	va_end(ap);

	ReleaseDC(pDC);		
}

void CHRView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar=='s')
	{
	}
	if(nChar=='r')
	{
	}
	if(nChar=='j')
	{
	} 

	CView::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CHRView::LoadCfg(void)
{
	FILE *FP;
	char name[_MAX_PATH+1];

	strcpy(name, m_file.GetBuffer(_MAX_PATH));

	if (!(FP=fopen(name,"rb")))
	{
		CString msg;
		msg.Format("Can't Open [%s]",name);
		AfxMessageBox(msg);
		return FALSE;
	}

	fread(&recordCount,sizeof(int),1,FP);
	fread(moveTable,MAX_MOVE,1,FP);

	fclose(FP);

	return TRUE;
}

BOOL CHRView::SaveCfg(void)
{

	// 레코드된 명령을 저장하는 부분 
	FILE *FP;
	char name[_MAX_PATH+1];
	int DUMMY=0;
	strcpy(name, m_file.GetBuffer(_MAX_PATH));

	if (!(FP=fopen(name,"wb")))
	{
		CString msg;
		msg.Format("Can't Open [%s]",name);
		AfxMessageBox(msg);
		return FALSE;
	}

	fwrite(&recordCount,sizeof(int),1,FP);
	fwrite(moveTable,MAX_MOVE,1,FP);
	
	fclose(FP);
	return TRUE;	
}

void CHRView::OnFileOpen() 
{
	CFileDialog	dlg(TRUE,NULL,".\\*.cfg",
	OFN_HIDEREADONLY,_T("Configuration File (*.CFG)|*.CFG|All Files (*.*)|*.*"), NULL);

	if (dlg.DoModal()!=IDOK)
	{
		return;
	}
	
	m_file.Format("%s",dlg.GetPathName());

	LoadCfg();
}

void CHRView::OnFileSave() 
{
	CFileDialog	dlg(FALSE,NULL,".\\*.cfg",
	OFN_OVERWRITEPROMPT,_T("Configuration File (*.CFG)|*.CFG|All Files (*.*)|*.*"), NULL);

	if (dlg.DoModal()!=IDOK)
	{
		return;
	}
	
	m_file.Format("%s",dlg.GetPathName());

	SaveCfg();
}

LONG CHRView::OnReceiveData(UINT WParam,LONG LParam)	
{
//	int ttt=(BYTE)m_abIn[0];
	return TRUE;
}


void CHRView::OnTimer(UINT nIDEvent) 
{
	CHRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDC	*pDC=GetDC();
	CString Test;
	BYTE speechCommand=0;

	if(nIDEvent==0)// global planner
	{
		globalTimer++;

		speechCommand=*lpSharedMemory;			

		Test.Format("recog %d     ",speechCommand);			
		pDC->TextOut(50,50,Test);			
		

		if(GetAsyncKeyState(0x20))
		{
			speechCommand = AMI1;
		}

		if( !speechMode && (speechCommand == AMI1  || speechCommand == AMI2) ) // 인식 시작
		{
			OnIdleDisable();

			speechCount = 0;
			speechMode = true;

			Test.Format("인식시작");			
			pDC->TextOut(50,400,Test);

			::SendMessage(HWND_BROADCAST,MsgEmotion,3060,3000);

			randnum = rand() % 3;

			switch(randnum){
			case 0:
				SendTTS("네에");
				break;
			case 1:
				SendTTS("왜요?");
				break;
			case 2:
				SendTTS("아미");
				break;
			}

			if(bWally || cleanMode || playAloneMode){
				KillTimer(55);
			}

									

			dir = STOP;
			bGo = TRUE;
			
			bSpeechGo=FALSE;

			playAloneMode = false;
			bSeeHere = false;
			bNavigation=FALSE;
			bWally=FALSE;
			
		}
		if(speechMode){
			speechCount++;
			if(speechCount >= 70){
				speechMode = false;
				*lpSharedMemory = 0;
				Test.Format("                  ");			
				pDC->TextOut(50,400,Test);
				Test.Format("                                    ");			
				pDC->TextOut(50,450,Test);			
			}
		}

		speechCommand=*lpSharedMemory;

		if(GetAsyncKeyState(0x31))
		{
			speechCommand = ACTION1;
		}
		else if(GetAsyncKeyState(0x32))
		{
			speechCommand = ACTION2;
		}
		else if(GetAsyncKeyState(0x33))
		{
			speechCommand = ACTION3;
		}
		else if(GetAsyncKeyState(0x34))
		{
			speechCommand = ACTION4;
		}
		else if(GetAsyncKeyState(0x35))
		{
			speechCommand = ACTION5;
		}
		else if(GetAsyncKeyState(0x36))
		{
			speechCommand = ACTION6;
		}
		else if(GetAsyncKeyState(0x37))
		{
			speechCommand = ACTION7;
		}
		else if(GetAsyncKeyState(0x38))
		{
			speechCommand = ACTION8;
		}
		else if(GetAsyncKeyState(0x39))
		{
			speechCommand = ACTION9;
		}
		else if(GetAsyncKeyState(0x30))
		{
			speechCommand = ACTION10;
		}
		else if(GetAsyncKeyState(0x51))
		{
			speechCommand = ACTION11;
		}
		else if(GetAsyncKeyState(0x57))
		{
			speechCommand = ACTION12;
		}
		else if(GetAsyncKeyState(0x45))
		{
			speechCommand = ACTION13;
		}
		else if(GetAsyncKeyState(0x52))
		{
			speechCommand = ACTION14;
		}
		else if(GetAsyncKeyState(0x54))
		{
			speechCommand = ACTION15;
		}
		else if(GetAsyncKeyState(0x59))
		{
			speechCommand = ACTION16;
		}
		else if(GetAsyncKeyState(0x55))
		{
			speechCommand = ACTION17;
		}
		else if(GetAsyncKeyState(0x49))
		{
			speechCommand = ACTION18;
		}
		else if(GetAsyncKeyState(0x4f))
		{
			speechCommand = ACTION19;
		}
		else if(GetAsyncKeyState(0x50))
		{
			speechCommand = ACTION20;
		}
		else if(GetAsyncKeyState(0x41))
		{
			speechCommand = ACTION21;
		}
		else if(GetAsyncKeyState(0x53))
		{
			speechCommand = ACTION22;
		}
		else if(GetAsyncKeyState(0x44))
		{
			speechCommand = ACTION23;
		}
		else if(GetAsyncKeyState(0x46))
		{
			speechCommand = ACTION24;
		}


		//LHW_Mapping between speech recognition and text2speech
///////////////////////// daniel 2001.4.26.
//			::SendMessage(HWND_BROADCAST,MsgEmotion,speechCommand-1,0);
////////////////////////////////////////////

		if(speechMode && speechCommand>=ACTION1){

			Test.Format("인식모드 안");
			pDC->TextOut(50,450,Test);

			speechMode = false;

			if(speechCommand==ACTION14) // 잘했어
			{
				if(readMode)
					return;
				if(cleanMode)
					return;

				emotionCount++;
				if(emotionCount>2)emotionCount=2;
				if(emotionCount<0)emotionCount=0;

				randnum = rand() % 7;
				
				::SendMessage(HWND_BROADCAST,MsgEmotion,9099,3000);
				switch(randnum){
				case 0:
					SendTTS("감사합니다");
					break;
				case 1:
					SendTTS("아이,좋아");
					break;
				case 2:
					SendTTS("고맙습니다");
					break;
				case 3:
					SendTTS("이정도 쯤이야");
					break;
				case 4:
					SendTTS("아이,뭘요");
					break;
				case 5:
					SendTTS("기본이죠");
					break;
				case 6:
					SendTTS("좋았어요?");
				}

				Speak(2);

			}

			Test.Format("                  ");			
			pDC->TextOut(50,400,Test);
			Test.Format("                                    ");			
			pDC->TextOut(50,450,Test);			

		}
//		*lpSharedMemory=0;

	}	
	else if(nIDEvent==77)
	{

		if(bWally && !cleanMode && !playAloneMode)
		{
			WallyVision();
		}
		else if(bSeeHere)
		{
		}
	}

	ReleaseDC(pDC);		

	CView::OnTimer(nIDEvent);
}


void CHRView::OnIdleEnable() 
{
	// TODO: Add your control notification handler code here
	idleMode = true;
	SetTimer(51,5000,NULL);
}

void CHRView::OnIdleDisable() 
{
	// TODO: Add your control notification handler code here
	idleMode = false;
	KillTimer(51);
}


void CHRView::OnClean() 
{
}

void CHRView::Speak(int t) 
{
	for(i=0;i<t;i++){
		pwm[4]=0x13;

		Sleep(500);
		pwm[4]=0x15;
		Sleep(300);
	}
/*	pwm[4]=0x13;
	SendFace();
	mouthOpen = true;
	SetTimer(2,700,NULL);
	SetTimer(56,t*1000,NULL);*/
}