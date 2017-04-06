// Face Engine
////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <fstream>
#include <thread>
#include <mutex>
#include <direct.h>

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <atlstr.h>

#include "vggFace/vgg_face_network.h"
#include "vggFace/stdafx.h"




#define NUM_PEOPLE 30
#define MIN_FACE_SIZE 0
#define FACE_WIDTH 224
#define FACE_HEIGHT 224
#define THRESHOLD 0.5
#define MATCH_KNN 3
#define FACE_DETECTION_TH 0.7


//==========================================================
// Structures
__declspec(dllexport) typedef struct RecognitionRequest
{
	RecognitionRequest()
	{
		killThread = false;
	}

	int contextID;
	cv::Mat image; // 얼굴 검색을 하는 대상 이미지
	cv::Point2d facePosition; // 얼굴 검색을 위한 얼굴위치 예측좌표
	int searchRadius; // 얼굴 검색을 위한 정사각형 ROI의 변의 길이
	int requestID;		
	bool killThread;
	int requestTime;
	int processingStartTime;
	int processingEndTime;

} RecognitionRequest;	



__declspec(dllexport) typedef struct RecognitionContext
{
	RecognitionContext()
	{
		contextID = -1;		
		numValidTrials = 0;
	}
	int contextID;
	int numValidTrials;
	std::map<std::string, int> votingTable; // key : user ID, value : number of votes  Similarity(int) based Voting Table
	std::mutex mtx;

} RecognitionContext;




__declspec(dllexport) typedef struct RequestCompleteResult
{
	RequestCompleteResult(RecognitionRequest _request,  bool _bRecognized)
	{
		request = _request;				
		bRecognized = _bRecognized;		
		totalDuration = _request.processingEndTime - _request.requestTime; // Time spent in the queue + Time spent during the actual processing(recognition)
		processingDuration = _request.processingEndTime - _request.processingStartTime; // Time spent during the actual processing(recognition)
		pendingDuration = totalDuration - processingDuration; // Time spent in the queue
	}
	RecognitionRequest request;
	bool bRecognized;	
	int totalDuration; // in milliseconds
	int pendingDuration; // in milliseconds
	int processingDuration; // in milliseconds
} RequestCompleteResult;

__declspec(dllexport) typedef struct ContextCompleteResult
{
	ContextCompleteResult()
	{
		contextID = -1;
		fConfidence = 0;		
	}
	int contextID;
	char strID[64];				// 인식된 유저의 아이디	
	float fConfidence;				// 인식 신뢰도 (0.0 ~ 1.0), 현재는 voting score로 계산
} ContextCompleteResult;

//==========================================================
// APIs

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

	//---------------------------------------------
	//	 init & destroy
	//---------------------------------------------
	/* 초기화 작업들을 수행, 디텍터 및 디스크립터 생성, 등록된 인원 정보 로드 */
	__declspec(dllexport) bool PsnWho_Initialize(std::string sTrainPath, std::string network_path, std::string network_meta, int numRecognitionThreads);

	/* 종료 전 호출, 메모리 해제 등 */
	__declspec(dllexport) void PsnWho_Destroy();

	//---------------------------------------------
	//	enrollment
	//---------------------------------------------

	__declspec(dllexport) void PsnWho_Initialize_for_Registration(std::string network_path, std::string network_meta, char* sTrainPath);

	
	/* 아이디와 영상스트림을 주면 DB에 등록한다. */
	__declspec(dllexport) void PsnWho_RegisterFaces(std::string id, std::vector<cv::Mat>& images, std::string faceSavePath="");

	/* 기 등록된 사람을 DB에서 삭제한다. (특정폴더에서 삭제) */
	__declspec(dllexport) bool PsnWho_Unregister(std::string id);

	
	//---------------------------------------------
	//	 recognition
	//---------------------------------------------

	//Request를 Queue자료구조에 전송
	__declspec(dllexport) void RecognizeFace(RecognitionRequest request); 

	//프레임을 몇개 단위(numValidTrials)로 묶어서 인식할지 조건을 결정한다.
	__declspec(dllexport) void SetContextEndCondition(int numValidTrials);

	// The function to be called when a context is completely processed.
	__declspec(dllexport) void SetContextCompleteCallback(void (*callback)(ContextCompleteResult result));

	__declspec(dllexport) bool GetDescribedVector(cv::Mat& vAlignedImg, cv::Mat& mVector,  int threadID);

	// The function to be called when a request is completely processed.
	__declspec(dllexport) void SetRequestCompleteCallback(void (*callback)(RequestCompleteResult result));

	//얼굴인식 결과 이미지를 저장할지 유무를 결정하는 함수
	__declspec(dllexport) void SaveTrialFaces(bool isSave);

	enum MATCHING_METHOD
	{
		MATCHING_BF=0,
		MATCHING_FLANN
	};

   __declspec(dllexport) void SetAffineSimulationParam(std::string sTrainDetector, int nTrainDetectionParam, std::string sTestDetector, int nTestDetectionParam, 
		int nTrainTilts, int nTrainPhiSimulation, int nTestTilts, int nTestPhiSimulation, int nPatchWidth, int nPatchHeight, std::vector<std::string> vPatchDescriptor);
	__declspec(dllexport) void SetConfidenceThreshold(float Th);
	__declspec(dllexport) void SetMatchingMethod(MATCHING_METHOD method);

	/* Flushing operation is 'triggered' when there is any request whose pendingDuration > triggerPendingDuration.
	   Through the flushing operation, only requests whose pendingDuration < survivingPendingDuration 'survive' in the queue. */
	__declspec(dllexport) void SetFlushingCondition(int triggerPendingDuration, int survivingPendingDuration);

	//모든 thread를 종료하는 함수
	__declspec(dllexport) void Terminate();

#ifdef __cplusplus
}
#endif