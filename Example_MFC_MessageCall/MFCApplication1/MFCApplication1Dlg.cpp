
// MFCApplication1Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SPEECHMODULLENAME "SpeechModule"
#define EMOTIONMODULENAME "Avatar"
#define MSG_SR2CENTER "WM_SR2CENTER"
#define MSG_CENTER2SR "WM_CENTER2SR"
#define MSG_CENTER2EMOTION "WM_CENTER2EMOTION"

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	srand( (unsigned)time( NULL ) );

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	SR2Center = RegisterWindowMessage( _T(MSG_SR2CENTER));
	Center2SR = RegisterWindowMessage(_T(MSG_CENTER2SR));
	Center2Emotion = RegisterWindowMessage(_T(MSG_CENTER2EMOTION));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CMFCApplication1Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // 메시지 수신
	
	//일단 하드코딩으로 보여주기식으로 하자
	if(message == this->SR2Center && (HWND)wParam != this->GetSafeHwnd())
	{
		int msg_answer=100;//음성 답변
		int msg_emotion=100;//표정 답변

		char ques_txt[256];
		char ans_txt[256];

		int random = rand() % 2;

		char buf[256];

		int flag = lParam;

		switch (flag)
		{
		case 0://아미
			//네
			msg_answer = 5;
			sprintf_s(ques_txt,"아미");
			sprintf_s(ans_txt,"네");
			break;
		case 1://기분이어때
			
			if(random == 0)//좋아요
			{
				msg_answer = 6;
				msg_emotion = 0;
				sprintf_s(ques_txt,"기분이어때");
				sprintf_s(ans_txt,"좋아요");
			}
			else//나빠요
			{
				msg_answer = 7;
				msg_emotion = 1;
				sprintf_s(ques_txt,"기분이어때");
				sprintf_s(ans_txt,"나빠요");
			}
			break;
		case 2://잘했어
			msg_answer = 8;
			msg_emotion = 0;
			sprintf_s(ques_txt,"잘했어");
			sprintf_s(ans_txt,"감사합니다");
			break;
		case 3://못했어
			msg_answer = 9;
			msg_emotion = 1;
			sprintf_s(ques_txt,"못했어");
			sprintf_s(ans_txt,"열심히할게요");
			break;
		case 4://안녕
			msg_answer = 4;
			sprintf_s(ques_txt,"안녕");
			sprintf_s(ans_txt,"안녕");
			break;
		default://없는 단어임 --> lParam 값이 100으로 고정
			break;
		}

		SetDlgItemText(IDC_STATIC1, (CA2W)ans_txt);

		if(msg_answer < 100)
		{
			

			CWnd *lp_tip_wnd = CWnd::FindWindow(NULL, _T(SPEECHMODULLENAME));
			//CWnd *lp_tip_wnd = CWnd::FindWindow(NULL, _T("power"));

			if(lp_tip_wnd == NULL)
			{
				AfxMessageBox(_T("No SpeechModulewindow found"));			
			}
			else
			{
				lp_tip_wnd->PostMessageW(Center2SR,(WPARAM)this->m_hWnd,msg_answer);
			}


			//음성 보내기
		}
		if(msg_emotion < 100)
		{
			CWnd *lp_tip_wnd = CWnd::FindWindow(NULL, _T(EMOTIONMODULENAME));
			//CWnd *lp_tip_wnd = CWnd::FindWindow(NULL, _T("power"));

			if(lp_tip_wnd == NULL)
			{
				AfxMessageBox(_T("No EmotionPanel window found"));			
			}
			else
			{
				lp_tip_wnd->SendMessage(Center2Emotion,(WPARAM)this->m_hWnd,msg_emotion);
			}
			//표정 보내기
		}

	}



    return CDialog::WindowProc(message, wParam, lParam);
}

void CMFCApplication1Dlg::OnBnClickedButton1()
{
	SetDlgItemText(IDC_STATIC1,_T("example_emotion_output"));

	CWnd *lp_tip_wnd = CWnd::FindWindow(NULL, _T(EMOTIONMODULENAME));
        if(lp_tip_wnd == NULL)
		{
			SetDlgItemText(IDC_STATIC1,_T("\nNo window found"));
			
		}
		else
		{
			SetDlgItemText(IDC_STATIC1,_T("\nWindow found"));
			int random = rand() % 2;
			lp_tip_wnd->SendMessage(Center2Emotion,(WPARAM)this->m_hWnd,random);
		}

	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}