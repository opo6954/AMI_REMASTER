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
using System.Windows.Interop;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Microsoft.Speech.Recognition;
using Microsoft.Speech.Synthesis;
using Microsoft.Speech.AudioFormat;




namespace WpfApplication1
{
    public class messageSet
    {
        public List<string> MSG_set = new List<string>()
        {
            "아미","기분이어때","잘했어","못했어","안녕","네","좋아요","나빠요","감사합니다.","열심히할게요"
        };
        public int p;
    }

    public partial class MainWindow : Window
    {
        private messageSet ms = new messageSet();

        private const string MAINMODULENAME = "MFCApplication1";
        private const string MSG_SR2CENTER = "WM_SR2CENTER";
        private const string MSG_CENTER2SR = "WM_CENTER2SR";

        

        private uint SR2Center;
        private uint Center2SR;

        private IntPtr handle;

        public const uint HWND_BROADCAST = 0xffff;

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        public static extern uint RegisterWindowMessage(string lpString);

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        public static extern bool PostMessage(IntPtr hWnd, uint Msg, uint wParam, uint lParam);


        private SpeechRecognitionEngine recognizer;


        public MainWindow()
        {
            
            InitializeComponent();
            this.Title = "SpeechModule";

        }

        protected override void OnSourceInitialized(EventArgs e)
        {
            base.OnSourceInitialized(e);

            recognizer = new SpeechRecognitionEngine("SR_MS_ko-KR_TELE_11.0");

            Grammar grammar = new Grammar("computer.xml");
            recognizer.LoadGrammar(grammar);
            recognizer.SetInputToDefaultAudioDevice();
            recognizer.SpeechRecognized += new EventHandler<SpeechRecognizedEventArgs>(recognizer_SpeechRecognized);
            recognizer.RecognizeAsync(RecognizeMode.Multiple);

            ComponentDispatcher.ThreadFilterMessage += new ThreadMessageEventHandler(ComponentDispatcher_ThreadFilterMessage);
            
            handle = new WindowInteropHelper(this).Handle;
            SR2Center = RegisterWindowMessage(MSG_SR2CENTER);
            Center2SR = RegisterWindowMessage(MSG_CENTER2SR);
            
        }

        void ComponentDispatcher_ThreadFilterMessage(ref MSG msg, ref bool handled)
        {

            if (msg.message == Center2SR && msg.wParam != handle)
            {
                
                SpeechSynthesizer tts2 = new SpeechSynthesizer();
                tts2.SelectVoice("Microsoft Server Speech Text to Speech Voice (ko-KR, Heami)");
                tts2.SetOutputToDefaultAudioDevice();
                int idx = (int)msg.lParam;
                tts2.Speak(ms.MSG_set[idx]);

                label1.Content = label1.Content + "\n " + msg.lParam.ToString() + "(" + ms.MSG_set[idx] + ") fromCenter...";
            }


        }

        //speech가 recognize 될 때의 event msg
        private void recognizer_SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            string recog_txt = e.Result.Text;
            uint idx=100;
            //만일 idx가 100일 경우 배우지 않은 단어로 취급...


            /*
            SpeechSynthesizer tts2 = new SpeechSynthesizer();
            tts2.SelectVoice("Microsoft Server Speech Text to Speech Voice (ko-KR, Heami)");
            tts2.SetOutputToDefaultAudioDevice();
            tts2.Speak(recog_txt);
            */



            

            for (int i = 0; i < ms.MSG_set.Count; i++)
            {
                if (ms.MSG_set[i] == recog_txt)
                {
                    idx = (uint)i;
                    break;
                }
            }

            uint msg = 100;

            if(idx <= 4)
                msg = idx;

            label1.Content = recog_txt + " " + msg.ToString();

 
            PostMessage((IntPtr)HWND_BROADCAST, SR2Center, (uint)handle, msg);
        }

        

 
    }
}
