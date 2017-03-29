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
DWORD CommWatchProc(LPVOID lpData){//쓰래드에서는 데이터가 왔는지 감시하다가 들어온 데이터를 읽기만 한면 된다
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
		WaitCommEvent(npCom->hCom,&dwEvtMask,NULL);//데이터가 들어오때까지 대기
		if((dwEvtMask &EV_RXCHAR) == EV_RXCHAR){//데이터가 들어 왔으면....
			do{
				dwLength = npCom->SetReadData();
				dwTotLen += dwLength;
			}while(dwLength);
			if(dwTotLen > 0)
				SendMessage(npCom->m_hwnd,WM_RECEIVEDATA,dwTotLen,0);//메세지를 날려준다.
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
{//각 멤버 변수에 넘겨받은 변수들을 대입시킴으로서 포트설정을 끝낸다.
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


DWORD CCom::WriteCommBlock(LPCVOID lpByte, DWORD dwToWrite)//보낼 데이터와 데이터의 크기를 인수로 받는다.
{
	DWORD dwWritten,dwError,dwErrorFlags;
	COMSTAT comstat;
	if(!WriteFile(hCom,lpByte,dwToWrite,&dwWritten,&osWrite))
	{
		if(GetLastError() == ERROR_IO_PENDING){//ERROR_IO_PENDING == 997
			/* 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우
			Overapped IO의
			특성에 따라 ERROR_IO_PENDING에러메세지가 전달 된다.
			timeout에 정해준 시간만큼 기다려준다.*/
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
	return dwWritten;//실제로 쓰여진 바이트수를 리턴
}


BOOL CCom::OpenComPort()//컴포트를 열고 연결을 시도한다.
{
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;
	//-------------------------------------------------------
	osWrite.Offset = 0;
	osWrite.OffsetHigh=0;
	osRead.Offset=0;
	osRead.OffsetHigh=0;
	osRead.hEvent = CreateEvent(NULL,TRUE, FALSE,NULL);
	if(osRead.hEvent == NULL)//이벤트가 발생하지 않으면
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
		SetupComm(hCom,4096,4096);//포트의 버퍼 크기 설정
		SetCommMask(hCom,EV_RXCHAR);//어떤 신호에 반응할것인가를 설정
		//포트비우기
		PurgeComm(hCom,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		CommTimeOuts.ReadIntervalTimeout = 100;
		CommTimeOuts.ReadTotalTimeoutMultiplier=1;
		CommTimeOuts.ReadTotalTimeoutConstant=1;
		
		CommTimeOuts.WriteTotalTimeoutMultiplier=5;
		CommTimeOuts.WriteTotalTimeoutConstant=5;
		SetCommTimeouts(hCom,&CommTimeOuts);
	}
	
	dcb.DCBlength = sizeof(DCB);
	GetCommState(hCom,&dcb);//dcb의 기본값은 받는다.
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
	dcb.fParity = TRUE; 이부문을 빼줘두 되고 안빼줘도 되지만 빼주니깐 왠쥐 더 잘되는듯 ^^;;*/

	if(SetCommState(hCom,&dcb)){
		fCnt = TRUE;
		if(!AfxBeginThread((AFX_THREADPROC)CommWatchProc,(LPVOID)this))//;//스레드 실행
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

BOOL CCom::CloseConnection()//컴포트 연결을 해제한다.
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

DWORD CCom::SetReadData()//읽은데이터를 버퍼에 저장
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;
	ClearCommError(hCom, &dwErrorFlags, &comstat);
	//--- system queue에 도착한 byte수만 미리 읽는다.
	dwRead = comstat.cbInQue;
	//--> 시스템 큐에서 읽을 거리가 있으면..
	if(dwRead > 0)
	{
		//--> 버퍼에 일단 읽어들이는데.. 읽는데 실패했으면
		if(!ReadFile(hCom,RcvData,2048,&dwRead,&osRead) )
		{
			//--> 읽을 거리가 남았있는지 확인 
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//입출력중인 데이터가 있는 경우 timeouts에 정해준 시간만큼 기다려준다.
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
	//--> 실제 읽어들인 갯수를 리턴.
	return dwRead;	
}

void CCom::SetHwnd(HWND hwnd)//메시지를 전달할 hwnd설정
{
	m_hwnd=hwnd;
}

