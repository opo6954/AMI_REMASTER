// SpInput.cpp : implementation file

#include "stdafx.h"
#include "spinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PR_NONE		0
#define PR_RECORD	1
#define PR_PLAY		2


// BY HYOK
#define	SENSITIVITY		20
#define	SENSITIVITYTIME 2
#define BLOCKLENGTH		10
#define RECORDTIME		100
int mingab=0xFF, maxgab=0;
int RECORDBLOCK=0;
///////////


/////////////////////////////////////////////////////////////////////////////
// Cspinput dialog
Cspinput::Cspinput(CWnd* pParent /*=NULL*/)
	: CDialog(Cspinput::IDD, pParent)
{
	//{{AFX_DATA_INIT(Cspinput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pWave = NULL;
	waveDataBlock = NULL;
	m_nSec = RECORDTIME; //record time 4 second
	m_nStatus = PR_NONE;
}

Cspinput::~Cspinput()
{
	wFreeMemory();
}


void Cspinput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cspinput)

		DDX_Control(pDX, IDC_SLIDER1, m_slider);		
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Cspinput, CDialog)
	//{{AFX_MSG_MAP(Cspinput)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL Cspinput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_slider.SetRange(0, m_nSec);
	m_slider.SetPos(0);
	
	for(int i=0; i<m_nSec; i++)
	{
		m_slider.SetTic(i);
	}

	UpdateData(FALSE);
/// HYOK
	OnButtonRecord();
///////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


int Cspinput::wAllocMemory()
{
	waveDataBlock = ::GlobalAlloc(GMEM_MOVEABLE, lDataSize);
	if(waveDataBlock == NULL)
		return FALSE;

	pWave = (LPBYTE)::GlobalLock(waveDataBlock);
	return TRUE;
}

void Cspinput::wFreeMemory()
{
	if(waveDataBlock)
	{
		::GlobalUnlock(waveDataBlock);
		::GlobalFree(waveDataBlock);
		pWave = NULL;
		waveDataBlock = NULL;
	}
}

void Cspinput::SetWaveInfo()
{
	WaveRecord.wf.wFormatTag = 1;
	WaveRecord.wf.nChannels = 1;
	WaveRecord.wf.nSamplesPerSec = 8000;
	WaveRecord.wf.nAvgBytesPerSec = 8000 * 1 * 2;
	WaveRecord.wf.nBlockAlign = 2;
	WaveRecord.wBitsPerSample = 16;
}

void Cspinput::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case 1:
		RecordCount();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void Cspinput::	wSetWaveHeader()
{
	WaveHeader.lpData = (LPSTR) pWave;
	WaveHeader.dwBufferLength = lDataSize;
	WaveHeader.dwFlags = 0L;
	WaveHeader.dwLoops = 0L;
	WaveHeader.dwBytesRecorded = lDataSize;
}

void Cspinput::RecordCount()
{
	CRect rect;
	CDC* pDC = GetDC();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_WAVE);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);


/// HYOK
	mingab=0xFF;
	maxgab=0;
//////

	m_nRecordPos++;
	DrawWave(pDC, rect, RGB(0,255,0), 4, m_nRecordPos);
	ReleaseDC(pDC);

/// HYOK
	static int SENSETERM =0;
	static int NOINPUT =0;
	if(RECORDBLOCK>0) RECORDBLOCK++;
	else{
		if(m_nRecordPos>3)  // Start Poping Noise ignore
			if(maxgab - mingab > SENSITIVITY)
			{
				SENSETERM ++;
				NOINPUT=0;
				if (SENSETERM > SENSITIVITYTIME)
				{
					printf("started!\n");
					RECORDBLOCK++;
				}
			} else
			{
				NOINPUT++;
				if(NOINPUT>SENSITIVITYTIME)
				{
					SENSETERM =0;
				} else SENSETERM++;
			}
	}

	if(RECORDBLOCK > BLOCKLENGTH) m_nRecordPos=m_nSec;
///////	

	m_slider.SetPos(m_nRecordPos);
	
	if(m_nRecordPos == m_nSec)
	{
		KillTimer(1);
		m_nRecordPos = 0;
		m_slider.SetPos(m_nRecordPos);
		StopRecord();
		SaveRawFile("speech.raw");
//		MyEncoder(); // speech.raw -> speech.bin
//		MyDecoder(); // speech.bin -> speech.out
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		m_nStatus = PR_NONE;
//HYOK
		OnOK();
////////
	}
}

int Cspinput::Bit16Value(int num)
{
	union ci{
		char c[2];
		short gab;
	}charint;
	int gab;
	
	charint.c[0] = WaveHeader.lpData[num];
	charint.c[1] = WaveHeader.lpData[num+1];
	gab = 65535 - charint.gab;

	gab = gab / 256;
	gab = gab - 256;
	return gab;
}

void Cspinput::DrawWave(CDC* pDC,CRect rect,COLORREF Color, int step, int pos)
{
	int i, xpos;
	int gab;
	float j;
	float yper, averloop;
	CPen Pen;
	CPen *pOldPen;


	Pen.CreatePen(PS_SOLID, 0, RGB(0,255,0));
	pOldPen = (CPen *)pDC->SelectObject(&Pen);

	yper = (float)(rect.bottom - rect.top) / 256;
	averloop = (float)lDataSize / (rect.right - rect.left);

	i = (pos-1) * (WaveRecord.wf.nAvgBytesPerSec / 5); // start byte each 200msec.
	xpos = (int)i / averloop;
	int k = 0;
	int middle;

	for( ;i < (int)(pos) * ((int)WaveRecord.wf.nAvgBytesPerSec/5); ) // draw Only 200msec.
	{
		for(j = 0; j < averloop + 0.5; j = j + step)
		{
			gab = Bit16Value(i);

/// HYOK
			if(gab < mingab) mingab = gab;
			if(gab > maxgab) maxgab = gab;
///////////

			i = i + step;
			middle = rect.bottom - ((rect.bottom-rect.top) / 2);
			if(k==0)
				pDC->MoveTo(xpos+rect.left, (int)middle+(gab*yper));
			else
				pDC->LineTo(xpos+rect.left, (int)middle+(gab*yper));
			k++;
		}
		xpos++;
	}

	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();
}

short *Cspinput::Loadfile(char *filename)
{
	short *sdata;
	HFILE fp;
	// 화일 열기 
	fp=_lopen(filename,OF_READ);
	if(fp<0)
	{
		MessageBox("File Open Error!!");
		exit(-1);
	}
	// 화일 길이 
	filesize=_llseek(fp,0L,FILE_END);
    _llseek(fp,0L,FILE_BEGIN);   

	// 포인터의 메모리 설정 
	sdata = (short *)malloc(filesize);

	// 데이타 읽기 
	_lread(fp,sdata,filesize);
	_lclose(fp);
	return sdata;
}

int Cspinput::Raw2Wav(char *filename)
{
	HFILE lnn;
	DWORD len, avgbyte;
	short *sdata;
	int size;
	char tmpname[30];

	sdata = Loadfile(filename);
	lstrcpy((LPSTR)tmpname,(LPSTR)"temp.wav");
	
	lnn=_lcreat(tmpname,0);				/* 화일 생성 */
	if(lnn==NULL){
		AfxMessageBox("공유규칙을 어겼습니다.");
		return FALSE;
	}

	_lwrite(lnn,(LPSTR)"RIFF",4);			
	len = 36 + filesize;					
	_lwrite(lnn,(LPSTR)&len,4);			
	_lwrite(lnn,(LPSTR)"WAVE",4);			
	_lwrite(lnn,(LPSTR)"fmt ",4);			
	len=16;									
	_lwrite(lnn,(LPSTR)&len,4);
	short i;								 
	i=1;
	_lwrite(lnn,(LPSTR)&i,2);
	i=1;									
	_lwrite(lnn,(LPSTR)&i,2);

											
	int samplefreq=8000;					
	//int	samplefreq=11025;					
	

	_lwrite(lnn,(LPSTR)&samplefreq,4);		

	avgbyte = samplefreq * 1 * 2;		
	
	_lwrite(lnn,(LPSTR)&avgbyte,4);
	i = 2;									
	_lwrite(lnn,(LPSTR)&i,2);
	i = 16;									
	_lwrite(lnn,(LPSTR)&i,2);
	_lwrite(lnn,(LPSTR)"data",4);			
	_lwrite(lnn,(LPSTR)&filesize,4);		


	size=(int) filesize/1;

	// 데이타를 넣는다. 
	if(size!=_lwrite(lnn,(LPSTR)sdata,size))
	{
		AfxMessageBox("저장할 용량이 없습니다.");
		return FALSE;
	}
	free(sdata);
	_lclose(lnn);
	return TRUE;
}



int Cspinput::RecordWave()
{
//open device
	errorCode = waveInOpen((LPHWAVEIN)&hWaveInn, WAVE_MAPPER, (tWAVEFORMATEX*)&WaveRecord, 0L, 0L, 0L);
	if(errorCode)
	{
		AfxMessageBox("Cannot Open record device!");
		return FALSE;
	}
	errorCode = waveInPrepareHeader(hWaveInn, &WaveHeader, sizeof(WaveHeader));
	if(errorCode)
	{
		AfxMessageBox("Wave in prepare header error!");
		return FALSE;
	}
//record
	errorCode = waveInAddBuffer(hWaveInn, &WaveHeader, sizeof(WaveHeader));
	if(errorCode)
	{
		AfxMessageBox("waveInAddBuffer error!");
		return FALSE;
	}
////HYOK
	RECORDBLOCK=0;
//////////////
	errorCode = waveInStart(hWaveInn);
	if(errorCode)
	{
		AfxMessageBox("wave record error!");
		return FALSE;
	}
	return TRUE;
}

int Cspinput::StopRecord()
{

/// HYOK
/*	errorCode = waveInStop(hWaveInn);
	if(errorCode)
	{
		AfxMessageBox("Wave Stop error!");
		return FALSE;
	}
*/
/////	
	
	errorCode = waveInReset(hWaveInn);
	if(errorCode)
	{
		AfxMessageBox("Wave Reset error!");
		return FALSE;
	}
	errorCode = waveInUnprepareHeader(hWaveInn, &WaveHeader, sizeof(WaveHeader));
	if(errorCode)
	{
		AfxMessageBox("Wave UnPrepare error!");
		return FALSE;
	}
	errorCode = waveInClose(hWaveInn);
	if(errorCode)
	{
		AfxMessageBox("Wave Close error!");
		return FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Cspinput message handlers

void Cspinput::OnButtonRecord() 
{
	SetWaveInfo();
	
	lDataSize = (DWORD)WaveRecord.wf.nAvgBytesPerSec * (m_nSec/5);
	wAllocMemory();

	wSetWaveHeader();

	m_nRecordPos = 0;

	m_nStatus = PR_RECORD;

	RecordWave();
	
	SetTimer(1, 200, NULL);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	
}

int Cspinput::SaveRawFile(LPSTR filename)
{
	HFILE Inn;
	DWORD len;

	Inn=_lcreat(filename,0);
	if(Inn==NULL)
	{
		AfxMessageBox("공유규칙을 어겼습니다.");
		return FALSE;
    }
	
	if(lDataSize!=_lwrite(Inn,(LPSTR)pWave,lDataSize))
	{
		AfxMessageBox("저장할 용량이 없습니다.");
		return FALSE;
    }
	_lclose(Inn);
   return TRUE;
}


int Cspinput::SaveWaveFile(LPSTR filename)
{
	HFILE Inn;
	DWORD len;

	Inn=_lcreat(filename,0);
	if(Inn==NULL)
	{
		AfxMessageBox("공유규칙을 어겼습니다.");
		return FALSE;
    }
	_lwrite(Inn,(LPSTR)"RIFF",4);
	len=40+lDataSize;
	
	_lwrite(Inn,(LPSTR)&len,4);
	_lwrite(Inn,(LPSTR)"WAVE",4);
	
	_lwrite(Inn,(LPSTR)"fmt ",4);
	
	len=16;
	_lwrite(Inn,(LPSTR)&len,4);
	short i;
	i=1;
	_lwrite(Inn,(LPSTR)&i,2);
	
	i=(short)WaveRecord.wf.nChannels;
	_lwrite(Inn,(LPSTR)&i,2);
	_lwrite(Inn,(LPSTR)&WaveRecord.wf.nSamplesPerSec,4);
	_lwrite(Inn,(LPSTR)&WaveRecord.wf.nAvgBytesPerSec,4);

	i=(short)WaveRecord.wf.nBlockAlign;
	_lwrite(Inn,(LPSTR)&i,2);
	i=(short)WaveRecord.wBitsPerSample;
	_lwrite(Inn,(LPSTR)&i,2);
	
	_lwrite(Inn,(LPSTR)"data",4);
	_lwrite(Inn,(LPSTR)&lDataSize,4);
	
	if(lDataSize!=_lwrite(Inn,(LPSTR)pWave,lDataSize))
	{
		AfxMessageBox("저장할 용량이 없습니다.");
		return FALSE;
    }
	_lclose(Inn);
   return TRUE;
}

void Cspinput::OnOK() 
{
	CDialog::OnOK();
}

void Cspinput::OnCancel() 
{
	CDialog::OnCancel();
	return;
}

void Cspinput::OnButtonPlay() 
{
	PlayRaw("speech.raw");
}

void Cspinput::PlayRaw(char *filename)
{
	if(FALSE==Raw2Wav("speech.raw")){
			AfxMessageBox("Raw2Wav Error");
	}
	sndPlaySound("temp.wav",SND_ASYNC );
}
