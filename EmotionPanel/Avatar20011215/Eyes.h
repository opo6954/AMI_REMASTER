// Eyes.h: interface for the CEyes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EYES_H__08050755_0267_11D2_AF36_00A0C98A1BF2__INCLUDED_)
#define AFX_EYES_H__08050755_0267_11D2_AF36_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Eye.h"

class CEyes : public CEye  
{
public:
	void SetEyeScale(float scale);
	void SetEyeDistance(float distance);
	void Draw0();
	void Draw();
	void SetEyesOri(float hori, float vert);
	CEyes();
	virtual ~CEyes();
	float VertDegree;
	float HoriDegree;

protected:
	CMyDib *EyeImage;
	float EyeScale;
	float EyeDistance;
	CEye EyeRight;
	CEye EyeLeft;
};

#endif // !defined(AFX_EYES_H__08050755_0267_11D2_AF36_00A0C98A1BF2__INCLUDED_)
