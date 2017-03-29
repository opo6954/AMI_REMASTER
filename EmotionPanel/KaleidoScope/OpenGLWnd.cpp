// OpenGLWnd.cpp : implementation file
//

#include "stdafx.h"
#include "EmotionPanel.h"
#include "OpenGLWnd.h"
#include "MainFrm.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd

COpenGLWnd::COpenGLWnd()
{
	m_spinx=0;
	m_spiny=0;

	m_lbutton=FALSE;
}

COpenGLWnd::~COpenGLWnd()
{
	delete hud;
}


BEGIN_MESSAGE_MAP(COpenGLWnd, CWnd)
	//{{AFX_MSG_MAP(COpenGLWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenGLWnd message handlers

int COpenGLWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	hDC = ::GetDC(m_hWnd);                                    // OpenGL rendering context�� �����ϰ� �ʱ�ȭ��
	SetDCPixelFormat(hDC);		
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	InitOpenGL();

	hud = new CHUD;

	return 0;
}

void COpenGLWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	wglMakeCurrent(hDC,NULL);                                 // OpenGL rendering context�� �����ϰ� window�� �ı���
	wglDeleteContext(hRC);
	::ReleaseDC( m_hWnd, hDC );
	
}

void COpenGLWnd::OnPaint() 
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // ������ scene�� �׸��� ���Ͽ� color buffer��
	                                                       // depth buffer�� clear�Ѵ�.
	glMatrixMode(GL_PROJECTION);                                  // projection matrix�� �ʱ�ȭ
	glLoadIdentity();

	InitProjection();

	glMatrixMode(GL_MODELVIEW);                                   // modelview matrix�� �ʱ�ȭ
	glLoadIdentity();

	glColor4f(0.6f,0.8f,0.6f,0.6f);


	DrawScene();


	glFlush();			// ���� API�� ���Ͽ� ������ instruction���� buffer
	SwapBuffers(hDC);	// �κ��� flush��Ų��. ���� page�� swap���Ѽ� ȭ����
	                    // �����Ѵ�.
	ValidateRect(NULL);
}

void COpenGLWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{

	m_lbutton = TRUE;                         // ���콺�� ���� ��ư�� ������ ��� flag�� set�ϰ�
                                              // ������ mouse pointer��ġ�� ������ѵд�.
	m_prevpoint.x=point.x;
	m_prevpoint.y=point.y;

}

void COpenGLWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_lbutton = FALSE;                        // ���콺�� ���ʹ�ư�� �׸� ���� ��� flag�� reset�Ѵ�
}

void COpenGLWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_lbutton)							  // ���콺�� ������ ��� ������ ������ �ξ��� mouse pointer��
		                                      // ��ġ�� ������ ��ġ�� ���Ͽ� x��� y���� ȸ������ ����Ѵ�.
											  // �� ���� OnPaint���� object�� ���� retation������ �̿�ȴ�.
	{
		m_spinx += (float)point.x-(float)m_prevpoint.x;
		m_spiny += (float)point.y-(float)m_prevpoint.y;

		m_prevpoint.x = point.x;
		m_prevpoint.y = point.y;
	
		InvalidateRect(NULL, FALSE);
	}
}

void COpenGLWnd::OnSize(UINT nType, int cx, int cy) 
{
	width=cx;
	height=cy;
	
	if(cx && cy)                            // ���� �������� ���γ� ���� ���̰� 0 �� �ƴ϶��
	{                                       // �׿� ���� OpenGL�� ViewPort�� �������Ѵ�.
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		InitProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glViewport(0, 0, cx, cy);
	}
}

BOOL COpenGLWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;                      // ���ȭ���� ������ �ʴ´�.
}

void COpenGLWnd::SetDCPixelFormat(HDC hDC)
{
    int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,					// Version of this structure	
		PFD_DRAW_TO_WINDOW |		// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |		// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,			// Double buffered mode
		PFD_TYPE_RGBA,			// RGBA Color mode
		16,					// Want 16 bit color 
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

void COpenGLWnd::InitOpenGL(void)
{
	glEnable(GL_DEPTH_TEST);                                      // depth buffering(z-buffering) ����

	glEnable(GL_NORMALIZE);                                       // ������ ���Ե� vector���� �ڵ����� unit vector�� ��ȯ��
	glEnable(GL_COLOR_MATERIAL);

//	Obj=gluNewQuadric();

	InitLight();
	InitTexture();
	InitAlphaTest();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	InitShadingModel();

	glMatrixMode(GL_PROJECTION);                                  // projection matrix�� �ʱ�ȭ
	glLoadIdentity();

	InitProjection();

	glMatrixMode(GL_MODELVIEW);                                   // modelview matrix�� �ʱ�ȭ
	glLoadIdentity();
}

BOOL COpenGLWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style|=CS_OWNDC;
	
	return CWnd::PreCreateWindow(cs);
}

void COpenGLWnd::OnChangeSize(int cx, int cy)
{
	width=cx;
	height=cy;

	SetWindowPos( &wndTop, 0,0,cx,cy,SWP_NOMOVE);
}


void COpenGLWnd::DrawScene()
{
	glPushMatrix();

//		glRotatef(m_spinx, 0.0f, 1.0f, 0.0f);
//		glRotatef(m_spiny-90.0f, 1.0f, 0.0f, 0.0f);

	hud->Draw();
			
	glPopMatrix();
}


void COpenGLWnd::InitProjection()
{
	gluPerspective( 45.0f, (float)width/height, 2.0f, 100.0f );
	gluLookAt( 2.0f, 4.0f, 17.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
}


void COpenGLWnd::InitLight()
{
	GLfloat lightposition[4] = {10.0f, 10.0f, 10.0f, 1.0f};
	GLfloat globalambient[4] = {0.3f, 0.3f, 0.3f, 1.0f};          // ������ ������ ���� Array
	GLfloat lightcolor[4] = {0.9f, 0.9f, 0.9f, 1.0f};

	GLfloat matambient[4] = {0.1f, 0.1f, 0.1f, 0.6f};             //  ��ü�� ���� ������ ���� Array
	GLfloat matdiffuse[4] = {0.6f, 0.8f, 0.6f, 0.6f};
	GLfloat matspecular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLbyte shine = 40;

	glEnable(GL_LIGHTING);                                        // ����ȿ�� �̿�
	glEnable(GL_LIGHT0);                                          // ����0 ON

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);             // ������ ��ġ����
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightcolor);     // ������ �� ����
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalambient);        // ambient ������ ����


	glMaterialfv(GL_FRONT, GL_AMBIENT, matambient);               // ��ü�� Ambient color���� ����
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdiffuse);               // ��ü�� diffuse color���� ����
	glMaterialfv(GL_FRONT, GL_SPECULAR, matspecular);             // ��ü�� spectular color���� ����
	glMaterialf(GL_FRONT, GL_SHININESS, shine);                   // ��ü�� shininess���� ����
}

void COpenGLWnd::InitTexture()
{
/*
	gluQuadricTexture(Obj, GL_TRUE);

	bits=LoadBMPfile("texture\\earth.bmp", &info);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, (GLint)info->bmiHeader.biWidth, (GLint)info->bmiHeader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, bits);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
*/
}

void *COpenGLWnd::LoadBMPfile(char *filename, BITMAPINFO **info)
{
/*
	FILE *fp;
	void *bits;
	unsigned long bitsize, infosize;
	BITMAPFILEHEADER header;
	GLubyte *rgb, temp;
	unsigned int i;

	if( (fp=fopen(filename, "rb")) == NULL)
		return NULL;

	fread(&header, sizeof(BITMAPFILEHEADER), 1, fp);
	
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	
	*info=(BITMAPINFO*)malloc(infosize);

	fread(*info, 1, infosize, fp);
	
	bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount+7)/8*abs((*info)->bmiHeader.biHeight);

	bits=malloc(bitsize);

	fread(bits, 1, bitsize, fp);

	rgb=(GLubyte*)bits;

	for(i=0; i<bitsize; i+=3)
	{
		temp=rgb[i];
		rgb[i]=rgb[i+2];
		rgb[i+2]=temp;
	}

	fclose(fp);
	return bits;
*/
	return 0;
}

void COpenGLWnd::InitAlphaTest()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void COpenGLWnd::InitShadingModel()
{
	glShadeModel(GL_SMOOTH);                                      // Smooth shading�� ����
}
