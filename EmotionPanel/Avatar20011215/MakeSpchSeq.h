// MakeSpchSeq.h: interface for the CMakeSpchSeq class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAKESPCHSEQ_H__E6984647_093F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
#define AFX_MAKESPCHSEQ_H__E6984647_093F_11D2_BDC3_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMakeSpchSeq  
{
public:
	void MakeSeqAs(char* seqcode);
	CMakeSpchSeq();
	virtual ~CMakeSpchSeq();

protected:
	void MakeFrame(int index, int count);
	int FrameCount;
	bool ChangeCode(char* string);
	int Findh(unsigned char *ch);
	SliderStatus* Shape;
	CFrameData Frame;

void Sr();
void Ss();
void Se();
void Sf();
void Sq();
void St();
void Sd();
void Sw();
void Sc();
void Sz();
void Sx();
void Sv();
void Sg();
void SR();
void SE();
void ST();
void SW();
void Sa();
void SQ();

/////////////////////////////////////////////
void Ea();
void Er();
void Es();
void Ee();
void Ef();
void Eq();
void Et();
void Ed();
void Ew();
void Ec();
void Ez();
void Ex();
void Ev();
void Eg();
void ER();
void Ert();
void Esw();
void Esg();
void Efr();
void Efa();
void Efq();
void Eft();
void Efx();
void Efv();
void Efg();
void Eqt();
void ET();

///////////////////////////////
void Mk();
void Mo();
void Mi();
void MO();
void Mj();
void Mp();
void Mu();
void MP();
void Mh();
void Mhk();
void Mho();
void Mhl();
void My();
void Mn();
void Mnj();
void Mnp();
void Mnl();
void Mb();
void Mm();
void Mml();
void Ml();

};

#endif // !defined(AFX_MAKESPCHSEQ_H__E6984647_093F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
