// PhotoBook.cpp : implementation file
//

#include "stdafx.h"
#include "efis.h"
#include "PhotoBook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhotoBook dialog


CPhotoBook::CPhotoBook(CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhotoBook)
	m_strResultID = _T("");
	//}}AFX_DATA_INIT

	m_pResultArray = NULL;

	m_pQueryDib = NULL;
	m_pResultDib = NULL;
}


void CPhotoBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhotoBook)
	DDX_Control(pDX, IDC_PIC_RESULT, m_picResult);
	DDX_Control(pDX, IDC_PIC_QUERY, m_picQuery);
	DDX_Text(pDX, IDC_RESULT_ID, m_strResultID);
	DDX_Control(pDX, IDC_IMAGELISTCTRL1, m_ImageList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhotoBook, CDialog)
	//{{AFX_MSG_MAP(CPhotoBook)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhotoBook message handlers

BOOL CPhotoBook::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_ImageList.SetHorizontalScroll(TRUE);
	m_ImageList.SetSizeX(110);
    m_ImageList.SetSizeY(90);
    m_ImageList.SetSelectedDisplay(1);	


	CString strHome = GetHomeDirectory();
	strHome = AddFinalBackslash(strHome);
	CString strFullShotPath = strHome +"..\\DB)Person_FullShot\\";

	int nPos;
	CString strSimil;
	CString strID;
	CString strTag;

	// PhotoBook 채우기
	for(int i = 0; i < m_pResultArray->GetSize(); i++)
	{
		nPos = m_pResultArray->GetAt(i).Find('_');
		strSimil = m_pResultArray->GetAt(i).Left(nPos);
		strID = m_pResultArray->GetAt(i).Mid(nPos+1);
		strTag.Format("%s(%s)", strID, strSimil);

		CDib *pDib;
		pDib = new CDib(strFullShotPath + strID + ".bmp");
		if(i == 0)		// Rank 1 정보 얻기
		{
			m_pResultDib = pDib->CopyCDib();
			m_strResultID = strTag;
		}
		m_ImageList.AddImagePtr(strTag, (long) pDib);
		delete pDib;
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPhotoBook::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	((CMyDib *)m_pQueryDib)->PaintDibOnPic(&m_picQuery, TRUE);
	((CMyDib *)m_pResultDib)->PaintDibOnPic(&m_picResult, TRUE);

	// Do not call CDialog::OnPaint() for painting messages
}

void CPhotoBook::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pResultDib)	delete m_pResultDib;
	
}

