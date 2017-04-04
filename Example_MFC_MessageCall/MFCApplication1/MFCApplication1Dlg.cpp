
// MFCApplication1Dlg.cpp : ���� ����
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

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	

// �����Դϴ�.
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


// CMFCApplication1Dlg ��ȭ ����



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


// CMFCApplication1Dlg �޽��� ó����

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	srand( (unsigned)time( NULL ) );

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	SR2Center = RegisterWindowMessage( _T(MSG_SR2CENTER));
	Center2SR = RegisterWindowMessage(_T(MSG_CENTER2SR));
	Center2Emotion = RegisterWindowMessage(_T(MSG_CENTER2EMOTION));

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CMFCApplication1Dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // �޽��� ����
	
	//�ϴ� �ϵ��ڵ����� �����ֱ������ ����
	if(message == this->SR2Center && (HWND)wParam != this->GetSafeHwnd())
	{
		int msg_answer=100;//���� �亯
		int msg_emotion=100;//ǥ�� �亯

		char ques_txt[256];
		char ans_txt[256];

		int random = rand() % 2;

		char buf[256];

		int flag = lParam;

		switch (flag)
		{
		case 0://�ƹ�
			//��
			msg_answer = 5;
			sprintf_s(ques_txt,"�ƹ�");
			sprintf_s(ans_txt,"��");
			break;
		case 1://����̾
			
			if(random == 0)//���ƿ�
			{
				msg_answer = 6;
				msg_emotion = 0;
				sprintf_s(ques_txt,"����̾");
				sprintf_s(ans_txt,"���ƿ�");
			}
			else//������
			{
				msg_answer = 7;
				msg_emotion = 1;
				sprintf_s(ques_txt,"����̾");
				sprintf_s(ans_txt,"������");
			}
			break;
		case 2://���߾�
			msg_answer = 8;
			msg_emotion = 0;
			sprintf_s(ques_txt,"���߾�");
			sprintf_s(ans_txt,"�����մϴ�");
			break;
		case 3://���߾�
			msg_answer = 9;
			msg_emotion = 1;
			sprintf_s(ques_txt,"���߾�");
			sprintf_s(ans_txt,"�������ҰԿ�");
			break;
		case 4://�ȳ�
			msg_answer = 4;
			sprintf_s(ques_txt,"�ȳ�");
			sprintf_s(ans_txt,"�ȳ�");
			break;
		default://���� �ܾ��� --> lParam ���� 100���� ����
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


			//���� ������
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
			//ǥ�� ������
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

	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}