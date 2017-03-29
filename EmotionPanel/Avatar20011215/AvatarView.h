// AvatarView.h : interface of the CAvatarView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVATARVIEW_H__A80E782E_F30D_11D1_9A10_444553540000__INCLUDED_)
#define AFX_AVATARVIEW_H__A80E782E_F30D_11D1_9A10_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAvatarView : public CView
{
protected: // create from serialization only
	CAvatarView();
	DECLARE_DYNCREATE(CAvatarView)

// Attributes
public:
	CAvatarDoc* GetDocument();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAvatarView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void RefreshExp(SliderStatus *status);
	void CleanHeap();
	void MakeSampleData();
	virtual ~CAvatarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	int m_SliderPos;

//	CObList* Initial;
//	CObList* Final;

	CVertexNode* VertexNode;

	POINT m_WinSize;
	bool m_fLoad;

// Generated message map functions
protected:
	//{{AFX_MSG(CAvatarView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AvatarView.cpp
inline CAvatarDoc* CAvatarView::GetDocument()
   { return (CAvatarDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVATARVIEW_H__A80E782E_F30D_11D1_9A10_444553540000__INCLUDED_)
