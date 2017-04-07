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
	cv::Mat image; // �� �˻��� �ϴ� ��� �̹���
	cv::Point2d facePosition; // �� �˻��� ���� ����ġ ������ǥ
	int searchRadius; // �� �˻��� ���� ���簢�� ROI�� ���� ����
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
	char strID[64];				// �νĵ� ������ ���̵�	
	float fConfidence;				// �ν� �ŷڵ� (0.0 ~ 1.0), ����� voting score�� ���
} ContextCompleteResult;

//==========================================================
// APIs

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

	//---------------------------------------------
	//	 init & destroy
	//---------------------------------------------
	/* �ʱ�ȭ �۾����� ����, ������ �� ��ũ���� ����, ��ϵ� �ο� ���� �ε� */
	__declspec(dllexport) bool PsnWho_Initialize(std::string sTrainPath, std::string network_path, std::string network_meta, int numRecognitionThreads);

	/* ���� �� ȣ��, �޸� ���� �� */
	__declspec(dllexport) void PsnWho_Destroy();

	//---------------------------------------------
	//	enrollment
	//---------------------------------------------

	__declspec(dllexport) void PsnWho_Initialize_for_Registration(std::string network_path, std::string network_meta, char* sTrainPath);

	
	/* ���̵�� ����Ʈ���� �ָ� DB�� ����Ѵ�. */
	__declspec(dllexport) void PsnWho_RegisterFaces(std::string id, std::vector<cv::Mat>& images, std::string faceSavePath="");

	/* �� ��ϵ� ����� DB���� �����Ѵ�. (Ư���������� ����) */
	__declspec(dllexport) bool PsnWho_Unregister(std::string id);

	
	//---------------------------------------------
	//	 recognition
	//---------------------------------------------

	//Request�� Queue�ڷᱸ���� ����
	__declspec(dllexport) void RecognizeFace(RecognitionRequest request); 

	//�������� � ����(numValidTrials)�� ��� �ν����� ������ �����Ѵ�.
	__declspec(dllexport) void SetContextEndCondition(int numValidTrials);

	// The function to be called when a context is completely processed.
	__declspec(dllexport) void SetContextCompleteCallback(void (*callback)(ContextCompleteResult result));

	__declspec(dllexport) bool GetDescribedVector(cv::Mat& vAlignedImg, cv::Mat& mVector,  int threadID);

	// The function to be called when a request is completely processed.
	__declspec(dllexport) void SetRequestCompleteCallback(void (*callback)(RequestCompleteResult result));

	//���ν� ��� �̹����� �������� ������ �����ϴ� �Լ�
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

	//��� thread�� �����ϴ� �Լ�
	__declspec(dllexport) void Terminate();

#ifdef __cplusplus
}
#endif