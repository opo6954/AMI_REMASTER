//
//  2000.5.25
//  daniel park
//
// HUD.h: interface for the CHUD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUD_H__2956FADF_B752_46CA_882F_2FFDD23D1645__INCLUDED_)
#define AFX_HUD_H__2956FADF_B752_46CA_882F_2FFDD23D1645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Cimage/cimage.h"

class CHUD  
{
public:
	void init();
	CHUD();
	virtual ~CHUD();

	void DrawIcosahedron();
	void DrawIcosahedronWire();

	void ProcessHeart();

	void SetState(int a)
	{
		state = a;
	}
	int GetState()
	{
		return state;
	}
	void SetSJ(float x)
	{
		sorrow_joy = x;
	}
	void SetA(float x)
	{
		anger = x;
	}
	float GetSJ()
	{
		return sorrow_joy;
	}
	float GetA()
	{
		return anger;
	}
	void IncSJ()
	{
		sorrow_joy+=0.05f;
		if(sorrow_joy>1.0f)
			sorrow_joy = 1.0f;
	}
	void DecSJ()
	{
		sorrow_joy-=0.05f;
		if(sorrow_joy<0.0f)
			sorrow_joy = 0.0f;
	}
	void IncA()
	{
		anger+=0.05f;
		if(anger>1.0f)
			anger = 1.0f;
	}
	void DecA()
	{
		anger-=0.05f;
		if(anger<0.0f)
			anger = 0.0f;
	}
	void DrawMap();
	void DrawSpeed();
	void DrawGear();
	void DrawRPM();
	void DrawAcceleration(float ax, float ay);
	void DrawPath();

	void DrawHeart();

	void DrawTitle();

	void Draw();
	void DrawSJA();

	int HeartBeatRate;
	float theta;
	float scalex;
	float scaley;
	float transx;
	float transy;

	float map_alpha;

protected:
	CImage *mapimage;
	CImage *numberimage;
	CImage *kmphimage;
	CImage *graphimage;

	CImage *titleimage;
	CImage *winimage;
	CImage *lostimage;

	unsigned int texnames[7];

	int state;   // 0:no  1:title  2:win  3:lost

	float sorrow_joy;
	float anger;

	float xx;
	float yy;
	float zz;

	int HeartBeatCount;
	float HeartSize;

};

#endif // !defined(AFX_HUD_H__2956FADF_B752_46CA_882F_2FFDD23D1645__INCLUDED_)
