// MorphObj.h: interface for the CMorphObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MORPHOBJ_H__A80E7836_F30D_11D1_9A10_444553540000__INCLUDED_)
#define AFX_MORPHOBJ_H__A80E7836_F30D_11D1_9A10_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMorphObj  
{
public:
	void ResetInitialVertexList();
	void CalculateMorph( CObList *VectorList, float influence );
	void DrawObj(int degree);
	void DrawObj(float degree);
	void DrawInitialObj();
	void DrawInitialFaceObj();
	void DrawInitialFaceObjLine();
	void DrawInitialFaceObjSmooth();
	void DrawInitialFaceObjTexture();

	CMorphObj();
	CMorphObj( CObList *l1, CObList *fl1);
	CMorphObj( CObList *l1, CObList *fl1, CObList *Ml);
	virtual ~CMorphObj();

protected:
	CObList *InitialFace;

	int VertexCount;
	bool FaceFlag;

	CObList InitialVertexList;
	CObList *MapVertexList;
	CInitialVertexNode *InitialVertexNode;

};

#endif // !defined(AFX_MORPHOBJ_H__A80E7836_F30D_11D1_9A10_444553540000__INCLUDED_)
