// TongueObj.h: interface for the CTongueObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TONGUEOBJ_H__77379F46_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
#define AFX_TONGUEOBJ_H__77379F46_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MorphObj.h"

class CTongueObj : public CMorphObj  
{
public:
	int TongueList;
	void Draw();
	CTongueObj( CObList *l1, CObList *fl1, CObList *ml);
	virtual ~CTongueObj();

protected:
	CMyDib *TongueImage;

};

#endif // !defined(AFX_TONGUEOBJ_H__77379F46_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
