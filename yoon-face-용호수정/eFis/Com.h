#if !defined(AFX_COM_H__C2F59CB9_7958_4475_963B_28FF2878BEDD__INCLUDED_)
#define AFX_COM_H__C2F59CB9_7958_4475_963B_28FF2878BEDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Com.h : header file
//
#define MAXBLOCK 80
#define MAXPORTS 4

#define FC_DTRDSR 0x01
#define FC_RTSCTS 0x02
#define FC_XONXOFF 0x04

#define ASCII_BEL 0x07
#define ASCII_BS 0x08
#define ASCII_LF 0x0A
#define ASCII_CR 0x0D
#define ASCII_XON 0x11
#define ASCII_XOFF 0x13
#define ASC_NUL 0x00
#define WM_RECEIVEDATA WM_USER+201
		


/********************************************************************************


					devpia �ڷ�� (Not Modified)


 *********************************************************************************/



/////////////////////////////////////////////////////////////////////////////
// CCom command target


class CCom : public CObject
{
	DECLARE_DYNCREATE(CCom)
// Attributes
public:
	HANDLE hCom;//����Ʈ�� ������ �ڵ鷯
	BOOL fCnt;//����Ʈ ���� ������ �Ǵ��� ����(TRUE = ����)
	HWND m_hwnd;//�޼����� ������ ���� �÷���
	CString bPort;//port���� ���´�.
	BOOL fXonOff;
	BYTE bByteSize, bFlowCtrl, bParity, bStopBits;
	DWORD dwBaudRate;
	HANDLE hWatchThread;
	OVERLAPPED osWrite,osRead;

	
// Operations
public:
	CCom();
	virtual ~CCom();
	
// Overrides
public:
	BYTE RcvData[2048];//����Ʈ������ �����͸� ������ �ִ´�.
	DWORD Error;
	void SetHwnd(HWND hwnd);
	DWORD SetReadData();
	BOOL CloseConnection();
	BOOL OpenComPort();
	DWORD WriteCommBlock(LPCVOID lpByte,DWORD dwToWrite);
	void SetDtrRts(BYTE chk);//Dtr Rts����
	void SetComPort(CString port,DWORD rate,BYTE bytesize,BYTE stop,BYTE parity);//����Ʈ����
	void SetXonOff(BOOL chk);//XonOff�� ����
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCom)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CCom)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COM_H__C2F59CB9_7958_4475_963B_28FF2878BEDD__INCLUDED_)
