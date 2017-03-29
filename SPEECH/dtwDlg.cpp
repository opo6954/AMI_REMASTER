// dtwDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dtw.h"
#include "dtwDlg.h"
#include "mydtwrtn.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// for shared memory
int dwSize=3000;
HANDLE hVTB;
PUCHAR lpSharedMemory;


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
// CDtwDlg dialog

CDtwDlg::CDtwDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDtwDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDtwDlg)
	m_word = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDtwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDtwDlg)
	DDX_Control(pDX, IDC_LIST, m_listbox);
	DDX_Text(pDX, IDC_EDIT_WORD, m_word);
	DDV_MaxChars(pDX, m_word, 32);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDtwDlg, CDialog)
	//{{AFX_MSG_MAP(CDtwDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, OnButtonTrain)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDtwDlg message handlers
void CDtwDlg::Alert(CString msg)
{
	AfxMessageBox(msg);
}

BOOL CDtwDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	int i;
	FILE *fp;

	m_nWord = 0;
	fp = fopen("init.dat","rb");
	if(!fp){
		AfxMessageBox("단어등록필수");
	}
	else{
		fread(&m_nWord,sizeof(int),1,fp);
		for(i=0;i<m_nWord;i++){
			fread(word[i].wordname,sizeof(char),32,fp);
			fread(&word[i].framenum,sizeof(int),1,fp);
			word[i].prCep =(double *)malloc(sizeof(double)*word[i].framenum*ORDER);
			fread(word[i].prCep,sizeof(double),word[i].framenum*ORDER,fp);

			fread(&word[i].framenum2,sizeof(int),1,fp);
			word[i].prCep2 =(double *)malloc(sizeof(double)*word[i].framenum2*ORDER);
			fread(word[i].prCep2,sizeof(double),word[i].framenum2*ORDER,fp);
		}
		for(i=0;i<m_nWord;i++){
			m_listbox.AddString(word[i].wordname);
		}
		fclose(fp);
	}

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

	if(lpSharedMemory==0)
	{
		Alert("Error Creat View of MM file!\n");
		ExitProcess(0); 
	}	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDtwDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDtwDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CDtwDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CDtwDlg::m_openBinary()
{
	FILE* Rfile;

	if((Rfile = fopen("speech.raw", "rb")) == NULL)
	{
		MessageBox("Can't Open file!", "Warning", MB_OK|MB_ICONHAND);
		return FALSE;
	}

	fseek(Rfile, 0, SEEK_END);
	m_FileLength = (UINT)(ftell(Rfile));
	m_totFr = (int)(m_FileLength / (2 * FR_SIZE));
	fseek(Rfile, 44, SEEK_SET);

	m_Data = new short[m_FileLength/sizeof(short)];
	fread(m_Data, sizeof(short), m_FileLength/sizeof(short), Rfile);

	fclose(Rfile);
	return TRUE;
}
/*
void CDtwDlg::Preemphasis()
{
	float	ALPHA = (float)0.95;
	short	*temp_data;

	temp_data = new short[m_FileLength/sizeof(short)];
	for(int i=0 ; i<m_FileLength/sizeof(short) ; i++){
		temp_data[i]=m_Data[i];
	}
	//memcpy(&temp_data[0], &m_Data[0],sizeof(short)*m_FileLength);

	for( i=1 ; i<m_FileLength/sizeof(short) ; i++)
	{
		m_Data[i] = temp_data[i] - (short)(ALPHA*temp_data[i-1]);
	}
	free(temp_data);
	
	FILE *TEST;
	if((TEST = fopen("preemphasis.raw", "wb"))==NULL)
	{
		MessageBox("Can't Open dcremove.raw file!", "Warning", MB_OK|MB_ICONHAND);
		return ;
	}
	fwrite(m_Data, sizeof(short)*m_FileLength, 1, TEST);
	fclose(TEST);
}
*/
double CDtwDlg::feat(int fr)
{
	int offset;
	double average, eng;

	offset = fr * FR_SIZE;

	average = 0.0;
	for(int i=0; i<FR_SIZE; i++)
	{
		average += (double)m_Data[offset + i];
	}
	average = average / FR_SIZE;

	eng = 0.0;
	for(int i=0; i<FR_SIZE; i++)
	{
		eng += fabs(m_Data[offset + i] - average) * fabs(m_Data[offset + i] - average);
	}
	return eng;
}

void CDtwDlg::level(double* Enn, int num, double* LEL, double* UEL)
{
	double ave_en = .0;
	for(int i=0; i<num; i++)
	{
		ave_en += Enn[i];
	}
	ave_en = ave_en / num;
	(*LEL) = 3.0 * ave_en;
	(*UEL) = 3.0 * (*LEL);
	return;
}

int CDtwDlg::biginDetect(int fr, double LEL, double UEL)
{
	int i, j;
	double ener;
	int tmp_bp = 0, bp_end = 0;

	for(i = 5; i < fr; i++)
	{
		ener = feat(i);
		if(ener >= UEL)
		{
			tmp_bp = i;
			break;
		}
	}

	for(i = tmp_bp; i < fr; i++)
	{
		ener = feat(i);
		if(ener >= LEL)
		{
			tmp_bp = i;
			break;
		}
	}

	if((tmp_bp - 10) >= 0)
		bp_end = tmp_bp - 10;
	else
		bp_end = 0;

	for(j = tmp_bp - 1; j >= bp_end; j--)
	{
		ener = feat(j);
		if(ener <= LEL)
		{
			tmp_bp = j;
			break;
		}
	}

	return tmp_bp;
}

int CDtwDlg::endDetect(int begin, int fr, double LEL, double UEL)
{
	int i, tmp_ep;
	double ener;

	tmp_ep = 0;

	for(i = begin; i < fr; i++)
	{
		ener = feat(i);
		if(ener <= UEL)
		{
			tmp_ep = i;
			break;
		}
	}

	for(i = tmp_ep; i < fr; i++)
	{
		ener = feat(i);
		if(ener <= LEL)
		{
			tmp_ep = i;
			break;
		}
	}

	return tmp_ep;
}

void CDtwDlg::m_detectEndPoint(int beginFr[], int endFr[], int *number)
{
	int i, j, count = 0;
	double LEL, UEL, En;
	double Enn[5] = {.0,};

	//m_openBinary();
	
	for(i = 0; i < 5; i++)
	{
		Enn[i] = feat(i);
	}
	level(Enn, 5, &LEL, &UEL);

	beginFr[0] = biginDetect(m_totFr, LEL, UEL);
	endFr[0] = endDetect(beginFr[0], m_totFr, LEL, UEL);

    *number = 1;

    count = 1;

	for(i = 0; i < 15; i++)
	{
		if(count != 0)
		{
			count = 0;
			for(j = endFr[i]; j < m_totFr; j++)
			{
				En = feat(j);
				if(En >= UEL)
				{
					count++;
					beginFr[i+1] = j;
					endFr[i+1] = endDetect(j, m_totFr, LEL, UEL);
						if(endFr[i+1]>beginFr[i+1])
							(*number)++;
					break;
				}
			}
		}
	}
	return;
}

int CDtwDlg::m_trans(int beginFr[],int endFr[], int number)
{ 
	int start, end, length;

	length = 0;

	for(int i=0 ; i<number ; i++)
	{
		start = (beginFr[i])*FR_SIZE;
		end = (endFr[i]+1)*FR_SIZE;
		for(int j=start ; j<end ; j++)
		{
			in_Data[length] = m_Data[j];
			length += 1;
		}
	}
	return length;
}

void CDtwDlg::hamming(short s[])
{
	for(int i = 0 ; i < FR_SIZE ; i++)
		s[i] = s[i] * (0.54 - 0.46 * cos(2*3.14*i/FR_SIZE));
}

void CDtwDlg::Lpc(short *in, double *out)
{
	int i, j, max=0;
	double acf[17]={0.0,}, E[17], a[17][17], k[17], n_in[FR_SIZE];
	double M;

	for(i = 0 ; i < FR_SIZE ; i++)
	{
		if(abs(in[i]) > max)
			max = abs(in[i]);
	}
	for(i = 0 ; i < FR_SIZE ; i++)
	{
		n_in[i] = (double)in[i] / max;
	}
	/* compute Autocorrelation R */
	for(i = 0; i <= ORDER; i++)
	{
		acf[i] = 0.0;
		for(j=0; j<(FR_SIZE-i); j++)
			acf[i] += (n_in[j] * n_in[j+i]);
	}
	/* compute tenth order LPC coefficients using Durbin Algorithm */
	E[0] = acf[0];

	for(i = 1; i <= ORDER; i++)
	{
		M = 0.0;
		for(j = 1; j < i; j++)
			M = M + (a[j][i-1] * acf[i-j]);

		k[i] = (acf[i] - M) / E[i-1];
		a[i][i] = k[i];

		for(j = 1; j < i; j++)
			a[j][i] = a[j][i-1] - (k[i] * a[i-j][i-1]);

		E[i] = (1 - (k[i] * k[i])) * E[i-1];
	}
	for(i = 0; i < ORDER; i++)
	{
		out[i] = (a[i+1][ORDER]);
	}
}

void CDtwDlg::Lpc2Cep(double *in, double *out)
{
	int i, j;
	double n = 2.0, k, temp;
	out[0] = in[0];

	for(i = 1 ; i < ORDER ; i++)
	{
		k = 1.0;
		temp = 0.0;
		for(j = 0 ; j <= i-1 ; j++)
		{
			temp += (1-k/n) * in[j] * out[i-j-1];
			k++;
		}
		n++;
		out[i] = temp + in[i];
	}
}

/*
void CDtwDlg::Normalize(double *rBuff, int nLength)
{
	int i;
	double  max;

	max=fabs(rBuff[0]);
	for(i=0;i<nLength;i++){
		if(max<fabs(rBuff[i]))max=fabs(rBuff[i]);
	}

	for(i=0;i<nLength;i++) rBuff[i]=rBuff[i]/max;
}
*/

int CDtwDlg::feat_extr1(int length)
{
	short s[FR_SIZE];
	int i, j, fr_num, number;
	double lpcof[ORDER], cep[ORDER];

	fr_num = ceil((float)length / OVERLAP) - 1;
	//if(fr_num >= 100) fr_num = 99;

	word[m_nWord].prCep =(double *)malloc(fr_num * ORDER * sizeof(double));

	for(i = 0 ; i < fr_num ; i++)
	{
		number = i * OVERLAP;
		for(j=0; j<FR_SIZE; j++)
			s[j] = in_Data[number + j];
		hamming(s);
		Lpc(s, lpcof);
		Lpc2Cep(lpcof, cep);
		//Normalize(cep,ORDER);

		for(j = 0 ; j < ORDER ; j++){
			word[m_nWord].prCep[i*ORDER+j]=cep[j];
		}
	}
	return fr_num;
}

int CDtwDlg::feat_extr2(int length)
{
	short s[FR_SIZE];
	int i, j, fr_num, number;
	double lpcof[ORDER], cep[ORDER];

	fr_num = ceil((float)length / OVERLAP) - 1;
	//if(fr_num >= 100) fr_num = 99;

	word[m_nWord].prCep2 =(double *)malloc(fr_num * ORDER * sizeof(double));

	for(i = 0 ; i < fr_num ; i++)
	{
		number = i * OVERLAP;
		for(j=0; j<FR_SIZE; j++)
			s[j] = in_Data[number + j];
		hamming(s);
		Lpc(s, lpcof);
		Lpc2Cep(lpcof, cep);
		//Normalize(cep,ORDER);
		
		for(j = 0 ; j < ORDER ; j++){
			word[m_nWord].prCep2[i*ORDER+j]=cep[j];
		}
	}
	return fr_num;
}

#include "spinput.h"
void CDtwDlg::OnButtonTrain() 
{
	UpdateData(TRUE);
	if(m_word==""){
		AfxMessageBox("단어를 먼저 입력하십시오.");
		return;
	}
	
	Cspinput spinput;
	if(spinput.DoModal()==IDOK){
		m_openBinary();
		int number = 0, beginFr[20] = {0}, endFr[20] = {0}, length=0;

		m_detectEndPoint(beginFr, endFr, &number);
		length = m_trans(beginFr, endFr, number);
		
		//if(length < 1500 || length > 31000){
		if(length < 1500 || length > 50000){

			TRACE("\n length = %d",length);
			MessageBox("다시발성해주십시오", "Warning", MB_OK|MB_ICONHAND);
			free(m_Data);
			return;
		}
		word[m_nWord].framenum=feat_extr1(length);
	}
	free(m_Data);

	if(spinput.DoModal()==IDOK){
		m_openBinary();
		int number = 0, beginFr[20] = {0}, endFr[20] = {0}, length=0;
		m_detectEndPoint(beginFr, endFr, &number);
		length = m_trans(beginFr, endFr, number);

		//if(length < 1500 || length > 31000){
		if(length < 1500 || length > 50000){

			MessageBox("다시발성해주십시오", "Warning", MB_OK|MB_ICONHAND);
			free(m_Data);
			return;
		}
		word[m_nWord].framenum2=feat_extr2(length);
	}
	free(m_Data);
	strcpy(word[m_nWord].wordname,m_word);
	m_listbox.AddString(word[m_nWord].wordname);
	m_nWord++;
	m_word="";
	UpdateData(FALSE);
}

BOOL CDtwDlg::Load2Memory()
{
	int beginFr[20]={0}, endFr[20]={0},length=0,number=0;

	Cspinput spinput;
	if(spinput.DoModal()== IDOK){
		m_openBinary();
		m_detectEndPoint(beginFr, endFr,&number);
		length = m_trans(beginFr, endFr,number);

		//if(length < 1500 || length > 31000){
		if(length < 1500 || length > 50000){
/////HYOK
			// 연속으로 녹음하기 위해 껐음.
			//			MessageBox("다시 발성해 주십시오", "Warning", MB_OK|MB_ICONHAND);
////
			return(FALSE);
		}
		word[m_nWord].framenum=feat_extr1(length);
	}
	else {
		return(FALSE);
	}
	free(m_Data);
	return(TRUE);
}

void CDtwDlg::OnButtonTest() 
{
	while(1){
		int i,j,index;
		double distance,**ppX,**ppY;
		double minVal;
		int    minIndex;
		int    piPath[(125+125)*ORDER];
	//// HYOK	
		while(Load2Memory()==FALSE);
	//////
		
		ppY = (double**)mallocMatrix(sizeof(double),word[m_nWord].framenum,ORDER);
		for(i=0;i<word[m_nWord].framenum;i++) for(j=0;j<ORDER;j++){
			ppY[i][j] = word[m_nWord].prCep[ORDER*i+j];
		}


		for(index=0;index<m_nWord;index++){
			//////////////////////////////////////////////////////////////////////////////
			ppX = (double**)mallocMatrix(sizeof(double),word[index].framenum,ORDER);
			for(i=0;i<word[index].framenum;i++){
				for(j=0;j<ORDER;j++) ppX[i][j] = word[index].prCep[ORDER*i+j];
			}
			if(word[index].framenum*2 <= word[m_nWord].framenum||word[index].framenum >= 2*word[m_nWord].framenum)
				distance = 100;
			else distance = DoMyDTW(piPath,ppX,ppY,3, word[index].framenum ,word[m_nWord].framenum,ORDER,1);
			
			if(index==0){
				minVal = distance;
				minIndex = index;
			}
			if(distance<minVal){
				minVal = distance;
				minIndex = index;
			}
			freeMatrix2(ppX);
			//////////////////////////////////////////////////////////////////////////////
			ppX = (double**)mallocMatrix(sizeof(double),word[index].framenum2,ORDER);
			for(i=0;i<word[index].framenum2;i++){
				for(j=0;j<ORDER;j++) ppX[i][j] = word[index].prCep2[ORDER*i+j];
			}
			if(word[index].framenum2*2 <= word[m_nWord].framenum||word[index].framenum2 >= 2*word[m_nWord].framenum)
				distance = 100;
			else distance = DoMyDTW(piPath,ppX,ppY,3, word[index].framenum2 ,word[m_nWord].framenum,ORDER,1);
			
			if(distance<minVal){
				minVal = distance;
				minIndex = index;
			}
			freeMatrix2(ppX);
			//////////////////////////////////////////////////////////////////////////////
		}
		freeMatrix2(ppY);
  
		CString sFromList;
		char buff[30];

		if(minVal < 0.04) 
		{
			m_listbox.GetText(minIndex,sFromList);
			strcpy(buff,sFromList);
			sFromList.Format("%s\nScore:%f index%d",buff,minVal,minIndex);
			
			if((minIndex==0 && minVal < 0.035) ||minIndex!=0) 
				*lpSharedMemory=minIndex+1;		

		}
		//AfxMessageBox(sFromList);
	}
/*
//// HYOK
	OnButtonTest();
//////
*/
}

void CDtwDlg::SaveAllData()
{
	int i;
	FILE *fp;
	fp = fopen("init.dat","wb");
	if(!fp){
		AfxMessageBox("저장하는 동안 에러가 발생하였습니다.");
		return;
	}

	fwrite(&m_nWord,sizeof(int),1,fp);
	for(i=0;i<m_nWord;i++){
		fwrite(word[i].wordname,sizeof(char),32,fp);
		fwrite(&word[i].framenum,sizeof(int),1,fp);
		fwrite(word[i].prCep,sizeof(double),word[i].framenum*ORDER,fp);
		fwrite(&word[i].framenum2,sizeof(int),1,fp);
		fwrite(word[i].prCep2,sizeof(double),word[i].framenum2*ORDER,fp);
	}
	fclose(fp);
}

void CDtwDlg::FreeMyMalloc()
{
	int i;
	for(i=0;i<m_nWord;i++){
		free(word[i].prCep);
		free(word[i].prCep2);
	}
}

void CDtwDlg::OnButtonExit() 
{
	CDialog::OnOK();
}

void CDtwDlg::OnOK() 
{
}

void CDtwDlg::OnButtonDel() 
{
	int nCurSel;

	nCurSel=m_listbox.GetCurSel();
	if(nCurSel<0||nCurSel>m_nWord) 
		AfxMessageBox("삭제할 단어를 선택하여 주세요");
	else{
		m_listbox.DeleteString(nCurSel);
		m_nWord--;
		for(int i=nCurSel;i<m_nWord;i++){
			strcpy(word[i].wordname,word[i+1].wordname);
			word[i].framenum = word[i+1].framenum;
			word[i].framenum2 = word[i+1].framenum2;
			free(word[i].prCep);
			free(word[i].prCep2);
			word[i].prCep =(double *)malloc(sizeof(double)*word[i+1].framenum*ORDER);
			word[i].prCep2 =(double *)malloc(sizeof(double)*word[i+1].framenum2*ORDER);
			for(int j=0;j<word[i+1].framenum*ORDER;j++)
				word[i].prCep[j]=word[i+1].prCep[j];
			for(int j=0;j<word[i+1].framenum2*ORDER;j++)
				word[i].prCep2[j]=word[i+1].prCep2[j];
		}
		free(word[m_nWord].prCep);
		free(word[m_nWord].prCep2);
	}
}

BOOL CDtwDlg::DestroyWindow() 
{
	SaveAllData();
	FreeMyMalloc();
	return CDialog::DestroyWindow();
}


