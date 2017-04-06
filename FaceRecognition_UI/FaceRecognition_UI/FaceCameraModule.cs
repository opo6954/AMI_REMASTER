using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Media;
using System.Windows.Media.Imaging;

using OpenCvSharp;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.Extensions;


namespace FaceRecognition_UI
{
    class FaceCameraModule
    {
        private Thread cameraCapture;
        private Thread cameraReadyTimer;//학습을 위한 시간 timer
        
        VideoCapture cap;
        List<Mat> faceContainer;

        FaceCenterModule centerModule;
        

       


        bool showWebCam;
        bool isTrainMode;
        bool isTestMode;

        int frameWidth;
        int frameHeight;

        int nofFrames2Train;
        int nofFrames2Recog;

        int totalFrameIdx = 0;

        private int frameIdx;
        WriteableBitmap wb;

        

        

        public FaceCameraModule(FaceCenterModule _centerModule)
        {
            centerModule = _centerModule;
        }

        public bool Initialize(int _frameWidth, int _frameHeight, int _nofFrames2Train, int _nofFrames2Recog)
        {
            frameWidth = _frameWidth;
            frameHeight = _frameHeight;

            nofFrames2Train = _nofFrames2Train;
            nofFrames2Recog = _nofFrames2Recog;

            wb = new WriteableBitmap(frameWidth, frameHeight, 96, 96, PixelFormats.Bgr24, null);
            centerModule.myWindow.webCamImg.Source = wb;

            frameIdx = 0;

            //여러 sequence의 face를 담는 container임, list로 구성
            if(faceContainer == null)
                faceContainer = new List<Mat>();

            faceContainer.Clear();

            



            return InitWebCam();
        }

        public bool startWebCam()
        {
            frameIdx = 0;
            showWebCam = true;
            
            InitWebCam();

            cameraCapture = new Thread(new ThreadStart(CaptureProcess));
            cameraCapture.Start();

            return true;
        }
        public bool stopWebCam()
        {
            /*
             * HARDCODING...
             * FOR INIT CAMERA SOURCE...
             * */
            totalFrameIdx = 0;
            showWebCam = false;
            Mat tmp = new Mat();
            Cv2.Resize(Cv2.ImRead("C:/test.jpg"),tmp,new Size(640,480));
            centerModule.drawImage(centerModule.myWindow.webCamImg, wb,tmp);

            return true;
        }

        private void CaptureProcess()
        {
            Mat frame = new Mat();
            while (showWebCam == true)
            {
                totalFrameIdx++;
                if (cap.Read(frame))
                {
                    centerModule.drawImage(wb, frame);

                   
                    //camera에서의 train단
                    if (isTrainMode == true)
                    {
                        faceContainer.Add(frame);
                        if(frameIdx == 0)
                            centerModule.printTxtOnDebug(totalFrameIdx + "th frame, Get Training Img...");
                        frameIdx++;

                        if (frameIdx >= nofFrames2Train)
                        {

                            frameIdx = 0;
                            isTrainMode = false;


                            //center한테 이미지 정보 보내기
                            centerModule.sendFaceInfo(faceContainer);
                            faceContainer.Clear();
                            

                            //centerModule.printTxtOnDebug("Finish Get Training Img...");
                        }
                        
                    }
                    //camera에서의 test단
                    //test단은 멈추지 않는 이상 계속 돌아야 합니다.
                        //다른 곳에서 isTestMode = false를 해줘야 합니다 기억하세여
                    else if (isTestMode == true)
                    {
                        faceContainer.Add(frame);

                        if (frameIdx > nofFrames2Recog)
                        {
                            frameIdx = 0;

                            centerModule.sendFaceInfo(faceContainer);

                            faceContainer.Clear();
                        }
                    }
                }
            }
        }

        public void start2getTrainImg()
        {
            isTrainMode = true;
            faceContainer.Clear();
            frameIdx = 0;

        }
        public void start2getRecogImg()
        {

        }
        
        //웹캠 초기화 부분
        private bool InitWebCam()
        {
            try
            {
                cap = VideoCapture.FromCamera(CaptureDevice.Any, 0);
                cap.FrameWidth = frameWidth;
                cap.FrameHeight = frameHeight;
                cap.Open(0);

                return true;
            }
            catch
            {
                return false;
            }
        }

        



    }
}
