#if !defined(AFX_COPENGLWND_H__90638734_FB68_11D1_BDB6_00A0C98A1BF3__INCLUDED_)
#define AFX_COPENGLWND_H__90638734_FB68_11D1_BDB6_00A0C98A1BF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OpenGLWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd window


#include "HUD.h"

class COpenGLWnd : public CWnd
{
public:
	COpenGLWnd();
	void OnChangeSize(int cx, int cy);
	int height;
	int width;
	virtual ~COpenGLWnd();

	CHUD *hud;

protected:
	HDC hDC;
	HGLRC hRC;

	void InitOpenGL();
	void InitProjection();
	void InitShadingModel();
	void InitAlphaTest();
	void InitTexture();
	void InitLight();
	void SetDCPixelFormat(HDC hDC);
	void DrawScene();

	void *LoadBMPfile(char *filename, BITMAPINFO **info);

	BITMAPINFO *info;
	void *bits;

	float m_spinx;
	float m_spiny;
	bool m_lbutton;
	POINT m_prevpoint;
//	GLUquadricObj *Obj;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGLWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(COpenGLWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPENGLWND_H__90638734_FB68_11D1_BDB6_00A0C98A1BF3__INCLUDED_)
