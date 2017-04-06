// FaceRecognizerForDLL.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ACF\ACF.h"
#include "WhoApi\WhoApi.h"

string detector_path="../data/detector/";
string featureSet_path="../data/train/";
string network_path="../data/vgg-face/";
string network_meta_path="../data/vgg-face/meta3.txt";
string testFilePath = "../data/test/";

//이 부분은 사실상 의미가 없는 듯 train image가 
void featureExtractorAndSave()
{
	string filePath = "../data/test/";
	string network_path = "../data/vgg-face/";
	string network_meta = "../data/vgg-face/meta3.txt";
	string outputPath = "../output/train/";
	
	int length = 300;
	string fileName = filePath + "0.jpg";




	PsnWho_Initialize_for_Registration(network_path,network_meta,"../data/train");



	




	cout << "Load Test Picture..."<< endl;
	vector<Mat> imgList;

	for(int i=0; i<length; i++)
	{
		Mat m = imread(filePath + to_string(i)+ ".jpg");

		imgList.push_back(m);
	}


	cout << "PSN WHO Register Face..." << endl;

	PsnWho_RegisterFaces("테스트",imgList,outputPath);



	

	printf("Hello word\n");
}

//feature만 뽑는 함수
void featureExtractor()
{
	
	
}
Mat detectFaceInner(cv::Mat img, CACF detectorEngine, st_ACFdetector detector){
	int pad_row = img.rows>500? img.rows:500;
	int pad_col = img.cols>500? img.cols:500;

	int down_sample = 4;
	int img_cent_x = pad_row/2;
	int img_cent_y = pad_col/2;
	cv::Mat padded, paddedr;

	padded.create(pad_row, pad_col, img.type());
	padded.setTo(cv::Scalar::all(0));

	img.copyTo(padded(Rect((pad_col-img.cols)/2, (pad_row-img.rows)/2,  img.cols, img.rows)));
	cv::resize(padded,paddedr,cv::Size(pad_col/down_sample,pad_row/down_sample));
	std::vector<stDetection> bbs = detectorEngine.Run(paddedr, detector);

	cv::Rect sel_rect(0,0,0,0);
	int min_sq_l2 = (int)1e10;

	// find most centered rect
	for (int i=0; i<bbs.size(); i++){
		if (sel_rect.area() == 0)
			sel_rect = bbs[i].box.cv();

		int dist_x = img_cent_x - (bbs[i].box.cv().x*down_sample + bbs[i].box.cv().width*down_sample/2);  // dist in x axis
		int dist_y = img_cent_y - (bbs[i].box.cv().y*down_sample + bbs[i].box.cv().height*down_sample/2);  // dist in y axis
		int sq_l2 = dist_x*dist_x + dist_y*dist_y;  // squared l2 distance

		if (min_sq_l2 > sq_l2){
			min_sq_l2 = sq_l2;
			sel_rect = bbs[i].box.cv();
		}
	}

	// restore original size
	sel_rect.x *= down_sample;
	sel_rect.y *= down_sample;
	sel_rect.width *= down_sample;
	sel_rect.height *= down_sample;

	

	if (bbs.size() != 0 && sel_rect.area() != 0){
		if (sel_rect.x < 0 || sel_rect.y < 0 || 
			sel_rect.x + sel_rect.width > pad_col || 
			sel_rect.y + sel_rect.height > pad_row)
			return Mat();
		else {
			return padded(sel_rect);
		}
	}
	else
		return img;
		//return Mat();
}

//face 검출하고 자르는 함수
void detectFace()
{
	//Detector 초기 설정

	cout << "Init detector.." << endl;
	int ACF_Sensitivity = 30;
	st_ACFdetector detector;
	char detpath[30];
	sprintf_s(detpath,sizeof(detpath),"%s",detector_path.c_str());
	

	detector.pDBPath = detpath;
	detector.nSensitivity = ACF_Sensitivity;
	CACF detectorEngine = CACF();
	detectorEngine.Initialize(detector);


	cout << "Start to apply ACF..." << endl;
	int length = 10;

	for(int i=0; i<length; i++)
	{
		Mat m = imread(testFilePath + to_string(i) + ".jpg");
		detectFaceInner(m,detectorEngine,detector);
		
		Mat m_ACF = detectFaceInner(m,detectorEngine,detector);

		
		if(m_ACF.size().width > 0)
		{
			cout << "detect success..." << endl;
			imshow("Before ACF",m);
			imshow("After ACF", m_ACF);
		}
		else
			cout << "detect fail..." << endl;
		

		waitKey(0);
		
	}

	/*
	


	cout << "Load Test Picture..."<< endl;
	vector<Mat> imgList;

	for(int i=0; i<length; i++)
	{
		Mat m = imread(filePath + to_string(i)+ ".jpg");

		imgList.push_back(m);
	}

	*/

	



	

	

	
	
}

//feature를 저장하는 함수
void featureSave()
{
}

//face의 feature를 match하는 함수
void featureMatch()
{
	SetConfidenceThreshold(0.45f);
	SetMatchingMethod(MATCHING_BF);
	SetFlushingCondition(3000,1000);


	//confidence 설정
	//matching method 설정
	//flushingCondition 설정
}



int _tmain(int argc, _TCHAR* argv[])
{
	detectFace();
	return 0;
}

