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

    public partial class MainWindow : Window
    {


        private uint message;
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

            handle = new WindowInteropHelper(this).Handle;
            message = RegisterWindowMessage("User Message");
        }

        private void recognizer_SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {

            SpeechSynthesizer tts2 = new SpeechSynthesizer();
            tts2.SelectVoice("Microsoft Server Speech Text to Speech Voice (ko-KR, Heami)");
            tts2.SetOutputToDefaultAudioDevice();
            tts2.Speak(e.Result.Text);

            uint msg = 0;
            switch (e.Result.Text)
            {
                case "아미" :
                    msg = 1;
                    break;
                case "잘했어":
                    msg = 2;
                    break;
                case "못했어":
                    msg = 3;
                    break;
            }
            PostMessage((IntPtr)HWND_BROADCAST, message, (uint)handle, msg);

        }

 
    }
}
