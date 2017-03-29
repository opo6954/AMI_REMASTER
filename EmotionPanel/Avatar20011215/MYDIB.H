//MYDIB.H BMP 화일을 출력하는 클래스

#define DEFAULTLEN 80
#define DIB_HEADER_MARKER ((WORD)('M'<<8) | 'B')
#define PALVERSION   0x300
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))


class CMyDib
{

public:
	CMyDib(LPSTR filename);
	~CMyDib();
	void  PaintImage(int x, int y, HDC hDC);
	void  PaintImage(int x, int y, int cx, int cy,HDC hDC );
	int	  SetSystemPalette(HDC hDC);
	int SaveImage(LPSTR filename=NULL);
	int GetXSize(){ return m_nWidthX;}
	int GetYSize(){ return m_nWidthY;}

	LPSTR bFindDib();//데이타에서 비트멥 데이타만 찾는다.

protected:				   
	//멤버
	char m_strFileName[MAX_PATH];//bmp 화일명
	char m_strMessageTemp[DEFAULTLEN];//에라 메세지를 담는 template
	BITMAPFILEHEADER bmpHeader; //bmp 화일의 헤더
	LPBITMAPINFOHEADER bmpInfoHeader; //bmp 정보헤더
	LPBITMAPCOREHEADER bmcInfoHeader; 
	LPBITMAPINFO bmpInfo; //bmp 정보헤더
	LPBITMAPCOREINFO bmcInfo; 
	int m_nWidthX;
	int	m_nWidthY;
	HANDLE szPal;
	LPLOGPALETTE lpPal;     

	HANDLE szDib;
	LPSTR  lpDib;
	int m_nColorNums;//현재 bmp의 컬러수
	int m_nDibStyle;//IS_WIN30_DIB 인가 아닌가 설정
	CPalette* m_palDIB;//빨레트


protected:
	//내부 함수
	int bLoadBmpFile();
	BOOL bCreatePalette(); //빨레트를 만든다.
	int bGetColorNums(); //칼라수를 얻는다.
    WORD bPaletteSize();//빨레트 크기를 얻는다.


};