// Com.cpp : implementation file
//

#include "stdafx.h"
#include "Com.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNCREATE(CCom, CObject)
/////////////////////////////////////////////////////////////////////////////
// CCom

CCom::CCom()
{
	hCom = NULL;
	bFlowCtrl = FC_XONXOFF;
	fCnt = FALSE;
}

CCom::~CCom()
{
	//CloseConnection();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CCom, CSocket)
	//{{AFX_MSG_MAP(CCom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CCom member functions
DWORD CommWatchProc(LPVOID lpData){//�����忡���� �����Ͱ� �Դ��� �����ϴٰ� ���� �����͸� �б⸸ �Ѹ� �ȴ�
/*	DWORD dwEvtMask,dwLength,dwTotLen;
	OVERLAPPED os;
	CCom* npCom=(CCom*) lpData;
	dwTotLen = 0;
	if(npCom == NULL || !npCom->IsKindOf(RUNTIME_CLASS(CCom))){

		return (DWORD)(-1);
	}
	memset(&os,0,sizeof(OVERLAPPED));
	os.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	if(os.hEvent == NULL){
		MessageBox(NULL,"Failed to create event for thread!","com Error!",MB_ICONEXCLAMATION|MB_OK);
		return (FALSE);
	}
	if(!SetCommMask(npCom->hCom,EV_RXCHAR)){
		AfxMessageBox("CommWatchProc Exit 3nd if");
		return(FALSE);
	}
	while(npCom->fCnt){
		dwEvtMask = 0;
		WaitCommEvent(npCom->hCom,&dwEvtMask,NULL);//�����Ͱ� ���������� ���
		if((dwEvtMask &EV_RXCHAR) == EV_RXCHAR){//�����Ͱ� ��� ������....
			do{
				dwLength = npCom->SetReadData();
				dwTotLen += dwLength;
			}while(dwLength);
			if(dwTotLen > 0)
				SendMessage(npCom->m_hwnd,WM_RECEIVEDATA,dwTotLen,0);//�޼����� �����ش�.
			dwTotLen = 0;
		}
	}
	CloseHandle(os.hEvent);
*/	return(TRUE);
}

void CCom::SetXonOff(BOOL chk)
{
	fXonOff=chk;
}

void CCom::SetComPort(CString port, DWORD rate, BYTE bytesize, BYTE stop, BYTE parity)
{//�� ��� ������ �Ѱܹ��� �������� ���Խ�Ŵ���μ� ��Ʈ������ ������.
	bPort=port;
	dwBaudRate=rate;
	bByteSize=bytesize;
	bStopBits=stop;
	bParity=parity;
}

void CCom::SetDtrRts(BYTE chk)
{
	bFlowCtrl=chk;
}


DWORD CCom::WriteCommBlock(LPCVOID lpByte, DWORD dwToWrite)//���� �����Ϳ� �������� ũ�⸦ �μ��� �޴´�.
{
	DWORD dwWritten,dwError,dwErrorFlags;
	COMSTAT comstat;
	if(!WriteFile(hCom,lpByte,dwToWrite,&dwWritten,&osWrite))
	{
		if(GetLastError() == ERROR_IO_PENDING){//ERROR_IO_PENDING == 997
			/* ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ���
			Overapped IO��
			Ư���� ���� ERROR_IO_PENDING�����޼����� ���� �ȴ�.
			timeout�� ������ �ð���ŭ ��ٷ��ش�.*/
			while(!GetOverlappedResult(hCom,&osWrite,&dwWritten,TRUE)){
				dwError = GetLastError();
				if(dwError != ERROR_IO_INCOMPLETE){
					ClearCommError(hCom,&dwErrorFlags,&comstat);
					break;
				}
			}
		}
		else{
			dwWritten = 0;
			ClearCommError(hCom,&dwErrorFlags,&comstat);
		}
	}
	return dwWritten;//������ ������ ����Ʈ���� ����
}


BOOL CCom::OpenComPort()//����Ʈ�� ���� ������ �õ��Ѵ�.
{
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;
	//-------------------------------------------------------
	osWrite.Offset = 0;
	osWrite.OffsetHigh=0;
	osRead.Offset=0;
	osRead.OffsetHigh=0;
	osRead.hEvent = CreateEvent(NULL,TRUE, FALSE,NULL);
	if(osRead.hEvent == NULL)//�̺�Ʈ�� �߻����� ������
		return FALSE;
	osWrite.hEvent = CreateEvent(NULL,TRUE, FALSE,NULL);
	if(NULL == osWrite.hEvent){
		CloseHandle(osRead.hEvent);
		return FALSE;
	}
	//-------------------------------------------------------
	
	if((hCom = CreateFile(bPort,GENERIC_READ|GENERIC_WRITE,0,NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL))==(HANDLE)-1)
	{

		fCnt = FALSE;
		return(fCnt);
	}
	else{
		SetupComm(hCom,4096,4096);//��Ʈ�� ���� ũ�� ����
		SetCommMask(hCom,EV_RXCHAR);//� ��ȣ�� �����Ұ��ΰ��� ����
		//��Ʈ����
		PurgeComm(hCom,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		CommTimeOuts.ReadIntervalTimeout = 100;
		CommTimeOuts.ReadTotalTimeoutMultiplier=1;
		CommTimeOuts.ReadTotalTimeoutConstant=1;
		
		CommTimeOuts.WriteTotalTimeoutMultiplier=5;
		CommTimeOuts.WriteTotalTimeoutConstant=5;
		SetCommTimeouts(hCom,&CommTimeOuts);
	}
	
	dcb.DCBlength = sizeof(DCB);
	GetCommState(hCom,&dcb);//dcb�� �⺻���� �޴´�.
	dcb.BaudRate = dwBaudRate;
	dcb.ByteSize = bByteSize;
	dcb.Parity = bParity;
	dcb.StopBits = bStopBits;
	/*
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fOutxCtsFlow = 0;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fInX = dcb.fOutX = 1;
	dcb.XonChar = ASCII_XON;
	dcb.XoffChar = ASCII_XOFF;
	dcb.XonLim = 100;
	dcb.XoffLim = 100;
	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	dcb.fBinary = TRUE;
	dcb.fParity = TRUE; �̺ι��� ����� �ǰ� �Ȼ��൵ ������ ���ִϱ� ���� �� �ߵǴµ� ^^;;*/

	if(SetCommState(hCom,&dcb)){
		fCnt = TRUE;
		if(!AfxBeginThread((AFX_THREADPROC)CommWatchProc,(LPVOID)this))//;//������ ����
		{
			AfxMessageBox("Thread Run Fail");
			CloseConnection();
			fCnt = FALSE;
		}
	}
	else{
		AfxMessageBox("Connect Failed. DCB Setting Error");
		fCnt=FALSE;
		CloseHandle(hCom);
	}
	return(fCnt);
}

BOOL CCom::CloseConnection()//����Ʈ ������ �����Ѵ�.
{
	fCnt = FALSE;
	SetCommMask(hCom,0);
	EscapeCommFunction(hCom,CLRDTR);
	PurgeComm(hCom,PURGE_TXABORT|PURGE_RXABORT|
		PURGE_TXCLEAR|PURGE_RXCLEAR);
	CloseHandle(hCom);
	
	CloseHandle(osRead.hEvent);
	CloseHandle(osWrite.hEvent);
	return(TRUE);
}

DWORD CCom::SetReadData()//���������͸� ���ۿ� ����
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;
	ClearCommError(hCom, &dwErrorFlags, &comstat);
	//--- system queue�� ������ byte���� �̸� �д´�.
	dwRead = comstat.cbInQue;
	//--> �ý��� ť���� ���� �Ÿ��� ������..
	if(dwRead > 0)
	{
		//--> ���ۿ� �ϴ� �о���̴µ�.. �дµ� ����������
		if(!ReadFile(hCom,RcvData,2048,&dwRead,&osRead) )
		{
			//--> ���� �Ÿ��� �����ִ��� Ȯ�� 
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//��������� �����Ͱ� �ִ� ��� timeouts�� ������ �ð���ŭ ��ٷ��ش�.
				while (! GetOverlappedResult(hCom,&osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError(hCom,&dwErrorFlags,&comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError(hCom,&dwErrorFlags, &comstat);
			}
		}
	}
	//--> ���� �о���� ������ ����.
	return dwRead;	
}

void CCom::SetHwnd(HWND hwnd)//�޽����� ������ hwnd����
{
	m_hwnd=hwnd;
}

