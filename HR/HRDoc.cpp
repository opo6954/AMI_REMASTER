// HRDoc.cpp : implementation of the CHRDoc class
//

#include "stdafx.h"
#include "HR.h"

#include "HRDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHRDoc

IMPLEMENT_DYNCREATE(CHRDoc, CDocument)

BEGIN_MESSAGE_MAP(CHRDoc, CDocument)
	//{{AFX_MSG_MAP(CHRDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHRDoc construction/destruction

CHRDoc::CHRDoc()
{
	// TODO: add one-time construction code here
//by Joyfully
	p[0] = 25;
	p[1] = 40;
	p[2] = 2.5;
	p[3] = 31;
	p[4] = 40;

	T[0].FromPosition(p[0],p[1],0);
	T[1].FromPosition(p[2],0,0);
	T[2].FromPosition(0,-p[3],0);
	T[3].FromPosition(0,-p[4],0);
	for(int i=0;i<5;i++){
		Oldr[i]=0;
		r[i]=0;
	}

	Tr[0].FromPosition(-p[0],p[1],0);
	Tr[1].FromPosition(-p[2],0,0);
	Tr[2].FromPosition(0,-p[3],0);
	Tr[3].FromPosition(0,-p[4],0);
	for(int i=0;i<5;i++){
		rr[i]=0;
		Oldrr[i]=0;
	}

	object[0] = 0;
	object[1] = 4;
	object[2] = 45;
//================

}

CHRDoc::~CHRDoc()
{
}

BOOL CHRDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle("Human Robot HostProgram!");
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHRDoc serialization

void CHRDoc::Serialize(CArchive& ar)
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
// CHRDoc diagnostics

#ifdef _DEBUG
void CHRDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHRDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHRDoc commands
