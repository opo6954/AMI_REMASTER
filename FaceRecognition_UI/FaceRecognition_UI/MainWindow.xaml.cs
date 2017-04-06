using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;

using OpenCvSharp;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.Extensions;





namespace FaceRecognition_UI
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : System.Windows.Window
    {
        /*
         *********************상수 입력 필드*******************
         * */
        const int frameWidth = 640;
        const int frameHeight = 480;
        const int nOfFrame2Train = 5;
        const int nOfFrame2Recog = 30;
        const int maximumPersonCount = 5;

        
        FaceCenterModule centerModule;
        public DebugWindow debugWindow;

        public MainWindow()
        {
            InitializeComponent();
        }
        
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            initWPFComponent();
            centerModule = new FaceCenterModule(this);

            debugWindow = new DebugWindow();
            debugWindow.Show();

            

            if (centerModule.Initialize(frameWidth, frameHeight,nOfFrame2Train,nOfFrame2Recog,maximumPersonCount) == false)
            {
                MessageBox.Show("Init Center Module Fail...");
            }
            else
            {
                MessageBox.Show("Init Center Module Success...");
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            /*
            showWebCam = false;
            //camera thread 멈추기

            if (cap != null)
            {
                if (cap.IsOpened())
                {
                    cap.Dispose();
                }
            }
             * */
        }
       
        private void initWPFComponent()
        {
            
            

        }

        private void StartWebButton_Click_1(object sender, RoutedEventArgs e)
        {
            centerModule.webCamStart();
            centerModule.printTxtOnDebug("Start Webcam...");
        }

        private void TestButton_Click_1(object sender, RoutedEventArgs e)
        {

        }

        private void TrainButton_Click_1(object sender, RoutedEventArgs e)
        {
            centerModule.trainStart(TrainFaceName.Text);
        }


        private void StopWebCam_Click_1(object sender, RoutedEventArgs e)
        {
            centerModule.webCamStop();
            centerModule.printTxtOnDebug("Finish Webcam...");
        }

    }

}
