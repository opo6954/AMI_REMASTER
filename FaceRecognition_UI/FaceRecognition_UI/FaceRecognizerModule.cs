using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
/*
 * Face 인식 모듈
 * face 추적단도 붙어있나???? 물어봐야할듯
 * Center로부터 특정 frame을 받아서 인식하기
 * Center로부터 Face set을 받아서(비디오 100개 frame)
 * WHOAMI api를 통해서 feature를 뽑아서 저장한다
 * */
namespace FaceRecognition_UI
{
    class FaceRecognizerModule
    {
        FaceCenterModule centerModule;
        public FaceRecognizerModule(FaceCenterModule _centerModule)
        {
            centerModule = _centerModule;
        }
        public bool Initialize(int _nofFrames2Recog, int _maxPersonCount)
        {
            return true;
        }
    }
}
