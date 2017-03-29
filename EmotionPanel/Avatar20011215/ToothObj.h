// ToothObj.h: interface for the CToothObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOTHOBJ_H__77379F44_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
#define AFX_TOOTHOBJ_H__77379F44_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MorphObj.h"

class CToothObj : public CMorphObj  
{
public:
	int ToothList;
	void Draw();
	CToothObj( CObList *l1, CObList *fl1, CObList *ml);
	virtual ~CToothObj();

protected:
	CMyDib *ToothImage;
};

#endif // !defined(AFX_TOOTHOBJ_H__77379F44_085F_11D2_BDC3_00A0C98A1BF2__INCLUDED_)
