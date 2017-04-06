#ifdef	ACF_DLL_EXPORTS
#define EXPORT_LIBRARY_DECL	__declspec(dllexport)
#else
#define EXPORT_LIBRARY_DECL __declspec(dllimport)
#endif

#pragma once

#include "ACF_Manager.h"

using namespace std;
using namespace cv;

struct st_ACFdetector
{
	int nSensitivity; //0~100
	int nCandidate;
	int imType; //0 RGB , 1 BGR, 2 YUV2
	double modelWidth, modelHeight;
	int nS1_INPUT_TYPE;
	int nS1_DETECTION_TYPE;	// 0:Head / 1:Full-body
	int nS1_DETECTOR_TYPE; // 0:no opnmp / 1:ACF /2:TEXT
	int nDET_TH;

	int nInitCalibration;
	double dMinHeight;
	double dMaxHeight;
	int minLoc;
	int maxLoc;
	int imheight;

	char* pDBPath;
};

class EXPORT_LIBRARY_DECL CACF
{
public:
	CACF(void);
	~CACF(void);

	//initialization
	bool Initialize(st_ACFdetector &detector);
	bool Finalize(void);

	// run detector
	std::vector<stDetection> Run(Mat matCurrentFrame, st_ACFdetector detector);
	mxArray* ACFRGBConvert(mxArray* mxArray_InputImage, int flag = 2, bool single = 1); // flag 0 = gray, 1 = rgb, 2 = luv, 3 = hsv, 4 = orig
	void ACFGetScales(void);
	void ACFComputeImagePyramid(std::vector<std::vector<mxArray*>> &pyr_chns, mxArray*& InputImage);
	void ACFComputeImagePyramidApp(std::vector<std::vector<mxArray*>> &pyr_chns);
	void ACFChannelSmoothETC(std::vector<std::vector<mxArray*>> &pyr_chns);
	void chnsCompute(mxArray* chns[], mxArray* InputImage);
	mxArray* ACFIMResample(mxArray* data_iR_j, int sz1_1, int sz1_2, float ratio = 1.0, mxArray* data_Output = NULL);
	void ACFIMResample_NoAlloc(mxArray* output, mxArray* data_iR_j, int sz1_1, int sz1_2, float ratio = 1.0);
	void ACFgradientMag(mxArray* pl[], mxArray* I, int channel=0, int normRad=5, float NormConst=0.005f, int full=0);
	mxArray* ConvTri(mxArray* I, float r, int s=1, int nomex=0);
	mxArray* convConst(char* type, mxArray* I, float p, int s);
	mxArray* ACFgradientHist(mxArray* M,mxArray* O, int binSize, int nOrients, int softBin, int useHog, float clipHog, int full);
	void ACFAddChn(mxArray **chns, mxArray *AfterResize_Convert, size_t *sz2, int idx);
	mxArray * ACFImPadMex(mxArray *chn, mxArray* pad,mxArray* padWidth);
	void ACFComputeImPad(std::vector<std::vector<mxArray*>> &pyr_chns);
	std::vector<stDetection> ACFComputeDetect(std::vector<mxArray *> &pyr_chn_concat, st_ACFdetector &detector);
	void ACFConcat(std::vector<mxArray *> &pyr_chn_concat,std::vector<std::vector<mxArray*>> &pyr_chn);
	mxArray* ACFDetect(mxArray *chn_concat, int curStep, st_ACFdetector &detector);
	void ACFClearMxarray(void);
	//ACF UTIL
	void imshow(mxArray* I, const char* windowName);
	void visualize(cv::Mat pDibArray, std::vector<stDetection> bbs);
	cv::Mat MxArray2CvMat(mxArray* I);
	mxArray* CvMat2MxArray(cv::Mat I, int flag = 0);
	mxArray* MxArrayResize(mxArray *BeforeResize,int crop_height, int crop_width);
	std::vector<stDetection> ACFGetRawDetection(void);
	double EstimateDetectionHeight(S1_Point2D bottomCenter, S1_Point2D topCenter, int k);
	// non-maximal suppression
	std::vector<stDetection> bbNms_maxg(std::vector<stDetection> st_bbs, double overlap);
private:
	std::vector<stDetection> m_DetectionResultVector[NUM_CAM];
	std::vector<stDetection> m_RawDetectionResultVector;
	std::vector<stDetection> m_DetectionResultVectorOneCam;
	stACFParameters m_ACFParameters;
	std::vector<double> m_scales;
	std::vector<Size2d> m_scaleshw;
	Size2d m_ImgSize;
	std::vector<mxArray*> m_pyr_chn_concat;
	std::vector<std::vector<mxArray*>> m_pyr_chn;

	//ChnsPyramid
	std::vector<int> m_isR;
	std::vector<int> m_isA;
	std::vector<int> m_isJ;
	std::vector<int> m_isN;
	double m_nScales;

	//ACFRgbConvert
	template<class oT> oT*  rgb2luv_setup( oT z, oT *mr, oT *mg, oT *mb,oT &minu, oT &minv, oT &un, oT &vn );
	template<class iT, class oT> void rgb2luv( iT *I, oT *J, int n, oT nrm );
	template<class iT> void rgb2luv_sse( iT *I, float *J, int n, float nrm );
	template<class iT, class oT> void rgb2hsv( iT *I, oT *J, int n, oT nrm );
	//template<class iT, class oT> void rgb2gray( iT *I, oT *J, int n, oT nrm );
	//template<> void rgb2gray( double *I, float *J, int n, float nrm );
	template<class iT, class oT> void normalize( iT *I, oT *J, int n, oT nrm );
	template<class iT, class oT> oT* rgbConvert( iT *I, int n, int d, int flag, oT nrm );
	//ACFIMResample
	template<class T> void resampleCoef( int ha, int hb, int &n, int *&yas, int *&ybs, T *&wts, int bd[2], int pad=0 );
	template<class T> void resample( T *A, T *B, int ha, int hb, int wa, int wb, int d, T r );
	//convConst
	void convBoxY( float *I, float *O, int h, int r, int s );
	void convBox( float *I, float *O, int h, int w, int d, int r, int s );
	void conv11Y( float *I, float *O, int h, int side, int s );
	void conv11( float *I, float *O, int h, int w, int d, int side, int s );
	void convTriY( float *I, float *O, int h, int r, int s );
	void convTri( float *I, float *O, int h, int w, int d, int r, int s );
	void convTri1Y( float *I, float *O, int h, float p, int s );
	void convTri1( float *I, float *O, int h, int w, int d, float p, int s );
	void convMaxY( float *I, float *O, float *T, int h, int r );
	void convMax( float *I, float *O, int h, int w, int d, int r );
	//gradientMag
	void grad1( float *I, float *Gx, float *Gy, int h, int w, int x ); // compute x and y gradients for just one column (uses sse)
	void grad2( float *I, float *Gx, float *Gy, int h, int w, int d ); // compute x and y gradients at each location (uses sse)
	float* acosTable(); // build lookup table a[] s.t. a[x*n]~=acos(x) for x in [-1,1]
	void gradMag( float *I, float *M, float *O, int h, int w, int d, bool full ); // compute gradient magnitude and orientation at each location (uses sse)
	void gradMagNorm( float *M, float *S, int h, int w, float norm ); // normalize gradient magnitude at each location (uses sse)
	void gradQuantize( float *O, float *M, int *O0, int *O1, float *M0, float *M1, // helper for gradHist, quantize O and M into O0, O1 and M0, M1 (uses sse)
	int nb, int n, float norm, int nOrients, bool full, bool interpolate );
	void gradHist( float *M, float *O, float *H, int h, int w, int bin, int nOrients, int softBin, bool full ); // compute nOrients gradient histograms per bin x bin block of pixels
	float* hogNormMatrix( float *H, int nOrients, int hb, int wb, int bin ); // HOG helper: compute 2x2 block normalization values (padded by 1 pixel)
	void hogChannels( float *H, const float *R, const float *N, // HOG helper: compute HOG or FHOG channels
	int hb, int wb, int nOrients, float clip, int type );
	void hog( float *M, float *O, float *H, int h, int w, int binSize, // compute HOG features
	int nOrients, int softBin, bool full, float clip );
	void fhog( float *M, float *O, float *H, int h, int w, int binSize, // compute FHOG features
	int nOrients, int softBin, float clip ); // Create [hxwxd] mxArray array, initialize to 0 if c=true
	/******************************************************************************/
	mxArray* mxCreateMatrix3( int h, int w, int d, mxClassID id, bool c, void **I );
	void checkArgs( int nl, mxArray *pl[], int nr, const mxArray *pr[], int nl0, // Check inputs and outputs to mex, retrieve first input I
	int nl1, int nr0, int nr1, int *h, int *w, int *d, mxClassID id, void **I );
	void mGrad2( int nl, mxArray *pl[], int nr, const mxArray *pr[] ); // [Gx,Gy] = grad2(I) - see gradient2.m
	void mGradMag( int nl, mxArray *pl[], int nr, const mxArray *pr[] ); // [M,O] = gradMag( I, channel, full ) - see gradientMag.m
	void mGradMagNorm( int nl, mxArray *pl[], int nr, const mxArray *pr[] ); // gradMagNorm( M, S, norm ) - operates on M - see gradientMag.m
	void mGradHist( int nl, mxArray *pl[], int nr, const mxArray *pr[] ); // H=gradHist(M,O,[...]) - see gradientHist.m
	void gradientMex( int nl, mxArray *pl[], int nr, const mxArray *pr[] );
	template<class T> void imPad( T *A, T *B, int h, int w, int d, int pt, int pb, int pl, int pr, int flag, T val ); //imPadMex
};

double overlapBBOX(int x11,int y11,int x12,int y12,int x21,int y21,int x22,int y22);

void ImageCopyWithROI(unsigned char* pdsc,unsigned char* pinputimage,int nCandidate,int* pCandidate,double exratio,int imwidth,int imheight,int nChannel);