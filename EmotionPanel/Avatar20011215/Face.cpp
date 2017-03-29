// Face.cpp: implementation of the CFace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDef.h"
#include "Avatar.h"
#include "MyDib.h"

#include "Eyes.h"
#include "Avatar.h"

#include "MorphObj.h"
#include "ToothObj.h"
#include "TongueObj.h"

#include "MainFrm.h"

#include "Parser.h"

#include "AvatarDoc.h"
#include "OpenGLVIew.h"
#include "AvatarView.h"

#include "ExpSliders.h"
#include "Keyframerdlg.h"
#include "Keyframer.h"

#include"gl\gl.h"
#include"gl\glu.h"
#include"gl\glaux.h"

#include "math.h"

#include "Face.h"

#include "ShapeArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CExpSliders ExpSliders;
extern CMorphObj *Obj;
extern CToothObj *ToothObj;
extern CTongueObj *TongueObj;
extern CParser *pParser0;
extern CParser *pParser1;
extern CParser *pParser2;
extern CParser *pParser3;
extern CParser *pParser4;
extern CParser *pParser5;
extern CParser *pParser6;
extern CParser *pParser7;
extern CParser *pParser8;
extern CParser *pParser9;
extern CParser *pParser10;
extern CParser *pParser11;
extern CParser *pParser12;

extern CParser *pParserToothDefault;
extern CParser *pParserToothOpen;
extern CParser *pParserTongueDefault;
extern CParser *pParserTongueOpen;
extern CParser *pParserToothMap;
extern CParser *pParserTongueMap;

extern CSliderCtrl m_ctrlSlider;
extern CKeyFramerDlg KeyFramerDlg;
extern CKeyFramer KeyFramer;
extern CKeyFramer KeyFramerWink;
extern CKeyFramer KeyFramerSmile;
extern CKeyFramer KeyFramerBrowUp;
extern CKeyFramer KeyFramerRealtime;

extern COpenGLView m_wndViewPort;

extern SliderStatus CurrentStatus;
extern SliderStatus NextStatus;

extern CEyes Eyes;
extern CFace Face;

extern CShapeArray ShapeArray;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFace::CFace()
{

	pParser0 = new CParser;
	pParser1 = new CParser;
	pParser2 = new CParser;
	pParser3 = new CParser;
	pParser4 = new CParser;
	pParser5 = new CParser;
	pParser6 = new CParser;
	pParser7 = new CParser;
	pParser8 = new CParser;
	pParser9 = new CParser;
	pParser10 = new CParser;
	pParser11 = new CParser;
	pParser12 = new CParser;

	pParserToothDefault = new CParser;
	pParserToothMap = new CParser;
	pParserToothOpen = new CParser;
	pParserTongueDefault = new CParser;
	pParserTongueOpen = new CParser;
	pParserTongueMap = new CParser;
	
	pParser0->parse("facedata\\faceDefault.obj");
	pParser1->parsevertex("facedata\\mouthopen.vec");
	pParser2->parsevertex("facedata\\liptailup.vec");
	pParser3->parsevertex("facedata\\liptaildown.vec");
	pParser4->parsevertex("facedata\\liplonger.vec");
	pParser5->parsevertex("facedata\\liptighteno.vec");
	pParser6->parsevertex("facedata\\liptightenu.vec");
	pParser7->parsevertex("facedata\\lipopen.vec");
	pParser8->parsevertex("facedata\\eyetailup.vec");
	pParser9->parsevertex("facedata\\eyeclose.vec");
	pParser10->parsevertex("facedata\\eyebrowup.vec");
	pParser11->parsevertex("facedata\\eyebrowglower.vec");
	pParser12->parsevertex("facedata\\FaceBase.obj");

	pParserToothDefault->parse("facedata\\ToothDefault.obj");
	pParserToothMap->parsevertex("facedata\\ToothDefault.obj");
	pParserToothOpen->parsevertex("facedata\\ToothOpen.vec");
	pParserTongueDefault->parse("facedata\\tongueDefault.obj");
	pParserTongueOpen->parsevertex("facedata\\tongueOpen.vec");
	pParserTongueMap->parsevertex("facedata\\tongueDefault.obj");

	Obj = new CMorphObj(pParser0->GetVertexList(), pParser0->GetFaceList(), pParser12->GetVertexList());
	ToothObj = new CToothObj(pParserToothDefault->GetVertexList(),
							 pParserToothDefault->GetFaceList(),
							 pParserToothMap->GetVertexList());
	TongueObj = new CTongueObj(pParserTongueDefault->GetVertexList(),
							 pParserTongueDefault->GetFaceList(),
							 pParserTongueMap->GetVertexList());

	KeyFramerWink.FileLoad("facedata\\wink.key");
	KeyFramerSmile.FileLoad("facedata\\smile.key");
	KeyFramerBrowUp.FileLoad("facedata\\eye_brow_up.key");

	srand( (unsigned)time( NULL ) );

	MotionDelayCount = 0;
	InitFlag=TRUE;

}

CFace::~CFace()
{
	delete Obj;
	delete pParser0;
	delete pParser1;
	delete pParser2;
	delete pParser3;
	delete pParser4;
	delete pParser5;
	delete pParser6;
	delete pParser7;
	delete pParser8;
	delete pParser9;
	delete pParser10;
	delete pParser11;
	delete pParser12;

	delete pParserToothDefault;
	delete pParserToothOpen;
	delete pParserToothMap;
	
	delete pParserTongueDefault;
	delete pParserTongueOpen;
	delete pParserTongueMap;

	delete ToothObj;
	delete TongueObj;
}

void CFace::FaceCalculate()
{
	Obj->ResetInitialVertexList();
	ToothObj->ResetInitialVertexList();
	TongueObj->ResetInitialVertexList();

	Obj->CalculateMorph(pParser1->GetVertexList(), CurrentStatus.mouth_open);
	Obj->CalculateMorph(pParser2->GetVertexList(), CurrentStatus.liptail_up);
	Obj->CalculateMorph(pParser3->GetVertexList(), CurrentStatus.liptail_down);
	Obj->CalculateMorph(pParser4->GetVertexList(), CurrentStatus.lip_longer);
	Obj->CalculateMorph(pParser5->GetVertexList(), CurrentStatus.lip_tighten_o);
	Obj->CalculateMorph(pParser6->GetVertexList(), CurrentStatus.lip_tighten_u);
	Obj->CalculateMorph(pParser7->GetVertexList(), CurrentStatus.lip_open);
	Obj->CalculateMorph(pParser8->GetVertexList(), CurrentStatus.eyetail_up);
	Obj->CalculateMorph(pParser9->GetVertexList(), CurrentStatus.eye_close);
	Obj->CalculateMorph(pParser10->GetVertexList(), CurrentStatus.eyebrow_up);
	Obj->CalculateMorph(pParser11->GetVertexList(), CurrentStatus.eyebrow_glower);

	ToothObj->CalculateMorph(pParserToothOpen->GetVertexList(), CurrentStatus.mouth_open+CurrentStatus.lip_tighten_o/2+CurrentStatus.lip_tighten_u/3-0.02f);
	TongueObj->CalculateMorph(pParserTongueOpen->GetVertexList(), CurrentStatus.mouth_open+CurrentStatus.lip_tighten_o/2+CurrentStatus.lip_tighten_u/3);
}

void CFace::OnTimer()
{
//	static float x = 0;
//	static float y = 0;
//	float dx;
//	float dy;

	bool flag=FALSE;

	Obj->ResetInitialVertexList();
	ToothObj->ResetInitialVertexList();
	TongueObj->ResetInitialVertexList();
/*
	do
	{
		dx=((float)rand()/RAND_MAX-0.5f);
	}
	while( x+dx < -10.0 || x+dx > 10.0 );
	x+=dx;
	
	do
	{
		dy=((float)rand()/RAND_MAX-0.5f);
	}
	while( y+dy < -6.0 || y+dy > 6.0 );
	y+=dy;
*/	
//	Eyes.SetEyesOri( x, y );

	if(!MotionDelayCount)
	{
		if(!InitFlag)
		{
			MakeInitializingSequence();
			MotionDelayCount = 30;
			KeyFramerRealtime.play();
		}
		else
		{
			
			if(rand()%50==1)
			{
				KeyFramerWink.play();
//				x=y=0.0;
			}
			if(rand()%150==1)
			{
				KeyFramerSmile.play();
				KeyFramerBrowUp.play();
			}
			if(rand()%150==1)
				KeyFramerBrowUp.play();

			
			if(KeyFramer.IsPlaying())
			{
				KeyFramer.NextFrame();
				flag=TRUE;
			}
			if(KeyFramerWink.IsPlaying())
			{
				KeyFramerWink.NextFrame();
				flag=TRUE;
			}
			if(KeyFramerSmile.IsPlaying())
			{
				KeyFramerSmile.NextFrame();
				flag=TRUE;
			}
			if(KeyFramerBrowUp.IsPlaying())
			{
				KeyFramerBrowUp.NextFrame();
				flag=TRUE;
			}
		}
	}

//	if(KeyFramerRealtime.IsPlaying())
	if(KeyFramer.IsPlaying())
	{
//		KeyFramerRealtime.NextFrame();
		KeyFramer.NextFrame();
		flag=TRUE;
	}

	if(!flag)
	{
		CurrentStatus.mouth_open		= (float)ExpSliders.m_mouth_open.GetPos()/100.0f;
		CurrentStatus.liptail_up		= (float)ExpSliders.m_liptail_up.GetPos()/100.0f;
		CurrentStatus.liptail_down		= (float)ExpSliders.m_liptail_down.GetPos()/100.0f;
		CurrentStatus.lip_longer		= (float)ExpSliders.m_lip_longer.GetPos()/100.0f;
		CurrentStatus.lip_tighten_o		= (float)ExpSliders.m_lip_tighten_o.GetPos()/100.0f;
		CurrentStatus.lip_tighten_u		= (float)ExpSliders.m_lip_tighten_u.GetPos()/100.0f;
		CurrentStatus.lip_open			= (float)ExpSliders.m_lip_open.GetPos()/100.0f;
		CurrentStatus.eyetail_up		= (float)ExpSliders.m_eyetail_up.GetPos()/100.0f;
		CurrentStatus.eye_close			= (float)ExpSliders.m_eye_close.GetPos()/100.0f;
		CurrentStatus.eyebrow_up		= (float)ExpSliders.m_eyebrow_up.GetPos()/100.0f;
		CurrentStatus.eyebrow_glower	= (float)ExpSliders.m_eyebrow_glower.GetPos()/100.0f;

		Face.FaceCalculate();
	}


	if(MotionDelayCount)
		MotionDelayCount-=1;
//	m_wndViewPort.InvalidateRect(NULL, FALSE);
}

void CFace::Draw(float x, float y)
{
	glRotatef(x, 0.0, 1.0, 0.0f);
	glRotatef(y, 1.0, 0.0, 0.0f);

	glTranslatef(0.0f, -1.0f, 0.0f);

	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, 0);


	Obj->DrawInitialFaceObj();
	glDisable(GL_BLEND);
//	Obj->DrawInitialFaceObjLine();
//	Obj->DrawInitialFaceObjSmooth();
//	Obj->DrawInitialFaceObjTexture();
	
	ToothObj->Draw();
	TongueObj->Draw();

	if(x*x<900.0 && y*y<625)
		Eyes.SetEyesOri( -x/1.9, -y/1.9 );

	glTranslatef(0.0f, 2.440983f, 0.145325f);
	Eyes.Draw();
}

void CFace::MakeSequence()
{
	CFrameData InitialFrame;
	CFrameData FinalFrame;

	InitialFrame.framenumber= 0;
	InitialFrame.mouth_open = CurrentStatus.mouth_open;
	InitialFrame.liptail_up = CurrentStatus.liptail_up;
	InitialFrame.liptail_down = CurrentStatus.liptail_down;
	InitialFrame.lip_longer = CurrentStatus.lip_longer;
	InitialFrame.lip_tighten_o = CurrentStatus.lip_tighten_o;
	InitialFrame.lip_tighten_u = CurrentStatus.lip_tighten_u;
	InitialFrame.lip_open = CurrentStatus.lip_open;
	InitialFrame.eyetail_up = CurrentStatus.eyetail_up;
	InitialFrame.eye_close = CurrentStatus.eye_close;
	InitialFrame.eyebrow_up = CurrentStatus.eyebrow_up;
	InitialFrame.eyebrow_glower = CurrentStatus.eyebrow_glower;

	FinalFrame.framenumber= 10;
	FinalFrame.mouth_open = NextStatus.mouth_open;
	FinalFrame.liptail_up = NextStatus.liptail_up;
	FinalFrame.liptail_down = NextStatus.liptail_down;
	FinalFrame.lip_longer = NextStatus.lip_longer;
	FinalFrame.lip_tighten_o = NextStatus.lip_tighten_o;
	FinalFrame.lip_tighten_u = NextStatus.lip_tighten_u;
	FinalFrame.lip_open = NextStatus.lip_open;
	FinalFrame.eyetail_up = NextStatus.eyetail_up;
	FinalFrame.eye_close = NextStatus.eye_close;
	FinalFrame.eyebrow_up = NextStatus.eyebrow_up;
	FinalFrame.eyebrow_glower = NextStatus.eyebrow_glower;

//	KeyFramerRealtime.EraseSequence();

//	KeyFramerRealtime.RecordFrameAs(InitialFrame);
//	KeyFramerRealtime.RecordFrameAs(FinalFrame);
	KeyFramer.EraseSequence();

	KeyFramer.RecordFrameAs(InitialFrame);
	KeyFramer.RecordFrameAs(FinalFrame);
}

void CFace::PlaySequence()
{
//	KeyFramer.stop();
	KeyFramerWink.stop();
	KeyFramerSmile.stop();
	KeyFramerBrowUp.stop();

	MotionDelayCount = 100;
//	KeyFramerRealtime.play();
	KeyFramer.play();
	InitFlag=FALSE;
}

void CFace::MakeInitializingSequence()
{
	CFrameData InitialFrame;
	CFrameData FinalFrame;

	InitialFrame.framenumber= 0;
	InitialFrame.mouth_open = CurrentStatus.mouth_open;
	InitialFrame.liptail_up = CurrentStatus.liptail_up;
	InitialFrame.liptail_down = CurrentStatus.liptail_down;
	InitialFrame.lip_longer = CurrentStatus.lip_longer;
	InitialFrame.lip_tighten_o = CurrentStatus.lip_tighten_o;
	InitialFrame.lip_tighten_u = CurrentStatus.lip_tighten_u;
	InitialFrame.lip_open = CurrentStatus.lip_open;
	InitialFrame.eyetail_up = CurrentStatus.eyetail_up;
	InitialFrame.eye_close = CurrentStatus.eye_close;
	InitialFrame.eyebrow_up = CurrentStatus.eyebrow_up;
	InitialFrame.eyebrow_glower = CurrentStatus.eyebrow_glower;

	FinalFrame.framenumber= 15;
	FinalFrame.mouth_open = 0.0f;
	FinalFrame.liptail_up = 0.0f;
	FinalFrame.liptail_down = 0.0f;
	FinalFrame.lip_longer = 0.0f;
	FinalFrame.lip_tighten_o = 0.0f;
	FinalFrame.lip_tighten_u = 0.0f;
	FinalFrame.lip_open = 0.0f;
	FinalFrame.eyetail_up = 0.0f;
	FinalFrame.eye_close = 0.0f;
	FinalFrame.eyebrow_up = 0.0f;
	FinalFrame.eyebrow_glower = 0.0f;

//	KeyFramerRealtime.EraseSequence();

//	KeyFramerRealtime.RecordFrameAs(InitialFrame);
//	KeyFramerRealtime.RecordFrameAs(FinalFrame);
	KeyFramer.EraseSequence();

	KeyFramer.RecordFrameAs(InitialFrame);
	KeyFramer.RecordFrameAs(FinalFrame);

	InitFlag=TRUE;

}

bool CFace::IsInitializing()
{
	return InitFlag;
}
