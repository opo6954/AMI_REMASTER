

/********************************************************************************


					eFace200(dll) Refer DLL (for Multi-Bio)


 *********************************************************************************/

// Regular DLL


// function declarations
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


// General Information
__declspec(dllexport) const char* __stdcall eBio_GetAlgorithmComment();
__declspec(dllexport) const char* __stdcall eBio_GetVersionNumber();
__declspec(dllexport) const char* __stdcall eBio_GetAuthorName();


// Enrollment
__declspec(dllexport) CBlob* __stdcall eBio_Enrollment(const char* pID);


// Matching
__declspec(dllexport) double __stdcall eBio_Verification(CBlob *pTemplate);
__declspec(dllexport) double __stdcall eBio_GetSimilarity(double dValue, double *pSame, double *pDiff);
__declspec(dllexport) CBlob* __stdcall eBio_GetFeature();
__declspec(dllexport) double __stdcall eBio_GetMatchingValue(CBlob *pTemplate, CBlob *pFeature);


#ifdef __cplusplus
}
#endif
