

/********************************************************************************


					eFace100(dll) API Functions


 *********************************************************************************/


#define		VF_ERR		(-101)	

// function declarations
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus



// General Information
__declspec(dllexport) const char* Face_GetAlgorithmComment();
__declspec(dllexport) const char* Face_GetVersionNumber();
__declspec(dllexport) const char* Face_GetAuthorName();


// Initialize & Destroy
__declspec(dllexport) BOOL Face_Initialize(CString strInitPath);
__declspec(dllexport) void Face_Destroy();

// Complement
__declspec(dllexport) BOOL Face_SetEnvironment(CDib *pDib, int &nMaxFindCnt);
__declspec(dllexport) int Face_IsRealFace(CDib *pDib, const int nMaxTryCnt);	// -1:Next Dib, 0:Photo, 1:Face


// Enrollment
__declspec(dllexport) CBlob* Face_GetFeature(CDib *pDib,CPoint *ptTempA,CPoint *ptTempB);
__declspec(dllexport) CDib* Face_GetFeatureFace(CBlob *pFeatureBlob);
__declspec(dllexport) CBlob* Face_Enrollment(const char *pID, CDib *pDib, CBlob *pOldTemplate);
__declspec(dllexport) CBlob* Face_EnrollFeatures(const char *pID, CBlobArray &BlobArray);	// for eFace200


// Verification
__declspec(dllexport) double Face_Verification(CBlob *pTemplate, CDib *pDib);
__declspec(dllexport) double Face_GetMatchingValue(CBlob *pTemplate, CBlob *pFeature);
__declspec(dllexport) double Face_RecommendedThreshold();


// Information
__declspec(dllexport) const char* Face_GetEnrolledID(CBlob *pTemplate);
__declspec(dllexport) int Face_GetEnrolledFaceCnt(CBlob *pTemplate);
__declspec(dllexport) CDib* Face_GetEnrolledFace(CBlob *pTemplate, int nFaceIndex = 0);
__declspec(dllexport) CBlob* Face_DeleteEnrolledFace(CBlob *pOldTemplate, int nFaceIndex);
__declspec(dllexport) CDib* Face_GetFaceRegion(CDib *pDib);
__declspec(dllexport) BOOL Face_GetEyePosition(CDib *pDib, CPoint *ptLeftEye, CPoint *ptRightEye);


// About detecting
__declspec(dllexport) int Face_GetLightWeight();					
__declspec(dllexport) void Face_SetLightWeight(int nNewWeight);		
__declspec(dllexport) char Face_GetDetectState();



#ifdef __cplusplus
}
#endif
