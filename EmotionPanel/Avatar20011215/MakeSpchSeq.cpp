// MakeSpchSeq.cpp: implementation of the CMakeSpchSeq class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pDef.h"
#include "Avatar.h"
#include "MakeSpchSeq.h"
#include "mainfrm.h"
#include "AvatarDoc.h"
#include "AvatarView.h"
#include "KeyFramer.h"
#include "KeyFramerdlg.h"
#include "ExpSliders.h"
#include "ShapeArray.h"

#pragma hdrstop
#include "tables.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern CKeyFramer KeyFramerRealtime;
extern CKeyFramer KeyFramer;
extern CShapeArray ShapeArray;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMakeSpchSeq::CMakeSpchSeq()
{

}

CMakeSpchSeq::~CMakeSpchSeq()
{

}

void CMakeSpchSeq::MakeSeqAs(char* seqcode)
{
	UINT i;
//	static LPCTSTR codelist="AUOWYIE78901234567890123456789GNDRMBS-JCKTPH456789";
//	static LPCTSTR codelist="kjhnmlpo8901234567890123456789rsefaqtdwczxvg456789";

	char ch[2];
	int s;
	int m;
	int e;
	
	FrameCount=0;

	ChangeCode(seqcode);         /* KS5601 ==> COMBI */

//	KeyFramerRealtime.EraseSequence();
	KeyFramer.EraseSequence();

	for(i=0;i<strlen(seqcode);i++)
	{
		if(seqcode[i]&0x80)
		{
			ch[0]=seqcode[i++];
			ch[1]=seqcode[i];

			s=ch[0]&0x7c;
			s = s >> 2;

			m=ch[0]&0x3;
			m = m << 8;
			m+=ch[1]&0xe0;
			m = m >> 5;

			e=ch[1]&0x1f;

			switch(s)
			{
			case 2:
				Sr();
				break;
			case 3:
				SR();
				break;
			case 4:
				Ss();
				break;
			case 5:
				Se();
				break;
			case 6:
				SE();
				break;
			case 7:
				Sf();
				break;
			case 8:
				Sa();
				break;
			case 9:
				Sq();
				break;
			case 10:
				SQ();
				break;
			case 11:
				St();
				break;
			case 12:
				ST();
				break;
			case 13:
				Sd();
				break;
			case 14:
				Sw();
				break;
			case 15:
				SW();
				break;
			case 16:
				Sc();
				break;
			case 17:
				Sz();
				break;
			case 18:
				Sx();
				break;
			case 19:
				Sv();
				break;
			case 20:
				Sg();
				break;
			default:
				break;
			}
////////////////////////////////////////////////////////////////
			switch(m)
			{
			case 3:
				Mk();
				break;
			case 4:
				Mo();
				break;
			case 5:
				Mi();
				break;
			case 6:
				MO();
				break;
			case 7:
				Mj();
				break;
			case 10:
				Mp();
				break;
			case 11:
				Mu();
				break;
			case 12:
				MP();
				break;
			case 13:
				Mh();
				break;
			case 14:
				Mhk();
				break;
			case 15:
				Mho();
				break;
			case 18:
				Mhl();
				break;
			case 19:
				My();
				break;
			case 20:
				Mn();
				break;
			case 21:
				Mnj();
				break;
			case 22:
				Mnp();
				break;
			case 23:
				Mnl();
				break;
			case 26:
				Mb();
				break;
			case 27:
				Mm();
				break;
			case 28:
				Mml();
				break;
			case 29:
				Ml();
				break;
			default:
				break;
			}
////////////////////////////////////////////////////////////////
			switch(e)
			{
			case 2:
				Er();
				break;
			case 3:
				ER();
				break;
			case 4:
				Ert();
				break;
			case 5:
				Es();
				break;
			case 6:
				Esw();
				break;
			case 7:
				Esg();
				break;
			case 8:
				Ee();
				break;
			case 9:
				Ef();
				break;
			case 10:
				Efr();
				break;
			case 11:
				Efa();
				break;
			case 12:
				Efq();
				break;
			case 13:
				Eft();
				break;
			case 14:
				Efx();
				break;
			case 15:
				Efv();
				break;
			case 16:
				Efg();
				break;
			case 17:
				Ea();
				break;
			case 19:
				Eq();
				break;
			case 20:
				Eqt();
				break;
			case 21:
				Et();
				break;
			case 22:
				ET();
				break;
			case 23:
				Ed();
				break;
			case 24:
				Ew();
				break;
			case 25:
				Ec();
				break;
			case 26:
				Ez();
				break;
			case 27:
				Ex();
				break;
			case 28:
				Ev();
				break;
			case 29:
				Eg();
				break;
			default:
				FrameCount = FrameCount+2;
				break;
			}

/*
			char temp[100];
			sprintf(temp,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %d  %d  %d\n",s,m,e);
			AfxMessageBox(temp);
*/			
/*
			Shape = ShapeArray.GetShape(j);

			Frame.framenumber= i*3;
			Frame.mouth_open = Shape->mouth_open*1.2f;
			Frame.liptail_up = Shape->liptail_up*1.2f;
			Frame.liptail_down = Shape->liptail_down*1.2f;
			Frame.lip_longer = Shape->lip_longer*1.2f;
			Frame.lip_tighten_o = Shape->lip_tighten_o*1.2f;
			Frame.lip_tighten_u = Shape->lip_tighten_u*1.2f;
			Frame.lip_open = Shape->lip_open*1.2f;
			Frame.eyetail_up = Shape->eyetail_up*1.2f;
			Frame.eye_close = Shape->eye_close*1.2f;
			Frame.eyebrow_up = Shape->eyebrow_up*1.2f;
			Frame.eyebrow_glower = Shape->eyebrow_glower*1.2f;

			KeyFramerRealtime.RecordFrameAs(Frame);
*/
		}
		else
			FrameCount = FrameCount + 5;
	}
	
	Frame.framenumber= FrameCount+5;
	Frame.mouth_open = 0.0f;
	Frame.liptail_up = 0.0f;
	Frame.liptail_down = 0.0f;
	Frame.lip_longer = 0.0f;
	Frame.lip_tighten_o = 0.0f;
	Frame.lip_tighten_u = 0.0f;
	Frame.lip_open = 0.0f;
	Frame.eyetail_up = 0.0f;
	Frame.eye_close = 0.0f;
	Frame.eyebrow_up = 0.0f;
	Frame.eyebrow_glower = 0.0f;

//	KeyFramerRealtime.RecordFrameAs(Frame);
	KeyFramer.RecordFrameAs(Frame);
}


bool CMakeSpchSeq::ChangeCode(char * string)
{
	int done=0;
	int count=0;
	int len=strlen(string);
	unsigned char ch[2];

	while(!done)
	{
                ch[0]=string[count++];
                if(count>len)
		{
			done=1;
			continue;
		}
		ch[1]=0;
		if(ch[0]&0x80) //MSB가 한글인가?
                        ch[1]=string[count++];
		if(ch[1])
		{
                        Findh(ch);
                        string[count-2]=ch[0];
                        string[count-1]=ch[1];
		}
		else
                        string[count-1]=ch[0];
	}

	return TRUE;
}

int CMakeSpchSeq::Findh(unsigned char *ch)
{
	 unsigned char* src=table_ks;
    unsigned char* dst=table_kssm;
    int ret=-1;

	{
		if(ch[0]>=ksSpHdr.start &&
			ch[0]<ksSpHdr.start+ksSpHdr.width &&
			ch[1]>=ksSpBody.start &&
			ch[1]<ksSpBody.start+ksSpBody.width)	//특수문자
		{
			int i=ch[0]-ksSpHdr.start;
			int w=ch[1]-ksSpBody.start;

			ch[0]=kssmSpHdr.start+i/2;
			ch[1]=kssmSpBody[i%2].start+w;
			ret=1;
		}
		else if(ch[0]>=ksHanjaHdr.start &&
			ch[0]<ksHanjaHdr.start+ksHanjaHdr.width &&
			ch[1]>=ksSpBody.start &&
			ch[1]<ksSpBody.start+ksSpBody.width)	//한자
		{
			int i=ch[0]-ksHanjaHdr.start;
			int w=ch[1]-ksSpBody.start;

			ch[0]=kssmHanjaHdr.start+i/2;
			ch[1]=kssmSpBody[i%2].start+w;
			ret=1;
		}
	}
/*	
	{
		if(ch[0]>=kssmSpHdr.start &&
			ch[0]<kssmSpHdr.start+kssmSpHdr.width)	//특수문자
		{
			int body=-1;
			if(ch[1]>=kssmSpBody[0].start &&
				ch[1]<kssmSpBody[0].start+kssmSpBody[0].width)
				body=0;
			else if(ch[1]>=kssmSpBody[1].start &&
				ch[1]<kssmSpBody[1].start+kssmSpBody[1].width)
				body=1;
			if(body>-1)
			{
				int i=ch[0]-kssmSpHdr.start;
				int w=ch[1]-kssmSpBody[body].start;

				ch[0]=ksSpHdr.start+i*2+body;
				ch[1]=ksSpBody.start+w;
				ret=1;
			}
		}
		else if(ch[0]>=kssmHanjaHdr.start &&
			ch[0]<kssmHanjaHdr.start+kssmHanjaHdr.width)		//한자
		{
			int body=-1;
			if(ch[1]>=kssmSpBody[0].start &&
				ch[1]<kssmSpBody[0].start+kssmSpBody[0].width)
				body=0;
			else if(ch[1]>=kssmSpBody[1].start &&
				ch[1]<kssmSpBody[1].start+kssmSpBody[1].width)
				body=1;
			if(body>-1)
			{
				int i=ch[0]-kssmHanjaHdr.start;
				int w=ch[1]-kssmSpBody[body].start;

				ch[0]=ksHanjaHdr.start+i*2+body;
				ch[1]=ksSpBody.start+w;
				ret=1;
			}
		}
	}
*/
  for(register int i=0; ret==-1 && src[i]; i+=2)
	{
		if(src[i]==ch[0] && src[i+1]==ch[1])
		{
			ret=i;
			ch[0]=dst[i];
			ch[1]=dst[i+1];
		}
	}
	return ret;
}

void CMakeSpchSeq::Sr()
{
	MakeFrame(30,3);
}
void CMakeSpchSeq::Ss()
{
	MakeFrame(31,3);
}
void CMakeSpchSeq::Se()
{
	MakeFrame(32,3);

}
void CMakeSpchSeq::Sf()
{
	MakeFrame(33,3);
}
void CMakeSpchSeq::Sq()
{
	MakeFrame(34,3);
}
void CMakeSpchSeq::SQ()
{
	MakeFrame(34,3);
}
void CMakeSpchSeq::St()
{
	MakeFrame(35,3);
}
void CMakeSpchSeq::Sd()
{
//	MakeFrame(36,3);
}
void CMakeSpchSeq::Sw()
{
	MakeFrame(38,3);
}
void CMakeSpchSeq::Sc()
{
	MakeFrame(39,3);
}
void CMakeSpchSeq::Sz()
{
	MakeFrame(40,3);
}
void CMakeSpchSeq::Sx()
{
	MakeFrame(41,3);
}
void CMakeSpchSeq::Sv()
{
	MakeFrame(42,3);
}
void CMakeSpchSeq::Sg()
{
	MakeFrame(43,3);
}
void CMakeSpchSeq::SR()
{
	MakeFrame(30,4);
}
void CMakeSpchSeq::SE()
{
	MakeFrame(32,4);
}
void CMakeSpchSeq::ST()
{
	MakeFrame(36,4);
}
void CMakeSpchSeq::SW()
{
	MakeFrame(38,4);
}
void CMakeSpchSeq::Sa()
{
	MakeFrame(34,3);
}

/////////////////////////////////////////////
void CMakeSpchSeq::Er()
{
//	MakeFrame(30,2);
	FrameCount = FrameCount + 2;
}
void CMakeSpchSeq::Es()
{
	MakeFrame(31,2);
}
void CMakeSpchSeq::Ee()
{
	MakeFrame(32,2);
}
void CMakeSpchSeq::Ef()
{
	MakeFrame(33,2);
}
void CMakeSpchSeq::Ea()
{
	MakeFrame(34,2);
}
void CMakeSpchSeq::Eq()
{
	MakeFrame(35,2);
}
void CMakeSpchSeq::Et()
{
	MakeFrame(36,2);
}
void CMakeSpchSeq::Ed()
{
	MakeFrame(32,2);//////////////////////////////////////////
}
void CMakeSpchSeq::Ew()
{
	MakeFrame(38,2);
}
void CMakeSpchSeq::Ec()
{
	MakeFrame(39,2);
}
void CMakeSpchSeq::Ez()
{
	MakeFrame(40,2);
}
void CMakeSpchSeq::Ex()
{
	MakeFrame(41,2);
}
void CMakeSpchSeq::Ev()
{
	MakeFrame(42,2);
}
void CMakeSpchSeq::Eg()
{
	MakeFrame(43,2);
}
void CMakeSpchSeq::ER()
{
//	MakeFrame(30,2);
	FrameCount = FrameCount + 2;
}
void CMakeSpchSeq::Ert()
{
	MakeFrame(30,2);
}
void CMakeSpchSeq::Esw()
{
	MakeFrame(31,2);
}
void CMakeSpchSeq::Esg()
{
	MakeFrame(31,2);
}
void CMakeSpchSeq::Efr()
{
	MakeFrame(33,2);
}
void CMakeSpchSeq::Efa()
{
	MakeFrame(33,2);
}
void CMakeSpchSeq::Efq()
{
	MakeFrame(33,2);
}
void CMakeSpchSeq::Eft()
{
	MakeFrame(33,2);
}
void CMakeSpchSeq::Efx()
{
	MakeFrame(33,2);
}
void CMakeSpchSeq::Efv()
{
	MakeFrame(33,2);
}
void CMakeSpchSeq::Efg()
{
	MakeFrame(33,2);
}
void CMakeSpchSeq::Eqt()
{
	MakeFrame(35,2);
}
void CMakeSpchSeq::ET()
{
	MakeFrame(36,2);
}

///////////////////////////////
void CMakeSpchSeq::Mk()
{
	MakeFrame(0,3);
}
void CMakeSpchSeq::Mo()
{
	MakeFrame(6,3);
}
void CMakeSpchSeq::Mi()
{
	MakeFrame(5,3);
}
void CMakeSpchSeq::MO()
{
	MakeFrame(2,3);
}
void CMakeSpchSeq::Mj()
{
	MakeFrame(1,3);
}
void CMakeSpchSeq::Mp()
{
	MakeFrame(6,3);
}
void CMakeSpchSeq::Mu()
{
	MakeFrame(5,2);
	MakeFrame(1,2);
}
void CMakeSpchSeq::MP()
{
	MakeFrame(5,2);
	MakeFrame(6,2);
}
void CMakeSpchSeq::Mh()
{
	MakeFrame(2,3);
}
void CMakeSpchSeq::Mhk()
{
	MakeFrame(2,2);
	MakeFrame(0,2);
}
void CMakeSpchSeq::Mho()
{
	MakeFrame(2,2);
	MakeFrame(6,2);
}
void CMakeSpchSeq::Mhl()
{
	MakeFrame(2,2);
	MakeFrame(5,2);
}
void CMakeSpchSeq::My()
{
	MakeFrame(5,2);
	MakeFrame(2,2);
}
void CMakeSpchSeq::Mn()
{
	MakeFrame(3,3);
}
void CMakeSpchSeq::Mnj()
{
	MakeFrame(3,2);
	MakeFrame(1,2);
}
void CMakeSpchSeq::Mnp()
{
	MakeFrame(3,2);
	MakeFrame(6,2);
}
void CMakeSpchSeq::Mnl()
{
	MakeFrame(3,2);
	MakeFrame(5,2);
}
void CMakeSpchSeq::Mb()
{
	MakeFrame(5,2);
	MakeFrame(3,2);
}
void CMakeSpchSeq::Mm()
{
	MakeFrame(4,3);
}
void CMakeSpchSeq::Mml()
{
	MakeFrame(4,2);
	MakeFrame(5,2);
}
void CMakeSpchSeq::Ml()
{
	MakeFrame(5,3);
}

void CMakeSpchSeq::MakeFrame(int index, int count)
{
	Shape = ShapeArray.GetShape(index);

	FrameCount = FrameCount + count;

	Frame.framenumber= FrameCount;
	Frame.mouth_open = Shape->mouth_open*1.2f;
	Frame.liptail_up = Shape->liptail_up*1.2f;
	Frame.liptail_down = Shape->liptail_down*1.2f;
	Frame.lip_longer = Shape->lip_longer*1.2f;
	Frame.lip_tighten_o = Shape->lip_tighten_o*1.2f;
	Frame.lip_tighten_u = Shape->lip_tighten_u*1.2f;
	Frame.lip_open = Shape->lip_open*1.2f;
	Frame.eyetail_up = Shape->eyetail_up*1.2f;
	Frame.eye_close = Shape->eye_close*1.2f;
	Frame.eyebrow_up = Shape->eyebrow_up*1.2f;
	Frame.eyebrow_glower = Shape->eyebrow_glower*1.2f;

//	KeyFramerRealtime.RecordFrameAs(Frame);
	KeyFramer.RecordFrameAs(Frame);
}
