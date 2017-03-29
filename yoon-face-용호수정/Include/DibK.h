
#ifndef __DIBK_H__
#define __DIBK_H__



/****************************************************************
*                                                               *
*       Include Files                                           *
*                                                               *
*****************************************************************/
#include <afxtempl.h>		// for CArray
#include <objbase.h> 
#include <strmif.h> 
#include <control.h> 
#include <evcode.h> 
#include <uuids.h> 


/****************************************************************
*                                                               *
*       Predefined Data Types                                   *
*                                                               *
*****************************************************************/
typedef CArray<int, int> CIntArray;
typedef CArray<float, float> CFloatArray;
typedef CArray<double, double> CDoubleArray;


/****************************************************************
*                                                               *
*       CDib Class (Bitmap Manipulation Class)                  *
*                                                               *
*...............................................................*
*       jLee    revised 97/01/24                                *
*****************************************************************/

#define WIDTHBYTES(bits)        (((bits) + 31) / 32 * 4)
#define TYPE_BITMAP             ((WORD) ('M' << 8) | 'B')

// Type of Files
#define FILETYPE_NONE           0
#define FILETYPE_BMP            1
#define FILETYPE_JPEG           2
#define FILETYPE_GIF            3
#define FILETYPE_TIF			4
#define FILETYPE_PPM			5

class CDib : public CObject
{
// protected data members
protected:
        unsigned char           *m_pPackedDib;

        // references
        BITMAPINFO              *m_pInfo;
        BITMAPINFOHEADER        *m_pInfoHeader;
        RGBQUAD                 *m_pRgbQuad;
        unsigned char           *m_pPattern;
        unsigned char           m_PaletteUsage[256]; 

// static data member
public:
        static unsigned char m_BitMask[8];
        static CString m_ExtensionDib;
        static CString m_ExtensionJpg;
        static CString m_ExtensionGif;
		static CString m_ExtensionTif;
		static CString m_ExtensionPpm;

// constructor and destructors
public:
        CDib();
        CDib(const char *pFileName);
		CDib(const char *pFileName, int nWidht, int nHeight);
        CDib(int width, int height, int bitcount, PALETTEENTRY *pEntries,  
             unsigned char *pPattern);
        CDib(const VARIANT &pImage);
        DECLARE_DYNCREATE(CDib)
        ~CDib();

// Serialization
public:
        // serialize는 DIB File을 읽어들임.
        virtual void Serialize(CArchive& ar);

// Protected Operations
protected:
        void ResetVariables();
        void DestroyContents();
        void AssignReferenceVariables();
        BITMAPINFO *GetCopyOfBitmapInfo();
        static int ParseFileType(const char *pFileName);

// File Operations
public:
        BOOL ReadImage(const char *pFileName);
        BOOL SaveImage(const char *pFileName);
        BOOL ReadDib(const char *pFileName);
        BOOL SaveDib(const char *pFileName);

		BOOL ReadJPG(const char *pFileName);
		BOOL ReadTif(const char *pFileName);
		BOOL ReadPpm(const char *pFileName);
		BOOL ReadRaw(const char *pFileName, int nWidth, int nHeight);

// Data Allocation
public:
        // raw pattern을 가지고 있는 경우, CDib를 초기화
        void Initialize(int width, int height, int bitcount, PALETTEENTRY *pEntries,
                        unsigned char *pPattern);
        // pattern은 지정하지 않고, memory만 allocate
        void Allocate(int width, int height, int bitcount);
        // Windows의 Packed DIB format(Clipboard에 사용되는 형식)에 의한 CDib의 초기화 
        void LoadPackedDib(void *pPackedDib);

// Create New CDib Functions
public:
        // Color CDib로부터 흑백 CDib를 작성
        CDib *GetGrayCDib();
        // 똑같은 CDib를 작성
        CDib *CopyCDib();
        // Size를 변경한 CDib를 작성
        CDib *GetSizedCDib(int width, int height=0);
        // Edge를 추출한 CDib를 작성 (8 bit gray image)
        CDib *GetEdgeCDib(double threshold, BOOL bIsGray=FALSE);
        // YUV Color Model로 변환된 Dib를 그대로 RGB 24 bit Format으로
        // save한 데이타를 작성한다. 따라서 일반적인 bitmap format과는
        // 다르며 display했을 경우, 이상한 image가 뜨게 되므로 특수한
        // 처리목적으로만 이용할 것.
        CDib *GetYUVCDib();

// Clipboard Functions
public:
        BOOL GetClipboard();
        BOOL SetClipboard();

// Information Functions
public:
        inline LONG Width();
        inline LONG Height();
        inline WORD BitCount();         // Windows BMP의 경우, bitcount는 1, 4, 8, 16, 24, 32임.
        inline BOOL HavePalette();      // 16 bit 이하의 Dib는 Palette를 가짐.
        inline BITMAPINFO *GetBitmapInfo();
        inline BITMAPINFOHEADER *GetBitmapInfoHeader();
        inline WORD ByteWidth();        // Width in Bytes (aligned to 4 bytes)
        static inline WORD ByteWidth(LONG nbits);
        inline BOOL IsInitialized();

        inline unsigned char *GetPackedDibPointer();
        inline unsigned char *GetPattern();             // pattern부에 대한 pointer
        unsigned char *GetPointer(int x, int y);        // 좌표 (x, y)에 대한 pointer
        unsigned char Intensity(int x, int y);          // 좌표 (x, y)의 pixel의 gray값 (color image의 경우, gray로 conversion 됨)
        unsigned char GetIndex(int x, int y);           // Palette Image의 index, full color의 경우는 0이 return됨
        RGBQUAD GetColor(int x, int y);                 // 좌표 위치 pixel의 color 값

        DWORD PaletteSize();     // size of palette entry data in bytes
        DWORD NumberOfColors();  // number of palette colors
        static DWORD CalcPackedDibSize(BITMAPINFOHEADER *pBi);
        static void RGBtoYIQ(RGBQUAD rgb, unsigned char *y, unsigned char *i, unsigned char *q);
        static unsigned char GetBrightness(RGBQUAD rgb);
        void CDib::RGBtoYUV(RGBQUAD rgb, unsigned char *y, unsigned char *u, unsigned char *v);
        RGBQUAD CDib::YUVtoRGB(unsigned char y, unsigned char u, unsigned char v);

// Operations
public:
        void SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, PALETTEENTRY *pEntries);     // set color table
        void GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, PALETTEENTRY *pEntries);     // get color table
        void SetGrayPalette();          // set current palette to gray palette
        void Inverse();
        void UpsideDown();              // 패턴의 상하 바꿈
		//void ResetContents();
        void ResetContents(BYTE value = 0);           // pattern을 무조건 0으로
        static RGBQUAD Decode16(WORD data);
        static WORD Encode16(unsigned char red, unsigned char green, unsigned char blue);

        // 패턴의 일부를 copy 할 경우
        BOOL CopyRect(CRect *pTargetRect, CDib *pSrcDib, CRect *pSrcRect, int BackColor);
        static void PaletteEntryToRgbQuad(PALETTEENTRY *pEntries, RGBQUAD *pRgbQuad, int count, 
                    unsigned char *pPaletteUsage=NULL);
        static void RgbQuadToPaletteEntry(RGBQUAD *pRgbQuad, PALETTEENTRY *pEntries, int count,
                    unsigned char *pPaletteUsage=NULL);

        // Get Image Dimension Without Loading it
        static BOOL GetImageInfo(const char *pFileName, UINT *pWidth, UINT *pHeight, UINT *pBitCount);
        static BOOL GetImageInfoBmp(const char *pFileName, UINT *pWidth, UINT *pHeight, UINT *pBitCount);

// GDI Operations
        CPalette *GetPalette();
        CPalette *GetPaletteNoCollapse();
        CPalette *GetPaletteNoFlag();
        CBitmap *GetCBitmap(CDC *pDC=NULL);

        // CDib Object를 화면 (pDC)에 그림 
        void LoadImage(CDC *pDC, CRect *pRectTarget=NULL, 
                       CRect *pRectSource=NULL, DWORD dwRop=SRCCOPY);
        // LoadImage와 같은 기능이나, Palette Operation까지를 function내에서 처리
        void LoadPaletteImage(CDC *pDC, CRect *pRectTarget=NULL,
                       CRect *pRectSource=NULL, DWORD dwRop=SRCCOPY);

// Additional Functions
        BOOL IsGrayImage();
        void SplitChannel(CDib **pDibR, CDib **pDibG, CDib **pDibB);
        static CDib *MergeChannel(CDib *pDibR, CDib *pDibG, CDib *pDibB, int bitcount=24);
        void ConvertYUV2RGB();  // YUV format의 CDib를 RGB format의 일반 CDib로 변환

};

inline LONG CDib::Width()
{
        return m_pInfoHeader->biWidth;
}

inline LONG CDib::Height()
{
        return m_pInfoHeader->biHeight;
}

inline WORD CDib::BitCount()
{
        return m_pInfoHeader->biBitCount;
}

inline BOOL CDib::HavePalette()
{
        return (BitCount()<=8);
}

inline BITMAPINFO * CDib::GetBitmapInfo()
{
        return m_pInfo;
}

inline BITMAPINFOHEADER * CDib::GetBitmapInfoHeader()
{
        return m_pInfoHeader;
}

inline WORD CDib::ByteWidth()
{
        return (WORD) ByteWidth(m_pInfoHeader->biWidth * m_pInfoHeader->biBitCount);
}

inline WORD CDib::ByteWidth(LONG nbits)
{
        return WIDTHBYTES(nbits);
}

inline BOOL CDib::IsInitialized()
{
        return (m_pPackedDib!=NULL);
}

inline unsigned char * CDib::GetPattern()
{
        return m_pPattern;
}

inline unsigned char * CDib::GetPackedDibPointer()
{
        return m_pPackedDib;
}



/*==============================================================*
*                                                               *
*       class CHistogram                                        *
*                                                               *
*===============================================================*/
class CHistogram : public CObject
{
// protected data members
protected :
        int m_Size;
        int *m_pData;
        
// constructors and destructors
public:
        CHistogram(int size=256);
		CHistogram(int* Histogram,int size=256);
        ~CHistogram();
        
// public operations
public:
        inline int GetSize();
        inline int GetValue(int index);
        inline int operator[](int index);
        inline void Increment(int index);
        inline void Decrement(int index);
        inline void SetValue(int index, int data);
        inline void ResetAll();
        int SumOfItems(int start=0, int count=0);       // count가 0인 경우, array의 크기(m_Size)와 같은 것으로 취급.
		int GetRatioThreshold(double dRatio);
};

inline int CHistogram::GetSize()
{
        return m_Size;
}

inline int CHistogram::operator[](int index)
{
        return GetValue(index);
}

inline int CHistogram::GetValue(int index)
{
#ifdef __DEBUG
        if ((index<0) || (index>=m_Size)) {
          AfxMessageBox("??? CHistogram Index Exceeded the Range");
          return 0;
        }
#endif
        return *(m_pData+index);
}

inline void CHistogram::Increment(int index)
{
#ifdef __DEBUG
        if ((index<0) || (index>=m_Size)) {
          AfxMessageBox("??? CHistogram Index Exceeded the Range");
          return 0;
        }
#endif
        (*(m_pData+index))++;
}

inline void CHistogram::Decrement(int index)
{
        (*(m_pData+index))--;
}

inline void CHistogram::SetValue(int index, int data)
{
        (*(m_pData+index)) = data;
}

inline void CHistogram::ResetAll()
{
        memset(m_pData, 0, sizeof(int) * m_Size);
}




/********************************************************************************


						Tiff Class

			- Modified by Kim Do-Hyung (dhkim008@etri.re.kr)
			- Last Modified  ( 2002 / 07 / 02 )


 *********************************************************************************/

#define TIFFbyte			1
#define TIFFascii			2
#define TIFFshort			3
#define TIFFlong			4
#define TIFFrational		5

//읽을 수 있는 헤더의 종류
#define NewSubFile			254
#define ImageWidth			256
#define ImageHeight			257
#define RowPerStrip			278
#define StripOffsets		273
#define StripByteCounts		279
#define SamplesPerPixel		277
#define BitPerSample		258
#define Compression			259
#define PhotometricInterp	262
#define ColorMap			320


//tag class ( tiff class내부에서 사용이 됩니다.)
class Tag
{
public:
	__int16 TagType;
	__int16 NumberSize;
	__int32 Length;
	__int32 OffSet;

	void TagRead(int,FILE *);
	void TagDefault(__int16,__int16,__int32,__int32);
	friend __int16 convert2(__int16);
	friend __int32 convert4(__int32);
};

//tiff class, tag를 헤독하여 tiff의 정보를 저장하는 곳입니다. (영상까지)
class CTiff
{
	char id[2];
	__int16 ver;
	__int32 startoffset;
	__int16 tag_num;

	int TypeTiff;

	__int32 newsubfile;
	__int32 rowperstrip;
	__int32 width;
	__int32 height;
	__int32 offsets;
	__int32 bytecounts;
	__int32 samplesperpixel;
	__int32 bitpersample;
	__int32 compression;
	__int32 photometricinterp;
	__int32 colormap;
	__int32 channel;

	int virtual_channel;
	int palette;

	LPBYTE TiffPalette;
	LPBYTE map;
	LPBYTE Image;
	//BYTE* TiffPalette;
	//BYTE* map;
	//BYTE* Image;

public:
	void HeaderRead(FILE *);
	void TiffInfo(FILE *);
	void RawInfo();
	void TiffRead(FILE *);
	void RawRead(FILE *);
	void WriteInfo();
	void TiffWrite(FILE *);
	void RawWrite(FILE *);
	LPBYTE getImage();
	void putImage(LPBYTE);
	void free();
	int GetTagNum() {return tag_num;}
	int IsExistPalette() {return palette;}
	
	__int32 getWidth()
	{return width;}
	
	__int32 getHeight()
	{return height;}
	
	__int32 getChannel()
	{return virtual_channel;}
	
	void SetWidth(int wd)
	{width = wd;}

	void SetHeight(int hg)
	{height = hg;}

	void SetChannel(int ch)
	{channel = ch;}

	friend __int16 convert2(__int16);
	friend __int32 convert4(__int32);
};




/****************************************************************


					CBlob
			

 ****************************************************************/

class CBlob : public CObject
{
// protected data members
protected:
        int m_Size;
        unsigned char *m_pPointer;

// constructors and destructors
public:
        CBlob(int size=0);
        CBlob(CBlob& blob);
        ~CBlob();
        void Serialize(CArchive& ar);

        inline void operator = (CBlob& blob);
        void Copy(CBlob& blob);

// protected operations
protected:
        void DestroyContents();

// public operations
public:
        inline void *GetPointer();
        inline int GetSize();
        void Alloc(int size);
        void ReadFile(const char *pFileName);
        void WriteFile(const char *pFileName);
        void Attach(void *pPointer, int size);
        void *Detach();
        void SetData(int size, unsigned char *pData);
		CDib* BYTEBlob2Dib();
};

inline void *CBlob::GetPointer()
{
        return m_pPointer;
}

inline int CBlob::GetSize()
{
        return m_Size;
}

inline void CBlob::operator = (CBlob& blob)
{
        Copy(blob);
}


/*----------------------------------------------------------
						CBlob Array	
 ----------------------------------------------------------*/

class CBlobArray : public CArray <CBlob *, CBlob *>
{
public:
	CBlobArray() {};
	~CBlobArray() {	RemoveAll(); }
	
	virtual void RemoveAll()
	{
		int nEleCnt = GetSize();
		for(int i = 0; i < nEleCnt; i++) 
			delete (CBlob *)GetAt(i);
		CArray <CBlob *, CBlob *>::RemoveAll();
	}

};




/********************************************************************************
*                                                                               *
*       Class for Manipulating User Defined Resource                            *
*                                                                               *
*...............................................................................*
*       /95/10/04       J. Lee                                                  *
*********************************************************************************/

class CUserResource : public CObject
{
// private data members
private:
        HRSRC m_hResource;
        HGLOBAL m_hGlobal;
        DWORD m_Size;
        void FAR *m_Ptr;
        
// constructors and destructors
public:
        CUserResource();
        CUserResource(int name, int type);
        CUserResource(int name, const char *pType);
        ~CUserResource();
        
// public operations
public:
        BOOL LoadResource(const char *pName, const char *pType);
        void FAR *GetPointer();
        void UnloadResource();
        DWORD GetSize();
};

inline CUserResource::CUserResource()
{
        m_Ptr = NULL;
        m_Size = 0;
}

inline CUserResource::CUserResource(int name, int type)
{
        m_Ptr = NULL;
        m_Size = 0;
        LoadResource(MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
}

inline CUserResource::CUserResource(int name, const char *pType)
{
        m_Ptr = NULL;
        m_Size = 0;
        LoadResource(MAKEINTRESOURCE(name), pType);
}

inline CUserResource::~CUserResource()
{
        if (m_Ptr) UnloadResource();
}

inline void FAR *CUserResource::GetPointer()
{         
        return m_Ptr;
}

inline DWORD CUserResource::GetSize()
{
        return m_Size;
}



/****************************************************************


					My Image Processing Class
			

			- Authord by Kim Do-Hyung (dhkim008@etri.re.kr)
			- Last Modified  ( 2002 / 10 / 16 )


 ****************************************************************/

#define		FILTER_EMBOSSING	0
#define		FILTER_MEAN			1


class CMyDib : public CDib  
{
public:
	CMyDib();
	virtual ~CMyDib();


	// Dimension coversion
	double* Dib2Double(CSize &size);
	unsigned char* Dib2Char(CSize &size);
	CBlob* Dib2CharBlob();
	

	// Image Size & Rotate
	CDib* FitBoundary(int nObjValue = 0);
	CDib* GetCropedCDib(CRect rtRegion);
	CDib* GetResizedCDib(int nWidth, int nHeight);
	CDib* GetRotatedCDib(float fAngleDegree, CPoint *ptTrans1 = NULL, CPoint *ptTrans2 = NULL);

	// Get Operated New CDib
	CDib* GetAddCDib(CDib *pTarget);
	CDib* GetDifferCDib(CDib *pTarget, int nThres);
	CDib* GetMaskedCDib(CDib *pMask);
	CDib* GetFilteredCDib(int *nMask, int nFilterName = -1);
	CDib* GetGradEdgeCDib(int *nMaskX, int *nMaskY);
	CDib* GetRatioEdgeCDib(int *nMaskX, int *nMaskY, double dRatio);
	CDib* GetBinaryCDib(int nThres);
	CDib* GetLocalDiffCDib(int nWinSize);
	CDib* GetNormalizedCDib(int nLower, int nUpper);
	CDib* GetMosaicCDib(int nBlockSize);
	CDib* GetMosaicReducedCDib(int nBlockSizeX, int nBlockSizeY);
	CDib* GetBrightedCDib(int nStep);
	CDib* GetLogCDib();
 



	// Histogram 
	CHistogram* GetHistogram();
	CDib* GetHistoEqualizedCDib();
	CDib* Equalization();


	// Display
	void PaintDibOnPic(CStatic *picture, BOOL bFitSize = TRUE);


	// Information
	int GetMeanValue();
	int GetWhiteValue();

};



/****************************************************************

						CDib Array

 ****************************************************************/

class CMyDibArray : public CArray <CDib *, CDib *>
{
public:
	~CMyDibArray() {	RemoveAll();	}
	virtual void RemoveAll()
	{
		int nEleCnt = GetSize();
		for(int i = 0; i < nEleCnt; i++)
			delete (CDib *)GetAt(i);
		CArray <CDib *, CDib *>::RemoveAll();
	}
};





/****************************************************************


					Global Functions
			

			- Authord by Kim Do-Hyung (dhkim008@etri.re.kr)


 ****************************************************************/


// Distance Method Define
#define		DM_EUCLIDIAN		01
#define		DM_L1_NORM			02

// Simailarity Method Define
#define		SM_MAHALANOBIS		11
#define		SM_CORRELATION		12
#define		SM_COVARIANCE		13

// Math Value Define
#define		M_PI		3.14159265




/*----------------------------------------------------------
	Distance Method
 ----------------------------------------------------------*/
float GetEuclidianDist(float *pfS1, float *pfS2, int nLength);
float GetL1NormDist(float *pfS1, float *pfS2, int nLength);
float GetMahDist(float *E, float *pfS1, float *pfS2, int nLength);
float GetCorrelation(float *pfS1, float *pfS2, int nLength);	
float GetCovariance(float *pfS1, float *pfS2, int nLength);	
float GetFeatureLineDist(float *pfQ, float *pfX1, float *pfX2, int nLength);


/*----------------------------------------------------------
	Vector Calculation Utility
 ----------------------------------------------------------*/
void VectorNormalize(float *pfS, int nLength);
void Standardization(float *pfValue, int nLength);
float GetDotProduct(float *pfValue1, float *pfValue2, int nLength);


/*----------------------------------------------------------
	General Global
 ----------------------------------------------------------*/
CString GetBetweenString(CString strSource, CString chDelFirst, CString chDelSecond);
void WriteSubFolderName(CString strFileName, CString strRoot);
void SortStringArray(CStringArray& array);


///97/05/15       J. Lee
int Dir(CStringArray *pFiles, const char *pPattern, DWORD attr=FILE_ATTRIBUTE_ARCHIVE);
int Dir(CStringArray *pFiles, const CString& directory, const CStringArray& extension, DWORD attr=FILE_ATTRIBUTE_ARCHIVE);
CString GetHomeDirectory();
BOOL IsFileExist(const char *pFileName);
DWORD GetSizeOfFile(const char *pFileName);
BOOL SelectFolder(CString *pFolder);
CString AddFinalBackslash(const char *pString);
CString RemoveFinalBackslash(const char *pString);



/*----------------------------------------------------------
	Message Handling
 ----------------------------------------------------------*/
BOOL PeekAndPump(BOOL bCancelOnESCkey = TRUE);


/*----------------------------------------------------------
	Simple math Function
 ----------------------------------------------------------*/
float GetAngleDegree(CPoint pt1, CPoint pt2);
void GetLineEquation(CPoint pt1, CPoint pt2, float *a, float *b);


/*----------------------------------------------------------
	Log File
 ----------------------------------------------------------*/
void WriteLog(const char *pchData);






/////////////////////////////////////////////////////////////////////////////
#endif // __DIBK_H__