#if !defined(AFX_COPENGLVIEW_H__90638734_FB68_11D1_BDB6_00A0C98A1BF2__INCLUDED_)
#define AFX_OPENGLVIEW_H__90638734_FB68_11D1_BDB6_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OpenGLView.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// COpenGLView window

#include "hud.h"

class COpenGLView : public CWnd
{
// Construction
public:
	COpenGLView();

// Attributes
public:

	CHUD *hud;
	float m_spinx;
	float m_spiny;

protected:
	HDC hDC;
	HGLRC hRC;

	void Init_OpenGL(void);
	void SetDCPixelFormat(HDC hDC);

	bool m_lbutton;
	POINT m_prevpoint;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void LoadNewMap();
	void SetUpFaceMap();
	int height;
	int width;
	virtual ~COpenGLView();

	// Generated message map functions
protected:
	//{{AFX_MSG(COpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPENGLVIEW_H__90638734_FB68_11D1_BDB6_00A0C98A1BF2__INCLUDED_)
