// Eye.h: interface for the CEye class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EYE_H__08050752_0267_11D2_AF36_00A0C98A1BF2__INCLUDED_)
#define AFX_EYE_H__08050752_0267_11D2_AF36_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Parser.h"

class CEye : public CParser  
{
public:
	void SetEyeOri(float hori, float vert);
	int DrawPolygonEye();
	int DrawEye();
	float HorizontalDegree;
	float VerticalDegree;
	CEye();
	virtual ~CEye();

protected:

	int EyeList;
//	CObList InitialVertexList;
//	CVertexNode *InitialVertexNode;


};

#endif // !defined(AFX_EYE_H__08050752_0267_11D2_AF36_00A0C98A1BF2__INCLUDED_)
