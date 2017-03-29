// AvatarDoc.h : interface of the CAvatarDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVATARDOC_H__A80E782C_F30D_11D1_9A10_444553540000__INCLUDED_)
#define AFX_AVATARDOC_H__A80E782C_F30D_11D1_9A10_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CAvatarDoc : public CDocument
{
protected: // create from serialization only
	CAvatarDoc();
	DECLARE_DYNCREATE(CAvatarDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAvatarDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAvatarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAvatarDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVATARDOC_H__A80E782C_F30D_11D1_9A10_444553540000__INCLUDED_)
