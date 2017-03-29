// ShapeArray.h: interface for the CShapeArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAPEARRAY_H__E6984645_093F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
#define AFX_SHAPEARRAY_H__E6984645_093F_11D2_BDC3_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CShapeArray  
{
public:
	SliderStatus* GetShape(int index);
	CShapeArray();
	virtual ~CShapeArray();

protected:
	void LoadShapes();
	SliderStatus Shapes[50];


};

#endif // !defined(AFX_SHAPEARRAY_H__E6984645_093F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
