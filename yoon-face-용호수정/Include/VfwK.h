
/****************************************************************


					Vfw Control Class
			

			- Modified by Kim Do-Hyung (dhkim008@etri.re.kr)
			- Last Modified ( 2002 / 05 / 18 )

 ****************************************************************/



#include <vfw.h>					
#pragma comment(lib,"vfw32.lib")	



#define IMAGE_SIZE_X		(320)
#define IMAGE_SIZE_Y		(240)
#define BYTE_PER_PIXEL		(3)

#define CAPTURE_INTVL_NORMAL	(200)
#define CAPTURE_INTVL_SLOW		(250)


 
 /********************************************************************************


	CVideoOwner

	1. this is Abstract,Interface Object
	2. which allows Inheriting object to manage CMultiSocket
	3. there 's no implementation					


 *********************************************************************************/


class CMultiSocket;
class CVideoOwner
{
public:// Constructor/Destructor
	CVideoOwner(){};
	~CVideoOwner(){};
public:
	// Notifications from VideoTool
	virtual LRESULT OnStatusMessageCallBack(HWND hwnd, int nID, LPCSTR lpszStatusText){return 0l;};
	virtual LRESULT OnErrorMessageCallBack(HWND hwnd, int nID, LPCSTR lpszErrorText){return 0l;};
	virtual LRESULT OnVideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr){return 0l;};
	virtual LRESULT OnFrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr){return 0l;};

	virtual void	OnTimeProc(UINT uID,UINT uMsg,DWORD dwUser){};
};






/********************************************************************************


	CVideoTool
	1. Video Capture Device를 다루고 
	2. Video Frame 을 Coding, framming, assembling decoding 하며
	3. Frame을 Display 해준다.

 *********************************************************************************/


class CVideoTool
{
	//// Public members & attribute
public:
	HWND GetCapWindow()	{ return m_hWndCap; }

	void SetParentWnd(HWND hWnd)	{ m_hParentWnd = hWnd; }
	int	 GetDriverIndex()	{ return m_nDriverIndex; }

	BITMAPINFO* GetBitmapInfo() { return &m_VideoBitmapInfo; }

	void SetVideoOwner(CVideoOwner* ptr) { m_pVideoOwner = ptr; }

	//// Public functions
	BOOL _CheckVideoSize(int nWidth, int nHeight);
	BOOL _CheckVideoFormat(DWORD dwBitCount);
	BOOL _SetPreviewRate(int msec);
	BOOL _SetPreviewMode(BOOL bPreview);

	BOOL _CreateCaptureView(HWND hParent);	// You should call it from initial update
	BOOL _ConnectToDriver(int nIndex);		// Connect to Driver
	BOOL _DestroyCaptureView();				// Clear up everything

	BOOL _SetPreviewSize(int cx,int cy);	// set size of previewing

	BOOL _BeginContinuousGrab();			// Continuous grab
	BOOL _AbortContinuousGrab();			// Stop Continuous grab
	BOOL _ToggleContinuousGrab();			// Toggle between 2 states
	BOOL _bIsContinuousGrab();				// check if in continuous grab mode

	BOOL _GrabFrame();						// Manual 1 time Grab

	void InvokeDlgVideoFormat();			// Invoke video format dialog
	void InvokeDlgVideoSource();			// Invoke video source control dialog
	void InvokeDlgCaptureSetup();			// Invoke capture setup dialog

	void _ShowDlgVideoFormat();	
	// draw output functions

	// safty utility
	BOOL OnQueryEndSession();
	BOOL bIsDialogUp() {return m_fDialogIsUp;}

	//// MM Timer utility
	MMRESULT timeSetEvent(DWORD dwUser, UINT uDelay);

	//// Timer Event CallBack handlers
	void	TimeProc(UINT uID, UINT uMsg, DWORD dwUser);

	//// Video Event CallBack handlers
	LRESULT OnStatusMessageCallBack(HWND hwnd, int nID, LPCSTR lpszStatusText);
	LRESULT OnErrorMessageCallBack(HWND hwnd, int nID, LPCSTR lpszErrorText);
	LRESULT OnVideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);
	LRESULT OnFrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);

public:

	//// Constructor & Destructor
public:
	CVideoTool();
	~CVideoTool();

	//// Internal memory
private:
	HWND m_hWndCap;			// VFW capture window it self
	HWND m_hParentWnd;		// capture preview window's parent

	CAPDRIVERCAPS	m_capDriverCaps;	// VFW structures
	CAPSTATUS		m_capStatus;
	CAPTUREPARMS	m_captureParms;	
	
	BITMAPINFO			m_VideoBitmapInfo;	// VFW video info for img display
	BITMAPINFOHEADER	m_bmiBlockFrame;	// for block display

	HDRAWDIB			m_drawDibDC;		// Handle to Draw Dib DC

	int m_nDriverIndex;						// capture driver index
	BOOL m_fDialogIsUp;						// whether one of cap Dialog is shown now.
	BOOL m_bIsContinuousGrab;				// TRUE if in continuous grab mode

	// Major user of this class
	CVideoOwner *m_pVideoOwner;

	// Internal implementations
	// timer related
	UINT m_recordTimerID;
	UINT m_playTimerID;

	unsigned int m_nFrameCount;				// frame rate calculation
};

