using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;


using OpenCvSharp;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.Extensions;


/*
 * 얼굴인식단에서 중앙처리 담당
 * 다른 학습, 인식 모듈이 모두 centermodule과 연결되어야 함
 * center에서 할 일: 저장하고 window에 보내줘서 값 업데이트하기
 * 
 * */
namespace FaceRecognition_UI
{
    class FaceCenterModule
    {
        FaceCameraModule cameraModule;
        FaceTrainingModule faceTrainingModule;
        FaceRecognizerModule faceRecognizeModule;

        public List<Mat> faceList;
        

        public MainWindow myWindow;

        public FaceCenterModule(MainWindow _myWindow)
        {
            myWindow = _myWindow;
            faceList = new List<Mat>();
        }
        

        //모든 모듈의 초기화
        public bool Initialize(int frameWidth, int frameHeight, int nofFrames2Train, int nofFrames2Recog, int maxPersonCount)
        {
            
            cameraModule = new FaceCameraModule(this);
            faceTrainingModule = new FaceTrainingModule(this);
            faceRecognizeModule = new FaceRecognizerModule(this);

            if (cameraModule.Initialize(frameWidth, frameHeight, nofFrames2Train, nofFrames2Recog) == false)
            {
                MessageBox.Show("Camera Init Fail...");    
                return false;
            }
            if (faceTrainingModule.Initialize(nofFrames2Train, maxPersonCount) == false)
            {
                MessageBox.Show("Training Init Fail...");
                return false;
            }
            if (faceRecognizeModule.Initialize(nofFrames2Recog, maxPersonCount) == false)
            {
                MessageBox.Show("Recognizer Init Fail...");
                return false;
            }
            return true;
        }

        //웹캠 시작
        public bool webCamStart()
        {
            if (cameraModule != null)
            {
                cameraModule.startWebCam();
                return true;
            }
            return false;
        }
        public bool webCamStop()
        {
            if (cameraModule != null)
            {
                cameraModule.stopWebCam();
                return true;
            }
            return false;
        }

        //Train 시작
        public bool trainStart(string name, int parameter=0)
        {
            if (faceTrainingModule != null)
            {
                cameraModule.start2getTrainImg();
                return true;
            }
            return false;
        }

        //Test 시작
        public bool testStart()
        {
            if (faceRecognizeModule != null)
            {
                //start recognizer Module
                return true;
            }
            return false;
        }

        public void sendFaceInfo(List<Mat> _faceList)
        {
            if (faceList.Count > 0)
                faceList.Clear();
            //Hard Copy
            faceList = new List<Mat>(_faceList);


            for (int i = 0; i < faceList.Count; i++)
            {
                Cv2.ImWrite(i.ToString() + ".jpg", faceList[i]);
            }
            

            /*
            ////FOR DEBUG....
            Cv2.NamedWindow("1");
            Cv2.ImShow("1", faceList[0]);
            Cv2.WaitKey(0);
            */
            /*
            for (int i = 0; i < _faceList.Count; i++)
            {
                Cv2.ImShow("1", faceList[i]);
                printTxtOnDebug("Shoing " + i + "th image...");
                Cv2.WaitKey(0);
            }
             * */
        }


        //원하는 image component에 mat출력, wb 새로 정의해야 함
        public void drawImage(WriteableBitmap wb, Mat source)
        {
            wb.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                       new Action(
                           delegate()
                           {
                               WriteableBitmapConverter.ToWriteableBitmap(source, wb);
                           }));
        }

        //원하는 image component에 mat을 출력, wb 새로 정의해야 함
        public void drawImage(System.Windows.Controls.Image imageInstance, WriteableBitmap wb, Mat source)
        {
             imageInstance.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                        new Action(
                            delegate()
                            {
                                WriteableBitmapConverter.ToWriteableBitmap(source, wb);
                                imageInstance.Source = wb;
                            }));
        }

        //debug 창에 txt 출력
        public void printTxtOnDebug(string txt)
        {
            myWindow.debugWindow.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        myWindow.debugWindow.addText(txt);
                    }
                    ));
        }

    }
}
