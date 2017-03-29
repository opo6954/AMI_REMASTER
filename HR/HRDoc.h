// HRDoc.h : interface of the CHRDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HRDOC_H__C15047CB_67C1_11D4_9BEE_00010263B61C__INCLUDED_)
#define AFX_HRDOC_H__C15047CB_67C1_11D4_9BEE_00010263B61C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "matrix.h"

class CHRDoc : public CDocument
{
protected: // create from serialization only
	CHRDoc();
	DECLARE_DYNCREATE(CHRDoc)

// Attributes
public:
//by Joyfully
	Matrix4 T[4];
	Matrix4 R[5];
	float r[5];
	float Oldr[5];

	Matrix4 Tr[4];
	Matrix4 Rr[5];
	float rr[5];
	float Oldrr[5];

	Posn3d object;
	float p[5];
//===================

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHRDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHRDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HRDOC_H__C15047CB_67C1_11D4_9BEE_00010263B61C__INCLUDED_)
