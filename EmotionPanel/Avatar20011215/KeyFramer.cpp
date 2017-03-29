// KeyFramer.cpp: implementation of the CKeyFramer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Avatar.h"
#include "pDef.h"
#include "mainfrm.h"
#include "AvatarDoc.h"
#include "AvatarView.h"
#include "KeyFramer.h"
#include "KeyFramerdlg.h"
#include "ExpSliders.h"
#include "parser.h"
#include "mydib.h"
#include "MorphObj.h"
#include "ToothObj.h"
#include "TongueObj.h"
#include "eye.h"
#include "EYES.h"
#include "OpenGLview.h"
#include "Face.h"

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

extern CSliderCtrl m_ctrlSlider;
extern CKeyFramerDlg KeyFramerDlg;

extern COpenGLView m_wndViewPort;
extern SliderStatus m_SliderStatus;
extern SliderStatus CurrentStatus;

extern CParser *pParserToothDefault;
extern CParser *pParserToothOpen;
extern CParser *pParserTongueDefault;
extern CParser *pParserTongueOpen;
extern CParser *pParserToothMap;
extern CParser *pParserTongueMap;

extern CFace Face;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyFramer::CKeyFramer(UINT eventid)
{
	FrameData = new CFrameData;

	CurrentFrame=0;

	FrameData->framenumber = 0;
	FrameData->mouth_open		= 0.0f;
	FrameData->liptail_up		= 0.0f;
	FrameData->liptail_down		= 0.0f;
	FrameData->lip_longer		= 0.0f;
	FrameData->lip_tighten_o	= 0.0f;
	FrameData->lip_tighten_u	= 0.0f;
	FrameData->lip_open			= 0.0f;
	FrameData->eyetail_up		= 0.0f;
	FrameData->eye_close		= 0.0f;
	FrameData->eyebrow_up		= 0.0f;
	FrameData->eyebrow_glower	= 0.0f;

	KeyFrameList.AddTail(FrameData);

	pFrame=0;
	
	flagstatusbar=TRUE;
	id = eventid;

	playing=FALSE;

}

CKeyFramer::CKeyFramer( UINT eventid, bool flag )
{
	FrameData = new CFrameData;

	CurrentFrame=0;

	FrameData->framenumber = 0;
	FrameData->mouth_open		= 0.0f;
	FrameData->liptail_up		= 0.0f;
	FrameData->liptail_down		= 0.0f;
	FrameData->lip_longer		= 0.0f;
	FrameData->lip_tighten_o	= 0.0f;
	FrameData->lip_tighten_u	= 0.0f;
	FrameData->lip_open			= 0.0f;
	FrameData->eyetail_up		= 0.0f;
	FrameData->eye_close		= 0.0f;
	FrameData->eyebrow_up		= 0.0f;
	FrameData->eyebrow_glower	= 0.0f;

	KeyFrameList.AddTail(FrameData);

	pFrame=0;
	
	flagstatusbar=flag;
	id = eventid;

	playing = FALSE;
}

CKeyFramer::~CKeyFramer()
{
	CleanUpHeap();
}

void CKeyFramer::SetCurrentFrame( int frame )
{
	PrevFrame=CurrentFrame;
	CurrentFrame=frame;
	RefreshFace();
}

void CKeyFramer::NextFrame()
{
	PrevFrame=CurrentFrame;
	CurrentFrame+=1;
	if(flagstatusbar)
		m_ctrlSlider.SetPos( CurrentFrame );
	RefreshFace();
}

void CKeyFramer::PreviousFrame()
{
	if(CurrentFrame>0)
	{
		PrevFrame=CurrentFrame;
		CurrentFrame-=1;
		if(flagstatusbar)
			m_ctrlSlider.SetPos( CurrentFrame );
		RefreshFace();
	}
}

void CKeyFramer::RefreshFace()
{
/*
	if(!pFrame)
	{
		pFrame=(CMainFrame*) AfxGetMainWnd();
		pView=(CAvatarView*) pFrame->GetActiveView();
	}
*/
	CFrameData* PreviousFrameData;
	CFrameData* NextFrameData;
	POSITION pos;
	float framedifference, frame, frame2;
	SliderStatus status;
	float mouthopen;
	float liptighteno;
	float liptightenu;

	KeyFramerDlg.RefreshSlider();

	if(!KeyFrameList.GetCount())
		return;
	
	for(pos=KeyFrameList.GetHeadPosition(); pos!=NULL ;)
	{
		PreviousFrameData = (CFrameData*)KeyFrameList.GetNext(pos);
		if(pos)
			NextFrameData = (CFrameData*)KeyFrameList.GetAt(pos);
		else
		{
			NextFrameData = PreviousFrameData;
			framedifference = (float)(CurrentFrame - PreviousFrameData->framenumber);
			if(!framedifference)
				framedifference=1;
//			pView->KillTimer( 0 );
			playing=FALSE;
			break;
		}

		if(NextFrameData->framenumber > CurrentFrame)
		{
			framedifference = (float)(NextFrameData->framenumber - PreviousFrameData->framenumber);
			break;
		}
	}
	frame = (float)(CurrentFrame - PreviousFrameData->framenumber);
	frame2 = framedifference - frame;

	frame/=framedifference;
	frame2/=framedifference;

	TRACE("frame %x %x %f, %f  %f  \n", PreviousFrameData->framenumber,NextFrameData->framenumber,frame2, frame, framedifference);

//	Obj->ResetInitialVertexList();

	mouthopen = PreviousFrameData->mouth_open*frame2 + NextFrameData->mouth_open*frame;
	Obj->CalculateMorph(pParser1->GetVertexList(), mouthopen);
	Obj->CalculateMorph(pParser2->GetVertexList(), PreviousFrameData->liptail_up*frame2 + NextFrameData->liptail_up*frame);
	Obj->CalculateMorph(pParser3->GetVertexList(), PreviousFrameData->liptail_down*frame2 + NextFrameData->liptail_down*frame);
	Obj->CalculateMorph(pParser4->GetVertexList(), PreviousFrameData->lip_longer*frame2 + NextFrameData->lip_longer*frame);
	liptighteno = PreviousFrameData->lip_tighten_o*frame2 + NextFrameData->lip_tighten_o*frame;
	Obj->CalculateMorph(pParser5->GetVertexList(), liptighteno);
	liptightenu = PreviousFrameData->lip_tighten_u*frame2 + NextFrameData->lip_tighten_u*frame;
	Obj->CalculateMorph(pParser6->GetVertexList(), liptightenu);
	Obj->CalculateMorph(pParser7->GetVertexList(), PreviousFrameData->lip_open*frame2 + NextFrameData->lip_open*frame);
	Obj->CalculateMorph(pParser8->GetVertexList(), PreviousFrameData->eyetail_up*frame2 + NextFrameData->eyetail_up*frame);
	Obj->CalculateMorph(pParser9->GetVertexList(), PreviousFrameData->eye_close*frame2 + NextFrameData->eye_close*frame);
	Obj->CalculateMorph(pParser10->GetVertexList(), PreviousFrameData->eyebrow_up*frame2 + NextFrameData->eyebrow_up*frame);
	Obj->CalculateMorph(pParser11->GetVertexList(), PreviousFrameData->eyebrow_glower*frame2 + NextFrameData->eyebrow_glower*frame);

	CurrentStatus.mouth_open = mouthopen;
	CurrentStatus.liptail_up = PreviousFrameData->liptail_up*frame2 + NextFrameData->liptail_up*frame;
	CurrentStatus.liptail_down = PreviousFrameData->liptail_down*frame2 + NextFrameData->liptail_down*frame;
	CurrentStatus.lip_longer = PreviousFrameData->lip_longer*frame2 + NextFrameData->lip_longer*frame;
	CurrentStatus.lip_tighten_o = liptighteno;
	CurrentStatus.lip_tighten_u = liptightenu;
	CurrentStatus.lip_open = PreviousFrameData->lip_open*frame2 + NextFrameData->lip_open*frame;
	CurrentStatus.eyetail_up = PreviousFrameData->eyetail_up*frame2 + NextFrameData->eyetail_up*frame;
	CurrentStatus.eye_close = PreviousFrameData->eye_close*frame2 + NextFrameData->eye_close*frame;
	CurrentStatus.eyebrow_up = PreviousFrameData->eyebrow_up*frame2 + NextFrameData->eyebrow_up*frame;
	CurrentStatus.eyebrow_glower = PreviousFrameData->eyebrow_glower*frame2 + NextFrameData->eyebrow_glower*frame;

	ToothObj->CalculateMorph(pParserToothOpen->GetVertexList(), mouthopen+liptighteno/2+liptightenu/3-0.02f);
	TongueObj->CalculateMorph(pParserTongueOpen->GetVertexList(), mouthopen+liptighteno/2+liptightenu/3);

	if(flagstatusbar)
	{
		status.mouth_open = (PreviousFrameData->mouth_open*frame2 + NextFrameData->mouth_open*frame);
		status.liptail_up = PreviousFrameData->liptail_up*frame2 + NextFrameData->liptail_up*frame;
		status.liptail_down = PreviousFrameData->liptail_down*frame2 + NextFrameData->liptail_down*frame;
		status.lip_longer = PreviousFrameData->lip_longer*frame2 + NextFrameData->lip_longer*frame;
		status.lip_tighten_o = PreviousFrameData->lip_tighten_o*frame2 + NextFrameData->lip_tighten_o*frame;
		status.lip_tighten_u = PreviousFrameData->lip_tighten_u*frame2 + NextFrameData->lip_tighten_u*frame;
		status.lip_open = PreviousFrameData->lip_open*frame2 + NextFrameData->lip_open*frame;
		status.eyetail_up = PreviousFrameData->eyetail_up*frame2 + NextFrameData->eyetail_up*frame;
		status.eye_close = PreviousFrameData->eye_close*frame2 + NextFrameData->eye_close*frame;
		status.eyebrow_up = PreviousFrameData->eyebrow_up*frame2 + NextFrameData->eyebrow_up*frame;
		status.eyebrow_glower = PreviousFrameData->eyebrow_glower*frame2 + NextFrameData->eyebrow_glower*frame;

		ExpSliders.UpdateSliders( &status );
		char temp[50];
		sprintf(temp, "%d frame   ", CurrentFrame);
		pFrame->m_wndStatusBar.SetWindowText( temp );
	}
//	else if(id==4 && Face.IsInitializing())
//	{
		status.mouth_open = (PreviousFrameData->mouth_open*frame2 + NextFrameData->mouth_open*frame);
		status.liptail_up = PreviousFrameData->liptail_up*frame2 + NextFrameData->liptail_up*frame;
		status.liptail_down = PreviousFrameData->liptail_down*frame2 + NextFrameData->liptail_down*frame;
		status.lip_longer = PreviousFrameData->lip_longer*frame2 + NextFrameData->lip_longer*frame;
		status.lip_tighten_o = PreviousFrameData->lip_tighten_o*frame2 + NextFrameData->lip_tighten_o*frame;
		status.lip_tighten_u = PreviousFrameData->lip_tighten_u*frame2 + NextFrameData->lip_tighten_u*frame;
		status.lip_open = PreviousFrameData->lip_open*frame2 + NextFrameData->lip_open*frame;
		status.eyetail_up = PreviousFrameData->eyetail_up*frame2 + NextFrameData->eyetail_up*frame;
		status.eye_close = PreviousFrameData->eye_close*frame2 + NextFrameData->eye_close*frame;
		status.eyebrow_up = PreviousFrameData->eyebrow_up*frame2 + NextFrameData->eyebrow_up*frame;
		status.eyebrow_glower = PreviousFrameData->eyebrow_glower*frame2 + NextFrameData->eyebrow_glower*frame;

		ExpSliders.UpdateSliders( &status );
		/*
		Obj->CalculateMorph(pParser1->GetVertexList(), m_SliderStatus.mouth_open);
		Obj->CalculateMorph(pParser2->GetVertexList(), m_SliderStatus.liptail_up);
		Obj->CalculateMorph(pParser3->GetVertexList(), m_SliderStatus.liptail_down);
		Obj->CalculateMorph(pParser4->GetVertexList(), m_SliderStatus.lip_longer);
		Obj->CalculateMorph(pParser5->GetVertexList(), m_SliderStatus.lip_tighten_o);
		Obj->CalculateMorph(pParser6->GetVertexList(), m_SliderStatus.lip_tighten_u);
		Obj->CalculateMorph(pParser7->GetVertexList(), m_SliderStatus.lip_open);
		Obj->CalculateMorph(pParser8->GetVertexList(), m_SliderStatus.eyetail_up);
		Obj->CalculateMorph(pParser9->GetVertexList(), m_SliderStatus.eye_close);
		Obj->CalculateMorph(pParser10->GetVertexList(), m_SliderStatus.eyebrow_up);
		Obj->CalculateMorph(pParser11->GetVertexList(), m_SliderStatus.eyebrow_glower);
		*/
//	}

//	m_wndViewPort.InvalidateRect(NULL,FALSE);

	PrevFrame=CurrentFrame;
}

void CKeyFramer::RecordFrame()
{
	CFrameData* pFrameData;
	POSITION pos;
	bool flag = FALSE;

	FrameData = new CFrameData;

	FrameData->framenumber		= CurrentFrame;
	FrameData->mouth_open		= (float)ExpSliders.m_mouth_open.GetPos()/100.0f;
	FrameData->liptail_up		= (float)ExpSliders.m_liptail_up.GetPos()/100.0f;
	FrameData->liptail_down		= (float)ExpSliders.m_liptail_down.GetPos()/100.0f;
	FrameData->lip_longer		= (float)ExpSliders.m_lip_longer.GetPos()/100.0f;
	FrameData->lip_tighten_o	= (float)ExpSliders.m_lip_tighten_o.GetPos()/100.0f;
	FrameData->lip_tighten_u	= (float)ExpSliders.m_lip_tighten_u.GetPos()/100.0f;
	FrameData->lip_open			= (float)ExpSliders.m_lip_open.GetPos()/100.0f;
	FrameData->eyetail_up		= (float)ExpSliders.m_eyetail_up.GetPos()/100.0f;
	FrameData->eye_close		= (float)ExpSliders.m_eye_close.GetPos()/100.0f;
	FrameData->eyebrow_up		= (float)ExpSliders.m_eyebrow_up.GetPos()/100.0f;
	FrameData->eyebrow_glower	= (float)ExpSliders.m_eyebrow_glower.GetPos()/100.0f;

	for(pos=KeyFrameList.GetHeadPosition(); pos!=NULL ;KeyFrameList.GetNext(pos))
	{
		pFrameData = (CFrameData*)KeyFrameList.GetAt(pos);
		if(pFrameData->framenumber == CurrentFrame)
		{
			pFrameData->mouth_open			= (float)ExpSliders.m_mouth_open.GetPos()/100.0f;
			pFrameData->liptail_up			= (float)ExpSliders.m_liptail_up.GetPos()/100.0f;
			pFrameData->liptail_down		= (float)ExpSliders.m_liptail_down.GetPos()/100.0f;
			pFrameData->lip_longer			= (float)ExpSliders.m_lip_longer.GetPos()/100.0f;
			pFrameData->lip_tighten_o		= (float)ExpSliders.m_lip_tighten_o.GetPos()/100.0f;
			pFrameData->lip_tighten_u		= (float)ExpSliders.m_lip_tighten_u.GetPos()/100.0f;
			pFrameData->lip_open			= (float)ExpSliders.m_lip_open.GetPos()/100.0f;
			pFrameData->eyetail_up			= (float)ExpSliders.m_eyetail_up.GetPos()/100.0f;
			pFrameData->eye_close			= (float)ExpSliders.m_eye_close.GetPos()/100.0f;
			pFrameData->eyebrow_up			= (float)ExpSliders.m_eyebrow_up.GetPos()/100.0f;
			pFrameData->eyebrow_glower		= (float)ExpSliders.m_eyebrow_glower.GetPos()/100.0f;

			flag=TRUE;

			delete FrameData;

			break;
		}

		if(pFrameData->framenumber > CurrentFrame)
		{
			KeyFrameList.InsertBefore( pos, FrameData);
			flag=TRUE;
			break;
		}
	}
	if(!flag)
		KeyFrameList.AddTail(FrameData);

}

void CKeyFramer::FileLoad(char * filename)
{
	char singleline[200];

	FILE *stream;

	int eof=FALSE;

	if( (stream  = fopen( filename, "r" )) == NULL )
	{
		AfxMessageBox( filename );
		return;
	}

//	CleanUpHeap();

	do
	{
		fgets(singleline,100,stream);
		if(strnicmp(singleline,"#",1)&&strlen(singleline)>3)
		{
			if(!strnicmp(singleline,"key",3))
			{
				FrameData = new CFrameData;

				sscanf(singleline, "key:%d\n", &FrameData->framenumber);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->mouth_open);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->liptail_up);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->liptail_down);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->lip_longer);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->lip_tighten_o);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->lip_tighten_u);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->lip_open);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->eyetail_up);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->eye_close);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->eyebrow_up);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &FrameData->eyebrow_glower);
		
				KeyFrameList.AddTail(FrameData);
			}
		}
		if(!strnicmp(singleline,"END",3))
			eof=TRUE;
	}while(!feof(stream)&&!eof);

	fclose( stream );
	

}

void CKeyFramer::FileSave(char * filename)
{
	char singleline[200];
	FILE *stream;

	if( (stream  = fopen( filename, "w" )) == NULL )
	{
		AfxMessageBox( filename );
		return;
	}

	fputs("#\n# Programmable Avatar Key Frame File v 0.1\n#\n",stream);

	POSITION pos;
	CFrameData* pFrameData;
	
	for(pos=KeyFrameList.GetHeadPosition(); pos!=NULL ;KeyFrameList.GetNext(pos))
	{
		pFrameData = (CFrameData*)KeyFrameList.GetAt(pos);

		sprintf(singleline, "key:%d\n", pFrameData->framenumber);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->mouth_open);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->liptail_up);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->liptail_down);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->lip_longer);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->lip_tighten_o);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->lip_tighten_u);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->lip_open);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->eyetail_up);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->eye_close);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->eyebrow_up);
		fputs(singleline,stream);
		
		sprintf(singleline, "%f\n", pFrameData->eyebrow_glower);
		fputs(singleline,stream);
	}

	fputs("END",stream);

	fclose( stream );
}

void CKeyFramer::CleanUpHeap()
{
	POSITION pos;
	CFrameData* pFrameData;
	
	for(pos=KeyFrameList.GetHeadPosition(); pos!=NULL ;KeyFrameList.GetNext(pos))
	{
		pFrameData = (CFrameData*)KeyFrameList.GetAt(pos);
		delete pFrameData;
	}
	KeyFrameList.RemoveAll();
}

void CKeyFramer::play()
{
	if(!playing)
	{
/*		if(!pFrame)
		{
			pFrame=(CMainFrame*) AfxGetMainWnd();
			pView=(CAvatarView*) pFrame->GetActiveView();
		}*/
		CurrentFrame=0;
//		pView->SetTimer( id , 25, NULL);
		playing=TRUE;
	}
}

bool CKeyFramer::IsPlaying()
{
	return playing;
}

void CKeyFramer::stop()
{
	playing=FALSE;
	CurrentFrame=0;
}

void CKeyFramer::RecordFrameAs(CFrameData &Frame)
{
	FrameData = new CFrameData;

	FrameData->framenumber		= Frame.framenumber;
	FrameData->mouth_open		= Frame.mouth_open;
	FrameData->liptail_up		= Frame.liptail_up;
	FrameData->liptail_down		= Frame.liptail_down;
	FrameData->lip_longer		= Frame.lip_longer;
	FrameData->lip_tighten_o	= Frame.lip_tighten_o;
	FrameData->lip_tighten_u	= Frame.lip_tighten_u;
	FrameData->lip_open			= Frame.lip_open;
	FrameData->eyetail_up		= Frame.eyetail_up;
	FrameData->eye_close		= Frame.eye_close;
	FrameData->eyebrow_up		= Frame.eyebrow_up;
	FrameData->eyebrow_glower	= Frame.eyebrow_glower;

	KeyFrameList.AddTail(FrameData);
}

void CKeyFramer::EraseSequence()
{
	CleanUpHeap();
	KeyFramerDlg.RefreshSlider(TRUE);
}
