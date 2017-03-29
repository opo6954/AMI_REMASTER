// Heart.h: interface for the CHeart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEART_H__627BA85B_B159_4AC4_99EA_B66CBD8CBCD6__INCLUDED_)
#define AFX_HEART_H__627BA85B_B159_4AC4_99EA_B66CBD8CBCD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHeart  
{
public:
	void Draw();
	void DrawBrokenHeart(float weight);
	CHeart();
	virtual ~CHeart();

	void Init();


	float vertex[162][3];
	float vertexnormal[162][3];
	int triangle[320][6];

	float brokenheart[162][3];

};

#endif // !defined(AFX_HEART_H__627BA85B_B159_4AC4_99EA_B66CBD8CBCD6__INCLUDED_)
