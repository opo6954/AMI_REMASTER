// OpenGLView.cpp : implementation file
//

#include "stdafx.h"
#include "Avatar.h"
#include "MainFrm.h"
#include "pDef.h"
#include "MyDIB.h"
#include "Eyes.h"
#include "OpenGLView.h"
#include "ExpSliders.h"

#include "MorphObj.h"

#include "Dibitmap.h"

#include "gl/gl.h"
#include "gl/glu.h"

#include "Face.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CExpSliders ExpSliders;
extern CMorphObj *Obj;
//extern CDIBitmap *m_pBitmap;
extern CFace Face;
extern CEyes Eyes;

extern COpenGLView m_wndViewPort;

/////////////////////////////////////////////////////////////////////////////
// COpenGLView

COpenGLView::COpenGLView()
{
	m_spinx=0;
	m_spiny=0;

	m_lbutton=FALSE;

	hud = 0;
}

COpenGLView::~COpenGLView()
{
	if(hud)
		delete hud;
}


BEGIN_MESSAGE_MAP(COpenGLView, CWnd)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	m_pBitmap = new CDIBitmap;
//	m_pBitmap->parse("facedata\\facebase.cnp");

	hDC = ::GetDC(m_hWnd);
	SetDCPixelFormat(hDC);		
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	Init_OpenGL();

	Eyes.SetEyeDistance(2.333064f);
	Eyes.SetEyeScale(1.0f);


	char buf[295*343*4];

	FILE *fp;

	fp = fopen("facedata/facebase20010429e.raw","rb");
	fread(buf,295*343*4,1,fp);

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 295, 343,
		      GL_RGBA, GL_UNSIGNED_BYTE, (const void * )buf);
/*	char temp[100];

	sprintf(temp,"%x   %d %d\n",m_pBitmap->bFindDib(),m_pBitmap->GetXSize(),m_pBitmap->GetYSize());

	AfxMessageBox(temp);
*/
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	fclose(fp);


	hud = new CHUD();

	return 0;
}

void COpenGLView::OnDestroy() 
{
	CWnd::OnDestroy();

//	delete m_pBitmap;
	
	wglMakeCurrent(hDC,NULL);
	wglDeleteContext(hRC);
	::ReleaseDC( m_hWnd, hDC );
	
}

void COpenGLView::OnPaint() 
{
//	static float rspinx=0.0f;
//	static float rspiny=0.0f;
//	CPaintDC dc(this); // device context for painting
	
//	float dspinx;
//	float dspiny;
/*
	do
	{
		dspinx=((float)rand()/RAND_MAX-0.5f)/5.0f;
	}
	while( rspinx+dspinx < -5.0 || rspinx+dspinx > 5.0 );
	rspinx+=dspinx;
	
	do
	{
		dspiny=((float)rand()/RAND_MAX-0.5f)/5.0f;
	}
	while( rspiny+dspiny < -10.0 || rspiny+dspiny > 10.0 );
	rspiny+=dspiny;
*/
	glClearColor(1.0,0.8,0.76,1.0);
	static float th = 0.0f;

	th+=0.01f;
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective( 45.0f, 1.0f, 3.0f, 20.0f );
//	glOrtho( 0.0f, 1000.0f,  0.0f, 1000.0f,0.1f, 2000.0f );
//	gluLookAt( 0.0f, 0.0f, 1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
	gluLookAt( 0.0f, 0.0f, 12.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	hud->Draw();
	glDisable(GL_BLEND);

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glScalef(1.3f,1.3f,1.3f);

//	glRotatef(m_spinx+rspinx, 0.0f, 1.0f, 0.0f);
//	glRotatef(m_spiny+rspiny, 1.0f, 0.0f, 0.0f);
//	glRotatef(20.0, 0.0f, 1.0f, 0.0f);
//	glRotatef(5.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.1f*sin(th),0.1f*cos(th*0.745f)-0.85f+m_wndViewPort.hud->sorrow_joy*1.3f,0.0f);
//	glTranslatef(0.5f,0.5f,0.0f);
	glRotatef(cos(th*0.352f)*3.5f,0.0f,0.0f,1.0f);
	glScalef(1.0f+0.1f*cos(th*0.32),1.0f+0.1f*cos(th*0.81),1.0f);

	if(m_wndViewPort.hud->state==1 && m_wndViewPort.hud->state2)
		Face.Draw(m_spinx,m_spiny);

	glPopMatrix();

//	glPushAttrib(GL_LIGHTING_BIT);
//	glDisable(GL_LIGHTING);
/*
	glPushMatrix();

	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(-10.0f,-2.5f,-10.0f);
		glVertex3f(10.0f,-2.5f,-10.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(10.0f,-3.0f,5.0f);
		glVertex3f(-10.0f,-3.0f,5.0f);
	glEnd();
	glShadeModel(GL_FLAT);

	glPopMatrix();
*/
/*
	glPopAttrib();

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
*/
	/*
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,width,0.0f,height,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glColor3f(0.2f,0.2f,0.2f);
	glRasterPos2i(11,9);
	PrintString(GLUT_BITMAP_HELVETICA_18,"djfrnf v0.01 (djf Module) cnp");
	glColor3f(0.5f,0.5f,0.5f);
	glRasterPos2i(10,10);
	PrintString(GLUT_BITMAP_HELVETICA_18,"djfrnf v0.01 (djf Module) cnp");
	glColor3f(1.0f,1.0f,0.0f);

	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
*/
//	glPopAttrib();


	glFlush();

	SwapBuffers(hDC);

//	m_pBitmap->PaintImage(0,0,hDC);

	ValidateRect(NULL);
}

void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_lbutton = TRUE;

	m_prevpoint.x=point.x;
	m_prevpoint.y=point.y;
	
	CWnd::OnLButtonDown(nFlags, point);
}

void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_lbutton = FALSE;
	
	CWnd::OnLButtonUp(nFlags, point);
}

void COpenGLView::OnMouseMove(UINT nFlags, CPoint point) 
{
/*
	if(m_lbutton)
	{
		m_spinx += ((float)point.x-(float)m_prevpoint.x)/2.0f;
		m_spiny += ((float)point.y-(float)m_prevpoint.y)/7.0f;

		if(m_spiny > 15)
			m_spiny=15;
		if(m_spiny < -15)
			m_spiny=-15;
		if(m_spinx > 54)
			m_spinx=54;
		if(m_spinx < -54)
			m_spinx=-54;

		m_prevpoint.x = point.x;
		m_prevpoint.y = point.y;
//		InvalidateRect(NULL, FALSE);
	}
*/
//	CWnd::OnMouseMove(nFlags, point);

//	Eyes.SetEyesOri((point.x-width/2)/10.0f, (point.y-height/2)/10.0f);

	SetCursor(NULL);
	
}

void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
//	CWnd::OnSize(nType, cx, cy);

	width=cx;
	height=cy;
	
	if(cx && cy)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective( 45.0f, (float)cx/(float)cy, 0.1f, 20.0f );
		gluLookAt( 0.0f, 0.0f, 12.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glViewport(0, 0, cx, cy);
	}
	
}

BOOL COpenGLView::OnEraseBkgnd(CDC* pDC) 
{
	return 1;
	
//	return CWnd::OnEraseBkgnd(pDC);
}

void COpenGLView::SetDCPixelFormat(HDC hDC)
{
    int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,					// Version of this structure	
		PFD_DRAW_TO_WINDOW |		// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |		// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,			// Double buffered mode
		PFD_TYPE_RGBA,			// RGBA Color mode
		32,					// Want 16 bit color 
		0,0,0,0,0,0,				// Not used to select mode
		0,0,					// Not used to select mode
		0,0,0,0,0,			// Not used to select mode
		24,					// Size of depth buffer
		0,					// Not used to select mode
		PFD_MAIN_PLANE,			// Draw in main plane
		0,					// Not used to select mode
		0,					// Not used to select mode
		0,0,0 };					// Not used to select mode

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

void COpenGLView::Init_OpenGL(void)
{

	GLfloat lightposition[4] = {10.0f, 10.0f, 10.0f, 1.0f};
	GLfloat globalambient[4] = {0.3f, 0.3f, 0.3f, 1.0f};          // 광원의 설정을 위한 Array
	GLfloat lightcolor[4] = {0.9f, 0.9f, 0.9f, 1.0f};

	GLfloat matambient[4] = {0.1f, 0.1f, 0.1f, 0.6f};             //  물체의 재질 설정을 위한 Array
	GLfloat matdiffuse[4] = {0.6f, 0.8f, 0.6f, 0.6f};
	GLfloat matspecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLbyte shine = 40;

	glEnable(GL_LIGHTING);                                        // 광원효과 이용
	glEnable(GL_LIGHT0);                                          // 광원0 ON

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);             // 광원의 위치설정
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightcolor);     // 광원의 색 설정
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalambient);        // ambient 광원을 설정


	glMaterialfv(GL_FRONT, GL_AMBIENT, matambient);               // 물체의 Ambient color값을 설정
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdiffuse);               // 물체의 diffuse color값을 설정
	glMaterialfv(GL_FRONT, GL_SPECULAR, matspecular);             // 물체의 spectular color값을 설정
	glMaterialf(GL_FRONT, GL_SHININESS, shine);                   // 물체의 shininess값을 설정
	
/*	
	//	GLfloat globalambient[4] = {0.3f, 0.3f, 0.3f, 0.3f};
	GLfloat lightcolor[4] = {0.9f, 0.9f, 0.9f, 1.0f};
//	GLfloat matambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
//	GLfloat matdiffuse[4] = {0.8f, 0.4f, 0.8f, 1.0f};
//	GLfloat matspecular[4] = {0.9f, 0.9f, 0.9f, 1.0f};
	GLbyte shine = 64;
	GLfloat lightposition[4] = {15.0f, 40.0f, 40.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightcolor);
*/	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
//	glCullFace(GL_FRONT);
	glShadeModel(GL_SMOOTH);    

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
/*
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, TRUE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matspecular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
*/
//	glLineWidth(1.1f);
//	glPointSize(2.0f);

//	glDisable(GL_LINE_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective( 45.0f, 1.0f, 3.0f, 20.0f );
//	glOrtho( 0.0f, 1000.0f,  0.0f, 1000.0f,0.1f, 2000.0f );
//	gluLookAt( 0.0f, 0.0f, 1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
	gluLookAt( 0.0f, 0.0f, 12.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

//	SetUpFaceMap();

//	auxSolidSphere(5.0f);
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style|=CS_OWNDC;
	
	return CWnd::PreCreateWindow(cs);
}

void COpenGLView::SetUpFaceMap()
{

}

void COpenGLView::LoadNewMap()
{
/*
	char TempPath[MAX_PATH];

	GetTempPath(MAX_PATH,TempPath);		//  window의 temp path를 받아옴.
	strcat(TempPath,"facemap.CNP");		// 얻어온 path이름 뒤에 "facebase.cnp"를 추가

//	delete m_pBitmap;

	m_pBitmap = new CDIBitmap;
	m_pBitmap->parse( TempPath );

	m_wndViewPort.SetUpFaceMap();
*/
}

void COpenGLView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	m_wndViewPort.LoadNewMap();
	
	CWnd::OnRButtonDblClk(nFlags, point);
}
