// Face.h: interface for the CFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACE_H__77379F43_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
#define AFX_FACE_H__77379F43_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFace  
{
public:
	bool IsInitializing();
	void MakeInitializingSequence();
	int MotionDelayCount;
	void PlaySequence();
	void MakeSequence();
//	void Draw();
	void Draw(float x, float y);
	void OnTimer();
	void FaceCalculate();
	CFace();
	virtual ~CFace();

protected:
	bool InitFlag;
};

#endif // !defined(AFX_FACE_H__77379F43_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
