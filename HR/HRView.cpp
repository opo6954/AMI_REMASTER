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
	ON_BN_CLICKED(IDC_HANDSTP, OnHandstp)
	ON_BN_CLICKED(IDC_TEST3, OnTest3)
	ON_BN_CLICKED(IDC_HANDCLS, OnHandClose)
	ON_BN_CLICKED(IDC_WALKING, OnWalking)
	ON_BN_CLICKED(IDC_STOPWALK, OnStopwalk)
	ON_BN_CLICKED(IDC_IDLEENABLE, OnIdleEnable)
	ON_BN_CLICKED(IDC_IDLEDISABLE, OnIdleDisable)
	ON_BN_CLICKED(IDC_HANDINIT, OnHandInit)
	ON_BN_CLICKED(IDC_CLEAN, OnClean)
	ON_BN_CLICKED(IDC_NICEMOTION, OnNiceMotion1)
	ON_BN_CLICKED(IDC_NICEMOTION2, OnNiceMotion2)
	ON_BN_CLICKED(IDC_NICEMOTION3, OnNiceMotion3)
	ON_BN_CLICKED(IDC_PLAYALONE, OnPlayalone)
	ON_BN_CLICKED(IDC_READREADY, OnReadReady)
	ON_BN_CLICKED(IDC_READING, OnReading)
	//}}AFX_MSG_MAP

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_MESSAGE(WM_RECEIVEDATA,OnReceiveData)

	ON_CBN_SELCHANGE(IDC_COMBO1,OnComboChange)


	ON_COMMAND(IDC_INIT, OnInit)

	ON_COMMAND(IDC_UP, OnUp)
	ON_COMMAND(IDC_DOWN, OnDown)
	ON_COMMAND(IDC_LEFT, OnLeft)
	ON_COMMAND(IDC_RIGHT, OnRight)
	ON_COMMAND(IDC_STOP, OnStop)

	ON_COMMAND(IDC_TEST, OnTest)
	ON_COMMAND(IDC_TEST2, OnTest2)
	ON_COMMAND(IDC_TEST4, OnTest4)

	ON_COMMAND(IDC_HANDRST, OnHandReset)
	
	ON_COMMAND(IDC_SENSOR, OnSensor)
	ON_COMMAND(IDC_WALLY, OnWally)

	ON_COMMAND(IDC_DEMO, OnDemo)

	ON_COMMAND(IDC_JOYSTICK, OnJoy)
	ON_COMMAND(IDC_ALLSTOP, OnAllStop)

	ON_COMMAND(IDC_READY, OnArmReady)
	ON_COMMAND(IDC_GRAB, OnArmGrab)
	ON_COMMAND(IDC_MOVELEFT, OnArmMoveLeft)
	ON_COMMAND(IDC_MOVERIGHT, OnArmMoveRight)
	ON_COMMAND(IDC_RESETARM, OnResetArm)



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

#ifdef JOYSTICK
	
	// TODO: Add your specialized creation code here
	if(joyGetNumDevs()<1)
	{
		CString msg;
		msg.Format("Can't find Joystick_Port!");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	if(joyGetDevCaps(JOYSTICKID1, &joycaps,sizeof(JOYCAPS))!=JOYERR_NOERROR)
	{
		/*MMSYSERR_NODRIVER :: The joystick driver is not present.	*/
		/*MMSYSERR_INVALPARAM  :: An invalid parameter was passed.	*/
		CString msg;
		msg.Format("Error in Joystick1!");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}
#endif

	
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

	if(!m_comm1.InitSerial(COMPORT_1,115200))
	{
		Alert("Can't Open ComPort [1] !");
		ExitProcess(0); 
	}

	m_comm1.SetHwnd(this->m_hWnd); 

	m_comm1.SetReadBuffer(m_abIn,MAX_BUFFER);  
	m_comm2.SetReadBuffer(m_abIn2,MAX_BUFFER);  
	m_comm3.SetReadBuffer(m_abIn3,MAX_BUFFER);  
	m_comm4.SetReadBuffer(m_abIn4,MAX_BUFFER);  
	//m_comm1.RunThread();

	if (yesNoDlg.DoModal()==IDOK)
	{
		OnInit();
	}
	else
	{
		
		// Sensor Board(196)의 OS모드 에서는 
		// Mobile Read(2051)명령이 충돌됨!

		m_pComm=&m_comm1;
		Sleep(500);

		// clear receive buffer!!!
		m_pComm->ReadCommBlock(MAX_BUFFER);
		m_pComm->ReadCommBlock(MAX_BUFFER);
		m_pComm->ReadCommBlock(MAX_BUFFER);
		m_pComm->ReadCommBlock(MAX_BUFFER);
		m_pComm->ReadCommBlock(MAX_BUFFER);
	}

#ifdef MOBILE	
	SetMobileSpeed(MAX_ACC,MAX_ROTATION_ACC);
	InitMobileParameters();
#endif

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

void CHRView::DownLoad()
{
	FILE* fp;
	struct _stat statbuf;
	CString msg;
	
	int result;
	unsigned long i=0;
	char buffer[90000];

	long sizet=0;	
	int c=0;

	if ( (fp=fopen(downloadFile,"r")) == NULL)
	{
		AfxMessageBox("No HEX file !\n"); 
		return;
	}

	// get information about the file 
	result=_fstat(fileno(fp), &statbuf);
	
	if( result != 0 )
	{	
		AfxMessageBox("Bad file handle\n" );
		return;
	}

	while(!m_pComm->SendChar('L'));

//	msg.Format("size %d",statbuf.st_size);
//	AfxMessageBox(msg);
	

	sizet=0;
	sizet=fread(buffer,1,statbuf.st_size,fp);
	fclose(fp);  

	//	msg.Format("rsize %d",sizet);
	//	AfxMessageBox(msg);
	
	i=0;
	c=0;
	
	int bufferSize=10000;
	while (sizet>0)
	{
		
		if(sizet>bufferSize)
		{
			m_pComm->WriteBlock(buffer+bufferSize*c,bufferSize,&i);	
			sizet-=bufferSize;
		}
		else
		{
			m_pComm->WriteBlock(buffer+bufferSize*c,sizet,&i);	
			sizet-=sizet;
		}
		c+=1;
		
		//TRACE("[%d %%]",(int)(bufferSize*c*100/statbuf.st_size));
	}
	//TRACE("[%d %%]",(int)(bufferSize*c*100/statbuf.st_size));	

	Sleep(500);
	while(!m_pComm->SendChar('R')); 
	Sleep(300);
	while(!m_pComm->SendChar('n')); 
	Sleep(300);
	//AfxMessageBox("complete!");

	// clear receive buffer!!!
	m_pComm->ReadCommBlock(MAX_BUFFER);
	m_pComm->ReadCommBlock(MAX_BUFFER);
	m_pComm->ReadCommBlock(MAX_BUFFER);
	m_pComm->ReadCommBlock(MAX_BUFFER);
	m_pComm->ReadCommBlock(MAX_BUFFER);

	//AfxMessageBox("complete!");

}



// download hex-files to 196processors
void CHRView::OnInit()
{
#ifdef SENSOR
	///////////////////////////
	// sensor
	///////////////////////////
#ifndef NEW_SENSOR

	m_pComm=&m_comm1;
	downloadFile=sensorFile;
	DownLoad();

#endif

#endif

#ifdef HEAD
	///////////////////////////
	// head
	///////////////////////////
	if(!m_comm2.InitSerial(COMPORT_2,115200))
	{
		Alert("Can't Open ComPort [2] !");
		ExitProcess(0); 
	}

	m_pComm=&m_comm2;
	downloadFile=headFile;
	DownLoad();
	m_comm2.CloseSerial();
#endif

#ifdef ARM
	///////////////////////////
	// right arm
	///////////////////////////
	if(!m_comm3.InitSerial(COMPORT_3,115200))
	{
		Alert("Can't Open ComPort [3] !");
		ExitProcess(0); 
	}

	m_pComm=&m_comm3;
	downloadFile=armFile;
	DownLoad();
	m_comm3.CloseSerial();

	///////////////////////////
	// left arm
	///////////////////////////
	if(!m_comm4.InitSerial(COMPORT_4,115200))
	{
		Alert("Can't Open ComPort [4] !");
		ExitProcess(0); 
	}

	m_pComm=&m_comm4;
	downloadFile=armFile;
	DownLoad();
	m_comm4.CloseSerial();	
#endif
	//AfxMessageBox("complete !");
}

void CHRView::Delay(long delay)
{
	while(delay--);
	
}

void CHRView::SetMobileSpeed(BYTE max_acc,BYTE max_rotation_acc)
{
#ifdef MOBILE

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='M';

	m_command[4]='S';
	m_command[5]=max_acc;
	m_command[6]=max_rotation_acc;

	m_command[7]='S';
	
	m_pComm=&m_comm1;

	
	//m_pComm->WriteCommBlock((LPSTR)m_command,8);
	DWORD dwBytesToWrite=8,dwBytesWritten=0;
	m_pComm->WriteBlock((LPSTR)m_command,dwBytesToWrite,&dwBytesWritten);
	while(dwBytesWritten<dwBytesToWrite);

#endif
}

BOOL CHRView::GoMobile(BYTE dir)
{
#ifdef MOBILE

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='M';

	m_command[4]='G';
	m_command[5]=dir;
	m_command[6]=0x00;

	m_command[7]='S';
	
	m_pComm=&m_comm1;

	for(int i=0;i<8;i++)
	{
		while(!m_pComm->SendChar(m_command[i]));
		Sleep(3);		
	}

// read Mobile Encoder 
	int readSize=0,size;
	
	count=MAX_SERIAL_RETRY_COUNT;
	while(count--)
	{
		size=m_pComm->ReadCommBlock(MOBILE_DATA_SIZE);
		memcpy(tempBuffer+readSize,m_abIn,size);
		readSize+=size;
		if(readSize>=MOBILE_DATA_SIZE) break;
	}

//	TRACE("\nreadSize %d count %d",readSize,count);
//	for(i=0;i<9;i++)
//		TRACE("\tGet %c",tempBuffer[i]);

	// if error occurs at receiving 
	if(!count || (tempBuffer[0]!='Y' || tempBuffer[1]!='h'))
	{
		// clear receive buffer!
		while(m_pComm->ReadCommBlock(MAX_BUFFER));
		//AfxMessageBox("encoder error!");
		printText(50,110,"ERROR : ReadMobileEncoder");
		return FALSE;
	}


	for(i=0;i<9;i++)
		TRACE("\tGet %c",tempBuffer[i]);

	WORD temp;
	temp=tempBuffer[4];
	temp<<=8;
	temp|=tempBuffer[5];

	encoder1=(_int16)temp;

	temp=tempBuffer[6];
	temp<<=8;
	temp|=tempBuffer[7];

	encoder2=(_int16)temp;


	if(dir==STOP && ABS((ABS(encoder1)-ABS(encoder2)))>30)
		encoder1=encoder2=0;

	TRACE("\nGetEncoder %d %d",encoder1,encoder2);
	printText(50,140,"GetEncoder %d %d       ",encoder1,encoder2);
	
	// pose estimation from Kinematic equation of two wheel platform	
	PredictMobilePose(&JtRobot,&JtSpeed);	

	printText(50,110,"Go %c",dir);
#endif
	return TRUE;
}


void CHRView::SensorEnable(BYTE	flag)
{
#ifdef SENSOR


#ifndef NEW_SENSOR

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='I';
	m_command[4]='U';
	m_command[5]=flag;
	m_command[6]='S';

	m_pComm=&m_comm1;

	//m_pComm->WriteCommBlock((LPSTR)m_command,7);	
	DWORD dwBytesToWrite=7,dwBytesWritten=0;
	m_pComm->WriteBlock((LPSTR)m_command,dwBytesToWrite,&dwBytesWritten);
	while(dwBytesWritten<dwBytesToWrite);

#endif

#endif
}

void CHRView::SendFace()
{
#ifdef HEAD
	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='0';

	
	m_command[4]=pwm[0];
	m_command[5]=pwm[1];
	m_command[6]=pwm[2];
	m_command[7]=pwm[3];
	m_command[8]=pwm[4];
	m_command[9]=pwm[5];

	m_command[10]='S';


	if(!m_comm2.InitSerial(COMPORT_2,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [2] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm2;	
	for(int i=0;i<11;i++)
	{
		while(!m_pComm->SendChar(m_command[i]));	
		Sleep(1);
	}
	m_comm2.CloseSerial();

#endif

}


void CHRView::SendHead()
{
#ifdef HEAD

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='H';
	m_command[4]='S';

	temp=headPosX;
	
	m_command[5]=(BYTE)(temp>>24);
	m_command[6]=(BYTE)(temp>>16);
	m_command[7]=(BYTE)(temp>>8);
	m_command[8]=(BYTE)(temp);

	temp=headPosY;
	
	m_command[9]=(BYTE)(temp>>24);
	m_command[10]=(BYTE)(temp>>16);
	m_command[11]=(BYTE)(temp>>8);
	m_command[12]=(BYTE)(temp);

	m_command[13]='S';

	if(!m_comm2.InitSerial(COMPORT_2,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [2] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm2;
	m_pComm->WriteCommBlock((LPSTR)m_command,14);
	m_comm2.CloseSerial();
	Sleep(1);
#endif
}

void CHRView::ReadHead()
{
	static char aaaa=0;
#ifdef HEAD
	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='H';
	m_command[4]='R';
	m_command[13]='S';

	if(!m_comm2.InitSerial(COMPORT_2,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [2] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm2;
	
	while(!m_pComm->SendChar(170));		
	while(!m_pComm->SendChar(170));	
	while(!m_pComm->SendChar(170));	
	m_pComm->WriteCommBlock((LPSTR)m_command,14);	
	Sleep(1);
/*
	for(int i=0;i<14;i++)
	{
		while(!m_pComm->SendChar(m_command[i]));	
	}

	Sleep(1);
	TRACE("\nwrite!");
*/
	// get Data from head 
	for(i=0;i<HEAD_DATA_SIZE;i++)
	{
		while(!m_pComm->ReadChar(&tempBuffer[i]));
		//Sleep(1);		
		//TRACE("\tGet %c",tempBuffer[i]);
	}

	temp=tempBuffer[4];
	temp<<=8;
	temp|=tempBuffer[5];
	temp<<=8;
	temp|=tempBuffer[6];
	temp<<=8;
	temp|=tempBuffer[7];

	R_headPosX=(LONG)temp;

	temp=tempBuffer[8];
	temp<<=8;
	temp|=tempBuffer[9];
	temp<<=8;
	temp|=tempBuffer[10];
	temp<<=8;
	temp|=tempBuffer[11];

	R_headPosY=(LONG)temp;
	
	TRACE("\nGetHeadPos %d %d",R_headPosX,R_headPosY);
	printText(50,260,"GetHeadPos %d %d %d",R_headPosX,R_headPosY,aaaa++);
	
	m_comm2.CloseSerial();
	Sleep(1);
#endif
}

/*
void CHRView::ReadHead()
{
#ifdef HEAD
	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='H';
	m_command[4]='R';
	m_command[13]='S';

	if(!m_comm2.InitSerial(COMPORT_2,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [2] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm2;

//	m_pComm->WriteCommBlock((LPSTR)m_command,15);	
//	Sleep(1);	

	DWORD dwBytesToWrite=14,dwBytesWritten=0;
	m_pComm->WriteBlock((LPSTR)m_command,dwBytesToWrite,&dwBytesWritten);
	while(dwBytesWritten<dwBytesToWrite);

//	for(int i=0;i<14;i++)
//	{
//		while(!m_pComm->SendChar(m_command[i]));	
//		Sleep(1);	
//	}

	TRACE("\nWrite Success!!!");
	
	// get Data from head 
	int readSize=0,size;
	
	count=500000;
	while(count--)
	{
		size=m_pComm->ReadCommBlock(HEAD_DATA_SIZE);
		memcpy(tempBuffer+readSize,m_abIn,size);
		readSize+=size;
		if(readSize>=HEAD_DATA_SIZE) break;
	}

	TRACE("\nreadSize %d count %d",readSize,count);
	for(i=0;i<9;i++)
		TRACE("\tGet %c",tempBuffer[i]);

	temp=tempBuffer[4];
	temp<<=8;
	temp|=tempBuffer[5];
	temp<<=8;
	temp|=tempBuffer[6];
	temp<<=8;
	temp|=tempBuffer[7];

	R_headPosX=(LONG)temp;

	temp=tempBuffer[8];
	temp<<=8;
	temp|=tempBuffer[9];
	temp<<=8;
	temp|=tempBuffer[10];
	temp<<=8;
	temp|=tempBuffer[11];

	R_headPosY=(LONG)temp;
	
	TRACE("\nGetPos %d %d",R_headPosX,R_headPosY);
  
	m_comm2.CloseSerial();

#endif

}
*/
void CHRView::HeadStop()
{
#ifdef HEAD

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='H';
	m_command[4]='F';

	m_command[13]='S';

	if(!m_comm2.InitSerial(COMPORT_2,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [2] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm2;
	m_pComm->WriteCommBlock((LPSTR)m_command,14);	
	m_comm2.CloseSerial();
	Sleep(1);

#endif
}

void CHRView::SendRightHand()
{
#ifdef HEAD
	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='1';

	
	m_command[4]=RightHand[0];
	m_command[5]=RightHand[1];
	m_command[6]=RightHand[2];
	m_command[7]=RightHand[3];
	m_command[8]=RightHand[4];
	m_command[9]=RightHand[5];

	m_command[10]='S';


	if(!m_comm2.InitSerial(COMPORT_2,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [2] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm2;	
	for(int i=0;i<11;i++)
	{
		while(!m_pComm->SendChar(m_command[i]));	
		Sleep(3);
	}
	m_comm2.CloseSerial();

#endif

}


void CHRView::SendLeftHand()
{
#ifdef HEAD
	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='2';

	
	m_command[4]=LeftHand[0];
	m_command[5]=LeftHand[1];
	m_command[6]=LeftHand[2];
	m_command[7]=LeftHand[3];
	m_command[8]=LeftHand[4];
	m_command[9]=LeftHand[5];

	m_command[10]='S';


	if(!m_comm2.InitSerial(COMPORT_2,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [2] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm2;	
	for(int i=0;i<11;i++)
	{
		while(!m_pComm->SendChar(m_command[i]));	
		Sleep(3);
	}
	m_comm2.CloseSerial();

#endif

}

void CHRView::SendRightArm()
{
#ifdef ARM

	RightArm[2]=(long)(RightArm[2]*M_ARM2);
	RightArm[3]=(long)(RightArm[3]*M_ARM3);

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='A';
	m_command[4]='S';

	temp=(DWORD)RightArm[0];
	
	m_command[5]=(BYTE)(temp>>24);
	m_command[6]=(BYTE)(temp>>16);
	m_command[7]=(BYTE)(temp>>8);
	m_command[8]=(BYTE)(temp);

	temp=(DWORD)RightArm[1];
	
	m_command[9]=(BYTE)(temp>>24);
	m_command[10]=(BYTE)(temp>>16);
	m_command[11]=(BYTE)(temp>>8);
	m_command[12]=(BYTE)(temp);

	temp=(DWORD)RightArm[2];
	
	m_command[13]=(BYTE)(temp>>24);
	m_command[14]=(BYTE)(temp>>16);
	m_command[15]=(BYTE)(temp>>8);
	m_command[16]=(BYTE)(temp);
	
	temp=(DWORD)RightArm[3];
	
	m_command[17]=(BYTE)(temp>>24);
	m_command[18]=(BYTE)(temp>>16);
	m_command[19]=(BYTE)(temp>>8);
	m_command[20]=(BYTE)(temp);

	temp=(DWORD)RightArm[4];
	
	m_command[21]=(BYTE)(temp>>24);
	m_command[22]=(BYTE)(temp>>16);
	m_command[23]=(BYTE)(temp>>8);
	m_command[24]=(BYTE)(temp);
	
	temp=(DWORD)RightArm[5];
	
	m_command[25]=(BYTE)(temp>>24);
	m_command[26]=(BYTE)(temp>>16);
	m_command[27]=(BYTE)(temp>>8);
	m_command[28]=(BYTE)(temp);
	
	m_command[29]='S';

	if(!m_comm3.InitSerial(COMPORT_3,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [3] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm3;
	m_pComm->WriteCommBlock((LPSTR)m_command,30);
	
	m_comm3.CloseSerial();
#endif
}

void CHRView::SendLeftArm()
{
#ifdef ARM

	LeftArm[2]=(long)(LeftArm[2]*M_ARM2);
	LeftArm[3]=(long)(LeftArm[3]*M_ARM3);

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='A';
	m_command[4]='S';

	temp=(DWORD)LeftArm[0];
	
	m_command[5]=(BYTE)(temp>>24);
	m_command[6]=(BYTE)(temp>>16);
	m_command[7]=(BYTE)(temp>>8);
	m_command[8]=(BYTE)(temp);

	temp=(DWORD)LeftArm[1];
	
	m_command[9]=(BYTE)(temp>>24);
	m_command[10]=(BYTE)(temp>>16);
	m_command[11]=(BYTE)(temp>>8);
	m_command[12]=(BYTE)(temp);

	temp=(DWORD)LeftArm[2];
	
	m_command[13]=(BYTE)(temp>>24);
	m_command[14]=(BYTE)(temp>>16);
	m_command[15]=(BYTE)(temp>>8);
	m_command[16]=(BYTE)(temp);
	
	temp=(DWORD)LeftArm[3];
	
	m_command[17]=(BYTE)(temp>>24);
	m_command[18]=(BYTE)(temp>>16);
	m_command[19]=(BYTE)(temp>>8);
	m_command[20]=(BYTE)(temp);

	temp=(DWORD)LeftArm[4];
	
	m_command[21]=(BYTE)(temp>>24);
	m_command[22]=(BYTE)(temp>>16);
	m_command[23]=(BYTE)(temp>>8);
	m_command[24]=(BYTE)(temp);
	
	temp=(DWORD)LeftArm[5];
	
	m_command[25]=(BYTE)(temp>>24);
	m_command[26]=(BYTE)(temp>>16);
	m_command[27]=(BYTE)(temp>>8);
	m_command[28]=(BYTE)(temp);
	
	m_command[29]='S';

	if(!m_comm4.InitSerial(COMPORT_4,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [4] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm4;
	m_pComm->WriteCommBlock((LPSTR)m_command,30);	

	m_comm4.CloseSerial();
#endif
}

BOOL CHRView::ReadRightArm()
{
#ifdef ARM

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='A';
	m_command[4]='R';

	if(!m_comm3.InitSerial(COMPORT_3,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [3] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm3;
	m_pComm->WriteCommBlock((LPSTR)m_command,30);
	Sleep(1);
	/* get Data from arm */
	for(int i=0;i<ARM_DATA_SIZE;i++)
	{
		while(!m_pComm->ReadChar(&tempBuffer[i]));	
		//Sleep(1);
		TRACE("\nGet %c",tempBuffer[i]);
	}

	temp=tempBuffer[4];
	temp<<=8;
	temp|=tempBuffer[5];
	temp<<=8;
	temp|=tempBuffer[6];
	temp<<=8;
	temp|=tempBuffer[7];

	R_RightArm[0]=(LONG)temp;

	temp=tempBuffer[8];
	temp<<=8;
	temp|=tempBuffer[9];
	temp<<=8;
	temp|=tempBuffer[10];
	temp<<=8;
	temp|=tempBuffer[11];

	R_RightArm[1]=(LONG)temp;

	temp=tempBuffer[12];
	temp<<=8;
	temp|=tempBuffer[13];
	temp<<=8;
	temp|=tempBuffer[14];
	temp<<=8;
	temp|=tempBuffer[15];

	R_RightArm[2]=(LONG)temp;

	temp=tempBuffer[16];
	temp<<=8;
	temp|=tempBuffer[17];
	temp<<=8;
 	temp|=tempBuffer[18];
	temp<<=8;
	temp|=tempBuffer[19];

	R_RightArm[3]=(LONG)temp;

	temp=tempBuffer[20];
	temp<<=8;
	temp|=tempBuffer[21];
	temp<<=8;
	temp|=tempBuffer[22];
	temp<<=8;
	temp|=tempBuffer[23];

	R_RightArm[4]=(LONG)temp;

	temp=tempBuffer[24];
	temp<<=8;
	temp|=tempBuffer[25];
	temp<<=8;
	temp|=tempBuffer[26];
	temp<<=8;
	temp|=tempBuffer[27];

	R_RightArm[5]=(LONG)temp;

	R_RightArm[2]=(long)(R_RightArm[2]/M_ARM2);
	R_RightArm[3]=(long)(R_RightArm[3]/M_ARM3);

	m_comm3.CloseSerial();
	Sleep(1);
#endif
	return TRUE;
}

BOOL CHRView::ReadLeftArm()
{
#ifdef ARM

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='A';
	m_command[4]='R';

	if(!m_comm4.InitSerial(COMPORT_4,115200))
	{
		CString msg;
		msg.Format("Can't Open ComPort [3] !");
		AfxMessageBox(msg);
		ExitProcess(0); 
	}

	m_pComm=&m_comm4;
	m_pComm->WriteCommBlock((LPSTR)m_command,30);
	Sleep(1);
	/* get Data from arm */
	for(int i=0;i<ARM_DATA_SIZE;i++)
	{
		while(!m_pComm->ReadChar(&tempBuffer[i]));	
		//Sleep(1);
		TRACE("\nGet %c",tempBuffer[i]);
	}

	temp=tempBuffer[4];
	temp<<=8;
	temp|=tempBuffer[5];
	temp<<=8;
	temp|=tempBuffer[6];
	temp<<=8;
	temp|=tempBuffer[7];

	R_LeftArm[0]=(LONG)temp;

	temp=tempBuffer[8];
	temp<<=8;
	temp|=tempBuffer[9];
	temp<<=8;
	temp|=tempBuffer[10];
	temp<<=8;
	temp|=tempBuffer[11];

	R_LeftArm[1]=(LONG)temp;

	temp=tempBuffer[12];
	temp<<=8;
	temp|=tempBuffer[13];
	temp<<=8;
	temp|=tempBuffer[14];
	temp<<=8;
	temp|=tempBuffer[15];

	R_LeftArm[2]=(LONG)temp;

	temp=tempBuffer[16];
	temp<<=8;
	temp|=tempBuffer[17];
	temp<<=8;
	temp|=tempBuffer[18];
	temp<<=8;
	temp|=tempBuffer[19];

	R_LeftArm[3]=(LONG)temp;

	temp=tempBuffer[20];
	temp<<=8;
	temp|=tempBuffer[21];
	temp<<=8;
	temp|=tempBuffer[22];
	temp<<=8;
	temp|=tempBuffer[23];

	R_LeftArm[4]=(LONG)temp;

	temp=tempBuffer[24];
	temp<<=8;
	temp|=tempBuffer[25];
	temp<<=8;
	temp|=tempBuffer[26];
	temp<<=8;
	temp|=tempBuffer[27];

	R_LeftArm[5]=(LONG)temp;

	R_LeftArm[2]=(long)(R_LeftArm[2]/M_ARM2);
	R_LeftArm[3]=(long)(R_LeftArm[3]/M_ARM3);
	
	m_comm4.CloseSerial();
	Sleep(1);
#endif
	return TRUE;
}


BOOL CHRView::ReadMobileEncoder()
{
#ifdef MOBILE
	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='M';
	m_command[4]='E';
	m_command[7]='S';
	
	m_pComm=&m_comm1;

	for(int i=0;i<8;i++)
	{
		while(!m_pComm->SendChar(m_command[i]));
		Sleep(1);		
	}
/*	
	DWORD dwBytesToWrite=8,dwBytesWritten=0;
	m_pComm->WriteBlock((LPSTR)m_command,dwBytesToWrite,&dwBytesWritten);
	while(dwBytesWritten<dwBytesToWrite);
	Sleep(1);
*/
//	TRACE("\nsend end!");
	
	int readSize=0,size;
	
	count=MAX_SERIAL_RETRY_COUNT;
	while(count--)
	{
		size=m_pComm->ReadCommBlock(MOBILE_DATA_SIZE);
		memcpy(tempBuffer+readSize,m_abIn,size);
		readSize+=size;
		if(readSize>=MOBILE_DATA_SIZE) break;
	}

//	TRACE("\nreadSize %d count %d",readSize,count);

	// if error occurs at receiving 
	if(!count || (tempBuffer[0]!='Y' || tempBuffer[1]!='h'))
	{
		// clear receive buffer!
		while(m_pComm->ReadCommBlock(MAX_BUFFER));
		//AfxMessageBox("encoder error!");
		return FALSE;
	}


//	for(i=0;i<9;i++)
//		TRACE("\tGet %c",tempBuffer[i]);

	WORD temp;
	temp=tempBuffer[4];
	temp<<=8;
	temp|=tempBuffer[5];

	encoder1=(_int16)temp;

	temp=tempBuffer[6];
	temp<<=8;
	temp|=tempBuffer[7];

	encoder2=(_int16)temp;

	TRACE("\nGetEncoder %d %d",encoder1,encoder2);
	printText(50,140,"GetEncoder %d %d       ",encoder1,encoder2);
#endif
	return TRUE;
}

BOOL CHRView::ReadSensor()
{
#ifdef SENSOR

	m_command[0]=170;
	m_command[1]='Y';
	m_command[2]='h';
	m_command[3]='I';
	m_command[4]='R';
	m_command[5]=0x00;
	m_command[6]='S';

	m_pComm=&m_comm1;


	DWORD dwBytesToWrite=7,dwBytesWritten=0;

	m_pComm->WriteBlock((LPSTR)m_command,dwBytesToWrite,&dwBytesWritten);
	while(dwBytesWritten<dwBytesToWrite);

	int readSize=0,size;
	count=MAX_SERIAL_RETRY_COUNT;
	while(count--)
	{
		size=m_pComm->ReadCommBlock(SENSOR_DATA_SIZE);
		memcpy(tempBuffer+readSize,m_abIn,size);
		readSize+=size;
		if(readSize>=SENSOR_DATA_SIZE) break;
	}
	
	TRACE("\nreadSize %d count %d",readSize,count);

	// if error occurs at receiving 
	if(!count || (tempBuffer[0]!='Y' || tempBuffer[1]!='h'))
	{
		while(m_pComm->ReadCommBlock(MAX_BUFFER));
		//AfxMessageBox("sensor error!");
		return FALSE;
	}

	infra=tempBuffer[4];
	infra<<=8;
	infra|=tempBuffer[5];

	for(i=0;i<MAX_SONAR_CHANNEL;i++)
		ultra[i]=tempBuffer[6+i];

	TRACE("\nGetInfra [%x]",infra);

	printText(50,200,"[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d]",
		ultra[0],ultra[1],ultra[2],ultra[3],ultra[4],ultra[5],ultra[6],ultra[7]
		,ultra[8],ultra[9],ultra[10],ultra[11],ultra[12],ultra[13],ultra[14],ultra[15]);
	
	printText(50,240,"GetInfra [%x]",infra);

	
#endif
	return TRUE;
}

void CHRView::PredictMobilePose(Posture *robot, Speed *speed)
{
	int v1t, v2t;
	double	qt;
/*
	if(!ReadMobileEncoder())
	{
		printText(50,110,"ERROR : ReadMobileEncoder");
		TRACE("\nERROR : ReadMobileEncoder");
		return;
	}
*/
	v1t = encoder1;
	v2t = encoder2;

	qt  = (Rp2cm(v1t) - Lp2cm(v2t)) / ROBOT_BASE;
	speed->w = qt;
	speed->v = (Rp2cm(v1t) + Lp2cm(v2t)) / 2;
	
	if (ABS(qt) < 0.005)
	    robot->y += speed->v * (double)sin(robot->q),
	    robot->x += speed->v * (double)cos(robot->q);
	else
	    robot->y += speed->v * ((double)(cos(robot->q) - cos(robot->q + qt))) / qt,
	    robot->x += speed->v * ((double)(sin(robot->q + qt) - sin(robot->q))) / qt;
	
	robot->q += qt;
	ADJUST_360(robot->q);

	TRACE("\nqt = %f, w = %f, v = %f, x = %f, y = %f, q = %f",qt,speed->w,speed->v,robot->x,robot->y,robot->q);
	
	printText(50,160,"x= %f, y= %f, q= %f",robot->x,robot->y,robot->q);
}

// yh new

int CHRView::SafetyCount(BYTE want_action)
{
/*	
	Infra Sensor Data                      +--맨뒤 센서 
		xxxx | 11 10 9 8 | 7 6 5 4 | 3 2 1 0

	Front 
						1	1 1 1 1  
	LEFT
									 1 1 1 
	RIGHT
				 1 1 1					
*/
	int count=0;

	if(want_action==FRONT)
	{
		if(infra&0x0010)
			count+=1;
		if(infra&0x0020)
			count+=1;
		if(infra&0x0040)
			count+=1;
		if(infra&0x0080)
			count+=1;
		if(infra&0x0100)
			count+=1;

	}
	else if(want_action==LEFT)
	{
		if(infra&0x0002)
			count+=1;
		if(infra&0x0004)
			count+=1;
		if(infra&0x0008)
			count+=1;
		if(infra&0x0010)
			count+=1;

	}
	else if(want_action==RIGHT)
	{
		if(infra&0x0100)
			count+=1;
		if(infra&0x0200)
			count+=1;
		if(infra&0x0400)
			count+=1;
		if(infra&0x0800)
			count+=1;
	}
	return count;	
}

BOOL CHRView::FindObstacle(BYTE want_action)
{
/*	
	want_action: FRONT, LEFT, RIGHT
	원하는 방향으로 갈수 있으면, 가고 true리턴,
	갈수 없으면, false리턴
  
	
	Infra Sensor Data                      +--맨뒤 센서 
		xxxx | 11 10 9 8 | 7 6 5 4 | 3 2 1 0

	Front 
						1	1 1 1 1  
	LEFT
									 1 1 1 
	RIGHT
				 1 1 1					

	UltraSonic Sensor Data
		40~185 (0=Not sensed,Too far!)
		max range : 30~250cm: 1m=74, 2.5m=185  Value * 0.74= cm Value 
*/
#define ALL_FILTER		0x0ffe

#define FRONT_FILTER	0x01f0
#define LEFT_FILTER		0x000e
#define RIGHT_FILTER	0x0e00
#define BACK_FILTER		0x0001

	if(cleanMode)infra=infra|0x0040;

	switch(want_action)
	{
		case ALL:
			if((infra&ALL_FILTER)==ALL_FILTER)
				return TRUE;
			break;

		case FRONT:
			if((infra&FRONT_FILTER)==FRONT_FILTER)
				return TRUE;
			break;
		case LEFT:
			if((infra&LEFT_FILTER)==LEFT_FILTER)
				return TRUE;
			break;
		case RIGHT:
			if((infra&RIGHT_FILTER)==RIGHT_FILTER)
				return TRUE;
			break;
		case BACK:
			if((infra&BACK_FILTER)==BACK_FILTER)
				return TRUE;
			break;
		default:
				return TRUE;
			break;
	}

	return FALSE;
}

void CHRView::InitMobileParameters()
{
	RfP_CM_L = PPR_WHEEL / (2 * F_PI * WHEEL_L);
	RfCM_P_L = 1 / RfP_CM_L;
	
	RfP_CM_R = PPR_WHEEL / (2 * F_PI * WHEEL_R);
	RfCM_P_R = 1 / RfP_CM_R;

	JtRobot.x = 0;
	JtRobot.y = 0;
	JtRobot.q = 0;

	JtSpeed.v1 = 0;
	JtSpeed.v2 = 0;
	JtSpeed.v  = 0;
	JtSpeed.w  = 0;
}	

double CHRView::calAngle(double x1,double y1,double x2,double y2)
{
	double zx,zy,fAngle;

	zx=x2-x1;
	zy=y2-y1;

	fAngle=atan2(zy,zx);

	while(fAngle < -F_PI)
		fAngle += 2 * F_PI;
	while(fAngle >= F_PI)
		fAngle -= 2 * F_PI;
	return fAngle;

//	return toBound2PI(fAngle);	
}

double CHRView::toBound2PI(double fAngle)
{
	while(fAngle < -F_PI)
		fAngle += 2 * F_PI;
	while(fAngle >= F_PI)
		fAngle -= 2 * F_PI;
	return fAngle;
	/*
	while(1) {

		if (fAngle < 0) fAngle += F_PI * 2;
		else if (fAngle >= F_PI*2) fAngle -= F_PI * 2;
		else break;

	}
	return fAngle;
	*/
}

double CHRView::GetAngleFromObjectPos(Posture  Object) 
{
	double toObjAngle;
	// calculate relative angle between angle of robot base, angle of object pos
	toObjAngle=calAngle(JtRobot.x, JtRobot.y, Object.x, Object.y)-JtRobot.q;

	while(toObjAngle < -F_PI)
		toObjAngle += 2 * F_PI;
	while(toObjAngle >= F_PI)
		toObjAngle -= 2 * F_PI;

	//TRACE("\n objAngle =%f",toObjAngle);		

	return toObjAngle;

//	return toBound2PI(toObjAngle);
}

double toObjAngle;
double toWantAngle;

#define GOAL_DISTANCE	30*30

void CHRView::Navigation() 
{
	BYTE want_action=STOP;

	if ((pow((JtRobot.x - JtObject.x), 2) + pow((JtRobot.y - JtObject.y), 2)) < GOAL_DISTANCE)
	{
		// rotate to wanted dir->q!
		toWantAngle=toBound2PI(JtObject.q-JtRobot.q);
		if( (toWantAngle <0 && toWantAngle >= -F_PI/60) || (toWantAngle >=0 && toWantAngle <= F_PI/60) )
		{
			TRACE("\n goal");		
			want_action=STOP;
			dir=FRONT;
			GoMobile(dir);
			bNavigation=0;
			
			bIsTable=TRUE;

			SetTimer(6, 2000, NULL);	
			return;
		}
		else
		{
			TRACE("\n rotate to want_dir");		
			if(JtRobot.q >= 0)
				want_action=RIGHT;  
			else
				want_action=LEFT;
		}
	}
	else
	{
		if( (toObjAngle <0 && toObjAngle >= -F_PI/8) || (toObjAngle >=0 && toObjAngle <= F_PI/8) )
			want_action=FRONT;
		else
		{
			if(toObjAngle >= 0)
				want_action=LEFT;
			else
				want_action=RIGHT;

		}
	}
		
/*
	// 원하는 방향으로 장애물이 없으면
	if(FindObstacle(want_action))
	{
		dir=want_action;
		TRACE("\n no obstacle");		
	}
*/
	if(FindObstacle(ALL))
	{
		dir=want_action;
		TRACE("\n no obstacle");		
	}
	else
	{
		//go front
		if(FindObstacle(FRONT))
		{
			dir=FRONT;
		}
		// 좌우 회전 모두 가능하면, 위험도가 낮은 턴으로 
		else if(FindObstacle(LEFT)&&FindObstacle(RIGHT))
		{
			if(SafetyCount(LEFT)>SafetyCount(RIGHT))
				dir=LEFT;
			else 
				dir=RIGHT;
		}
		else if(FindObstacle(LEFT))
		{
			dir=LEFT;
		}
		else if(FindObstacle(RIGHT))
		{
			dir=RIGHT;
		}
		else
		{
			dir=STOP;
		}
		TRACE("\n avoid 0");				
	}

	TRACE("\t dir = %c",dir);		
/*	
	//go front
	if(FindObstacle(FRONT))
	{
		dir=FRONT;
	}
	else if(FindObstacle(LEFT))
	{
		dir=LEFT;
	}
	else if(FindObstacle(RIGHT))
	{
		dir=RIGHT;
	}
	else
	{
		dir=STOP;
	}
*/
}

BYTE headDir=STOP;
void CHRView::headLeftScan()
{
	headDir=LEFT;
	headPosX=-3000;	
	SendHead();
}

void CHRView::headRightScan()
{
	headDir=RIGHT;
	headPosX=3000;	
	SendHead();
}

void CHRView::headStop()
{
	HeadStop();
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

		headPosX=0;
		headPosY=-4200;
		//headPosY=0;
		SendHead();

		headDir=STOP;
		TRACE("\nINIT VIS");
	}

	memcpy(&Vstereo[0],lpVisionData,sizeof(VISION_DATA));

	ReadHead();

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
				headStop();
				findTarget=1;

				randnum = rand()%5;

				::SendMessage(HWND_BROADCAST,MsgEmotion,5099,3000);

// 얼굴 trancking 시 대사는 eFis 얼굴검출 프로그램으로 이전				
/*
				switch(randnum){
				case 0:
					SendTTS("앗 찾앗다!");
//					Speak(2);
					break;
				case 1:
					SendTTS("거기 있군요");
//					Speak(2);
					break;
				case 2:
					SendTTS("찾아냈어요");
//					Speak(2);
					break;
				default:
					break;
				}
*/

			}
			
			//SendTTS("빨간공, 찾았습니다");
			//visionCount++;
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

	if(headPosX<-1500)
		headPosX=-1500;

	if(headPosX>1500)
		headPosX=1500;


	if(headPosY<-4600)
	headPosY=-4600;

	if(headPosY>-2000)
		headPosY=-2000;

	SendHead();

	visionCount++;
}

#define WANDERING	0
#define FIND_TARGET	1
#define GOTO_TARGET	2

int uMask[30]={0,};
int uMask2[30]={0,};

/*
#define ThreshDistFar 80
#define ThreshDist 60
#define ThreshDistNear 40
*/
// new 초음파 센서
#define ThreshDistFar 90
#define ThreshDist 70
#define ThreshDistNear 60

BOOL CHRView::IsSafe(BYTE want_action) 
{
	int i;

#ifdef NEW_SENSOR
	// new 초음파 보정
	ultra[5]=(ultra[4]+ultra[6])/2;
#endif

	for(i=0;i<MAX_SONAR_CHANNEL;i++)
	{
		if(ultra[i]>ThreshDist||ultra[i]==0)
			uMask[i]=1;
		else
			uMask[i]=0;
	}

	for(i=0;i<MAX_SONAR_CHANNEL;i++)
	{
		if(ultra[i]>ThreshDistNear||ultra[i]==0)
			uMask2[i]=1;
		else
			uMask2[i]=0;
	}
	
	switch(want_action)
	{
		case ALL:
			if((uMask[2] && uMask[3] && uMask[4] && uMask[5] && uMask[6] && uMask[7] && uMask[8])
			||want_action==STOP)
				return TRUE;
			break;
		case FRONT:
			if(uMask[4] && uMask[5] && uMask[6])
				return TRUE;
			break;
		case LEFT:
			if(uMask2[1] && uMask2[2] && uMask2[3])
				return TRUE;
			break;
		case RIGHT:
			if(uMask2[7] && uMask2[8] && uMask2[9])
			return TRUE;
			break;
		case BACK:
			if(uMask2[12] && uMask2[13] && uMask2[14])
			return TRUE;
			break;
		default:
			break;
	}

	return FALSE;
}


void CHRView::Wally() 
{
	static BYTE want_action=STOP;

	int threshDistance=ThreshDist;

	if(cleanMode){
		threshDistance=ThreshDistFar;

		want_action = FRONT;

	}
	else if(playAloneMode){

		threshDistance=ThreshDistFar;

		//일단 정지
		randnum = rand() % (15 * 5);
		switch(randnum){
		case 0:
			want_action=STOP;
			break;
		case 1:
			want_action=FRONT;
			break;
		case 2:
			want_action=LEFT;
			break;
		case 3:
			want_action=RIGHT;
			break;
		case 4:
			want_action=FRONT;
			break;
		default:
			break;
		}

	}
	else {

		threshDistance=ThreshDist;

		//만약 공이 있으면,
		if(!Vstereo[0].empty)
		{
			//목이 앞을 향하고 있으면 전진 
			if(headPosX<600 && headPosX>-600)
				want_action=FRONT;
			//목이 왼쪽을 향하고 있으면 왼쪽 회전
			else if(headPosX<-600)
				want_action=LEFT;
			else if(headPosX>600)
			//목이 오른쪽을 향하고 있으면 오른쪽 회전
				want_action=RIGHT;
		}
		else
		{
			//일단 정지
			randnum = rand() % (15 * 5);
			switch(randnum){
			case 0:
			case 1:
				want_action=FRONT;
				break;
			case 2:
				want_action=LEFT;
				break;
			case 3:
				want_action=RIGHT;
				break;
			case 4:
				want_action=STOP;
				break;
			default:
				break;
			}

		}
	}

	for(i=0;i<MAX_SONAR_CHANNEL;i++)
	{
		if(ultra[i]>threshDistance||ultra[i]==0)
			uMask[i]=1;
		else
			uMask[i]=0;
	}

	//초음파 센서에 의한 회피
	BYTE uDir=STOP;

	if((uMask[2] && uMask[3] && uMask[4] && uMask[5] && uMask[6] && uMask[7] && uMask[8])
	||want_action==STOP)
	{

		uDir=want_action;
		printText(50,250,"no obstacle");	

	}
	else
	{
		//go front
		if(uMask[4] && uMask[5] && uMask[6])
		{
			uDir=FRONT;
			printText(50,250,"front!");	
		}
		// 좌우 회전 모두 가능하면, 위험도가 낮은 턴으로 
		else if(uMask[2] && uMask[3] && uMask[7] && uMask[8])
		{
			if((ultra[2]+ultra[3]) >= (ultra[7]+ultra[8]))
			{
				printText(50,250,"left!");	
				uDir=LEFT;
			}
			else 
			{
				printText(50,250,"right!");	
				uDir=RIGHT;
			}
		}
		else if(uMask[2] && uMask[3])
		{
			printText(50,250,"left!");	
			uDir=LEFT;
		}
		else if(uMask[7] && uMask[8])
		{
			printText(50,250,"right!");	
			uDir=RIGHT;
		}
		else
		{
			printText(50,250,"stop!");	
			uDir=STOP;
			//Modified !
			uDir=LEFT;

			
		}
	}

	//포토 스위치에 의한 회피
	if(FindObstacle(ALL)||want_action==STOP)
	{
		dir=uDir;
		TRACE("\n no obstacle");		
	}
	else
	{
		//go front
		if(FindObstacle(FRONT))
		{
			dir=FRONT;
		}
		// 좌우 회전 모두 가능하면, 위험도가 낮은 턴으로 
		else if(FindObstacle(LEFT)&&FindObstacle(RIGHT))
		{
			if(SafetyCount(LEFT)>=SafetyCount(RIGHT))
				dir=LEFT;
			else 
				dir=RIGHT;
		}
		else if(FindObstacle(LEFT))
		{
			dir=LEFT;
		}
		else if(FindObstacle(RIGHT))
		{
			dir=RIGHT;
		}
		else
		{
			dir=STOP;
		}
		TRACE("\n avoid 0");				
	}

	TRACE("\t dir = %c",dir);		

}

void CHRView::OnTest4()
{
/*
	if(bIsTable)
		return;

	bNavigation=0;
	dir=STOP;
	GoMobile(dir);

	RightArm[0]=-12000;
	RightArm[1]=20000;
	RightArm[2]=0;
	RightArm[3]=-60000;
	RightArm[4]=30000;
				
	LeftArm[0]=5000;
	LeftArm[1]=-10000;
	LeftArm[2]=-5000;
	LeftArm[3]=25000;
	LeftArm[4]=0;
	LeftArm[5]=0;
	
	SendRightArm();
	SendLeftArm();

	// 팔이 자세를 취하기 까지 기다림
	Sleep(7000);

	SendTTS(",안녕하세요?, 저는 휴먼 로봇 입니다,,만나서,반갑습니다");

	shakeCount=0;

	mouthCount=0;
	maxMouthCount=10;

	// 입벌렸다 닫기
	SetTimer(2, 700, NULL);
	// 팔흔들기 
	SetTimer(3, 1000, NULL);
	// 종료
	SetTimer(10, 7000, NULL);
*/
/*

	SendTTS("테이블로,가");
	Speak(2);
	bNavigation=TRUE;
*/
	//AfxMessageBox("test4");
	ReadHead();
	return;


}

void CHRView::OnTest()
{
//	OnIdleDisable();
//	ReadHead();

/*
  //눈썹 및 입 test
	if(pwm[0]==0x6)
	{
		pwm[0]=0x09;
		pwm[1]=0x13;
		pwm[4]=0x13;
	}
	else
	{
		pwm[0]=0x06;
		pwm[1]=0x15;
		pwm[4]=0x15;

	}
	SendFace();

	//SendTTS("빨간공, 찾았습니다");
*/
	randnum = rand()%4;
	::SendMessage(HWND_BROADCAST,MsgEmotion,5070,3000);

	switch(randnum){
	case 0:
		SendTTS("멋진 모습");
		Speak(2);
		break;
	case 1:
		SendTTS("오예");
		Speak(2);
		break;
	case 2:
		SendTTS("여길봐요");
		Speak(2);
		break;
	case 3:
		SendTTS("따라해 보세요");
		Speak(2);
		break;

	}

	LeftArm[1] = -25000;
	RightArm[1] = 25000;

	SendArm();
	Sleep(2000);

	headPosX=3500;
	headPosY=-4500;
	SendHead();
	Sleep(1000);
	//청소 준비자세 
	RightArm[0]=-55000;
	RightArm[1]=30000;
	RightArm[2]=10000;
	RightArm[3]=-20000;
	RightArm[4]=0;
				
	LeftArm[0]=20000;
	LeftArm[1]=-5000;
	LeftArm[2]=-25000;
	LeftArm[3]=50000;
	LeftArm[4]=10000;
	LeftArm[5]=0;	

	SendRightArm();
	SendLeftArm();
	Sleep(4000);
	//손벌려서 끝 약간 구부리기 
	RightHand[0]=0x16;
	RightHand[1]=0x14;
	RightHand[2]=0x11;
	RightHand[3]=0x15;
	RightHand[4]=0xc;
	RightHand[5]=0x15;

	SendRightHand();
	
	LeftHand[0]=0x10;
	LeftHand[1]=0x10;
	LeftHand[2]=0x14;
	LeftHand[3]=0x11;
	LeftHand[4]=0x1b;
	LeftHand[5]=0x11;
	SendLeftHand();

	::SendMessage(HWND_BROADCAST,MsgEmotion,8080,3000);
	randnum = rand()%2;
	switch(randnum){
	case 0:
		SendTTS("짜잔");
		Speak(1);
		break;
	case 1:
		SendTTS("멋지죠?");
		Speak(2);
		break;
	}
}

void CHRView::OnTest2()
{
//	OnIdleDisable();
//	ReadSensor();
	headPosX=3500;
	headPosY=-1500;
	SendHead();
	Sleep(1000);
	//청소 준비자세 
	RightArm[0]=-10000;
	RightArm[1]=572;
	RightArm[2]=14060;
	RightArm[3]=-30000;
	RightArm[4]=14620;
				
	LeftArm[0]=-510;
	LeftArm[1]=-2361;
	LeftArm[2]=-4488;
	LeftArm[3]=56160;
	LeftArm[4]=42673;
	LeftArm[5]=-5396;
	
	SendRightArm();
	SendLeftArm();
	Sleep(4000);

	//손벌려서 끝 약간 구부리기 

	OnHandReset();
/*
	bNavigation^=1;

	if(!bNavigation)
	{
		dir=STOP;
		GoMobile(dir);
	}
*/
/*	//서있는 멋진자세
	headPosX=0;
	headPosY=-4000;
	SendHead();

	RightArm[0]=-5000;
	RightArm[1]=6000;
	RightArm[2]=2000;
	RightArm[3]=-10000;
	RightArm[4]=0;
				
	LeftArm[0]=5000;
	LeftArm[1]=-6000;
	LeftArm[2]=-2000;
	LeftArm[3]=10000;
	LeftArm[4]=0;
	LeftArm[5]=0;
	
	SendRightArm();
	SendLeftArm();
*/
/*
	headPosX=0;
	headPosY=-1000;
	SendHead();

	Ready();
*/
}

void CHRView::OnSensor()
{
	static BYTE flag=0;
	flag^=1;
	SensorEnable(flag);
}

void CHRView::OnWally()
{
	bWally=TRUE;
	readMode = true;
	*lpVisionCommand=6;			
	Sleep(500);

//	*lpVisionCommand=2;
	*lpVisionCommand=7;

	visionCount=0;
	SensorEnable(1);

	printText(50,20,"Wally start!   ");
}

void CHRView::OnHandReset()
{
//	OnIdleDisable();
/*
	      --+--
 		  1   0
  --+--	
   3 2    5	  4
		  --+--
			
	[right hand ]
	open ~ close 
	0 => 7,B,10 
	1 => 9,D,10
	
	2 => 17,11,E
	3 => 15,13,10

	4 => 17,13,10
	5 => 17,12,F
	
	[left hand ]
	open ~ close 

	0 => 17,13,F 
	1 => 15,10,D
	
	2 => 9,C,F
	3 => 9,?,A

	4 => 7,C,F
	5 => 7,C,F
*/
/*
	//손바로 
	RightHand[0]=0x07;
	RightHand[1]=0x09;
	RightHand[2]=0x17;
	RightHand[3]=0x10;
	RightHand[4]=0x17;
	RightHand[5]=0x17;

	SendRightHand();
	
	LeftHand[0]=0x17;
	LeftHand[1]=0x15;
	LeftHand[2]=0x09;
	LeftHand[3]=0x0A;
	LeftHand[4]=0x07;
	LeftHand[5]=0x07;
	SendLeftHand();
*/
/*
	//엄지손 펴기 
	RightHand[0]=0x07;
	RightHand[1]=0x09;
	RightHand[2]=0x17;
	RightHand[3]=0x15;
	RightHand[4]=0x17;
	RightHand[5]=0x17;

	SendRightHand();
	
	LeftHand[0]=0x17;
	LeftHand[1]=0x15;
	LeftHand[2]=0x09;
	LeftHand[3]=0x09;
	LeftHand[4]=0x07;
	LeftHand[5]=0x07;
	SendLeftHand();
*/
	//손벌려서 끝 약간 구부리기

	// 손 펴기
	RightHand[0]=0x11;
	RightHand[1]=0x17;
	RightHand[2]=0x11;
	RightHand[3]=0x15;
	RightHand[4]=0x15;
	RightHand[5]=0x11;

	SendRightHand();
	
	LeftHand[0]=0x15;
	LeftHand[1]=0xd;
	LeftHand[2]=0x14;
	LeftHand[3]=0x11;
	LeftHand[4]=0x12;
	LeftHand[5]=0x15;
	SendLeftHand();
/*
	//손닫기 
	RightHand[0]=0x0B;
	RightHand[1]=0x10;
	RightHand[2]=0x11;
	RightHand[3]=0x13;
	RightHand[4]=0x13;
	RightHand[5]=0xF;

	SendRightHand();
	
	LeftHand[0]=0x17;
	LeftHand[1]=0x15;
	LeftHand[2]=0x09;
	LeftHand[3]=0x09;
	LeftHand[4]=0x07;
	LeftHand[5]=0x07;
	SendLeftHand();
*/
}

void CHRView::OnUp()
{
//	OnIdleDisable();
	if(bCommandMode==MOBILE_MODE)
	{
		//GoMobile(FRONT);
		dir=FRONT;
		bGo=TRUE;
	}
	else
	{
		headPosY-=500;
		SendHead();
	}
}

void CHRView::OnDown()
{
//	OnIdleDisable();
	if(bCommandMode==MOBILE_MODE)
	{
		//GoMobile(BACK);
		dir=BACK;
		bGo=TRUE;
	}
	else
	{
		headPosY+=500;
		if(headPosY>0) headPosY=0;
		SendHead();
	}
}

void CHRView::OnLeft()
{
//	OnIdleDisable();
	if(bCommandMode==MOBILE_MODE)
	{
		//GoMobile(LEFT);
		dir=LEFT;
		bGo=TRUE;
	}
	else
	{
		headPosX-=500;
		SendHead();
	}
}

void CHRView::OnRight()
{
//	OnIdleDisable();
	if(bCommandMode==MOBILE_MODE)
	{
		//GoMobile(RIGHT);
		dir=RIGHT;
		bGo=TRUE;
	}
	else
	{
		headPosX+=500;
		SendHead();
	}
}

void CHRView::OnStop()
{
//	OnIdleDisable();
	if(bCommandMode==MOBILE_MODE)
	{
		//GoMobile(STOP);
		dir=STOP;
		bGo=TRUE;
	}
	else
	{
		headPosX=0;
		headPosY=-4000;
		SendHead();
	}
}

void CHRView::InitPos()
{
	*lpVisionCommand=0;

	OnIdleDisable();

	if(readMode)*lpVisionCommand = 6;

	readMode = false;

	bSeeHere = false;
	bNavigation=FALSE;
	bWally=FALSE;
	
	SensorEnable(0);
	dir=STOP;
	GoMobile(dir);

	if(cleanMode){
		cleanMode = false;
		OnHandReset();
		KillTimer(55);
		Sleep(5000);
	}
	else if(bWally || playAloneMode){
		playAloneMode = false;
		KillTimer(55);
	}


	pwm[4]=0x15;
	SendFace();
	mouthOpen = false;

	OnHandInit();
	OnHandstp();

	headPosX=0;
	headPosY=-4000;
	SendHead();

	RightArm[0]=0;
	RightArm[1]=25000;
	RightArm[2]=0;
	RightArm[3]=0;
	RightArm[4]=0;

	LeftArm[0]=0;
	LeftArm[1]=-25000;
	LeftArm[2]=0;
	LeftArm[3]=0;
	LeftArm[4]=0;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();

	Sleep(2000);

	
	RightArm[0]=0;
	RightArm[1]=0;
	RightArm[2]=0;
	RightArm[3]=0;
	RightArm[4]=0;

	LeftArm[0]=0;
	LeftArm[1]=0;
	LeftArm[2]=0;
	LeftArm[3]=0;
	LeftArm[4]=0;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();

}

void CHRView::OnAllStop()
{
//	OnIdleDisable();
	
	InitPos();
	headPosX=0;
	headPosY=0;
	SendHead();

}


void CHRView::OnJoy()
{
	bJoy^=1;
}

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

void CHRView::OnDemo()
{
//	OnIdleDisable();
/*
	headMotionCount=0;
	armMotionCount=0;

	RightArm[0]=-12000;
	RightArm[1]=20000;
	RightArm[2]=0;
	RightArm[3]=-60000;
	RightArm[4]=30000;
				
	LeftArm[0]=5000;
	LeftArm[1]=-10000;
	LeftArm[2]=-5000;
	LeftArm[3]=25000;
	LeftArm[4]=0;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();

	// 앞으로 가기~~~
	SetTimer(3, 2000, NULL);	
*/
	pFrame->SetWindowPos(&wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
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

/////////////////////////
/// 호연이 팔부분 
#define PI 3.141592653589793238462643383

bool CHRView::MoveArm(int whichArm){

	CHRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CString message;

	double distance,d2;
	double theta1,theta2,theta3,theta4,theta5;

	Matrix4 M;
	Matrix4 Mr;

	Posn3d left;
	Posn3d right;

	if(whichArm == 0 || whichArm == 2){
		Posn3d leftPoint;
		double leftArm1;
		double leftArm2;
		leftPoint.Set(pDoc->p[0]+pDoc->p[2],pDoc->p[1],0);
		leftArm1 = pDoc->p[3];
		leftArm2 = pDoc->p[4];

		distance = sqrt(pow(leftPoint[0]-pDoc->object[0],2)+pow(leftPoint[1]-pDoc->object[1],2)+pow(leftPoint[2]-pDoc->object[2],2));
		if(distance>(leftArm1+leftArm2))return false;

		theta1 = (PI - acos( (leftArm1*leftArm1 + leftArm2*leftArm2 - distance*distance) / (2 * leftArm1 * leftArm2) )) * 180 / PI;
		if( (pDoc->object[1] - leftPoint[1]) == 0 ) theta2 = 90;
		else theta2 = atan( (pDoc->object[2] - leftPoint[2]) / (leftPoint[1] - pDoc->object[1]) ) * 180 / PI;
		if( ( pow(pDoc->object[1]-leftPoint[1],2) + pow(pDoc->object[2]-leftPoint[2],2) ) == 0)
			if( (leftPoint[0] - pDoc->object[0]) > 0 ) theta3 = 90;
			else theta3 = -90;
		else theta3 = atan( (leftPoint[0] - pDoc->object[0]) / sqrt( pow(pDoc->object[1]-leftPoint[1],2) + pow(pDoc->object[2]-leftPoint[2],2) ) ) * 180 / PI;
		if( distance == 0) theta4 = 90;
		else theta4 = acos( (leftArm1*leftArm1 - leftArm2*leftArm2 + distance*distance) / (2 * leftArm1 * distance) ) * 180 / PI;

		pDoc->r[3] = (float)theta1;
		pDoc->r[0] = (float)theta2;
		pDoc->r[1] = (float)(theta3 - theta4);
		pDoc->R[0].FromAngle(-pDoc->r[0],1,0,0);
		pDoc->R[1].FromAngle(-pDoc->r[1],0,0,1);
		pDoc->R[3].FromAngle(-pDoc->r[3],1,0,0);

		M = pDoc->T[0] * pDoc->R[0] * pDoc->T[1] * pDoc->R[1] 
			* pDoc->T[2] * pDoc->R[3] * pDoc->T[3];
		leftPoint.Init();
		leftPoint = M * leftPoint;

		d2 = sqrt(pow(leftPoint[0]-pDoc->object[0],2)+pow(leftPoint[1]-pDoc->object[1],2)+pow(leftPoint[2]-pDoc->object[2],2));
		if(theta4 == 0 || distance == 0) theta5 = 0;
		else theta5 = acos( ( pow(distance * sin(theta4*PI/180),2) + pow(distance * sin(theta4*PI/180),2) - d2*d2 ) / (2 * distance * sin(theta4*PI/180) * distance * sin(theta4*PI/180) ) ) * 180 / PI;

		pDoc->r[2] = (float)theta5;
		pDoc->R[2].FromAngle(-pDoc->r[2],0,1,0);

		Posn3d v;
		REAL ag;
		REAL r_ag;
		REAL lr_ag;
		REAL tx;
		v.Set(-5,0,0);
		for(int i=-18;i<18;i++){
			ag=i*10;
			pDoc->R[4].FromAngle(-ag,0,1,0);
			M = pDoc->T[0] * pDoc->R[0] * pDoc->T[1] * pDoc->R[1] 
				* pDoc->R[2] * pDoc->T[2] * pDoc->R[3] * pDoc->T[3] * pDoc->R[4];
			v = M * v;
			if(i==-18){
				r_ag = ag;
				tx = v[0];
			}
			else if(v[0]<tx){
				r_ag = ag;
				tx = v[0];
			}
			v.Set(-5,0,0);
		}
		lr_ag=r_ag;
		for(i=-9;i<10;i++){
			ag=r_ag+i;
			pDoc->R[4].FromAngle(-ag,0,1,0);
			M = pDoc->T[0] * pDoc->R[0] * pDoc->T[1] * pDoc->R[1] 
				* pDoc->R[2] * pDoc->T[2] * pDoc->R[3] * pDoc->T[3] * pDoc->R[4];
			v = M * v;
			if(v[0]<tx){
				lr_ag = ag;
				tx = v[0];
			}
			v.Set(-5,0,0);
		}	

		pDoc->r[4] = lr_ag;
		pDoc->R[4].FromAngle(-lr_ag,0,1,0);
	}


	//============right arm ================
	if(whichArm == 1 || whichArm == 2){
		Posn3d rightPoint;
		double rightArm1;
		double rightArm2;
		rightPoint.Set(-(pDoc->p[0]+pDoc->p[2]),pDoc->p[1],0);
		rightArm1 = pDoc->p[3];
		rightArm2 = pDoc->p[4];

		distance = sqrt(pow(rightPoint[0]-pDoc->object[0],2)+pow(rightPoint[1]-pDoc->object[1],2)+pow(rightPoint[2]-pDoc->object[2],2));
		if(distance > (rightArm1+rightArm2) ) return false;

		theta1 = (PI - acos( (rightArm1*rightArm1 + rightArm2*rightArm2 - distance*distance) / (2 * rightArm1 * rightArm2) )) * 180 / PI;
		if( (pDoc->object[1] - rightPoint[1]) == 0 ) theta2 = 90;
		else theta2 = atan( (pDoc->object[2] - rightPoint[2]) / (rightPoint[1] - pDoc->object[1]) ) * 180 / PI;
		if( ( pow(pDoc->object[1]-rightPoint[1],2) + pow(pDoc->object[2]-rightPoint[2],2) ) == 0)
			if( (rightPoint[0] - pDoc->object[0]) > 0 ) theta3 = -90;
			else theta3 = 90;
		else theta3 = atan( (pDoc->object[0] - rightPoint[0]) / sqrt( pow(pDoc->object[1]-rightPoint[1],2) + pow(pDoc->object[2]-rightPoint[2],2) ) ) * 180 / PI;
		if( distance == 0) theta4 = 90;
		else theta4 = acos( (rightArm1*rightArm1 - rightArm2*rightArm2 + distance*distance) / (2 * rightArm1 * distance) ) * 180 / PI;

		pDoc->rr[3] = (float)theta1;
		pDoc->rr[0] = (float)theta2;
		pDoc->rr[1] = (float)(theta3 - theta4);
		pDoc->Rr[0].FromAngle(-pDoc->rr[0],1,0,0);
		pDoc->Rr[1].FromAngle(pDoc->rr[1],0,0,1);
		pDoc->Rr[3].FromAngle(-pDoc->rr[3],1,0,0);

		Mr = pDoc->Tr[0] * pDoc->Rr[0] * pDoc->Tr[1] * pDoc->Rr[1] 
			* pDoc->Tr[2] * pDoc->Rr[3] * pDoc->Tr[3];
		rightPoint.Init();
		rightPoint = Mr * rightPoint;

		d2 = sqrt(pow(rightPoint[0]-pDoc->object[0],2)+pow(rightPoint[1]-pDoc->object[1],2)+pow(rightPoint[2]-pDoc->object[2],2));
		if(theta4 == 0 || distance == 0) theta5 = 0;
		else theta5 = acos( ( pow(distance * sin(theta4*PI/180),2) + pow(distance * sin(theta4*PI/180),2) - d2*d2 ) / (2 * distance * sin(theta4*PI/180) * distance * sin(theta4*PI/180) ) ) * 180 / PI;

		pDoc->rr[2] = (float)theta5;
		pDoc->Rr[2].FromAngle(pDoc->rr[2],0,1,0);

		Posn3d v;
		REAL ag;
		REAL r_ag;
		REAL lr_ag;
		REAL tx;
		v.Set(5,0,0);
		for(int i=-18;i<18;i++){
			ag=i*10;
			pDoc->Rr[4].FromAngle(ag,0,1,0);
			Mr = pDoc->Tr[0] * pDoc->Rr[0] * pDoc->Tr[1] * pDoc->Rr[1] 
				* pDoc->Rr[2] * pDoc->Tr[2] * pDoc->Rr[3] * pDoc->Tr[3] * pDoc->Rr[4];
			v = Mr * v;
			if(i==-18){
				r_ag = ag;
				tx = v[0];
			}
			else if(v[0]>tx){
				r_ag = ag;
				tx = v[0];
			}
			v.Set(5,0,0);
		}
		lr_ag=r_ag;
		for(i=-9;i<10;i++){
			ag=r_ag+i;
			pDoc->Rr[4].FromAngle(ag,0,1,0);
			Mr = pDoc->Tr[0] * pDoc->Rr[0] * pDoc->Tr[1] * pDoc->Rr[1] 
				* pDoc->Rr[2] * pDoc->Tr[2] * pDoc->Rr[3] * pDoc->Tr[3] * pDoc->Rr[4];
			v = Mr * v;
			if(v[0]>tx){
				lr_ag = ag;
				tx = v[0];
			}
			v.Set(5,0,0);
		}	

		pDoc->rr[4] = lr_ag;
		pDoc->Rr[4].FromAngle(-lr_ag,0,1,0);

	}

	M = pDoc->T[0] * pDoc->R[0] * pDoc->T[1] * pDoc->R[1] 
		* pDoc->R[2] * pDoc->T[2] * pDoc->R[3] * pDoc->T[3] * pDoc->R[4];
	left = M * left;

	Mr = pDoc->Tr[0] * pDoc->Rr[0] * pDoc->Tr[1] * pDoc->Rr[1] 
		* pDoc->Rr[2] * pDoc->Tr[2] * pDoc->Rr[3] * pDoc->Tr[3] * pDoc->Rr[4];
	right = Mr * right;

	RightArm[0]=-(long)pDoc->rr[0]*50000/90;
	RightArm[1]=-(long)pDoc->rr[1]*50000/90;
	RightArm[2]=(long)pDoc->rr[2]*24120/90;
	RightArm[3]=-(long)pDoc->rr[3]*24120/90;
	RightArm[4]=(long)pDoc->rr[4]*50000/90;

	LeftArm[0]=(long)pDoc->r[0]*50000/90;
	LeftArm[1]=(long)pDoc->r[1]*50000/90;
	LeftArm[2]=-(long)pDoc->r[2]*24120/90;
	LeftArm[3]=(long)pDoc->r[3]*24120/90;
	LeftArm[4]=-(long)pDoc->r[4]*50000/90;

/*--------------------------*/
/* Arm Part					*/
/*--------------------------*/
/*	Right part				
  		1:   25000 ~ -45000
  		2:  -4700  ~ 49000
  		3:   50000 ~ -50000
  		4:  -60000 ~ 0
  		5:   85000 ~ -85000
 	Left part				
  		1:  -25000 ~ 45000
  		2:   4700  ~ -49000
  		3:  -50000 ~ 50000
  		4:   60000 ~ 0
  		5:  -85000 ~ 85000
  (6)waist:	-5500  ~ 5500
   (순서 : 뒤,안,안,위,안,왼이 먼저)
*/	

/*	if( (RightArm[0]>25000) || (RightArm[0]<-45000) ||
		(RightArm[1]>49000) || (RightArm[1]<-4700) ||
		(RightArm[2]>50000) || (RightArm[2]<-50000) ||
		(RightArm[3]>0) || (RightArm[3]<-60000) ||
		(RightArm[4]>85000) || (RightArm[4]<-85000) ||
		(LeftArm[0]>45000) || (LeftArm[0]<-25000) ||
		(LeftArm[1]>4700) || (LeftArm[1]<-49000) ||
		(LeftArm[2]>50000) || (LeftArm[2]<-50000) ||
		(LeftArm[3]>60000) || (LeftArm[3]<0) ||
		(LeftArm[4]>85000) || (LeftArm[4]<-85000) )
		return false;*/

//	message.Format("left (%f,%f,%f)   right (%f,%f,%f)",left[0],left[1],left[2],right[0],right[1],right[2]);
//	AfxMessageBox(message);


//	AfxMessageBox("complete 1!");

	return true;
}

void CHRView::ResetArm(){
	CHRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Ready();

	while(isMoveArm(1));

	pDoc->r[0] = 0;
	pDoc->r[1] = 0;
	pDoc->r[2] = 0;
	pDoc->r[3] = 0;
	pDoc->r[4] = 0;
	pDoc->rr[0] = 0;
	pDoc->rr[1] = 0;
	pDoc->rr[2] = 0;
	pDoc->rr[3] = 0;
	pDoc->rr[4] = 0;

	RightArm[2]=0;
	LeftArm[2]=0;
	LeftArm[5]=0;

	SendArm();

//	while(isMoveArm(2));
	Sleep(2000);

	RightArm[0]=0;
	RightArm[3]=0;
	RightArm[4]=0;
	LeftArm[0]=0;
	LeftArm[3]=0;
	LeftArm[4]=0;

	SendArm();

//	while(isMoveArm(0));
	Sleep(3000);

	RightArm[1]=0;
	LeftArm[1]=0;

	SendArm();

//	AfxMessageBox("complete 1!");
	bCatchBallReady=FALSE;
}

void CHRView::SendArm(){
	SendRightArm();
	SendLeftArm();
}

bool CHRView::isMoveArm(int i){
	ReadRightArm();
	ReadLeftArm();
	LONG left = R_LeftArm[i];
	LONG right = R_RightArm[i];

	ReadRightArm();
	ReadLeftArm();

	if( (left == R_LeftArm[i]) && (right == R_RightArm[i]) )
		return false;
	else
		return true;
}

void CHRView::Ready(){
	CHRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	headPosX=0;
	headPosY=0;
	SendHead();

	LeftArm[1] = -25000;
	RightArm[1] = 25000;
	LeftArm[5] = 0;

	SendArm();

	while(isMoveArm(1));
	
	pDoc->object[0] = 30;
	pDoc->object[1] = 12;
	pDoc->object[2] = 50;

	MoveArm(0);

	pDoc->object[0] = -30;
	pDoc->object[1] = 12;
	pDoc->object[2] = 50;

	MoveArm(1);
	SendArm();

	while(isMoveArm(3));

	pDoc->object[0] = 20;
	pDoc->object[1] = 12;
	pDoc->object[2] = 50;

	MoveArm(0);

	pDoc->object[0] = -20;
	pDoc->object[1] = 12;
	pDoc->object[2] = 50;

	MoveArm(1);
	SendArm();
	
	bCatchBallReady=TRUE;
}

void CHRView::Lift() 
{
	// TODO: Add your command handler code here
	ReadRightArm();
	ReadLeftArm();
	RightArm[0] = R_RightArm[0] - 5000;
	LeftArm[0] = R_LeftArm[0] + 5000;
	SendArm();
}

void CHRView::Drop() 
{
	// TODO: Add your command handler code here
	ReadRightArm();
	ReadLeftArm();
	RightArm[0] = R_RightArm[0] + 5000;
	LeftArm[0] = R_LeftArm[0] - 5000;
	SendArm();
	
}

#include "Grab.h"
void CHRView::OnArmGrab()
{
//	OnIdleDisable();
	CHRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	Grab dlg;

	dlg.m_objx = 0;
	dlg.m_objy = 4;
	dlg.m_objz = 45;

	if(dlg.DoModal()==IDOK){

		pDoc->object[0] = 20;
		pDoc->object[1] = dlg.m_objy;
		pDoc->object[2] = dlg.m_objz;

		bool b1=MoveArm(0);

		pDoc->object[0] = -20;

		bool b2=MoveArm(1);
		
		if(b1&&b2){
			SendArm();
			while(isMoveArm(0));

			pDoc->object[0] = 0;

			if(MoveArm(2)){
				SendArm();
				while(isMoveArm(1));
				Lift();
			}
		}
	}  // if

	delete dlg;
	
}

void CHRView::OnArmReady()
{
//	OnIdleDisable();
	Ready();
}

void CHRView::OnArmMoveLeft()
{
//	OnIdleDisable();
	CHRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	ReadLeftArm();
	LeftArm[5] = R_LeftArm[5] - 2000;
	SendLeftArm();
	while(isMoveArm(5));
	
	Drop();

	while(isMoveArm(0));

/*	ReadLeftArm();
	ReadRightArm();
	RightArm[3] = R_RightArm[3] + 3000;
	LeftArm[3] = R_LeftArm[3] - 3000;
	SendArm();
	Lift();
	Sleep(500);

	pDoc->object[0] = 30;

	bool b1=MoveArm(0);

	pDoc->object[0] = -30;

	bool b2=MoveArm(1);
	
	if(b1&&b2){
		SendArm();
		while(isMoveArm(1));
	}
*/
	LeftArm[1] = -25000;
	RightArm[1] = 25000;

	SendArm();
	Lift();
	Sleep(1000);

	pDoc->object[0] = 20;
	pDoc->object[1] = 12;
	pDoc->object[2] = 50;

	MoveArm(0);

	pDoc->object[0] = -20;
	pDoc->object[1] = 12;
	pDoc->object[2] = 50;

	MoveArm(1);
	
	SendArm();

	OnHandInit();
	OnHandstp();

	bCatchBallReady=TRUE;
	while(isMoveArm(1));

	LeftArm[5] = R_LeftArm[5] + 2000;
	SendLeftArm();
}

void CHRView::OnArmMoveRight()
{
//	OnIdleDisable();
	CHRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	ReadLeftArm();
	LeftArm[5] = R_LeftArm[5] + 2000;
	SendLeftArm();
	while(isMoveArm(5));

	Drop();
	while(isMoveArm(0));

/*	ReadLeftArm();
	ReadRightArm();
	RightArm[3] = R_RightArm[3] + 3000;
	LeftArm[3] = R_LeftArm[3] - 3000;
	SendArm();
	Lift();
	Sleep(500);

	pDoc->object[0] = 30;

	bool b1=MoveArm(0);

	pDoc->object[0] = -30;

	bool b2=MoveArm(1);
	
	if(b1&&b2){
		SendArm();
		while(isMoveArm(1));
	}
*/
	LeftArm[1] = -25000;
	RightArm[1] = 25000;

	SendArm();
	Lift();
	Sleep(1000);

	pDoc->object[0] = 20;
	pDoc->object[1] = 12;
	pDoc->object[2] = 50;

	MoveArm(0);

	pDoc->object[0] = -20;
	pDoc->object[1] = 12;
	pDoc->object[2] = 50;

	MoveArm(1);
	
	SendArm();
	bCatchBallReady=TRUE;

	OnHandInit();
	OnHandstp();

	while(isMoveArm(1));

	LeftArm[5] = R_LeftArm[5] - 2000;
	SendLeftArm();

}

void CHRView::OnResetArm()
{
//	OnIdleDisable();
	ResetArm();
}

////////////////////
///
///	호연이 팔부분 끝
////////////////////

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

			if(!bCatchBallReady && !readMode){
				Speak(1);
				headPosX=0;
				headPosY=-4000;
				SendHead();
			}

			// yhseo : AMI1 or 2
			SensorEnable(0);											

			dir = STOP;
			bGo = TRUE;
			
			bSpeechGo=FALSE;

			playAloneMode = false;
			bSeeHere = false;
			bNavigation=FALSE;
			bWally=FALSE;

			if(walkingMode)
				OnStopwalk();
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

///////////////////////// daniel 2001.4.26.
//			::SendMessage(HWND_BROADCAST,MsgEmotion,speechCommand-1,0);
////////////////////////////////////////////

		if(speechMode && speechCommand>=ACTION1){

			Test.Format("인식모드 안");
			pDC->TextOut(50,450,Test);

			speechMode = false;
			if(speechCommand==ACTION1) // 인사해
			{
				if(bIsTable)
					return;
				if(cleanMode)
					return;
				if(readMode)
					return;

				bNavigation=0;
				dir=STOP;
				GoMobile(dir);
				

				::SendMessage(HWND_BROADCAST,MsgEmotion,6080,15000);

				RightArm[0]=-12000;
				RightArm[1]=20000;
				RightArm[2]=0;
				RightArm[3]=-60000;
				RightArm[4]=30000;
							
				LeftArm[0]=5000;
				LeftArm[1]=-10000;
				LeftArm[2]=-5000;
				LeftArm[3]=25000;
				LeftArm[4]=0;
				LeftArm[5]=0;
				
				SendRightArm();
				SendLeftArm();

				OnHandInit();

				// 팔이 자세를 취하기 까지 기다림
				Sleep(4000);


				SendTTS(",안녕하세요?, 저는 카이스트, 휴먼 로봇, 아미  입니다,,여러분을,만나서,반갑습니다");
//				Speak(7);
				shakeCount=0;

//				mouthCount=0;
//				maxMouthCount=30;

				pwm[4]=0x13;
				SendFace();
				mouthOpen = true;

				// 입벌렸다 닫기
				SetTimer(2, 700, NULL);
				// 팔흔들기 
				SetTimer(3, 1000, NULL);
				// 종료
				SetTimer(10, 9000, NULL);
			}
			else if(speechCommand==ACTION2) // 악수해
			{
				if(bIsTable)
					return;
				if(cleanMode)
					return;
				if(readMode)
					return;
				
				::SendMessage(HWND_BROADCAST,MsgEmotion,7070,9000);

				headPosY=-4000;
				headPosX=700;
				SendHead();

				bNavigation=0;
				dir=STOP;
				GoMobile(dir);

				RightArm[0]=-15000;
				RightArm[1]=-1000;
				RightArm[2]=4000;
				RightArm[3]=-30000;
				RightArm[4]=5000;

				SendRightArm();

				OnHandInit();
				// 팔이 자세를 취하기 까지 기다림
				Sleep(3000);

				SendTTS(",만나서,반갑습니다");
//				Speak(2);
	/*4
				pwm[0]=0x06;
				pwm[1]=0x15;
				pwm[4]=0x13;
				SendFace();
	*/
				shakeCount=0;

//				mouthCount=0;
//				maxMouthCount=30;

				pwm[4]=0x13;
				SendFace();
				mouthOpen = true;

				// 입벌렸다 닫기
				SetTimer(2, 700, NULL);
				// 팔흔들기 
				SetTimer(4, 1000, NULL);
				// 종료
				SetTimer(10, 4000, NULL);

			}
			else if(speechCommand==ACTION3) // 내가 누구니?
			{
				/*
				if(bIsTable)
					return;
				if(cleanMode)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,1060,3000);
				SendTTS("테이블로,가죠");
				Speak(2);
				bNavigation=TRUE;
				*/

				//내가 누구니?
				*lpVisionCommand=8;
			}
			else if(speechCommand==ACTION4) // 빨간공 들어
			{
			
				if(cleanMode)
					return;
				if(readMode)
					return;

				*lpVisionCommand=1;
				lpVisionData=lpSharedMemory+2;

				::SendMessage(HWND_BROADCAST,MsgEmotion,3060,3000);
				SendTTS("빨간공,듭니다");

				OnHandInit();
				OnHandstp();
				/*
				CHRDoc* pDoc = GetDocument();
				ASSERT_VALID(pDoc);

				pDoc->object[0] = 30;
				pDoc->object[1] = 7;
				pDoc->object[2] = 45;

				bool b1=MoveArm(0);

				pDoc->object[0] = -30;

				bool b2=MoveArm(1);
		
				if(b1&&b2){
					SendArm();
					while(isMoveArm(0));

					pDoc->object[0] = 0;

					if(MoveArm(2)){
						SendArm();
						while(isMoveArm(1));
						Lift();
					}
				}
				*/

				//발간공, 찾아 
				// 머리를 좌우로 돌려, 공을 찾아 카메라 좌표계에서 중간에 오도록 함
				// 허리를 현재 머리가 돌아간 만큼 돌림
				

				if(!bCatchBallReady)
				{
					Ready();
					Sleep(4000);
				}

				int scanCount=0;
				headDir=STOP;
				ReadHead();

				while(1){

					Vstereo[1].empty=1;
					memcpy(&Vstereo[0],lpVisionData,sizeof(VISION_DATA));
					memcpy(&Vstereo[1],lpVisionData+sizeof(VISION_DATA),sizeof(VISION_DATA));
					memcpy(&sp,lpVisionData+2*sizeof(VISION_DATA),sizeof(RTarget));

					//만약 공이 있으면,
					if(Vstereo[1].empty==0)
					{
						//카메라 좌표계에서 중간에 오도록 함	
						if(Vstereo[1].MC.x>50 && Vstereo[1].MC.x<110)
						{

							headStop();
							Sleep(1000);
							ReadHead();

							ReadLeftArm();
							//머리가 돌아 있는 만큼 허리를 돌리면서, 머리는 제자리로				
							LeftArm[5] =(R_headPosX*40000/26400)+R_LeftArm[5];
							SendLeftArm();
							headPosX=0;
							headPosY=0;
							SendHead();				

							::SendMessage(HWND_BROADCAST,MsgEmotion,9999,3000);
							SendTTS("빨간공, 찾았습니다");
							// 들어 
							pDoc->object[0] = 20;
							pDoc->object[1] = 4;
							pDoc->object[2] = 77-0.4*Vstereo[1].MC.y;

							Test.Format("x=%f y=%f z=%f",pDoc->object[0],pDoc->object[1],pDoc->object[2] );			
							pDC->TextOut(50,350,Test);

							if(pDoc->object[2]> 50 || pDoc->object[2] < 40 )
								break;

							bool b1=MoveArm(0);

							pDoc->object[0] = -20;

							bool b2=MoveArm(1);
					
							if(b1&&b2){
								SendArm();
								while(isMoveArm(0));

								pDoc->object[0] = 0;

								if(MoveArm(2)){
									SendArm();
									while(isMoveArm(1));
									Lift();
								}
								else{
									Sleep(2000);
									::SendMessage(HWND_BROADCAST,MsgEmotion,0,3000);
									SendTTS("공이,멀어요");
								}

							}
							else{
								Sleep(3000);
								::SendMessage(HWND_BROADCAST,MsgEmotion,0,3000);
								SendTTS("공이,멀어요");
							}
							
							break;
						}
						else if(Vstereo[1].MC.x<70)
						{
							headLeftScan();
						}
						else if(Vstereo[1].MC.x>80)
						{
							headRightScan();
						}
						//printText(50,200,"%d x0 %d y0 %d, x1 %d y1 %d, rx %f ry %f ",Vstereo[1].empty,Vstereo[0].MC.x,Vstereo[0].MC.y,Vstereo[1].MC.x,Vstereo[1].MC.y,sp.rx,sp.ry);
						
					}
					//공이 없으면, 찾음
					else if(scanCount==0)
					{
						headLeftScan();
						scanCount++;
						TRACE("\nA");
					}
					else if(scanCount==1 && R_headPosX<-2000)
					{
						headRightScan();
						scanCount++;
						TRACE("\nB");
					}
					else if(scanCount>1 && R_headPosX>2000)
					{
						::SendMessage(HWND_BROADCAST,MsgEmotion,2020,3000);
						SendTTS("공을 찾을수 없습니다");		
						TRACE("\nC");

						headPosX=0;
						headPosY=R_headPosY;

						SendHead();

						break;
					}
					ReadHead();
					//Sleep(30);

					//TRACE(" scanCount %d gTimer %d",scanCount,scanTimer);
				}
			}
			else if(speechCommand==ACTION5) // 공들기 준비
			{
				if(cleanMode)
					return;
				if(readMode)
					return;
				
				::SendMessage(HWND_BROADCAST,MsgEmotion,1062,3000);
				SendTTS("공들기,준비합니다");
				Speak(2);

				bIsTable = TRUE;
				InitPos();
				Ready();
			}
			else if(speechCommand==ACTION6) // 빨간공 오른쪽에 놔
			{
				if(cleanMode)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,1070,3000);
				SendTTS("빨간공,오른쪽에,놓습니다");
				OnArmMoveRight();
			}
			else if(speechCommand==ACTION7) // 빨간공 가운데 놔
			{
				if(cleanMode)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,1168,3000);
				SendTTS("빨간공,가운데,놓습니다");

				LeftArm[5] =0;
				SendLeftArm();
				while(isMoveArm(5));
				
				Drop();
				while(isMoveArm(0));

/*				pDoc->object[0] = 30;

				bool b1=MoveArm(0);

				pDoc->object[0] = -30;

				bool b2=MoveArm(1);
				
				if(b1&&b2){
					SendArm();
					while(isMoveArm(1));
				}
*/
				LeftArm[1] = -25000;
				RightArm[1] = 25000;

				SendArm();
				Lift();
				Sleep(1000);

				pDoc->object[0] = 20;
				pDoc->object[1] = 12;
				pDoc->object[2] = 50;

				MoveArm(0);

				pDoc->object[0] = -20;
				pDoc->object[1] = 12;
				pDoc->object[2] = 50;

				MoveArm(1);
				
				SendArm();

				OnHandInit();
				OnHandstp();

				bCatchBallReady=TRUE;
			}
			else if(speechCommand==ACTION8) // 빨간공 왼쪽에 놔
			{
				if(cleanMode)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,1464,3000);
				SendTTS("빨간공,왼쪽에,놓습니다");
				OnArmMoveLeft();
			}
			else if(speechCommand==ACTION9) // 바로
			{
				if(cleanMode)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,3075,3000);
				SendTTS("바로");
				Speak(1);
				
				
				ResetArm();

				headPosX=0;
				headPosY=-4000;
				SendHead();

				OnHandInit();
				OnHandstp();

				bIsTable = FALSE;
				*lpVisionCommand=0;

			}
			else if(speechCommand==ACTION10) // 같이놀자
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,8090,3000);
				SendTTS("같이 놀아요");
				Speak(2);

				RightArm[0]=-5000;
				RightArm[1]=0;
				RightArm[2]=2000;
				RightArm[3]=-10000;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=0;
				LeftArm[2]=-2000;
				LeftArm[3]=10000;
				LeftArm[4]=0;
				LeftArm[5]=0;
				
				SendRightArm();
				SendLeftArm();

				OnWally();

				SetTimer(55,5000,NULL);

			}
			else if(speechCommand==ACTION11) // 청소준비
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,60,3000);
				SendTTS("청소 준비 합니다");
				Speak(2);
				OnTest2();
			}
			else if(speechCommand==ACTION12) // 청소시작
			{
				if(bIsTable)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,1065,3000);
				SendTTS("청소 합니다");
				Speak(2);
				OnClean();
			}
			else if(speechCommand==ACTION13) // 이제그만
			{				

				randnum = rand() % 3;
				
				::SendMessage(HWND_BROADCAST,MsgEmotion,6075,3000);
				switch(randnum){
				case 0:
					SendTTS("이제 그만");
					break;
				case 1:
					SendTTS("그만 할께요");
					break;
				case 2:
					SendTTS("그만 하자");
					break;
				}
				Speak(2);
				InitPos();
			}
			else if(speechCommand==ACTION14) // 잘했어
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
			else if(speechCommand==ACTION15) // 못했어
			{
				if(readMode)
					return;
				if(cleanMode)
					return;

				emotionCount--;
				if(emotionCount<-2)emotionCount=-2;
				if(emotionCount>0)emotionCount=0;

				headPosX = 0;
				headPosY = -2000;
				SendHead();

				randnum = rand() % 5;
				
				switch(randnum){
				case 0:
					::SendMessage(HWND_BROADCAST,MsgEmotion,10,3000);
					SendTTS("죄송합니다");
					break;
				case 1:
					::SendMessage(HWND_BROADCAST,MsgEmotion,0,3000);
					SendTTS("미안합니다");
					break;
				case 2:
					::SendMessage(HWND_BROADCAST,MsgEmotion,7010,3000);
					SendTTS("너무 어려워요");
					break;
				case 3:
					::SendMessage(HWND_BROADCAST,MsgEmotion,3010,3000);
					SendTTS("다음엔 잘할께요");
					break;
				case 4:
					::SendMessage(HWND_BROADCAST,MsgEmotion,9000,3000);
					SendTTS("너무해요");
					break;
				}
				Speak(2);
				headPosX = 0;
				headPosY = -4000;
				SendHead();
			}
			else if(speechCommand==ACTION16) // 멋진모습
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;
				if(readMode)
					return;

				if(nicemotion == 0)
					OnTest();
				else if(nicemotion == 1)
					OnTest3();
				else if(nicemotion == 2)
					OnNiceMotion1();
				else if(nicemotion == 3)
					OnNiceMotion2();
				else
					OnNiceMotion3();

				nicemotion = (nicemotion+1)%5;

				Sleep(5000);

/*				headPosX=0;
				headPosY=-4000;
				SendHead();
				RightArm[0]=0;
				RightArm[1]=0;
				RightArm[2]=0;
				RightArm[3]=0;
				RightArm[4]=0;
							
				LeftArm[0]=0;
				LeftArm[1]=0;
				LeftArm[2]=0;
				LeftArm[3]=0;
				LeftArm[4]=0;

				SendRightArm();
				SendLeftArm();
				OnHandInit();
				OnHandstp();
				Sleep(2000);*/

			}
			else if(speechCommand==ACTION17) // 앞으로가
			{
				if(bIsTable)
					return;
				if(readMode)
					return;
				if(cleanMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,75,3000);
				SendTTS("앞으로 갑니다");
				Speak(2);
				
				SensorEnable(1);				
				dir=FRONT;
				bSpeechGo=TRUE;
				OnWalking();
			}
			else if(speechCommand==ACTION18) // 뒤로가
			{
				if(bIsTable)
					return;
				if(readMode)
					return;
				if(cleanMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,73,3000);
				SendTTS("뒤로 갑니다");
				Speak(2);
				
				SensorEnable(1);								
				dir=BACK;
				bSpeechGo=TRUE;
				OnWalking();
			}
			else if(speechCommand==ACTION19) // 오른쪽으로 돌아
			{
				if(bIsTable)
					return;
				if(readMode)
					return;
				if(cleanMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,70,3000);
				SendTTS("오른쪽으로 돌아요");
				Speak(2);
				
				SensorEnable(1);								
				dir=RIGHT;
				bSpeechGo=TRUE;
				OnWalking();
			}
			else if(speechCommand==ACTION20) // 왼쪽으로 돌아
			{
				if(bIsTable)
					return;
				if(readMode)
					return;
				if(cleanMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,168,3000);
				SendTTS("왼쪽으로 돌아요");
				Speak(2);
				
				SensorEnable(1);								
				dir=LEFT;
				bSpeechGo=TRUE;
				OnWalking();
			}
			else if(speechCommand==ACTION21) // 정지
			{
				if(bIsTable)
					return;
				if(readMode)
					return;
				if(cleanMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,1077,3000);
				SendTTS("정지합니다");
				Speak(1);
				dir=STOP;
				bGo=TRUE;
				
				SensorEnable(0);								
				bSpeechGo=FALSE;

				if(walkingMode)
					OnStopwalk();
			}
			else if(speechCommand==ACTION22) // 쉬어
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,8088,3000);
				randnum = rand()%3;
				switch(randnum){
				case 0:
					SendTTS("네에에에");
					Speak(1);
					break;
				case 1:
					SendTTS("그러죠");
					Speak(1);
					break;
				case 2:
					SendTTS("그럴까요?");
					Speak(2);
					break;
				}

				InitPos();
				OnIdleEnable();
			}
			else if(speechCommand==ACTION23) // 여길봐
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,69,3000);
				SendTTS("어딜 볼까요?");
				Speak(1);
				OnSeeHere();

			}
			else if(speechCommand==ACTION24) // 혼자놀아
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;
				if(readMode)
					return;

				::SendMessage(HWND_BROADCAST,MsgEmotion,35,3000);

				SendTTS("혼자 놀자");
				Speak(2);
				OnPlayalone();

			}
			else if(speechCommand==ACTION25) // 기분이 어떠니?
			{

				randnum = rand()%5;
				float emotionWeight = (rand()%100)/100.0;

				if(emotionCount != 0)
					randnum = randnum + (int)(emotionCount*emotionWeight+(emotionCount/abs(emotionCount))*.5);

				if(randnum<0)randnum=0;
				if(randnum>4)randnum=4;

				switch(randnum){
				case 0:
					::SendMessage(HWND_BROADCAST,MsgEmotion,0,3000);
					SendTTS("나빠요");
					Speak(1);
					break;
				case 1:
					::SendMessage(HWND_BROADCAST,MsgEmotion,3025,3000);
					SendTTS("별로에요");
					Speak(1);
					break;
				case 2:
					::SendMessage(HWND_BROADCAST,MsgEmotion,1045,3000);
					SendTTS("그저 그래요");
					Speak(2);
					break;
				case 3:
					::SendMessage(HWND_BROADCAST,MsgEmotion,3075,3000);
					SendTTS("좋아요");
					Speak(1);
					break;
				case 4:
					::SendMessage(HWND_BROADCAST,MsgEmotion,9999,3000);
					SendTTS("아주 좋아요");
					Speak(2);
					break;
				}
				
			}
			else if(speechCommand==ACTION26) // 나 어때?
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;
				if(readMode)
					return;

				OnLove();
			}
			else if(speechCommand==ACTION27) // 읽기 준비
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;

				OnReadReady();
				readMode = true;
			}
			else if(speechCommand==ACTION28) // 읽어봐
			{
				if(cleanMode)
					return;
				if(bIsTable)
					return;

				OnReading();
			}
			else if(speechCommand==100) // 다시 말씀해 주세요
			{
				::SendMessage(HWND_BROADCAST,MsgEmotion,2025,3000);
				randnum = rand()%3;
				switch(randnum){
				case 0:
					SendTTS("다시 말씀해 주세요");
					Speak(2);
					break;
				case 1:
					SendTTS("뭐라고요?");
					Speak(1);
					break;
				case 2:
					SendTTS("잘,안들려요");
					Speak(2);
					break;
				}
				speechCount = 0;
				speechMode = false;
			}
			Test.Format("                  ");			
			pDC->TextOut(50,400,Test);
			Test.Format("                                    ");			
			pDC->TextOut(50,450,Test);			

		}
//		*lpSharedMemory=0;

	}
	else if(nIDEvent==1)// navigation & mobile
	{
		//TRACE("\ntimer0!");
#ifdef MOBILE

		JtObject.x=80;
		JtObject.y=160;
		JtObject.q=0.33;

		toObjAngle=GetAngleFromObjectPos(JtObject); 
		TRACE("\n toObjAngle = %f",toObjAngle);		

		if(!ReadSensor())
		{
			Test.Format("ERROR: ReadSensor             ");			
			pDC->TextOut(50,110,Test);			
		}

		if(bNavigation||bWally)
		{
			if(bNavigation)
				Navigation();

#ifdef WALLY_NAVIGATION
			else if(bWally)
				Wally();
#endif
			
			Test.Format("Navigation start");
		}
		else
		{
			Test.Format("Navigation stop");		
		}

		if(bNavigation||bGo||bJoy||bWally||bSpeechGo)
		{
			if(bSpeechGo)
			{

				if(!IsSafe(dir))
				{
					
					dir=STOP;
					GoMobile(dir);
					bGo=TRUE;
					bSpeechGo=FALSE;
					OnStopwalk();

					::SendMessage(HWND_BROADCAST,MsgEmotion,8010,3000);

					randnum = rand()%2;

					switch(randnum){
					case 0:
						SendTTS("비켜  주세요");
						Speak(2);
						break;
					case 1:
						SendTTS("앗 장애물");
						Speak(2);
						break;
					default:
						break;
					}
				}
			}
			
			
			OnIdleDisable();
			GoMobile(dir);

			
			if(bGo)
				bGo=FALSE;
		}
		//Mobile(dir);	
		
		pDC->TextOut(50,80,Test);			
		timer0_Count++;
#endif
	}
	else if(nIDEvent==77)
	{

		if(bWally && !cleanMode && !playAloneMode)
		{
			WallyVision();
		}
		else if(bSeeHere)
		{
			SeeHere();
		}

//		ReadHead();
	}
	/*입모양 이벤트 timer */
	else if(nIDEvent==2)
	{
//		pwm[0]=0x06;
//		pwm[1]=0x15;

		if(mouthOpen){
			pwm[4] = 0x15;
			mouthOpen = false;
		}
		else {
			mouthOpen = true;
			pwm[4] = 0x13;
		}

/*		if(pwm[4]==0x15)
			pwm[4]=0x13;
		else
			pwm[4]=0x15;
*/		

//		mouthCount++;
		
//		if(mouthCount>maxMouthCount)
//			KillTimer(2);

		SendFace();

	}
	/*오른팔 흔들기 이벤트 timer */
	else if(nIDEvent==3)
	{
		if(shakeCount%3==0)
		{
			RightArm[0]=-12000;
			RightArm[1]=20000;
			RightArm[2]=-5000;
			RightArm[3]=-60000;
			RightArm[4]=30000;
		}
		else if(shakeCount%3==1)
		{
			RightArm[0]=-12000;
			RightArm[1]=20000;
			RightArm[2]=5000;
			RightArm[3]=-60000;
			RightArm[4]=30000;
		}
		else if(shakeCount%3==2)
		{
			RightArm[0]=-12000;
			RightArm[1]=20000;
			RightArm[2]=0;
			RightArm[3]=-60000;
			RightArm[4]=30000;
		}
		SendRightArm();

		shakeCount++;

	}
	// 악수 팔 흔들기
	else if(nIDEvent==4)
	{
		if(RightArm[0]==-15000)
			RightArm[0]=-12000;
		else
			RightArm[0]=-15000;

		RightArm[1]=-1000;
		RightArm[2]=4000;
		RightArm[3]=-30000;
		RightArm[4]=5000;

		SendRightArm();
	}
	// 앞으로 약간 전진
	else if(nIDEvent==6)
	{
		KillTimer(6);
		dir=STOP;
		GoMobile(dir);
	}
	// 입모양 종료
	else if(nIDEvent==56)
	{
		KillTimer(2);
		KillTimer(56);

		pwm[4]=0x15;
		SendFace();
		mouthOpen = false;
	}
	// 인사해 종료
	else if(nIDEvent==10)
	{
		headPosX=0;
		headPosY=-4000;
		SendHead();

//		pwm[0]=0x06;
//		pwm[1]=0x15;
		pwm[4]=0x15;
		SendFace();
		mouthOpen = false;

		RightArm[0]=0;
		RightArm[1]=0;
		RightArm[2]=0;
		RightArm[3]=0;
		RightArm[4]=0;

		LeftArm[0]=0;
		LeftArm[1]=0;
		LeftArm[2]=0;
		LeftArm[3]=0;
		LeftArm[4]=0;
		LeftArm[5]=0;

		SendRightArm();
		SendLeftArm();

		OnHandInit();
		OnHandstp();

		KillTimer(2);
		KillTimer(3);
		KillTimer(4);
		KillTimer(10);
	}
	// Joystick timer event
	else if(nIDEvent==15)
	{

#ifdef JOYSTICK

		if(joyGetPos(JOYSTICKID1,&joyinfo)!=JOYERR_NOERROR)
			Test.Format("joystick error");
		
		if(bJoy)		
		if(joyinfo.wYpos<1500)
		{
			Test.Format("up                ");
			dir=UP;
		}
		else if(joyinfo.wYpos>63500)
		{
			Test.Format("down               ");
			dir=DOWN;
		}
		else if(joyinfo.wXpos<1500)
		{
			Test.Format("left      ");
			dir=LEFT;
		}
		else if(joyinfo.wXpos>63500)
		{
			Test.Format("right             ");
			dir=RIGHT;
		}
		else
		{
			Test.Format("center             ");
			dir=CENTER;
		}
		
		if(joyinfo.wButtons==JOY_BUTTON1)
		{
			bJoy^=1;	
			if(bJoy)
			{
				bNavigation=0;
				bGo=0;
				bWally=0;
				
				bPlay=0;
				Test.Format("joy  start  ");
				SendTTS("원격조정 모드");
			}
			else
			{
				Test.Format("joy  end  ");
				SendTTS("원격조정 모드 해제");
			}
		}
		else if(joyinfo.wButtons==JOY_BUTTON2)
		{
			bNavigation=0;
			bGo=0;
			bJoy=0;
			OnWally();
		}

		pDC->TextOut(50,20,Test);	
#endif
	}
	// walking mode
	else if(nIDEvent==50) 
	{
//		OnIdleDisable();

		if(walking){
			walking=false;
			RightArm[0]=5000;
			RightArm[1]=5000;
			RightArm[3]=0;
						
			LeftArm[0]=10000;
			LeftArm[1]=0;
			LeftArm[3]=5000;

			SendRightArm();
			SendLeftArm();
		}
		else {
			walking=true;
			RightArm[0]=-10000;
			RightArm[1]=0;
			RightArm[3]=-5000;
						
			LeftArm[0]=-5000;
			LeftArm[1]=-5000;
			LeftArm[3]=0;

			SendRightArm();
			SendLeftArm();
		}
	}
//idle mode
	else if(nIDEvent==51)
	{
		int idleMotion;
		int Hx,Hy,heory;

		idleMotion = rand()%23;
		Hx = rand()%7000 - 3500;
		Hy = rand()%3000 - 5500;
		heory = rand()%3000 - 1500;

		switch(idleMotion){
		case 0:
			headPosY=-5000;
			SendHead();

			::SendMessage(HWND_BROADCAST,MsgEmotion,545,3000);

			SendTTS("하아아아아암");
			pwm[4] = 0x10;
			SendFace();
			Sleep(2000);
			pwm[4] = 0x15;
			SendFace();
			break;

		case 1:
		case 2:
		case 5:
		case 6:
		case 7:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			randnum = rand() % 7;
			switch(randnum){
			case 0:
				::SendMessage(HWND_BROADCAST,MsgEmotion,1020,3000);
				SendTTS("아이, 심심해");
				Speak(2);
				break;
			case 1:
				::SendMessage(HWND_BROADCAST,MsgEmotion,550,3000);
				SendTTS("으음");
				break;
			case 2:
				::SendMessage(HWND_BROADCAST,MsgEmotion,1069,3000);
				SendTTS("아하!");
				Speak(1);
				break;
			default:
				break;
			}
			headPosX=Hx;
			headPosY=Hy;
			SendHead();
			LeftArm[5] = heory;
			SendLeftArm();
			break;
		case 3:
			KillTimer(51);

			headPosX=1300;
			headPosY=-2000;
			SendHead();
			RightArm[0]=-13000;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=-15000;
			RightArm[4]=0;
						
			LeftArm[0]=0;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=0;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();
			Sleep(2000);

			//손 폈다 접었다.. 반복

			randnum = rand() % 7;
			switch(randnum){
			case 0:
				::SendMessage(HWND_BROADCAST,MsgEmotion,5071,3000);
				SendTTS("오호");
				break;
			case 1:
				::SendMessage(HWND_BROADCAST,MsgEmotion,2068,3000);
				SendTTS("아아");
				break;
			case 2:
				::SendMessage(HWND_BROADCAST,MsgEmotion,1565,3000);
				SendTTS("하");
				break;
			case 3:
				::SendMessage(HWND_BROADCAST,MsgEmotion,4065,3000);
				SendTTS("잼잼");
				break;
			default:
				break;
			}

			RightHand[0]=0x11;
			RightHand[1]=0x17;
			RightHand[2]=0x11;
			RightHand[3]=0x15;
			RightHand[4]=0x15;
			RightHand[5]=0x11;
			SendRightHand();
			Sleep(500);

			RightHand[0]=0x16;
			RightHand[1]=0x14;
			RightHand[2]=0x1a;
			RightHand[3]=0x11;
			RightHand[4]=0xc;
			RightHand[5]=0x15;
			SendRightHand();
			Sleep(500);

			RightHand[0]=0x11;
			RightHand[1]=0x17;
			RightHand[2]=0x11;
			RightHand[3]=0x15;
			RightHand[4]=0x15;
			RightHand[5]=0x11;
			SendRightHand();
			Sleep(500);

			RightHand[0]=0x16;
			RightHand[1]=0x14;
			RightHand[2]=0x1a;
			RightHand[3]=0x11;
			RightHand[4]=0xc;
			RightHand[5]=0x15;
			SendRightHand();
			Sleep(500);

			RightHand[0]=0x11;
			RightHand[1]=0x17;
			RightHand[2]=0x11;
			RightHand[3]=0x15;
			RightHand[4]=0x15;
			RightHand[5]=0x11;
			SendRightHand();
			Sleep(500);

			//손바로 
			OnHandInit();
			OnHandstp();

			headPosX=0;
			headPosY=-4000;
			SendHead();
			RightArm[0]=0;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=0;
			RightArm[4]=0;
						
			LeftArm[0]=0;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=0;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();

			SetTimer(51,5000,NULL);
			break;
		case 4:
/*--------------------------*/
/* Arm Part					*/
/*--------------------------*/
/*	Right part				
  		0:   25000 ~ -45000
  		1:  -4700  ~ 49000
  		2:   50000 ~ -50000
  		3:  -60000 ~ 0
  		4:   85000 ~ -85000
 	Left part				
  		0:  -25000 ~ 45000
  		1:   4700  ~ -49000
  		2:  -50000 ~ 50000
  		3:   60000 ~ 0
  		4:  -85000 ~ 85000
  (5)waist:	-5500  ~ 5500
   (순서 : 뒤,안,안,위,안,왼이 먼저)
*/	

			KillTimer(51);

			headPosX=-1300;
			headPosY=-2000;
			SendHead();
			RightArm[0]=0;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=0;
			RightArm[4]=0;
						
			LeftArm[0]=13000;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=15000;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();
			Sleep(2000);

			randnum = rand() % 7;
			switch(randnum){
			case 0:
				::SendMessage(HWND_BROADCAST,MsgEmotion,4075,3000);
				SendTTS("오호");
				break;
			case 1:
				::SendMessage(HWND_BROADCAST,MsgEmotion,2075,3000);
				SendTTS("아아");
				break;
			case 2:
				::SendMessage(HWND_BROADCAST,MsgEmotion,2073,3000);
				SendTTS("하");
				break;
			case 3:
				::SendMessage(HWND_BROADCAST,MsgEmotion,4070,3000);
				SendTTS("잼잼");
				break;
			default:
				break;
			}

			LeftHand[0]=0x15;
			LeftHand[1]=0xd;
			LeftHand[2]=0x14;
			LeftHand[3]=0x11;
			LeftHand[4]=0x12;
			LeftHand[5]=0x15;
			SendLeftHand();
			Sleep(500);

			LeftHand[0]=0x10;
			LeftHand[1]=0x10;
			LeftHand[2]=0xc;
			LeftHand[3]=0x15;
			LeftHand[4]=0x1b;
			LeftHand[5]=0x11;
			SendLeftHand();
			Sleep(500);

			LeftHand[0]=0x15;
			LeftHand[1]=0xd;
			LeftHand[2]=0x14;
			LeftHand[3]=0x11;
			LeftHand[4]=0x12;
			LeftHand[5]=0x15;
			SendLeftHand();
			Sleep(500);

			LeftHand[0]=0x10;
			LeftHand[1]=0x10;
			LeftHand[2]=0xc;
			LeftHand[3]=0x15;
			LeftHand[4]=0x1b;
			LeftHand[5]=0x11;
			SendLeftHand();
			Sleep(500);

			LeftHand[0]=0x15;
			LeftHand[1]=0xd;
			LeftHand[2]=0x14;
			LeftHand[3]=0x11;
			LeftHand[4]=0x12;
			LeftHand[5]=0x15;
			SendLeftHand();
			Sleep(500);
			//손바로 
			
			OnHandInit();
			OnHandstp();

			headPosX=0;
			headPosY=-4000;
			SendHead();
			RightArm[0]=0;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=0;
			RightArm[4]=0;
						
			LeftArm[0]=0;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=0;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();

			SetTimer(51,5000,NULL);
			break;
		case 8:
			KillTimer(51);

			OnTest();
			Sleep(5000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			RightArm[0]=0;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=0;
			RightArm[4]=0;
						
			LeftArm[0]=0;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=0;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();
			OnHandInit();
			OnHandstp();

			SetTimer(51,5000,NULL);

			break;
		case 9:
			KillTimer(51);

			OnTest3();
			Sleep(5000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			RightArm[0]=0;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=0;
			RightArm[4]=0;
						
			LeftArm[0]=0;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=0;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();
			OnHandInit();
			OnHandstp();

			SetTimer(51,5000,NULL);

			break;
		case 17:
		case 18:
		case 19:
			break;
		case 20:
			KillTimer(51);

			OnNiceMotion1();
			Sleep(5000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			RightArm[0]=0;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=0;
			RightArm[4]=0;
						
			LeftArm[0]=0;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=0;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();
			OnHandInit();
			OnHandstp();

			SetTimer(51,5000,NULL);

			break;
		case 21:
			KillTimer(51);

			OnNiceMotion2();
			Sleep(5000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			RightArm[0]=0;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=0;
			RightArm[4]=0;
						
			LeftArm[0]=0;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=0;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();
			OnHandInit();
			OnHandstp();

			SetTimer(51,5000,NULL);

			break;
		case 22:
			KillTimer(51);

			OnNiceMotion3();
			Sleep(5000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			RightArm[0]=0;
			RightArm[1]=0;
			RightArm[2]=0;
			RightArm[3]=0;
			RightArm[4]=0;
						
			LeftArm[0]=0;
			LeftArm[1]=0;
			LeftArm[2]=0;
			LeftArm[3]=0;
			LeftArm[4]=0;

			SendRightArm();
			SendLeftArm();
			OnHandInit();
			OnHandstp();

			SetTimer(51,5000,NULL);

			break;
		}

	}
	// 청소시 살펴보기
	else if(nIDEvent==55)
	{
		
		//SendTTS("타");
		if(cleanMode)
		{
			randnum = rand()%3;
			if(randnum == 0){
				headPosX=4000;
				headPosY=-1500;
				SendHead();
			}
			else if(randnum == 1){
				headPosX=3000;
				headPosY=-1500;
				SendHead();
			}
			else {
				headPosX=3500;
				headPosY=-1500;
				SendHead();
			}
		}
		else if(playAloneMode)
		{
			// 목과 팔 적당히 움직인다...간혹 말도 씨불거린다.
			int idleMotion;
			int Hx,Hy,heory,armR,armL;

			idleMotion = rand()%23;
			Hx = rand()%7000 - 3500;
			Hy = rand()%3000 - 5500;
			heory = rand()%3000 - 1500;
			armR = rand()%6000 - 3000;
			armL = rand()%6000 - 3000;

			switch(idleMotion){
			case 0:
				headPosY=-5000;
				SendHead();
				::SendMessage(HWND_BROADCAST,MsgEmotion,555,3000);
				SendTTS("하아아아아암");
				pwm[4] = 0x10;
				SendFace();
				Sleep(2000);
				pwm[4] = 0x15;
				SendFace();
				break;

			case 1:
			case 2:
			case 5:
			case 6:
			case 7:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
				randnum = rand() % 18;
				switch(randnum){
				case 0:
					::SendMessage(HWND_BROADCAST,MsgEmotion,520,3000);
					SendTTS("아이, 심심해");
					Speak(2);
					break;
				case 1:
					::SendMessage(HWND_BROADCAST,MsgEmotion,3040,3000);
					SendTTS("으음");
					break;
				case 2:
					::SendMessage(HWND_BROADCAST,MsgEmotion,4080,3000);
					SendTTS("아하!");
					Speak(1);
					break;
				case 3:
					::SendMessage(HWND_BROADCAST,MsgEmotion,8088,3000);
					SendTTS("같이 놀아요");
					Speak(2);
					break;
				case 4:
					::SendMessage(HWND_BROADCAST,MsgEmotion,7080,3000);
					SendTTS("놀아줘요");
					Speak(1);
					break;
				case 5:
					::SendMessage(HWND_BROADCAST,MsgEmotion,9595,3000);
					SendTTS("신난다");
					Speak(1);
					break;
				case 6:
					::SendMessage(HWND_BROADCAST,MsgEmotion,3080,3000);
					SendTTS("아미에요");
					Speak(1);
					break;
				case 7:
					::SendMessage(HWND_BROADCAST,MsgEmotion,4083,3000);
					SendTTS("나는 휴먼로봇");
					Speak(2);
					break;
				case 8:
					::SendMessage(HWND_BROADCAST,MsgEmotion,8080,3000);
					SendTTS("재미있다");
					Speak(2);
					break;
				default:
					break;
				}
				headPosX=Hx;
				headPosY=Hy;
				SendHead();
				LeftArm[5] = heory;
				SendLeftArm();

				//서있는 멋진자세

				RightArm[0]=-5000;
				RightArm[1]=6000;
				RightArm[2]=2000;
				RightArm[3]=-10000+armR;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=-6000;
				LeftArm[2]=-2000;
				LeftArm[3]=10000+armL;
				LeftArm[4]=0;
				
				SendRightArm();
				SendLeftArm();

				break;
			case 3:
				KillTimer(55);

				dir=STOP;
				GoMobile(dir);

				headPosX=1300;
				headPosY=-2000;
				SendHead();
				RightArm[0]=-13000;
				RightArm[1]=0;
				RightArm[2]=0;
				RightArm[3]=-15000;
				RightArm[4]=0;
							
				LeftArm[0]=0;
				LeftArm[1]=0;
				LeftArm[2]=0;
				LeftArm[3]=0;
				LeftArm[4]=0;

				SendRightArm();
				SendLeftArm();
				Sleep(2000);

				//손 폈다 접었다.. 반복

				randnum = rand() % 7;
				switch(randnum){
				case 0:
					::SendMessage(HWND_BROADCAST,MsgEmotion,4080,3000);
					SendTTS("오호");
					break;
				case 1:
					::SendMessage(HWND_BROADCAST,MsgEmotion,1565,3000);
					SendTTS("아아");
					break;
				case 2:
					::SendMessage(HWND_BROADCAST,MsgEmotion,1065,3000);
					SendTTS("하");
					break;
				case 3:
					::SendMessage(HWND_BROADCAST,MsgEmotion,4060,3000);
					SendTTS("잼잼");
					break;
				default:
					break;
				}

				RightHand[0]=0x11;
				RightHand[1]=0x17;
				RightHand[2]=0x11;
				RightHand[3]=0x15;
				RightHand[4]=0x15;
				RightHand[5]=0x11;
				SendRightHand();
				Sleep(500);

				RightHand[0]=0x16;
				RightHand[1]=0x14;
				RightHand[2]=0x1a;
				RightHand[3]=0x11;
				RightHand[4]=0xc;
				RightHand[5]=0x15;
				SendRightHand();
				Sleep(500);

				RightHand[0]=0x11;
				RightHand[1]=0x17;
				RightHand[2]=0x11;
				RightHand[3]=0x15;
				RightHand[4]=0x15;
				RightHand[5]=0x11;
				SendRightHand();
				Sleep(500);

				RightHand[0]=0x16;
				RightHand[1]=0x14;
				RightHand[2]=0x1a;
				RightHand[3]=0x11;
				RightHand[4]=0xc;
				RightHand[5]=0x15;
				SendRightHand();
				Sleep(500);

				RightHand[0]=0x11;
				RightHand[1]=0x17;
				RightHand[2]=0x11;
				RightHand[3]=0x15;
				RightHand[4]=0x15;
				RightHand[5]=0x11;
				SendRightHand();
				Sleep(500);

				//손바로 
				OnHandInit();
				OnHandstp();

				headPosX=0;
				headPosY=-4000;
				SendHead();
				RightArm[0]=-5000;
				RightArm[1]=6000;
				RightArm[2]=2000;
				RightArm[3]=-10000+armR;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=-6000;
				LeftArm[2]=-2000;
				LeftArm[3]=10000+armL;
				LeftArm[4]=0;
				LeftArm[5]=0;

				SendRightArm();
				SendLeftArm();

				SetTimer(55,5000,NULL);
				break;
			case 4:
	/*--------------------------*/
	/* Arm Part					*/
	/*--------------------------*/
	/*	Right part				
  			0:   25000 ~ -45000
  			1:  -4700  ~ 49000
  			2:   50000 ~ -50000
  			3:  -60000 ~ 0
  			4:   85000 ~ -85000
 		Left part				
  			0:  -25000 ~ 45000
  			1:   4700  ~ -49000
  			2:  -50000 ~ 50000
  			3:   60000 ~ 0
  			4:  -85000 ~ 85000
	  (5)waist:	-5500  ~ 5500
	   (순서 : 뒤,안,안,위,안,왼이 먼저)
	*/	

				KillTimer(55);

				dir=STOP;
				GoMobile(dir);

				headPosX=-1300;
				headPosY=-2000;
				SendHead();
				RightArm[0]=0;
				RightArm[1]=0;
				RightArm[2]=0;
				RightArm[3]=0;
				RightArm[4]=0;
							
				LeftArm[0]=13000;
				LeftArm[1]=0;
				LeftArm[2]=0;
				LeftArm[3]=15000;
				LeftArm[4]=0;

				SendRightArm();
				SendLeftArm();
				Sleep(2000);

				randnum = rand() % 7;
				switch(randnum){
				case 0:
					::SendMessage(HWND_BROADCAST,MsgEmotion,6065,3000);
					SendTTS("오호");
					break;
				case 1:
					::SendMessage(HWND_BROADCAST,MsgEmotion,3068,3000);
					SendTTS("아아");
					break;
				case 2:
					::SendMessage(HWND_BROADCAST,MsgEmotion,563,3000);
					SendTTS("하");
					break;
				case 3:
					::SendMessage(HWND_BROADCAST,MsgEmotion,4570,3000);
					SendTTS("잼잼");
					break;
				default:
					break;
				}

				LeftHand[0]=0x15;
				LeftHand[1]=0xd;
				LeftHand[2]=0x14;
				LeftHand[3]=0x11;
				LeftHand[4]=0x12;
				LeftHand[5]=0x15;
				SendLeftHand();
				Sleep(500);

				LeftHand[0]=0x10;
				LeftHand[1]=0x10;
				LeftHand[2]=0xc;
				LeftHand[3]=0x15;
				LeftHand[4]=0x1b;
				LeftHand[5]=0x11;
				SendLeftHand();
				Sleep(500);

				LeftHand[0]=0x15;
				LeftHand[1]=0xd;
				LeftHand[2]=0x14;
				LeftHand[3]=0x11;
				LeftHand[4]=0x12;
				LeftHand[5]=0x15;
				SendLeftHand();
				Sleep(500);

				LeftHand[0]=0x10;
				LeftHand[1]=0x10;
				LeftHand[2]=0xc;
				LeftHand[3]=0x15;
				LeftHand[4]=0x1b;
				LeftHand[5]=0x11;
				SendLeftHand();
				Sleep(500);

				LeftHand[0]=0x15;
				LeftHand[1]=0xd;
				LeftHand[2]=0x14;
				LeftHand[3]=0x11;
				LeftHand[4]=0x12;
				LeftHand[5]=0x15;
				SendLeftHand();
				Sleep(500);
				//손바로 
				
				OnHandInit();
				OnHandstp();

				headPosX=0;
				headPosY=-4000;
				SendHead();
				RightArm[0]=-5000;
				RightArm[1]=6000;
				RightArm[2]=2000;
				RightArm[3]=-10000+armR;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=-6000;
				LeftArm[2]=-2000;
				LeftArm[3]=10000+armL;
				LeftArm[4]=0;
				LeftArm[5]=0;

				SendRightArm();
				SendLeftArm();

				SetTimer(55,5000,NULL);
				break;
			case 8:
				KillTimer(55);

				dir=STOP;
				GoMobile(dir);

				OnTest();
				Sleep(5000);
				headPosX=0;
				headPosY=-4000;
				SendHead();
				RightArm[0]=-5000;
				RightArm[1]=6000;
				RightArm[2]=2000;
				RightArm[3]=-10000+armR;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=-6000;
				LeftArm[2]=-2000;
				LeftArm[3]=10000+armL;
				LeftArm[4]=0;
				LeftArm[5]=0;

				SendRightArm();
				SendLeftArm();
				OnHandInit();
				OnHandstp();

				SetTimer(55,5000,NULL);

				break;
			case 9:
				KillTimer(55);

				dir=STOP;
				GoMobile(dir);

				OnTest3();
				Sleep(5000);
				headPosX=0;
				headPosY=-4000;
				SendHead();
				RightArm[0]=-5000;
				RightArm[1]=6000;
				RightArm[2]=2000;
				RightArm[3]=-10000+armR;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=-6000;
				LeftArm[2]=-2000;
				LeftArm[3]=10000+armL;
				LeftArm[4]=0;
				LeftArm[5]=0;

				SendRightArm();
				SendLeftArm();
				OnHandInit();
				OnHandstp();

				SetTimer(55,5000,NULL);

				break;
			case 17:
			case 18:
			case 19:
				break;
			case 20:
				KillTimer(55);

				dir=STOP;
				GoMobile(dir);

				OnNiceMotion1();
				Sleep(5000);
				headPosX=0;
				headPosY=-4000;
				SendHead();
				RightArm[0]=-5000;
				RightArm[1]=6000;
				RightArm[2]=2000;
				RightArm[3]=-10000+armR;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=-6000;
				LeftArm[2]=-2000;
				LeftArm[3]=10000+armL;
				LeftArm[4]=0;
				LeftArm[5]=0;

				SendRightArm();
				SendLeftArm();
				OnHandInit();
				OnHandstp();

				SetTimer(55,5000,NULL);

				break;
			case 21:
				KillTimer(55);

				dir=STOP;
				GoMobile(dir);

				OnNiceMotion2();
				Sleep(5000);
				headPosX=0;
				headPosY=-4000;
				SendHead();

				RightArm[0]=-5000;
				RightArm[1]=6000;
				RightArm[2]=2000;
				RightArm[3]=-10000+armR;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=-6000;
				LeftArm[2]=-2000;
				LeftArm[3]=10000+armL;
				LeftArm[4]=0;
				LeftArm[5]=0;

				SendRightArm();
				SendLeftArm();
				OnHandInit();
				OnHandstp();

				SetTimer(55,5000,NULL);

				break;
			case 22:
				KillTimer(55);

				dir=STOP;
				GoMobile(dir);

				OnNiceMotion3();
				Sleep(5000);
				headPosX=0;
				headPosY=-4000;
				SendHead();

				RightArm[0]=-5000;
				RightArm[1]=6000;
				RightArm[2]=2000;
				RightArm[3]=-10000+armR;
				RightArm[4]=0;
							
				LeftArm[0]=5000;
				LeftArm[1]=-6000;
				LeftArm[2]=-2000;
				LeftArm[3]=10000+armL;
				LeftArm[4]=0;
				LeftArm[5]=0;

				SendRightArm();
				SendLeftArm();
				OnHandInit();
				OnHandstp();

				SetTimer(55,5000,NULL);

				break;
			}
		}
		else if(bWally)
		{
			int armR,armL;

			armR = rand()%6000 - 3000;
			armL = rand()%6000 - 3000;

			OnHandInit();
			OnHandstp();
			//서있는 멋진자세

			RightArm[0]=-5000;
			RightArm[1]=0;
			RightArm[2]=2000;
			RightArm[3]=-10000+armR;
			RightArm[4]=0;
						
			LeftArm[0]=5000;
			LeftArm[1]=0;
			LeftArm[2]=-2000;
			LeftArm[3]=10000+armL;
			LeftArm[4]=0;
			
			SendRightArm();
			SendLeftArm();

			randnum = rand()%24;
			switch(randnum){
			case 0:
				::SendMessage(HWND_BROADCAST,MsgEmotion,34,3000);
				SendTTS("가지 마세요");
//				Speak(1);
				break;
			case 1:
				::SendMessage(HWND_BROADCAST,MsgEmotion,45,3000);
				SendTTS("숨지 말아요");
//				Speak(1);
				break;
			case 2:
				::SendMessage(HWND_BROADCAST,MsgEmotion,2060,3000);
				SendTTS("같이 놀아줘요");
//				Speak(1);
				break;
			case 3:
				::SendMessage(HWND_BROADCAST,MsgEmotion,3070,3000);
				SendTTS("같이 놀아요");
//				Speak(1);
				break;
			case 4:
				::SendMessage(HWND_BROADCAST,MsgEmotion,6080,3000);
				SendTTS("재미있다");
//				Speak(1);
				break;
			case 5:
				::SendMessage(HWND_BROADCAST,MsgEmotion,520,3000);
				SendTTS("아이, 심심해");
//				Speak(2);
				break;
			case 6:
				::SendMessage(HWND_BROADCAST,MsgEmotion,3040,3000);
				SendTTS("으음");
				break;
			case 7:
				::SendMessage(HWND_BROADCAST,MsgEmotion,4080,3000);
				SendTTS("아하!");
//				Speak(1);
				break;
			case 8:
				::SendMessage(HWND_BROADCAST,MsgEmotion,7080,3000);
				SendTTS("놀아줘요");
//				Speak(1);
				break;
			case 9:
				::SendMessage(HWND_BROADCAST,MsgEmotion,9595,3000);
				SendTTS("신난다");
//				Speak(1);
				break;
			case 10:
				::SendMessage(HWND_BROADCAST,MsgEmotion,3080,3000);
				SendTTS("아미에요");
//				Speak(1);
				break;
			case 11:
				::SendMessage(HWND_BROADCAST,MsgEmotion,4083,3000);
				SendTTS("나는 휴먼로봇");
//				Speak(2);
				break;
			default:
				break;
			}

		}
	}
	ReleaseDC(pDC);		

	CView::OnTimer(nIDEvent);
}


void CHRView::OnHandstp() 
{
//	OnIdleDisable();
	// TODO: Add your control notification handler code here
	RightHand[0]=0x0;
	RightHand[1]=0x0;
	RightHand[2]=0x0;
	RightHand[3]=0x0;
	RightHand[4]=0x0;
	RightHand[5]=0x0;

	SendRightHand();
	
	LeftHand[0]=0x0;
	LeftHand[1]=0x0;
	LeftHand[2]=0x0;
	LeftHand[3]=0x0;
	LeftHand[4]=0x0;
	LeftHand[5]=0x0;
	SendLeftHand();
	
}

void CHRView::OnTest3() 
{
//	OnIdleDisable();
	// TODO: Add your control notification handler code here
	::SendMessage(HWND_BROADCAST,MsgEmotion,4078,3000);
	randnum = rand()%4;
	switch(randnum){
	case 0:
		SendTTS("멋진 모습");
		Speak(2);
		break;
	case 1:
		SendTTS("자, 보세요");
		Speak(2);
		break;
	case 2:
		SendTTS("이건 어때요?");
		Speak(2);
		break;
	case 3:
		SendTTS("따라해 보세요");
		Speak(2);
		break;
	}

	LeftArm[1] = -25000;
	RightArm[1] = 25000;

	SendArm();
	Sleep(2000);

	headPosX=0;
	headPosY=-3500;
	SendHead();
	Sleep(1000);
	//청소 준비자세 
	RightArm[0]=-40000;
	RightArm[1]=5000;
	RightArm[2]=25000;
	RightArm[3]=-50000;
	RightArm[4]=0;
				
	LeftArm[0]=40000;
	LeftArm[1]=-5000;
	LeftArm[2]=-25000;
	LeftArm[3]=50000;
	LeftArm[4]=0;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();
	Sleep(4000);
	//손벌려서 끝 약간 구부리기 
	RightHand[0]=0x16;
	RightHand[1]=0x14;
	RightHand[2]=0x11;
	RightHand[3]=0x15;
	RightHand[4]=0xc;
	RightHand[5]=0x15;

	SendRightHand();
	
	LeftHand[0]=0x10;
	LeftHand[1]=0x10;
	LeftHand[2]=0x14;
	LeftHand[3]=0x11;
	LeftHand[4]=0x1b;
	LeftHand[5]=0x11;
	SendLeftHand();

	::SendMessage(HWND_BROADCAST,MsgEmotion,6083,3000);
	randnum = rand()%2;
	switch(randnum){
	case 0:
		SendTTS("이쁘죠?");
		Speak(2);
		break;
	case 1:
		SendTTS("괜찮아요?");
		Speak(2);
		break;
	}
	
}

void CHRView::OnHandClose() 
{
//	OnIdleDisable();
	// TODO: Add your control notification handler code here
	RightHand[0]=0x16;
	RightHand[1]=0x14;
	RightHand[2]=0x1a;
	RightHand[3]=0x11;
	RightHand[4]=0xc;
	RightHand[5]=0x15;

	SendRightHand();
	
	LeftHand[0]=0x10;
	LeftHand[1]=0x10;
	LeftHand[2]=0xc;
	LeftHand[3]=0x15;
	LeftHand[4]=0x1b;
	LeftHand[5]=0x11;
	SendLeftHand();
	
}

void CHRView::OnWalking() 
{
	OnIdleDisable();
	walkingMode = true;
	walking=true;
	SetTimer(50,2000,NULL);
	headPosX=0;
	headPosY=-4000;
	SendHead();

	RightArm[0]=0;
	RightArm[1]=0;
	RightArm[2]=0;
	RightArm[3]=0;
	RightArm[4]=0;

	LeftArm[0]=0;
	LeftArm[1]=0;
	LeftArm[2]=0;
	LeftArm[3]=0;
	LeftArm[4]=0;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();

}

void CHRView::OnStopwalk() 
{
	// TODO: Add your control notification handler code here
	OnIdleDisable();
	walkingMode = false;
	KillTimer(50);
	RightArm[0]=0;
	RightArm[1]=0;
	RightArm[3]=0;
				
	LeftArm[0]=0;
	LeftArm[1]=0;
	LeftArm[3]=0;

	SendRightArm();
	SendLeftArm();
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

void CHRView::OnHandInit() 
{
	// TODO: Add your control notification handler code here
//	OnIdleDisable();
	RightHand[0]=0x11;
	RightHand[1]=0x11;
	RightHand[2]=0x11;
	RightHand[3]=0x15;
	RightHand[4]=0x15;
	RightHand[5]=0x11;
	SendRightHand();

	LeftHand[0]=0x15;
	LeftHand[1]=0x14;
	LeftHand[2]=0x14;
	LeftHand[3]=0x11;
	LeftHand[4]=0x12;
	LeftHand[5]=0x15;
	SendLeftHand();
	
}

void CHRView::OnClean() 
{
	// TODO: Add your control notification handler code here
/*	RightHand[0]=0x11;
	RightHand[1]=0x17;
	RightHand[2]=0x11;
	RightHand[3]=0x15;
	RightHand[4]=0x15;
	RightHand[5]=0x11;

	SendRightHand();
	
	LeftHand[0]=0x15;
	LeftHand[1]=0xd;
	LeftHand[2]=0x14;
	LeftHand[3]=0x11;
	LeftHand[4]=0x12;
	LeftHand[5]=0x15;
	SendLeftHand();*/

//	OnHandClose();
	RightHand[0]=0x19;
	RightHand[1]=0x13;
	RightHand[2]=0x1b;
	RightHand[3]=0x0f;
	RightHand[4]=0xb;
	RightHand[5]=0x17;

	SendRightHand();
	
	LeftHand[0]=0x0f;
	LeftHand[1]=0x11;
	LeftHand[2]=0xb;
	LeftHand[3]=0x16;
	LeftHand[4]=0x1c;
	LeftHand[5]=0x10;
	SendLeftHand();

	cleanMode = true;
	SetTimer(55,1000,NULL);

	headPosX=3500;
	headPosY=-1500;
	SendHead();
	
	OnWally();
}

void CHRView::OnPlayalone() 
{
	// TODO: Add your control notification handler code here
	OnHandClose();

	playAloneMode = true;

	headPosX=0;
	headPosY=-4000;
	SendHead();

	RightArm[0]=-5000;
	RightArm[1]=6000;
	RightArm[2]=2000;
	RightArm[3]=-10000;
	RightArm[4]=0;
				
	LeftArm[0]=5000;
	LeftArm[1]=-6000;
	LeftArm[2]=-2000;
	LeftArm[3]=10000;
	LeftArm[4]=0;
	LeftArm[5]=0;
	
	SendRightArm();
	SendLeftArm();

	SetTimer(55,5000,NULL);

	OnWally();
}

void CHRView::Speak(int t) 
{
	for(i=0;i<t;i++){
		pwm[4]=0x13;
		SendFace();
		Sleep(500);
		pwm[4]=0x15;
		SendFace();
		Sleep(300);
	}
/*	pwm[4]=0x13;
	SendFace();
	mouthOpen = true;
	SetTimer(2,700,NULL);
	SetTimer(56,t*1000,NULL);*/
}

void CHRView::OnSeeHere()
{
	bSeeHere = true;
	readMode = true;

	visionCount=0;
	*lpVisionCommand = 6;

	headPosX=0;
	headPosY=-4000;
	SendHead();
	
}
void CHRView::SeeHere()
{
	//Vision Initialize
	if(visionCount==0)
	{
		scanCount=0;

		headPosX=0;
		headPosY=-4000;
		//headPosY=0;
		SendHead();

		headDir=STOP;
		TRACE("\nINIT VIS");
	}

	Vstereo[0].empty=1;
	memcpy(&Vstereo[0],lpVisionData,sizeof(VISION_DATA));


	//만약 움직임이 있으면,
	if(!Vstereo[0].empty)
	{
		randnum = rand() % 6;
		switch(randnum){
		case 0:
			::SendMessage(HWND_BROADCAST,MsgEmotion,4075,3000);
			SendTTS("오호");
			break;
		case 1:
			::SendMessage(HWND_BROADCAST,MsgEmotion,2075,3000);
			SendTTS("아아");
			break;
		case 2:
			::SendMessage(HWND_BROADCAST,MsgEmotion,2073,3000);
			SendTTS("하");
			break;
		case 3:
			::SendMessage(HWND_BROADCAST,MsgEmotion,4070,3000);
			SendTTS("앗싸");
			break;
		default:
			break;
		}

		if(Vstereo[0].MC.x<80 && Vstereo[0].MC.y < 60)
		{
			headPosX=-2000;
			headPosY=-4500;
			SendHead();
			Sleep(2000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			Sleep(2000);
		}
		else if(Vstereo[0].MC.x>80 && Vstereo[0].MC.y < 60)
		{
			headPosX=2000;
			headPosY=-4500;
			SendHead();
			Sleep(2000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			Sleep(2000);
		}
		else if(Vstereo[0].MC.x<80 && Vstereo[0].MC.y > 60)
		{
			headPosX=-2000;
			headPosY=-3500;
			SendHead();
			Sleep(2000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			Sleep(2000);
		}
		else if(Vstereo[0].MC.x>80 && Vstereo[0].MC.y > 60)
		{
			headPosX=2000;
			headPosY=-3500;
			SendHead();
			Sleep(2000);
			headPosX=0;
			headPosY=-4000;
			SendHead();
			Sleep(2000);
		}
		
	}

	visionCount++;

}

void CHRView::OnNiceMotion1() 
{
	// TODO: Add your control notification handler code here
	::SendMessage(HWND_BROADCAST,MsgEmotion,4575,3000);
	randnum = rand()%4;
	switch(randnum){
	case 0:
		SendTTS("멋진 모습");
		Speak(2);
		break;
	case 1:
		SendTTS("오예");
		Speak(2);
		break;
	case 2:
		SendTTS("여길봐요");
		Speak(2);
		break;
	case 3:
		SendTTS("따라해 보세요");
		Speak(2);
		break;
	}


	LeftArm[1] = -25000;
	RightArm[1] = 25000;

	SendArm();
	Sleep(2000);

	headPosX=3500;
	headPosY=-4500;
	SendHead();
	Sleep(1000);

	//멋진 모습
	RightArm[0]=-55000;
	RightArm[1]=30000;
	RightArm[2]=10000;
	RightArm[3]=-20000;
	RightArm[4]=0;
				
	LeftArm[0]=0;
	LeftArm[1]=-15000;
	LeftArm[2]=-40000;
	LeftArm[3]=36000;
	LeftArm[4]=0;
	LeftArm[5]=0;	

	SendRightArm();
	SendLeftArm();
	Sleep(4000);
	//손벌려서 끝 약간 구부리기 
	RightHand[0]=0x16;
	RightHand[1]=0x14;
	RightHand[2]=0x11;
	RightHand[3]=0x15;
	RightHand[4]=0xc;
	RightHand[5]=0x15;

	SendRightHand();
	
	LeftHand[0]=0x15;
	LeftHand[1]=0xd;
	LeftHand[2]=0x14;
	LeftHand[3]=0x11;
	LeftHand[4]=0x12;
	LeftHand[5]=0x15;
	SendLeftHand();

	::SendMessage(HWND_BROADCAST,MsgEmotion,7086,3000);
	randnum = rand()%2;
	switch(randnum){
	case 0:
		SendTTS("짜잔");
		Speak(1);
		break;
	case 1:
		SendTTS("멋지죠?");
		Speak(2);
		break;
	}
	
}

void CHRView::OnNiceMotion2() 
{
	// TODO: Add your control notification handler code here
	::SendMessage(HWND_BROADCAST,MsgEmotion,5080,3000);
	randnum = rand()%4;
	switch(randnum){
	case 0:
		SendTTS("멋진 모습");
		Speak(2);
		break;
	case 1:
		SendTTS("정말 멋져요");
		Speak(2);
		break;
	case 2:
		SendTTS("여길봐요");
		Speak(2);
		break;
	case 3:
		SendTTS("따라해 보세요");
		Speak(2);
		break;

	}

	LeftArm[1] = -25000;
	RightArm[1] = 25000;

	SendArm();
	Sleep(2000);

	headPosX=0;
	headPosY=-3500;
	SendHead();
	Sleep(1000);

	//멋진 모습
	RightArm[0]=0;
	RightArm[1]=15000;
	RightArm[2]=40000;
	RightArm[3]=-36000;
	RightArm[4]=0;
				
	LeftArm[0]=0;
	LeftArm[1]=-15000;
	LeftArm[2]=-40000;
	LeftArm[3]=36000;
	LeftArm[4]=0;
	LeftArm[5]=0;	

	SendRightArm();
	SendLeftArm();
	Sleep(4000);
	//손벌려서 끝 약간 구부리기 

	OnHandReset();

	::SendMessage(HWND_BROADCAST,MsgEmotion,7086,3000);

	randnum = rand()%3;
	switch(randnum){
	case 0:
		SendTTS("짜짠");
		Speak(1);
		break;
	case 1:
		SendTTS("멋지죠?");
		Speak(2);
		break;
	case 2:
		SendTTS("나 어때요?");
		Speak(2);
		break;
	}
	
}

void CHRView::OnNiceMotion3() 
{
	// TODO: Add your control notification handler code here
	OnHandInit();

	::SendMessage(HWND_BROADCAST,MsgEmotion,5078,3000);

	randnum = rand()%4;
	switch(randnum){
	case 0:
		SendTTS("멋진 모습");
		Speak(2);
		break;
	case 1:
		SendTTS("자, 보세요");
		Speak(2);
		break;
	case 2:
		SendTTS("이건 어때요?");
		Speak(2);
		break;
	case 3:
		SendTTS("따라해 보세요");
		Speak(2);
		break;
	}

	LeftArm[1] = -25000;
	RightArm[1] = 25000;

	SendArm();
	Sleep(2000);

	headPosX=-500;
	headPosY=-3500;
	SendHead();
	Sleep(1000);

	RightArm[0]=0;
	RightArm[1]=15000;
	RightArm[2]=40000;
	RightArm[3]=-36000;
	RightArm[4]=0;
				
	LeftArm[0]=40000;
	LeftArm[1]=-5000;
	LeftArm[2]=-25000;
	LeftArm[3]=50000;
	LeftArm[4]=0;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();
	Sleep(4000);
	//손벌려서 끝 약간 구부리기 
	RightHand[0]=0x16;
	RightHand[1]=0x14;
	RightHand[2]=0x11;
	RightHand[3]=0x15;
	RightHand[4]=0xc;
	RightHand[5]=0x15;

	SendRightHand();
	
	LeftHand[0]=0x10;
	LeftHand[1]=0x10;
	LeftHand[2]=0x14;
	LeftHand[3]=0x11;
	LeftHand[4]=0x1b;
	LeftHand[5]=0x11;
	SendLeftHand();

	::SendMessage(HWND_BROADCAST,MsgEmotion,7080,3000);
	randnum = rand()%2;
	switch(randnum){
	case 0:
		SendTTS("이쁘죠?");
		Speak(2);
		break;
	case 1:
		SendTTS("괜찮아요?");
		Speak(2);
		break;
	}
	
}

void CHRView::OnLove()
{
	::SendMessage(HWND_BROADCAST,MsgEmotion,9999,13000);

	RightArm[1]=20000;
	LeftArm[1]=-20000;
	SendRightArm();
	SendLeftArm();
	Sleep(1000);

	RightArm[0]=-25000;
	RightArm[1]=5000;
	RightArm[2]=35000;
	RightArm[3]=-40000;
	RightArm[4]=-45000;
				
	LeftArm[0]=30000;
	LeftArm[1]=-20000;
	LeftArm[2]=-40000;
	LeftArm[3]=40000;
	LeftArm[4]=60000;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();

	Sleep(2000);

	pwm[4]=0x13;
	SendFace();
	SendTTS("당신을, 사랑해요");

	Sleep(1000);

	pwm[4]=0x15;
	SendFace();

	RightArm[3]=-10000;
	LeftArm[3]=10000;
	SendRightArm();
	SendLeftArm();
	Sleep(500);

	pwm[4]=0x13;
	SendFace();

	RightArm[0]=-40000;
	RightArm[1]=0;
	RightArm[2]=40000;
	RightArm[4]=-70000;
				
	LeftArm[0]=40000;
	LeftArm[1]=0;
	LeftArm[2]=-40000;
	LeftArm[4]=70000;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();

	Sleep(500);
	pwm[4]=0x15;
	SendFace();

	Sleep(4500);

	InitPos();
}

void CHRView::OnReadReady()
{
	::SendMessage(HWND_BROADCAST,MsgEmotion,2080,7000);
	SendTTS("읽어 볼까요?");

	InitPos();

	*lpVisionCommand = 6;
	headPosX=0;
//	headPosY=-1500;
	headPosY=-3000;
	SendHead();

/*	RightArm[0]=-15000;
	RightArm[1]=0;
	RightArm[3]=-28000;
	RightArm[4]=-45000;
				
	LeftArm[0]=15000;
	LeftArm[1]=-0;
	LeftArm[3]=28000;
	LeftArm[4]=45000;
	LeftArm[5]=0;

	SendRightArm();
	SendLeftArm();
	Sleep(1000);

	RightArm[2]=15000;
	LeftArm[2]=-15000;
	SendRightArm();
	SendLeftArm();

	RightHand[0]=0x12;
	RightHand[1]=0x17;
	RightHand[2]=0x11;
	RightHand[3]=0x11;
	RightHand[4]=0x15;
	RightHand[5]=0x15;

	SendRightHand();
	
	LeftHand[0]=0x14;
	LeftHand[1]=0xd;
	LeftHand[2]=0x14;
	LeftHand[3]=0x15;
	LeftHand[4]=0x12;
	LeftHand[5]=0x11;
	SendLeftHand();
*/

}
/*잡기
	RightHand[0]=0x16;
	RightHand[1]=0x14;
	RightHand[2]=0x1a;
	RightHand[3]=0x11;
	RightHand[4]=0xc;
	RightHand[5]=0x15;

	SendRightHand();
	
	LeftHand[0]=0x10;
	LeftHand[1]=0x10;
	LeftHand[2]=0xc;
	LeftHand[3]=0x15;
	LeftHand[4]=0x1b;
	LeftHand[5]=0x11;
	SendLeftHand();
펴기
	RightHand[0]=0x11;
	RightHand[1]=0x17;
	RightHand[2]=0x11;
	RightHand[3]=0x15;
	RightHand[4]=0x15;
	RightHand[5]=0x11;

	SendRightHand();
	
	LeftHand[0]=0x15;
	LeftHand[1]=0xd;
	LeftHand[2]=0x14;
	LeftHand[3]=0x11;
	LeftHand[4]=0x12;
	LeftHand[5]=0x15;
	SendLeftHand();
*/
void CHRView::OnReading()
{
	*lpVisionCommand = 5;
}

void CHRView::ReturnRead(int r)
{
	switch(r)
	{
//	case 0:
//		::SendMessage(HWND_BROADCAST,MsgEmotion,2020,4000);
//		SendTTS("잘 모르겠어요");
//		Speak(2);
//		break;
	case 1:
		::SendMessage(HWND_BROADCAST,MsgEmotion,2080,4000);
		SendTTS("아미,,엇 나네?");
		Speak(2);
		break;
	case 2:
		::SendMessage(HWND_BROADCAST,MsgEmotion,1070,4000);
		SendTTS("휴먼로봇");
		Speak(2);
		break;

	case 3:
		::SendMessage(HWND_BROADCAST,MsgEmotion,1060,4000);
		SendTTS("카이스트");
		Speak(2);
		break;

	case 4:
		::SendMessage(HWND_BROADCAST,MsgEmotion,2090,4000);
		SendTTS("인공지능");
		Speak(2);
		break;

	case 5:
		::SendMessage(HWND_BROADCAST,MsgEmotion,3060,4000);
		SendTTS("캐어투");
		Speak(2);
		break;

	case 6:
		::SendMessage(HWND_BROADCAST,MsgEmotion,3070,3000);
		SendTTS("코엑스");
		Speak(1);
		break;

	case 7:
		::SendMessage(HWND_BROADCAST,MsgEmotion,2070,4000);
		SendTTS("가나다라");
		Speak(2);
		break;

	case 8:
		::SendMessage(HWND_BROADCAST,MsgEmotion,2080,4000);
		SendTTS("대한민국");
		Speak(2);
		break;

	case 9:
		::SendMessage(HWND_BROADCAST,MsgEmotion,1080,3000);
		SendTTS("로보트");
		Speak(1);
		break;

	case 10:
		::SendMessage(HWND_BROADCAST,MsgEmotion,2080,3000);
		SendTTS("전시장");
		Speak(1);
		break;

	case 11:
		::SendMessage(HWND_BROADCAST,MsgEmotion,6090,4000);
		SendTTS("사랑해요");
		Speak(2);
		break;

	case 12:
		::SendMessage(HWND_BROADCAST,MsgEmotion,3060,4000);
		SendTTS("한글읽기");
		Speak(2);
		break;

	case 13:
		::SendMessage(HWND_BROADCAST,MsgEmotion,3070,3000);
		SendTTS("인사해");
		Speak(1);
		break;

	case 14:
		::SendMessage(HWND_BROADCAST,MsgEmotion,3030,3000);
		SendTTS("화장실");
		Speak(1);
		break;

	default:
		::SendMessage(HWND_BROADCAST,MsgEmotion,1010,5000);
		SendTTS("글자판을 똑바로 놓아주세요");
		Speak(3);
		break;
	}
}
