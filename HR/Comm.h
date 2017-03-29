//	Coded by yhseo
//	Comm.h	for Rs232c Communication

// ascii definitions
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#define WM_RECEIVEDATA WM_USER+1
/////////////////////////////////////////////////////////////////////////////
// CComm window

class CComm : public CObject
{

   DECLARE_DYNCREATE( CComm )

public:
   HANDLE	m_hCom ;//컴포트 디바이스 연결 핸들러
   BOOL		m_bConnected;//컴포트가 연결되면 1로 설정
   LPSTR	m_readBuffer;
	int m_nBufferSize;
   HWND m_hwnd;//메세지를 전달할 윈도우 플러그

// Construction
public:
	CComm();
    //컴포트를 설정함
    BOOL InitSerial(int,DWORD);
	void CloseSerial(void);
    
	int  ReadCommBlock(int);
    BOOL WriteCommBlock(LPSTR, DWORD);
	BOOL WriteBlock(LPCVOID lpByte, DWORD dwBytesToWrite, LPDWORD dwBytesWritten); 
	BOOL SendChar(int);
	BOOL ReadChar(BYTE *);
	BOOL SetBaud(DWORD);
	void SetReadBuffer(LPSTR,int);
	void SetHwnd(HWND);
	void RunThread(void);

// Implementation
public:
	virtual ~CComm();

protected:
};


/////////////////////////////////////////////////////////////////////////////
