// eFisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eFis.h"
#include "eFisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// for shared memory
int dwSize=3000;
HANDLE hVTB;
PUCHAR lpSharedMemory;
PUCHAR lpVisionCommand;
PUCHAR lpVisionData;

CString g_strTrackingID;

typedef struct {
    int x,y;
} PV;                   /* Position Vector in Image   */

struct VISION_DATA {
	int		bx,by;          // FACE x,y.
	int		minx,miny;
	int		maxx,maxy;
	int BX,BY,TX,TY;      /* Minimum box containing region */
	unsigned char empty;
	int			area;		/* Mass of Area */
	PV			MC;			/* Center of Mass  */
	short int 	BL;			/* Contour Length */
	int 		count;		/* count of processing similiar to above */
	double 		sum_dis; 	/* Sum of Distance from MC to boundary */
	double 		mean_dis;	/* Mean of Distance from MC to boundary */
	double 		variance_dis;/* Variance of Distance from MC to boundary */
	double 		mean_var;	/* mean / sqrt of variance */
	double 		periment;	/* Square of periment / Area */
	double		eccentricity;/* max_distance / min_distance */

	int 		ellipse_success; /* whether ellipse or not */
	double 		moment_rr;	/* moment rr */
	double		moment_rc;	/* moment rc */
	double		moment_cc;	/* moment cc */
	double		major_axis;	/* major axis length */
	double		minor_axis;	/* minor axis length */
	double		ellipse_area; /* area of ellipse */
	double 		ratio;		/* area / ellipse */
	double		orientation;/* orientation of major axis */
	double		dist;
};


VISION_DATA V;// OBJECT 인식 DATA

#define		MAX_VERIFY_INPUT_COUNT		3
#define		MAX_VERIFY_TRY_COUNT		10


#define		PROC_TERMINATED			100
#define		PROC_FEATURE_EXTRACT	101
#define		PROC_ENROLL				102
#define		PROC_VERIFY				103
#define		PROC_RECOGNIZE			104	
#define		PROC_TRACKING			105
#define		PROC_PHOTOCHECK			106
#define		PROC_TUNNING			107


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

/////////////////////////////////////////////////////////////////////////////
// CEFisDlg dialog

CEFisDlg::CEFisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEFisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEFisDlg)
	m_etOutput = _T("");
	m_nPersonListCnt = 0;
	m_strID = _T("");
	m_bIsTracking = TRUE;
	m_bIsPhotoCheck = FALSE;
	m_bIsRealTimeRecogn = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Init member variable
	m_pDib = NULL;
	m_pPersonListArray = NULL;
	m_nMaxTryCnt = 0;
	m_nMaxBlobCnt = 0;

	m_nProcessID = PROC_TRACKING;
	m_chMsg[0] = 'l';	m_chMsg[1] = ASC_NUL;

	m_ptLeft = m_ptRight = CPoint(0,0);
	m_strTrackingID = "Who?";
	g_strTrackingID = "모르겠어요";
	
	srand( (unsigned)time( NULL ) );
}


void CEFisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEFisDlg)
	DDX_Control(pDX, IDC_EDIT_DEBUG, m_dbgEdit);
	DDX_Control(pDX, IDC_RECOGNIZE, m_btRecognize);
	DDX_Control(pDX, IDC_PIC_SMALL4, m_picSmall4);
	DDX_Control(pDX, IDC_PIC_SMALL5, m_picSmall5);
	DDX_Control(pDX, IDC_PIC_SMALL3, m_picSmall3);
	DDX_Control(pDX, IDC_PIC_SMALL2, m_picSmall2);
	DDX_Control(pDX, IDC_PIC_SMALL1, m_picSmall1);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_SAVE_STILL_IMAGE, m_btSaveStillImage);
	DDX_Control(pDX, IDC_ENROLLMENT, m_btEnrollment);
	DDX_Control(pDX, IDC_DETECT_FACE, m_btDetectFace);
	DDX_Control(pDX, IDC_LB_IMAGE_TITLE, m_lbImageTitle);
	DDX_Control(pDX, IDC_VERIFY, m_btVerify);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_PIC_VERIFY_FACE, m_picVerifyFace);
	DDX_Control(pDX, IDC_PIC_MAIN, m_picMain);
	DDX_Control(pDX, IDC_LB_PERSON, m_lbPerson);
	DDX_Control(pDX, IDC_INITIALIZE, m_btInitialize);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_etOutput);
	DDX_Text(pDX, IDC_PERSON_LIST_CNT, m_nPersonListCnt);
	DDX_Text(pDX, IDC_ID, m_strID);
	DDX_Check(pDX, IDC_TRACKING_EYES, m_bIsTracking);
	DDX_Check(pDX, IDC_PHOTO_CHECK, m_bIsPhotoCheck);
	DDX_Check(pDX, IDC_REAL_TIME_RECOGN, m_bIsRealTimeRecogn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEFisDlg, CDialog)
	//{{AFX_MSG_MAP(CEFisDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_INITIALIZE, OnInitialize)
	ON_BN_CLICKED(IDC_VERIFY, OnVerify)
	ON_LBN_SELCHANGE(IDC_LB_PERSON, OnSelchangeLbPerson)
	ON_LBN_SELCHANGE(IDC_LB_IMAGE_TITLE, OnSelchangeLbImageTitle)
	ON_BN_CLICKED(IDC_DETECT_FACE, OnDetectFace)
	ON_BN_CLICKED(IDC_ENROLLMENT, OnEnrollment)
	ON_BN_CLICKED(IDC_SAVE_STILL_IMAGE, OnSaveStillImage)
	ON_LBN_DBLCLK(IDC_LB_PERSON, OnDblclkLbPerson)
	ON_BN_CLICKED(IDC_ETRI_LOGO, OnEtriLogo)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_TRACKING_EYES, OnTrackingEyes)
	ON_BN_CLICKED(IDC_RECOGNIZE, OnRecognize)
	ON_BN_CLICKED(IDC_REAL_TIME_RECOGN, OnRealTimeRecogn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Callback Func

LRESULT CEFisDlg::OnFrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{	
	// Get 1 frame from Cam
	if (m_pDib) delete m_pDib;
	BITMAPINFOHEADER *pBi = (BITMAPINFOHEADER *) m_VideoTool.GetBitmapInfo();
	m_pDib = new CDib(pBi->biWidth, pBi->biHeight, pBi->biBitCount, NULL, lpVHdr->lpData);  
	((CMyDib *)m_pDib)->PaintDibOnPic(&m_picMain, FALSE);

	ShowCaptureTag(&m_picMain);
	ProcessTrigger();

	return 0l;
}


void CEFisDlg::ProcessTrigger()
{
	switch(m_nProcessID)
	{
		case PROC_FEATURE_EXTRACT:
			ExtractFeature();
			break;

		case PROC_ENROLL:
			Enroll();
			break;

		case PROC_VERIFY:
			Verify();
			break;

		case PROC_RECOGNIZE:
			Recognize();
			break;

		case PROC_TRACKING:
			Tracking();
			break;

		case PROC_PHOTOCHECK:
			PhotoCheck();
			break;

		case PROC_TUNNING:
			Tunning();
			break;

		case PROC_TERMINATED:
			break;
	}
}


void CEFisDlg::ShowCaptureTag(CStatic *pPic)
{
	static DWORD dwFrameCnt = 0;

	UpdateData(TRUE);
	
	CDC *pDC = pPic->GetDC();

	// 환경 설정 중일 경우
	if(m_nProcessID == PROC_TUNNING && (dwFrameCnt % 4) > 1)	
	{
		pDC->SetTextColor(RGB(0,0,255));
		pDC->TextOut(0, 0, "Environment setting...");
	}

	// Tracking 중이며 Real Time 인식을 할 경우
	if(m_nProcessID == PROC_TRACKING && m_ptLeft != CPoint(0,0) && m_bIsRealTimeRecogn)
	{
		// 얼굴 출력
		CPen FacePen, *pOldPen;
		CDC *pDC = m_picMain.GetDC();
		pDC->SelectStockObject(NULL_BRUSH);
		
		FacePen.CreatePen(PS_SOLID, 2, RGB(0,0,255));
		pOldPen=(CPen *)pDC->SelectObject(&FacePen);
		int nDist = (m_ptRight.x - m_ptLeft.x);
		CRect rt = CRect(m_ptLeft.x-(int)(0.6*nDist), m_ptLeft.y-(int)(1.0*nDist), m_ptRight.x+(int)(0.6*nDist), m_ptLeft.y+(int)(1.6*nDist));
		//pDC->Rectangle(rt);
		pDC->Ellipse(rt);
		FacePen.DeleteObject();

		// 트래킹 결과 아이디 출력
		pDC->SetBkColor(RGB(0, 0, 255));
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(m_ptLeft.x-(int)(0.5*nDist), m_ptLeft.y-(int)(1.0*nDist)-10, m_strTrackingID);
	}

	// Error code
	CString strNotice;
	strNotice.Format("%c", Face_GetDetectState());
	pDC->SetBkColor(RGB(255, 255, 255));
	pDC->SetTextColor(RGB(0,0,255));
	pDC->TextOut(312, 225, strNotice);

/*
	// Etri logo
	pDC->SetTextColor(RGB(250,250,0));
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(0, 220, "● ETRI Proprietary");
*/
	ReleaseDC(pDC);
	
	
	dwFrameCnt++;
}


/////////////////////////////////////////////////////////////////////////////
// CEFisDlg message handlers

BOOL CEFisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//----------------------------
	//	User Interface
	//----------------------------

	// Buttons
	m_btInitialize.SetIcon(IDI_ICON1);
	m_btInitialize.SetColor(CButtonST::BTNST_COLOR_BK_IN, GetSysColor(COLOR_INFOBK));
	m_btInitialize.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(220, 230, 240));
	m_btInitialize.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(220, 230, 240));
	
	m_btDetectFace.SetIcon(IDI_ICON1);
	m_btDetectFace.SetColor(CButtonST::BTNST_COLOR_BK_IN, GetSysColor(COLOR_INFOBK));
	m_btDetectFace.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(220, 230, 240));
	m_btDetectFace.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(220, 230, 240));

	m_btSaveStillImage.SetIcon(IDI_ICON1);
	m_btSaveStillImage.SetColor(CButtonST::BTNST_COLOR_BK_IN, GetSysColor(COLOR_INFOBK));
	m_btSaveStillImage.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(220, 230, 240));
	m_btSaveStillImage.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(220, 230, 240));

	m_btEnrollment.SetIcon(IDI_ICON2);
	m_btEnrollment.SetColor(CButtonST::BTNST_COLOR_BK_IN, GetSysColor(COLOR_INFOBK));
	m_btEnrollment.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(230, 220, 255));
	m_btEnrollment.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(230, 220, 255));

	m_btVerify.SetIcon(IDI_ICON2);
	m_btVerify.SetColor(CButtonST::BTNST_COLOR_BK_IN, GetSysColor(COLOR_INFOBK));
	m_btVerify.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(230, 220, 255));
	m_btVerify.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(230, 220, 255));

	m_btRecognize.SetIcon(IDI_ICON2);
	m_btRecognize.SetColor(CButtonST::BTNST_COLOR_BK_IN, GetSysColor(COLOR_INFOBK));
	m_btRecognize.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(230, 220, 255));
	m_btRecognize.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(230, 220, 255));

	// Static Text
	m_static1.SetBkTrans();
	m_static2.SetBkTrans();
	

	// 리스트 파일 로딩하기
	CString strHome = GetHomeDirectory();
	strHome = AddFinalBackslash(strHome);
	m_pPersonListArray = ReadPersonList(strHome + "..\\DB)Person\\PersonList.BDL");

	for(int i = 0; i < m_pPersonListArray->GetSize(); i++)
	{
		m_lbPerson.AddString(m_pPersonListArray->GetAt(i));
	}
	m_nPersonListCnt = m_pPersonListArray->GetSize();	// Person Count
	UpdateData(FALSE);

	// 모든 템플릿 로딩
	LoadAllTemplates();


	// Setting Cap mode 
	m_VideoTool.SetVideoOwner(this);
	if (!m_VideoTool._CreateCaptureView(this->m_hWnd))
	{
		AfxMessageBox("USB 카메라를 작동시킬 수 없습니다 !");
		ExitProcess(0); 
	}

	// RS232C Connect
	m_Com.SetComPort("COM1", 9600, 8, 0, 0);//com,bps,data,parity,stop
	if(!m_Com.OpenComPort())	OutputDebugString("RS232C Connect Failed !!");


	// Init 파일 로딩
	CString strInitPath = strHome + "..\\Init\\";
	if(!Face_Initialize(strInitPath))
	{
		AfxMessageBox("초기화가 실패하였습니다");
		return FALSE;
	}
	else
		DisplayThreshold(Face_GetLightWeight());		
	

	SetWindowPos(NULL, 500, 0, 0, 0, SWP_NOSIZE);

	// create memory-mapped file for shared memory
	hVTB=CreateFileMapping((HANDLE)0xffffffff,0,PAGE_READWRITE,0,
		dwSize,(const char*)"VTB_DATA");
	
	if(hVTB==0)
	{
		AfxMessageBox("Error Creat MM file!\n");
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
		AfxMessageBox("Error Creat View of MM file!\n");
		ExitProcess(0);
	}	
	
	*lpVisionCommand=0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/*----------------------------------------------------------
	등록된 모든 템플릿 로딩
 ----------------------------------------------------------*/
void CEFisDlg::LoadAllTemplates()
{
	CString strHome = GetHomeDirectory();
	strHome = AddFinalBackslash(strHome);
	m_TemplateArray.RemoveAll();
	for(int i = 0; i < m_pPersonListArray->GetSize(); i++)
	{
		CBlob *pTemplate = new CBlob;
		CString strTemplateName = strHome + "..\\DB)Person\\" + m_pPersonListArray->GetAt(i) + ".EPI";
		pTemplate->ReadFile(strTemplateName);
		m_TemplateArray.Add(pTemplate);
	}
}


void CEFisDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEFisDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
/*		CPaintDC dc(this);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap bitmap;
		BITMAP bm;
		bitmap.LoadBitmap(IDB_BG);

		bitmap.GetBitmap(&bm);
		CBitmap* pOld = MemDC.SelectObject(&bitmap);
		dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOld);
		MemDC.DeleteDC();
*/		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEFisDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEFisDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	// Memory Free
	if(m_pDib)				delete m_pDib;
	if(m_pPersonListArray)	delete m_pPersonListArray;
	m_BlobArray.RemoveAll();
	m_TemplateArray.RemoveAll();


	// Face Process Destroy
	Face_Destroy();

	// Destroy Capture Driver 
	m_VideoTool._DestroyCaptureView();	
	
	// RS232C
	m_Com.CloseConnection();

}



/********************************************************************************


					ListBox Control


 *********************************************************************************/


/*----------------------------------------------------------
	등록 인물 정보 로딩
 ----------------------------------------------------------*/
void CEFisDlg::OnSelchangeLbPerson() 
{

	UpdateData(TRUE);

	// Get Selected Person Index
	CString strID;
	int nIndex = m_lbPerson.GetCurSel();
	m_lbPerson.GetText(nIndex, strID);

	// Load Selected Person's Template
	CString strHome = GetHomeDirectory();
	strHome = AddFinalBackslash(strHome);

	CString strFileName = strHome + "..\\DB)Person\\" + strID + ".EPI";
	CBlob *pTemplate = new CBlob;
	pTemplate->ReadFile(strFileName);

	// Display Person's basic Info
	m_strID = strID;										// ID
	m_lbImageTitle.ResetContent();
	CString strTemp;
	for(int i = 0; i < 5; i++) {					// Face Image Title
		strTemp.Format("%d", i+1);
		m_lbImageTitle.AddString(strTemp);
	}
	CDib *pFaceDib = Face_GetEnrolledFace(pTemplate, 0);	// the first face
	((CMyDib *)pFaceDib)->PaintDibOnPic(&m_picVerifyFace, FALSE);


	delete pTemplate;
	delete pFaceDib;

	UpdateData(FALSE);
}

/*----------------------------------------------------------
	등록 인물의 SubImage 리스트 선택	
 ----------------------------------------------------------*/
void CEFisDlg::OnSelchangeLbImageTitle() 
{
	// Get Selected Person Index
	CString strID;
	int nIndex = m_lbPerson.GetCurSel();
	m_lbPerson.GetText(nIndex, strID);

	// Load Selected Person's Template
	CString strHome = GetHomeDirectory();
	strHome = AddFinalBackslash(strHome);
	CString strFileName = strHome + "..\\DB)Person\\" + strID + ".EPI";

	CBlob *pTemplate = new CBlob;
	pTemplate->ReadFile(strFileName);

	// Get Selected Face Image Index & Display
	nIndex = m_lbImageTitle.GetCurSel();
	CDib *pFaceDib = Face_GetEnrolledFace(pTemplate, nIndex);
	((CMyDib *)pFaceDib)->PaintDibOnPic(&m_picVerifyFace, FALSE);
	
	delete pTemplate;
	delete pFaceDib;
}


void CEFisDlg::OnDblclkLbPerson() 
{
	UpdateData(TRUE);

	// Get Selected Index
	CString strID;
	int nIndex = m_lbPerson.GetCurSel();
	m_lbPerson.GetText(nIndex, strID);

	CString strTemp;
	strTemp.Format("Want to delete %s data ?", strID);
	if(MessageBox(strTemp, NULL, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		// Delete Selected Person's Info 
		CString strHome = GetHomeDirectory();
		strHome = AddFinalBackslash(strHome);
		CString strFileName = strHome + "..\\DB)Person\\" + strID + ".EPI";
		DeleteFile("..\\DB)Person\\" + strID + ".EPI");
		DeleteFile("..\\DB)Person_FullShot\\" + strID + ".bmp");
		m_lbPerson.DeleteString(nIndex);
		m_lbImageTitle.ResetContent();

		// Modify PersonList & Save
		m_pPersonListArray->RemoveAll();
		for(int i = 0; i < m_lbPerson.GetCount(); i++)
		{
			m_lbPerson.GetText(i, strID);
			m_pPersonListArray->Add(strID);
		}
		WritePersonList(m_pPersonListArray, "..\\DB)Person\\PersonList.BDL");
		m_nPersonListCnt--;
		LoadAllTemplates();		// 템플릿 다시 로딩
	}
	
	UpdateData(FALSE);
}


/*----------------------------------------------------------
	다이얼로그 종료 방지
 ----------------------------------------------------------*/
BOOL CEFisDlg::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message == WM_KEYDOWN) 
	{
		if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
			return TRUE;
    }

	return CDialog::PreTranslateMessage(pMsg);
}

void CEFisDlg::DisplayThreshold(int nThres)
{
	CString strCaption;
	strCaption.Format("AMI - Face Detection & Recognition (%d)", nThres);
	SetWindowText(strCaption);
}


/*----------------------------------------------------------
	환경 설정
 ----------------------------------------------------------*/
void CEFisDlg::OnInitialize() 
{
	int nResult = AfxMessageBox("시스템 최적화를 위한 환경 설정을 하시겠습니까? \n (카메라 응시후 수초간 대기)", MB_YESNO);
	if(nResult == IDYES)	m_nProcessID = PROC_TUNNING;
}

void CEFisDlg::Tunning()
{
	int nMaxFindCnt = -1;
	if(Face_SetEnvironment(m_pDib, nMaxFindCnt))
	{
		UpdateData(TRUE);
		if(m_bIsTracking)	m_nProcessID = PROC_TRACKING;
		else				m_nProcessID = PROC_TERMINATED;
		DisplayThreshold(Face_GetLightWeight());

		CString strTemp;
		strTemp.Format("%d", nMaxFindCnt);
		AfxMessageBox("환경 설정이 완료 되었습니다.(" + strTemp + ")");
	}
}

/*----------------------------------------------------------
	눈 검출 성능 테스트
 ----------------------------------------------------------*/
void CEFisDlg::OnDetectFace() 
{
	RedrawWindow(CRect(0,300,1000,1000));
	CDib *pFaceDib = Face_GetFaceRegion(m_pDib);

	if(pFaceDib != NULL)	// success
	{
		((CMyDib *)pFaceDib)->PaintDibOnPic(&m_picSmall1, FALSE);
		delete pFaceDib;
	}

	else
	{
		AfxMessageBox("정면으로 카메라를 응시하세요 !");
		return;
	}

}

/*----------------------------------------------------------
	특징 추출
 ----------------------------------------------------------*/
void CEFisDlg::ExtractFeature()
{
	static int nTryCnt = 0;
	m_progress.DisplayEpochCnt(nTryCnt, m_nMaxTryCnt);
	
	nTryCnt++;

	if(nTryCnt > m_nMaxTryCnt)		// 모든 특징 추출 실패
	{
		nTryCnt = 0;
		UpdateData(TRUE);
		if(m_bIsTracking)	m_nProcessID = PROC_TRACKING;
		else				m_nProcessID = PROC_TERMINATED;
		AfxMessageBox("특징 추출이 실패하였습니다 !");
	}

	// 특징 추출
	CPoint ptTemp;
	CBlob *pBlob = Face_GetFeature(m_pDib, &ptTemp, &ptTemp);
	if(!pBlob)	return;
	else		m_BlobArray.Add(pBlob);

	// 중간 과정 디스플레이
	int nBlobCnt = m_BlobArray.GetSize();
	
	CDib *pDib = Face_GetFeatureFace(pBlob);
	CStatic *pPic;
	if(nBlobCnt == 1)		{ pPic = &m_picSmall1;	}
	else if(nBlobCnt == 2)	{ pPic = &m_picSmall2;	}
	else if(nBlobCnt == 3)	{ pPic = &m_picSmall3;	}
	else if(nBlobCnt == 4)	{ pPic = &m_picSmall4;	}
	else if(nBlobCnt == 5)	{ pPic = &m_picSmall5;	}
	((CMyDib *)pDib)->PaintDibOnPic(pPic, FALSE);
	delete pDib;

	if(nBlobCnt == m_nMaxBlobCnt)	// 모든 특징 추출 성공
	{
		nTryCnt = 0;
		m_progress.SetPos(0);
		m_nProcessID = m_nNextProcessID;	// 등록 or 인증
	}
}

/*----------------------------------------------------------
	등록 버튼 클릭
 ----------------------------------------------------------*/
void CEFisDlg::OnEnrollment() 
{

	RedrawWindow(CRect(0,300,1000,1000));
	UpdateData(TRUE);

	if(m_strID == "")	{
		AfxMessageBox("등록할 인물의 아이디를 입력하세요 !");
		return;
	}

	m_BlobArray.RemoveAll();
	m_nMaxTryCnt = 20;
	m_nMaxBlobCnt = 5;

	m_nProcessID = PROC_FEATURE_EXTRACT;
	m_nNextProcessID = PROC_ENROLL;

	
}

/*----------------------------------------------------------
	추출된 특징들 등록
 ----------------------------------------------------------*/
void CEFisDlg::Enroll()
{
	UpdateData(TRUE);

	CBlob *pTemplate = Face_EnrollFeatures(m_strID, m_BlobArray);

	// 템플릿 저장
	CString strHome = GetHomeDirectory();
	strHome = AddFinalBackslash(strHome);
	CString strPersonFileName = strHome + "..\\DB)Person\\" + m_strID + ".EPI";	// Personal filename
	pTemplate->WriteFile(strPersonFileName);
	delete pTemplate;

	// FullShot 한장 저장
	CString strFullShotPath = strHome +"..\\DB)Person_FullShot\\";
	CreateDirectory(strFullShotPath, NULL);
	CString strFullShotName= strFullShotPath + m_strID + ".bmp";
	m_pDib->SaveDib(strFullShotName);


	// 새로운 인물이 등록되었을 경우 인물 리스트 박스 수정
	if(m_lbPerson.FindString(0, m_strID) < 0)
	{
		m_pPersonListArray->Add(m_strID);
		CString strFileName = strHome + "..\\DB)Person\\PersonList.BDL";
		WritePersonList(m_pPersonListArray, strFileName);
		m_lbPerson.AddString(m_strID);		
		m_nPersonListCnt = m_nPersonListCnt+1;
		UpdateData(FALSE);

		LoadAllTemplates();		// 템플릿 다시 로딩
	}

	// 이미지 리스트 박스 수정
	m_lbPerson.SetCurSel(m_lbPerson.FindString(0, m_strID));
	m_lbImageTitle.ResetContent();
	CString strTemp;
	for(int i = 0; i < 5; i++) {
		strTemp.Format("%d", i+1);
		m_lbImageTitle.AddString(strTemp);
	}

	UpdateData(TRUE);
	if(m_bIsTracking)	m_nProcessID = PROC_TRACKING;
	else				m_nProcessID = PROC_TERMINATED;

	AfxMessageBox("등록이 완료되었습니다 !");
}

/*----------------------------------------------------------
	검증 버튼 클릭
 ----------------------------------------------------------*/
#include "ResultDlg.h"
void CEFisDlg::OnVerify() 
{

	RedrawWindow(CRect(0,300,1000,1000));
	UpdateData(TRUE);

	if(m_strID == "")	{
		AfxMessageBox("등록 인물 리스트에서 본인의 아이디를 선택하세요 !");
		return;
	}

	m_BlobArray.RemoveAll();
	m_nMaxTryCnt = 10;
	m_nMaxBlobCnt = 3;

	if(m_bIsPhotoCheck)		m_nProcessID = PROC_PHOTOCHECK;
	else					m_nProcessID = PROC_FEATURE_EXTRACT;

	m_nNextProcessID = PROC_VERIFY;
	
}

/*----------------------------------------------------------
	추출된 특징들 검증
 ----------------------------------------------------------*/
void CEFisDlg::Verify() 
{
	UpdateData(TRUE);

	// 타겟 인물의 템플릿 로딩
	CString strHome = GetHomeDirectory();
	strHome = AddFinalBackslash(strHome);
	CString strPersonFileName = strHome + "..\\DB)Person\\" + m_strID + ".EPI";
	CBlob *pTemplate = new CBlob;
	pTemplate->ReadFile(strPersonFileName);		

	// 유사도 모으기
	CDoubleArray dSimilArray;
	for(int i = 0; i < m_BlobArray.GetSize(); i++)
	{
		double dSimilarity = Face_GetMatchingValue(pTemplate, m_BlobArray[i]);
		dSimilArray.Add(dSimilarity);
	}

	// 유사도 결과 출력
	CString strTemp;
	double dMaxSimil = -1000;

	m_etOutput = "";
	for(int j = 0; j < dSimilArray.GetSize(); j++)
	{
		strTemp.Format("%d : %f \r\n", j, dSimilArray[j]);
		m_etOutput += strTemp;

		if(dSimilArray[j] > dMaxSimil)
			dMaxSimil = dSimilArray[j];
	}
	dSimilArray.RemoveAll();
	UpdateData(FALSE);
	
	// 판정
	CResultDlg dlg;

	if(dMaxSimil > 3.8)
	{	dlg.m_bAccept = TRUE;	
		if(m_chMsg[0] == 'u')	m_chMsg[0] = 'l';	
		else					m_chMsg[0] = 'u';
	}
	else	dlg.m_bAccept = FALSE;

	m_Com.WriteCommBlock((LPCVOID)&m_chMsg,2);		// Send Message to RS232C
	
	dlg.DoModal();

	UpdateData(TRUE);
	if(m_bIsTracking)	m_nProcessID = PROC_TRACKING;
	else				m_nProcessID = PROC_TERMINATED;

	delete pTemplate;
}


/*----------------------------------------------------------
	인식 버튼 클릭	
 ----------------------------------------------------------*/
#include "PhotoBook.h"
void CEFisDlg::OnRecognize() 
{
	RedrawWindow(CRect(0,300,1000,1000));
	UpdateData(TRUE);

	m_BlobArray.RemoveAll();
	m_nMaxTryCnt = 10;
	m_nMaxBlobCnt = 3;

	if(m_bIsPhotoCheck)		m_nProcessID = PROC_PHOTOCHECK;
	else					m_nProcessID = PROC_FEATURE_EXTRACT;

	m_nNextProcessID = PROC_RECOGNIZE;
}

static My_Str_Compare(const void *p1, const void *p2)
{
    CString *pStr1 = (CString *) p1;
    CString *pStr2 = (CString *) p2;

	if(pStr1->Left(1) == '-' && pStr2->Left(1) == '-')
	{
		if (*pStr1>*pStr2) return 1;
		else if (*pStr1==*pStr2) return 0;
		else return -1;

	}
	else
	{
		if (*pStr1<*pStr2) return 1;
		else if (*pStr1==*pStr2) return 0;
		else return -1;
	}
}

void CEFisDlg::Recognize() 
{

	CStringArray ResultArray;
	GetAllMatchingResult(ResultArray);

	// 다이얼로그 뿌리기
	CPhotoBook dlg;
	dlg.m_pResultArray = &ResultArray;
	dlg.m_pQueryDib = m_pDib;
	dlg.DoModal();
	ResultArray.RemoveAll();

	UpdateData(TRUE);
	if(m_bIsTracking)	m_nProcessID = PROC_TRACKING;
	else				m_nProcessID = PROC_TERMINATED;

}

void CEFisDlg::GetAllMatchingResult(CStringArray &ResultArray)
{
	int i, j, k;

	// 템플릿 별 유사도와 아이디 구하기  (유사도_아이디)
	for(k = 0; k < m_TemplateArray.GetSize(); k++)
	{
		CDoubleArray dSimilArray;
		for(i = 0; i < m_BlobArray.GetSize(); i++)		// Query 영상 개수
		{
			double dSimilarity = Face_GetMatchingValue(m_TemplateArray[k], m_BlobArray[i]);
			dSimilArray.Add(dSimilarity);
		}
		double dMaxSimil = -1000;
		for(j = 0; j < dSimilArray.GetSize(); j++)		// 각 Query영상 유사도 중 최대 유사도
		{
			if(dSimilArray[j] > dMaxSimil)
				dMaxSimil = dSimilArray[j];
		}
		dSimilArray.RemoveAll();

		CString strResult;
		strResult.Format("%.4f_%s", dMaxSimil, m_pPersonListArray->GetAt(k));
		ResultArray.Add(strResult);
	}

	// 유사도별 소팅
	qsort(ResultArray.GetData(), ResultArray.GetSize(), sizeof(CString *), My_Str_Compare);

}

/*----------------------------------------------------------
	사진 여부 판별	
 ----------------------------------------------------------*/
void CEFisDlg::PhotoCheck()
{
	const int nMaxTryCnt = 20;
	static int nTryCnt = 0;

	if(nTryCnt == 0)
		m_progress.SetRange(0, nMaxTryCnt);

	switch(Face_IsRealFace(m_pDib, nMaxTryCnt))
	{
		case 0:			// 사진 판정	
			nTryCnt = 0;
			UpdateData(TRUE);
			if(m_bIsTracking)	m_nProcessID = PROC_TRACKING;
			else				m_nProcessID = PROC_TERMINATED;
			AfxMessageBox("실제 얼굴이 아닙니다");
			break;
		case 1:			// 실제 얼굴 판정
			nTryCnt = 0;
			m_nProcessID = PROC_FEATURE_EXTRACT;
			break;
		case -1:		// 검사중
			nTryCnt++;
			m_progress.SetPos(nTryCnt);
			break;
		case -2:		// 검사중 (눈검출 실패)
			OutputDebugString("눈검색 실패\n");
	}
	
}


/*----------------------------------------------------------
	눈 트래킹
 ----------------------------------------------------------*/
void CEFisDlg::OnTrackingEyes() 
{
	UpdateData(TRUE);

	if(m_bIsTracking)	m_nProcessID = PROC_TRACKING;
	else				m_nProcessID = PROC_TERMINATED;

	m_strTrackingID = "Who?";
}

void CEFisDlg::OnRealTimeRecogn() 
{
	m_BlobArray.RemoveAll();
	m_strTrackingID = "Who?";
	g_strTrackingID = "모르겠어요";
}

void CEFisDlg::Tracking()
{
	static int nBlobCnt = 0;
	int randnum;

	CBlob *pBlob = Face_GetFeature(m_pDib, &m_ptLeft, &m_ptRight);

	if(pBlob == NULL)	
	{	
		m_ptLeft = CPoint(0,0);
		m_ptRight = CPoint(0,0);

		V.empty=1;		
		memcpy(lpVisionData,&V,sizeof(VISION_DATA));
	}
	else
	{

		V.empty=0;

		V.MC.x=(m_ptLeft.x+m_ptRight.x)/4;
		V.MC.y=(m_ptLeft.y+m_ptRight.y)/4;

		AddDebugText("\r x = %d, y = %d \n",V.MC.x,V.MC.y);

		memcpy(lpVisionData,&V,sizeof(VISION_DATA));

		//----------------------------
		//	Tracking과 동시에 인증하기
		//----------------------------
		UpdateData(TRUE);

		if(m_bIsRealTimeRecogn)
		{
			if(nBlobCnt == 0)	m_BlobArray.RemoveAll();
			m_BlobArray.Add(pBlob);		nBlobCnt++;
			if(nBlobCnt == 3)	// 3장 받으면 결과 내보내기
			{	
				CStringArray ResultArray;
				GetAllMatchingResult(ResultArray);
				
				CString strSimil = ResultArray[0].Left(6);
				double dMaxSimil = atof(strSimil);

				if(dMaxSimil > 1.0) {
					int nPos = ResultArray[0].Find('_');
					m_strTrackingID = ResultArray[0].Mid(nPos+1);
					m_strTrackingID += "("+strSimil+")";

					g_strTrackingID = ResultArray[0].Mid(nPos+1);

					randnum = rand()%3;
					if(randnum==0)
						SendTTS(g_strTrackingID);

				}
				else	{
					m_strTrackingID = "Who?";
					m_strTrackingID += "("+strSimil+")";
					
					g_strTrackingID = "모르겠어요";
				}


				ResultArray.RemoveAll();
				nBlobCnt = 0;
			}

			if(*lpVisionCommand==8)
			{
				SendTTS(g_strTrackingID);			
				*lpVisionCommand=0;
			}

		}

		else	delete pBlob;


		//----------------------------
		//	화면에 눈 출력하기
		//----------------------------
		CPen EyePen, *pOldPen;
		CDC *pDC = m_picMain.GetDC();
		pDC->SelectStockObject(NULL_BRUSH);

		if(m_ptLeft == CPoint(0,0))
		{
			OutputDebugString("뜨면 안되는 메세지");
		}

		// 눈 출력
		EyePen.CreatePen(PS_SOLID,1, RGB(0,255,0));
		pOldPen=(CPen *)pDC->SelectObject(&EyePen);
		pDC->Ellipse(m_ptLeft.x-12, m_ptLeft.y-6, m_ptLeft.x+12, m_ptLeft.y+6);
		pDC->Ellipse(m_ptRight.x-12, m_ptRight.y-6, m_ptRight.x+12, m_ptRight.y+6);
		pDC->MoveTo(m_ptLeft.x-16 , m_ptLeft.y);
		pDC->LineTo(m_ptLeft.x+16 , m_ptLeft.y);
		pDC->MoveTo(m_ptLeft.x , m_ptLeft.y-8);
		pDC->LineTo(m_ptLeft.x , m_ptLeft.y+8);
		pDC->MoveTo(m_ptRight.x-16 , m_ptRight.y);
		pDC->LineTo(m_ptRight.x+16 , m_ptRight.y);
		pDC->MoveTo(m_ptRight.x , m_ptRight.y-8);
		pDC->LineTo(m_ptRight.x , m_ptRight.y+8);
		EyePen.DeleteObject();

		ReleaseDC(pDC);
	}	

}


/*----------------------------------------------------------
	원영상 저장
 ----------------------------------------------------------*/
void CEFisDlg::OnSaveStillImage() 
{

	CTime today;
	today = CTime::GetCurrentTime();
	CString strFileName = today.Format("%Y.%m.%d_(%H.%M.%S).bmp");

	CString strHome = GetHomeDirectory();
	strHome = AddFinalBackslash(strHome);
	m_pDib->SaveDib(strHome + "..\\SrcImage\\"+strFileName);	

	AfxMessageBox(strFileName + "  Saved!!!"); 
}



/*----------------------------------------------------------
	OnLine Stop Offline File Open
 ----------------------------------------------------------*/
void CEFisDlg::OnEtriLogo() 
{

	m_VideoTool._BeginContinuousGrab();		// Callback Stop	

	CFileDialog dlg(TRUE, "BMP", "*.bmp", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"얼굴 이미지 데이터(*.bmp)|*.bmp|All(*.*)|*.*|");
	
	if(dlg.DoModal() == IDOK) {
		CString strFileName = dlg.GetPathName();
		if(m_pDib)	delete m_pDib;
		m_pDib = new CDib(strFileName);
		((CMyDib *)m_pDib)->PaintDibOnPic(&m_picMain, FALSE);
	}
}



/********************************************************************************


					File Manager


 *********************************************************************************/

/*----------------------------------------------------------
	등록 인물 리스트 로딩
 ----------------------------------------------------------*/
CStringArray* CEFisDlg::ReadPersonList(CString strFileName)
{
	int nPersonCnt;
	char achPersonCnt[50];
	CString strPersonID;
	CStringArray *PersonListArray = new CStringArray; 

	CStdioFile file(strFileName, CFile::modeRead);	

	file.ReadString(achPersonCnt, 50);
	nPersonCnt = atoi(achPersonCnt);		// 등록 인물 #

	for(int i = 0; i < nPersonCnt; i++)		// 등록 인물 ID
	{
		file.ReadString(strPersonID);
		PersonListArray->Add(strPersonID);
	}

	file.Close();

	return PersonListArray;
}


/*----------------------------------------------------------
	등록 인물 리스트 저장
 ----------------------------------------------------------*/
void CEFisDlg::WritePersonList(CStringArray *PersonListArray, CString strFileName)
{

	CStdioFile output(strFileName, CFile::modeWrite);

	CString strTemp;
	strTemp.Format("%d\n", PersonListArray->GetSize());
	output.WriteString(strTemp);

	for(int i = 0; i < PersonListArray->GetSize(); i++)	// 등록 인물 ID
	{
		output.WriteString(PersonListArray->GetAt(i));
		output.WriteString("\n");
	}

	output.Close();
}


void CEFisDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int nNewThres = Face_GetLightWeight() + 5;
	Face_SetLightWeight(nNewThres);
	DisplayThreshold(nNewThres);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CEFisDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int nNewThres = Face_GetLightWeight() - 5;
	Face_SetLightWeight(nNewThres);
	DisplayThreshold(nNewThres);
		
	CDialog::OnRButtonDown(nFlags, point);
}

void CEFisDlg::AddDebugText(char *fmt, ... )
{
	char msg[80]={0,};
	CString str;
	int nCharIndex;
	int nBackcount = 0;


	va_list ap;
	va_start( ap, fmt );
	vsprintf(msg, fmt, ap );
	va_end(ap);

	// AfxMessageBox("Get!");
	// Make sure existing text doesn't get too large.
	
	int len = m_dbgEdit.GetWindowTextLength();
	if (len >= 16384)
	{
		m_dbgEdit.GetWindowText(str);
		m_dbgEdit.SetWindowText(str.GetBuffer(len)+4097);
		len -= 4097;
	}

	// Handle backspace characters.
	str = msg;
	while ((nCharIndex = str.Find(TCHAR(VK_BACK))) != -1)
	{
		if (nCharIndex == 0)
		{
			nBackcount++;
			str = str.Mid(nCharIndex+1);
		}
		else
			str = str.Left(nCharIndex-1) + str.Mid(nCharIndex+1);
	}

	// Set text insertion point according to number of backspaces
	// at the begining of strData.
	if (nBackcount>len)
		nBackcount = len;
	m_dbgEdit.SetSel(len-nBackcount,len);

	// Eliminate line feeds.
	while ((nCharIndex = str.Find(TCHAR('\n'))) != -1)
		str = str.Left(nCharIndex) + str.Mid(nCharIndex + 1);

	// Ensure all carriage returns have a line feed following.
	LPCTSTR pchStart = (LPCTSTR)str;
	LPTSTR pchIndex = _tcschr(pchStart,(TCHAR)'\r');

	while (pchIndex != NULL)
	{
		int nPos = pchIndex - pchStart; 

		str = str.Left(nPos+1) + CString('\n') + str.Mid(nPos+1);
		
		// Above operation probably allocated a new string buffer.
		pchStart = (LPCTSTR)str;			
		pchIndex = _tcschr(pchStart + nPos + 1,(TCHAR)'\r');

	}

	// Add filtered data to edit control.
	m_dbgEdit.ReplaceSel(str);
}


void CEFisDlg::SendTTS(CString msg) 
{
	HWND cvwHandle;
	COPYDATASTRUCT cds;

	cds.dwData=1;
	cds.cbData=msg.GetLength();  
	cds.lpData=(void *)LPCTSTR(msg); 

	cvwHandle=::FindWindow(NULL,_T("거원TTS"));


	if(cvwHandle==0)
		return;
	else
	::SendMessage(cvwHandle, WM_COPYDATA, (WPARAM)(this->m_hWnd), LPARAM(&cds));
}

