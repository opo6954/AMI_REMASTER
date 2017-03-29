

#ifndef __FACEK_H__
#define	__FACEK_H__



#include <math.h>
#include <float.h>
#include <assert.h>
#include <string.h>




#define		EYE_DISTANCE	40

// Upper EF Crop Offset
#define		U_L_OFFSET	20	
#define		U_R_OFFSET	20	
#define		U_T_OFFSET	30	
#define		U_B_OFFSET	30			

// Standard EF Crop Offset
#define		S_L_OFFSET	20	
#define		S_R_OFFSET	20	
#define		S_T_OFFSET	30	
#define		S_B_OFFSET	60	

#define		RECT_STANDARD	CRect(S_L_OFFSET, S_T_OFFSET, S_R_OFFSET, S_B_OFFSET)
#define		RECT_UPPER		CRect(U_L_OFFSET, U_T_OFFSET, U_R_OFFSET, U_B_OFFSET)
#define		RECT_TZONE		CRect(T_L_OFFSET, T_T_OFFSET, T_R_OFFSET, T_B_OFFSET)


// Tzone EF Crop Offset
#define		T_L_OFFSET	15	
#define		T_R_OFFSET	15	
#define		T_T_OFFSET	10	
#define		T_B_OFFSET	30

// Raw FaceData Size
#define		RAW_FACE_WIDTH		EYE_DISTANCE + S_L_OFFSET + S_R_OFFSET
#define		RAW_FACE_HEIGHT		S_T_OFFSET + S_B_OFFSET



/********************************************************************************


							Data Structure


 *********************************************************************************/

/*----------------------------------------------------------
	Face Information Data	(1차원 float 형 데이터)	
 ----------------------------------------------------------*/
class CFaceData  
{
public:
	// Functions
	CFaceData();
	virtual ~CFaceData();

	void Alloc(int nLength, CSize size);
	BOOL IsExistData();
	void SetInitValue(float fInitValue);
	int GetMeanValue();
	virtual void Serialize(CArchive &ar);

	CFaceData* GetNormalizedFaceData();
	CDib* GetFaceData2Dib();
	CDib* GetFaceData2MaskedDib(CDib *pMask);
	void PaintFaceData(CStatic *picture, BOOL bRealSize = TRUE);
	void PaintMaskedFaceData(CDib *pMask, CStatic *picture, BOOL bRealSize = TRUE);

	//	Variables
	float *m_pfValue;			
	CSize m_Size;
	int m_nLength;			

};


/*----------------------------------------------------------
						CFaceData Array	
 ----------------------------------------------------------*/

class CFaceArray : public CArray <CFaceData*, CFaceData *>
{
public:
	CFaceArray() {};
	~CFaceArray() {	RemoveAll(); }
	
	BOOL IsEmpty() { if(GetSize() == 0) return TRUE; return FALSE; }

	virtual void RemoveAll()
	{
		int nEleCnt = GetSize();
		for(int i = 0; i < nEleCnt; i++) 
			delete (CFaceData *)GetAt(i);
		CArray <CFaceData*, CFaceData *>::RemoveAll();
	}

	virtual void Serialize(CArchive &ar)
	{
		if (ar.IsStoring()) 
		{
			ar << GetSize();
			for (int i = 0; i < GetSize(); i++)
				GetAt(i)->Serialize(ar);
		}
		else 
		{
			int nSize;
			ar >> nSize;
			for (int i = 0; i < nSize; i++) 
			{
				CFaceData *pNode = new CFaceData;
				pNode->Serialize(ar);
				Add(pNode);
			}
		}
	}
};



/*----------------------------------------------------------
	EigenFace	(CFaceData + Eigenvalue)	
 ----------------------------------------------------------*/
class CEigenFace  
{
public:
	CEigenFace();
	virtual ~CEigenFace();
	void Alloc();
	virtual void Serialize(CArchive &ar);

	// Variables
	CFaceData *m_pEigenFace;
	float m_fEigenValue;

};

/*----------------------------------------------------------
	CEigenFace Array	
 ----------------------------------------------------------*/
class CEigenArray : public CArray <CEigenFace*, CEigenFace *>
{
public:
	CEigenArray();
	~CEigenArray();
	
	BOOL IsEmpty() { if(GetSize() == 0) return TRUE; return FALSE; }

	virtual void RemoveAll();
	virtual void RemoveAt(int nIndex);
	virtual void Serialize(CArchive &ar);
};


/*----------------------------------------------------------
		Feature Information Data	
 ----------------------------------------------------------*/
class CFeature   
{
public:
	CFeature();
	virtual ~CFeature();

	void AllocFace(CSize size);
	void AllocUpper(int nLength);
	void AllocStd(int nLength);
	void AllocTzone(int nLength);
	void AllocEdge(int nLength);
	void AllocGabor(int nLength);

	CDib *FeatureFace2Dib();
	void PaintFeatureFace(CStatic *picture , BOOL bFitSize);

	virtual void Serialize(CArchive& ar);
	void ReadMemoryBlock(void *pData, int nSize);
	void *WriteMemoryBlock(int *pnSize);

	//----------------------------
	//	Basic Infomation
	//----------------------------
	CString m_strTitle;				// Image Tilte
	CSize m_FaceSize;				// Image Size
	unsigned char *m_pFaceData;		// Raw FaceData

	//----------------------------
	//	Feature List
	//----------------------------
	int m_nUpperLength;
	float *m_pfUpperWeight;

	int m_nStdLength;
	float *m_pfStdWeight;

	int m_nTzoneLength;
	float *m_pfTzoneWeight;	

	int m_nEdgeLength;
	float *m_pfEdgeWeight;

	int m_nGaborLength;
	int *m_pnGaborWeight;

};


/*----------------------------------------------------------
	CFeature Array	(1인당 하나의 Array 부여)	
 ----------------------------------------------------------*/
class CFeatureArray : public CArray <CFeature*, CFeature *>
{
public:
	// Functions
	CFeatureArray();
	~CFeatureArray();

	BOOL IsEmpty() { if(GetSize() == 0) return TRUE; return FALSE; }
	virtual void RemoveAll();
	virtual void RemoveAt(int nIndex);
	virtual void Serialize(CArchive& ar);
	void ReadFile(const char* pFileName);
	void WriteFile(const char* pFileName);
	void ReadMemoryBlock(void *pData, int nSize);
	void *WriteMemoryBlock(int *pnSize);

	// Variables
	CString m_strID;	// 등록 인물의 ID
};



/********************************************************************************


					Eye Detect (Yoon)


 *********************************************************************************/

void FaceDetecter(CDib *pDib);
void SecondProcessing(CDib *pDib);
BOOL DetectEye(CDib *pDib, CDib *pMask, CFaceData *pMeanFace, CPoint *ptSrcLeft, CPoint *ptSrcRight, CPoint *ptRotLeft, CPoint *ptRotRight);
char GetNotice();
CDib *GetRotatedDib();
BOOL SetOptEyeThreshold(CDib *pDib, CDib *pMask, CFaceData *pMeanFace, int &nMaxFindCnt);
int GetThreshold();
void SetThreshold(int nThreshold);
int IsRealFace(CDib *pDib, CDib *pMask, CFaceData *pMeanFace, int nMaxTryCnt);		// return -1:Next pDib, 0:Photo, 1:RealFace





/********************************************************************************


							PreProcessing


 *********************************************************************************/
class CFaceDib : public CMyDib  
{
public:
	CFaceDib();
	virtual ~CFaceDib();

	CFaceData* GetSrc2NormalFaceData(CDib *pMask, CPoint ptLeftEye, CPoint ptRightEye, CRect rtOffset);
	CFaceData* GetMug2NormalFaceData(CDib *pMask);
	CDib* GetSrc2MugDib(CPoint ptLeftEye, CPoint ptRightEye, CRect rtOffset);
	CDib* GetSrc2PosDib(CPoint *ptLeftEye, CPoint *ptRightEye);
	CDib* GetPos2MugDib(CPoint ptLeftEye, CPoint ptRightEye, CRect rtOffset);
	int GetFgMaskLength();
	CFaceData* GetForegroundFace(CDib *pMask);
	CFaceData* Dib2Face();
	CHistogram* GetMaskedHistogram(CDib *pMask);
	CDib* GetMaskedHistoEqualCDib(CDib *pMask);
	CDib* GetMaskedAdjustMeanCDib(CDib *pMask);

	// Face Region Verify
	BOOL IsFaceRegion(CFaceData *pMean, CDib *pMask, CPoint ptLeftEye, CPoint ptRightEye);
	float GetSymCorrelation(CPoint ptLeftEye, CPoint ptRightEye);
	CDib* GetHLightMapDib(CDib *pSrc);
};



/********************************************************************************


						Feature Extractor


 *********************************************************************************/


/*----------------------------------------------------------
	PCA (Principal Component Analysis)	
 ----------------------------------------------------------*/
class CPca  
{
public:
	CPca();
	virtual ~CPca();

	void ResetAllData();
	int MakeEigenFace(CFaceArray &TrainArray);
	float* GetFaceWeight(CFaceData *pSrcFace);
	float* GetEigenValue();
	int ModifySelectedEigenCnt(int nNewCnt);
	int RemoveFirstEigen();
	CFaceData* GetRecoveryFace(CFaceData *pSrcFace);


	// File Handling
	virtual void Serialize(CArchive& ar);
	void ReadFile(const char* pFileName);
	void WriteFile(const char* pFileName);


	//	Variables
	CEigenArray m_EigenArray;
	CFaceData *m_pMeanFace;


private:
	CFaceData* GetMeanFace(CFaceArray& FaceArray);
	CFaceData* GetDifferMean(CFaceData *pSrcFace);
	float* GetCovarianceMatrix(CFaceArray& Array1, CFaceArray& Array2, int nRow /* row == col */ );
	void MakeDifferArray(CFaceArray &DifferArray, CFaceArray &TrainArray);
	void WriteEigenVectorCovariance(CString strFileName);
};


/*----------------------------------------------------------
	Jacobi	(Intel)
 ----------------------------------------------------------*/
void cvJacobiEigens_32f(float* A, float* V, float* E, int n, float eps);
void cvJacobiEigens_64d(double* A, double* V, double* E, int n, double eps);


// For Gaber
#define XM_UL		0.05	/* lower frequency */
#define XM_UH	    0.4		/* upper frequency */
#define XM_FLAG			1	/* remove the DC */

#define GW_SCALE		4
#define GW_ORIENTATION	6
#define sqr(x) ((x)*(x))

typedef struct MatrixStruct 
{
	double **data;
	int height, width;
} Matrix;


/*----------------------------------------------------------
	Gabor Wavelet
 ----------------------------------------------------------*/

class CGaborWavelet
{
public:
	CGaborWavelet();
	virtual ~CGaborWavelet();

	float *GetFeature(CDib *pMugDib);

};




class CFeatureExtor  
{
public:

	CFeatureExtor();
	CFeatureExtor(CString strUpperEfFileName, CString strStdEfFileName, CString strTzoneEfFileName);
	virtual ~CFeatureExtor();

	
	CFeature* GetFeature(CDib *pSrc, CDib* pUMask, CDib *pSMask, CDib* pTMask, CPoint ptLeftEye, CPoint ptRightEye);
	CFeature* GetFeatureFromeMug(CDib *pMugDib, CDib* pUMask, CDib *pSMask, CDib *pTMask);
	float* GetEdgeMap(CDib *pMugDib);
	int* GetGaborFeature(CDib *pMugDib);
	float* GetGaborWaveletFeature(CDib *pPosDib, CPoint ptLeftEye, CPoint ptRightEye);
	
	
	//------------------------------------
	//	Variables & Instances
	//------------------------------------
	CPca m_UpperPca;
	CPca m_StandardPca;
	CPca m_TzonePca;
	CGaborWavelet m_GaborWavelet;
	

};


/********************************************************************************


							Enrollment


 *********************************************************************************/

#define		MAX_ENROLL_COUNT	5

/*----------------------------------------------------------
	1인 등록시 필요한 데이터 버퍼
 ----------------------------------------------------------*/
class CEnrollData  
{
public:
	CEnrollData();
	virtual ~CEnrollData();

	// save and load data
	virtual void Serialize(CArchive& ar);
	void ReadFile(const char* pFileName);
	void WriteFile(const char* pFileName);

	void ResetAllData();
	BOOL IsDataFull();

	CFaceData* GetSelectedMugFace(int nIndex);
	BOOL EnrollSample(CString strID, CDib *pDib, CPoint ptLeftEye, CPoint ptRightEye);
	BOOL MakePosReference(CDib *pPosDib, CPoint ptRotLeft, CPoint ptRotRight);
	BOOL MakeLightReference(CDib *pMugDib);


	CDib *m_pStdMask;				// MaskDib Pointer

	//----------------------------
	//	저장 데이터
	//----------------------------
	CString m_strID;				// 등록 ID
	CFaceArray m_MugArray;			// 원본 얼굴
	CFeatureArray m_FeatureArray;	// 각종 Feature
	CPca m_Pca;						// I-PCA


	// Temporary
	CFaceArray m_ReferArray;		// 참조 얼굴 (eigenface 생성후 삭제)
	CStatic *m_picResult;
	
};


/********************************************************************************


							Verification


 *********************************************************************************/


#define		PCA_UPPER		0
#define		PCA_STANDARD	1
#define		PCA_TZONE		2
#define		EDGE_DENSITY	3


class CClassifier  
{
public:
	CClassifier(float *pfUpperEValue, float *pfStdEValue, float *pfTzoneValue);
	CClassifier(float *pfUpperEValue, float *pfStdEValue, float *pfTzoneValue, CString strModelFile, CString strMaxMinFile);
	CClassifier(float *pfUpperEValue, float *pfStdEValue, float *pfTzoneValue, CString strModelFile, CString strMaxMinFile, CString strNNFile);
	virtual ~CClassifier();

	double GetSimilarity(CFeature *pQueryFeature, CFeatureArray &EnrollArray);
	double GetSimilarity(CFeature *pQueryFeature, CFeature *pEnrollFeature);
	void GetSimilarityArray(CFeature *pQueryFeature, CFeatureArray &EnrollArray, CFloatArray &InputArray);
	void GetSimilarityArray(CFeature *pQueryFeature, CFeature *pEnrollFeature, CFloatArray &InputArray);

private:	

	
	void GetPcaSimilarity(float *pfWeight, CFeatureArray &EnrollArray, CFloatArray &InputArray, int nPcaMode); 
	void GetEdgeSimilarity(float *pfWeight, CFeatureArray &EnrollArray, CFloatArray &InputArray); 
	void GetGaborSimilarity(int *pnWeight, CFeatureArray &EnrollArray, CFloatArray &InputArray); 

	float GetMinDistance(float *pfWeight, CFeatureArray &EnrollArray, int nMeasure, int nPcaMode);
	float GetRangeMinDistance(float *pfWeight, CFeatureArray &EnrollArray, int nLow, int nHigh, int nPcaMode);
	float GetMaxSimilarity(float *pfWeight, CFeatureArray &EnrollArray, int nMeasure, int nPcaMode);
	float GetNearestFeatureLine(float *pfWeight, CFeatureArray &EnrollArray, int nPcaMode);

	float *m_pfUpperEValue;
	float *m_pfStdEValue;
	float *m_pfTzoneEValue;

};






/////////////////////////////////////////////////////////////////////////////
#endif // __FACEK_H__
