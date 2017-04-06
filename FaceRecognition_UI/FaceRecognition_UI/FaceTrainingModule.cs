using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
/*
 * Face 학습 모듈
 * Center로부터 Face set을 받아서(비디오 100개 frame)
 * WHOAMI api를 통해서 feature를 뽑아서 저장한다
 * */
namespace FaceRecognition_UI
{
    class FaceTrainingModule
    {
        FaceCenterModule centerModule;
        public FaceTrainingModule(FaceCenterModule _centerModule)
        {
            centerModule = _centerModule;
        }

        public bool Initialize(int _nofFrames2Train, int _maxPersonCount)
        {
            return true;
        }
    }
}
