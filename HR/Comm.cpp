//	Coded by yhseo
//	Comm.cpp for Rs232c Communication
#include "stdafx.h"
#include "comm.h"

IMPLEMENT_DYNCREATE(CComm, CObject)

CComm::CComm( )
{
   m_hCom=NULL;
   m_bConnected= FALSE ;
}

CComm::~CComm( )
{
	m_bConnected = FALSE;
	CloseHandle(m_hCom);
}
DWORD ReceiveThread(LPVOID lpData)
{	
	CComm*      pComm = (CComm*) lpData ;	
	while (pComm->m_bConnected)
	{
		if (pComm->ReadCommBlock(1))
		{	
			//TRACE("\n[%c] [%c]",*pComm->m_readBuffer,*(pComm->m_readBuffer+1));
			SendMessage(pComm->m_hwnd,WM_RECEIVEDATA,0,0);     
		}
	}

	//CloseHandle(pComm->m_hCom);
	return TRUE;
} 


void CComm::SetHwnd(HWND hwnd)
{
   m_hwnd=hwnd;
}
void CComm::SetReadBuffer(LPSTR readBuff,int size)
{

	m_readBuffer=readBuff;
	m_nBufferSize=size;
}

void CComm::CloseSerial(void)
{
	CloseHandle(m_hCom);

//	m_bConnected = FALSE;
}

BOOL CComm::InitSerial(int port,DWORD baud)
{
	char	szPort[15] ;
    DWORD	dwError;
	BOOL	bSuccess;
	DCB		dcb;

	wsprintf( szPort, "COM%d", port ) ;

	m_hCom = CreateFile(szPort,
			GENERIC_WRITE|GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
	);

	if(m_hCom == INVALID_HANDLE_VALUE)
	{
		dwError = GetLastError();
	}

	bSuccess = GetCommState(m_hCom,&dcb);

	if(!bSuccess)
	{
		return FALSE;
	}

	dcb.BaudRate = baud;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	bSuccess = SetCommState(m_hCom,&dcb);

	if(!bSuccess) {
		return FALSE;
	}

	return TRUE;
}

BOOL CComm::SetBaud(DWORD baud)
{
	DCB		dcb;
	BOOL	bSuccess;
	bSuccess = GetCommState(m_hCom,&dcb);

	if(!bSuccess)
	{
		return FALSE;
	}

	dcb.BaudRate = baud;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	bSuccess = SetCommState(m_hCom,&dcb);

	if(!bSuccess) {
		return FALSE;
	}

	return TRUE;
}

void CComm::RunThread(void)
{
	m_bConnected = TRUE ;
	AfxBeginThread((AFX_THREADPROC)ReceiveThread,(LPVOID)this);
}

int CComm::ReadCommBlock(int nMaxLength )
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength;

	// only try to read number of bytes in queue 
	ClearCommError( m_hCom, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;


	memset(m_readBuffer,0,m_nBufferSize);
	if (dwLength > 0)
	{
		fReadStat = ReadFile( m_hCom, m_readBuffer,
		                    dwLength, &dwLength, NULL) ;
	}
   
	return ( dwLength ) ;
} 

BOOL CComm::WriteCommBlock(LPSTR lpByte , DWORD dwBytesToWrite)
{

	DWORD       dwBytesWritten ;


	return WriteFile( m_hCom, lpByte, dwBytesToWrite,
	                       &dwBytesWritten, NULL) ;
} 

BOOL CComm::WriteBlock(LPCVOID lpByte, DWORD dwBytesToWrite, LPDWORD dwBytesWritten) 
{
	return WriteFile( m_hCom, lpByte, dwBytesToWrite,
	                       dwBytesWritten, NULL) ;
	
}


BOOL CComm::SendChar(int ch)
{
	DWORD      dwLength;
	
	return WriteFile(m_hCom,(LPSTR)&ch,1,&dwLength,NULL);
}

int CComm::ReadChar(BYTE *ch)
{
//	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength;
	int nMaxLength=1;

	// only try to read number of bytes in queue 
	ClearCommError( m_hCom, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

	dwLength=1;
	if (dwLength > 0)
		dwLength=ReadFile(m_hCom,(LPSTR)ch,dwLength,&dwLength,NULL);

	return ( dwLength ) ;
}

