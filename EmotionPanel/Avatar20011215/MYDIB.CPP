//MYDIB.CPP BMP 화일을 출력하는 클래스

#include "stdafx.h"
#include "mydib.h"


//생성자 화일명을 주어서 클래스가 만들어지면서 이미지를 가지고 
//있게 한다.
CMyDib::CMyDib(LPSTR filename)
{
	//화일명을 m_strFileName에 기록한다.
	lstrcpy((LPSTR)m_strFileName,(LPSTR)filename);
	//이미지 포인터와 버퍼를 null로 설정한다.
	szDib=NULL;
    lpDib=NULL;
	m_palDIB=NULL;
	//팔레트를 null로 설정한다.
	szPal=NULL;
	//bmp 화일을 로드시킨다.
	if(!bLoadBmpFile())
        AfxMessageBox(m_strMessageTemp);


}

CMyDib::~CMyDib()
{
	if(szDib)
	{
		::GlobalUnlock(szDib);
		::GlobalFree(szDib);
	}
	if(m_palDIB)
		delete m_palDIB;
	if(szPal)
	{
		::GlobalUnlock((HGLOBAL) szPal);
		::GlobalFree((HGLOBAL) szPal);
	}

}
BOOL CMyDib::bLoadBmpFile()
{
	HFILE bmpFile;

	//화일을 연다.
	bmpFile=_lopen((LPSTR)m_strFileName,OF_READ);

	if(bmpFile<0)
	{
		sprintf(m_strMessageTemp,"%s 화일이 없습니다",m_strFileName);
	   return FALSE;
	}
	//bmpHeder 구조체에 BMPHEADER 헤더 정보를 읽어 온다.
	if(_lread(bmpFile,(LPSTR)&bmpHeader,sizeof(bmpHeader))!=sizeof(bmpHeader))
	{
		sprintf(m_strMessageTemp,"%s BMPHEADER 를 읽을수 없습니다!",m_strFileName);
		return FALSE;
	}
	//화일이 bmp 일경우에는 화일 처음에 "BM"이라고 설정해있음
	if (bmpHeader.bfType != DIB_HEADER_MARKER)
	{
		sprintf(m_strMessageTemp,"%s BMP 화일이 아닙니다.",m_strFileName);
		return FALSE;
	}
	//이미지 데이타 크리만큼 메모리를 설정한다.
	szDib =::GlobalAlloc(GHND,bmpHeader.bfSize);
	if(szDib==NULL)
	{
		sprintf(m_strMessageTemp,"%s 메모리가 부족합니다.",m_strFileName);
		return FALSE;
	}
	//이미지 데이타 버퍼를 컨트롤 할수 있도록 lpDip 에 락킹한다.
	lpDib=(LPSTR)::GlobalLock(szDib);
	 UINT len;
	 //이미지 데이타를 읽는다.
	if((len=_lread(bmpFile,(LPSTR)lpDib,bmpHeader.bfSize-sizeof(bmpHeader)))!=bmpHeader.bfSize-sizeof(bmpHeader))
	{
		::GlobalUnlock(szDib);
		::GlobalFree(szDib);
		szDib=NULL;
		lpDib=NULL;
		sprintf(m_strMessageTemp,"%s 화일을 읽을수 없습니다.",m_strFileName);
		return FALSE;
	}
    bmpInfoHeader = (LPBITMAPINFOHEADER)lpDib;
	bmcInfoHeader = (LPBITMAPCOREHEADER)lpDib;
	bmpInfo =(LPBITMAPINFO)lpDib;
	bmcInfo = (LPBITMAPCOREINFO)lpDib;
	bCreatePalette();
	_lclose(bmpFile);
 return TRUE;
}
//팔레트를 만든다.
BOOL CMyDib::bCreatePalette()
{
	//현재 사용하는 칼라수를 받고
	m_nColorNums=bGetColorNums();
	if(m_nColorNums==0)
		return TRUE;

   //필레트 메모리를 확보한다음
	szPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* m_nColorNums);

	if (szPal == NULL)
	{
		sprintf(m_strMessageTemp,"%s palette 메모리가 부족합니다.",m_strFileName);
		return FALSE;
	}
	//lpPal 락킹하고
	lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) szPal);

	lpPal->palVersion = PALVERSION;
	lpPal->palNumEntries =(WORD)m_nColorNums;
    //칼라수만큼 팔레트를 칼레트 엔트리에 넣는다.
	for (int i = 0; i < (int)m_nColorNums; i++)
	{
		if (m_nDibStyle) //win3.0 이상버전일경우 다음과 같이 하고
		{
			lpPal->palPalEntry[i].peRed = bmpInfo->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen =bmpInfo->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue = bmpInfo->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}
		else //그외에는 다음과 같이 한다.
		{
			lpPal->palPalEntry[i].peRed = bmcInfo->bmciColors[i].rgbtRed;
			lpPal->palPalEntry[i].peGreen = bmcInfo->bmciColors[i].rgbtGreen;
			lpPal->palPalEntry[i].peBlue = bmcInfo->bmciColors[i].rgbtBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}
	}
//    CPalette 구조체에 lpPal를 링크시켜서 팔레트를 만듭니다.
	m_palDIB = new CPalette;
    m_palDIB->CreatePalette(lpPal);

	return TRUE;
}

//현재 비트맵의 칼라수를 찾는다.
int CMyDib::bGetColorNums()
{

	//3.0 버전의 비트맵이면
   	if (IS_WIN30_DIB(lpDib))
	{
		//m_nDibStyle  를 3.0 비트맵 버전이라고 설정
		m_nDibStyle=TRUE;
		m_nWidthX=bmpInfoHeader->biWidth;
		m_nWidthY=bmpInfoHeader->biHeight;
		DWORD dwClrUsed;
		//사용된 칼라수를 받아서 만약에 그것이 0이 아니면
		//그것이 현재 비트맵이 사용하는 칼라수 이므로 설정
		dwClrUsed = bmpInfoHeader->biClrUsed;
		if (dwClrUsed != 0)
			return (int)dwClrUsed;
	}
	else
	{
		m_nDibStyle=FALSE;
		m_nWidthX=bmcInfoHeader->bcWidth;
		m_nWidthY=bmcInfoHeader->bcHeight;
	}

	WORD wBitCount;
	//비트 카운트를 받는다.
	if (IS_WIN30_DIB(lpDib))
		wBitCount = bmpInfoHeader->biBitCount;
	else
		wBitCount = bmcInfoHeader->bcBitCount;

	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}

}

LPSTR CMyDib::bFindDib()
{
	return (lpDib + *(LPDWORD)lpDib + bPaletteSize());
}
//팔래트 크기를 알아본다.
WORD CMyDib::bPaletteSize()
{
	//3.0이상 버전 이미지 이면
   if (m_nDibStyle)
	  return (WORD)(m_nColorNums * sizeof(RGBQUAD));
   else
	  return (WORD)(m_nColorNums * sizeof(RGBTRIPLE));
}
//윈도우에 설정되어있는 팔레트를 사용한다.
int CMyDib::SetSystemPalette(HDC hDC)
{
	
	if(m_nColorNums != 0) return FALSE;
	//현재 트루칼라모드이면 설정안함
	if(!::GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE) return FALSE;
	//현재 모드가 몇칼라 모드이며 팔레트엔트리가 몇개인지 확인
	int nSysColors = ::GetDeviceCaps(hDC, NUMCOLORS);
	int nPalEntries = ::GetDeviceCaps(hDC, SIZEPALETTE);

	if(nPalEntries ==0)
		return FALSE;
	
	
	int nEntries = (nPalEntries == 0) ? nSysColors : nPalEntries;

	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2 * sizeof(WORD) +
		nEntries * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = nEntries;
	
	//시스템 팔레트를 얻고
	::GetSystemPaletteEntries(hDC, 0, nEntries,
		(LPPALETTEENTRY) ((LPBYTE) pLogPal + 2 * sizeof(WORD)));
    HPALETTE m_hPalette;
	
	//시스템 팔레트를 설정한다.
	m_hPalette = ::CreatePalette(pLogPal);
	SelectPalette(hDC, m_hPalette, TRUE);
	delete pLogPal;
	return TRUE;
}


void  CMyDib::PaintImage(int x, int y, HDC hDC)
{
	LPSTR    lpDIBBits;          
	BOOL     bSuccess=FALSE;     
	HPALETTE hPal=NULL;          
	HPALETTE hOldPal=NULL;       

	if (lpDib == NULL)
		return;
	//실질 이미지로 분리한다.
	lpDIBBits = bFindDib();

	if (m_palDIB != NULL)
	{	
		hPal = (HPALETTE) m_palDIB->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	    RealizePalette(hDC);
	}
	else
	{
		if(SetSystemPalette(hDC))
		    RealizePalette(hDC);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);

	::SetDIBitsToDevice(hDC,                    // hDC
								   x,             // DestX
								   y,              // DestY
								   m_nWidthX,        // nDestWidth
								   m_nWidthY,       // nDestHeight
								   0,            // SrcX
								   0,   // SrcY
								   0,                          // nStartScan
								   (WORD)m_nWidthY,  // nNumScans
								   lpDIBBits,                  // lpBits
								   bmpInfo,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage


	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

}


void  CMyDib::PaintImage(int x, int y, int cx, int cy,HDC hDC )
{    
	LPSTR    lpDIBBits;          
	BOOL     bSuccess=FALSE;     
	HPALETTE hPal=NULL;          
	HPALETTE hOldPal=NULL;       

	if (lpDib == NULL)
		return;

	lpDIBBits = bFindDib();

	if (m_palDIB != NULL)
	{	
		hPal = (HPALETTE) m_palDIB->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	    RealizePalette(hDC);
	}
	else
	{
		if(SetSystemPalette(hDC))
		    RealizePalette(hDC);
	}


	::SetStretchBltMode(hDC, COLORONCOLOR);

	::StretchDIBits(hDC,                   // hDC
								   x,             // DestX
								   y,              // DestY
								   cx,        // nDestWidth
								   cy,       // nDestHeight
								   0,            // SrcX
								   0,   // SrcY
								   m_nWidthX,  
								   m_nWidthY,
								   lpDIBBits,                  // lpBits
								   bmpInfo,     // lpBitsInfo
								   DIB_RGB_COLORS,SRCCOPY);            // wUsage


	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

}
		
//이미지를 저장한다.
int CMyDib::SaveImage(LPSTR filename)
{

	if(filename!=NULL)
		lstrcpy((LPSTR)m_strFileName,(LPSTR)filename);
	int OffBits;
	HFILE bmpFile;
	OffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * m_nColorNums;	
	bmpFile=_lcreat(m_strFileName,0);
	UINT len;
	len=_lwrite(bmpFile,(LPSTR)&bmpHeader,sizeof(BITMAPFILEHEADER));
	len=_lwrite(bmpFile,(LPSTR)lpDib,bmpHeader.bfSize-sizeof(bmpHeader));
	_lclose(bmpFile);
	return TRUE;
}








