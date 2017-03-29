// AvatarDoc.cpp : implementation of the CAvatarDoc class
//

#include "stdafx.h"
#include "PDef.h"
#include "Avatar.h"

#include "AvatarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAvatarDoc

IMPLEMENT_DYNCREATE(CAvatarDoc, CDocument)

BEGIN_MESSAGE_MAP(CAvatarDoc, CDocument)
	//{{AFX_MSG_MAP(CAvatarDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAvatarDoc construction/destruction

CAvatarDoc::CAvatarDoc()
{
	// TODO: add one-time construction code here

}

CAvatarDoc::~CAvatarDoc()
{
}

BOOL CAvatarDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAvatarDoc serialization

void CAvatarDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAvatarDoc diagnostics

#ifdef _DEBUG
void CAvatarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAvatarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAvatarDoc commands
