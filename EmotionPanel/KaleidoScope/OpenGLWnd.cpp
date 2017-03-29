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
	
	hDC = ::GetDC(m_hWnd);                                    // OpenGL rendering context를 설정하고 초기화함
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

	wglMakeCurrent(hDC,NULL);                                 // OpenGL rendering context를 해제하고 window가 파괴됨
	wglDeleteContext(hRC);
	::ReleaseDC( m_hWnd, hDC );
	
}

void COpenGLWnd::OnPaint() 
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // 새로은 scene을 그리기 위하여 color buffer와
	                                                       // depth buffer를 clear한다.
	glMatrixMode(GL_PROJECTION);                                  // projection matrix의 초기화
	glLoadIdentity();

	InitProjection();

	glMatrixMode(GL_MODELVIEW);                                   // modelview matrix의 초기화
	glLoadIdentity();

	glColor4f(0.6f,0.8f,0.6f,0.6f);


	DrawScene();


	glFlush();			// 위의 API에 의하여 구성된 instruction들을 buffer
	SwapBuffers(hDC);	// 로부터 flush시킨다. 그후 page를 swap시켜서 화면을
	                    // 갱신한다.
	ValidateRect(NULL);
}

void COpenGLWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{

	m_lbutton = TRUE;                         // 마우스의 왼쪽 버튼이 눌러질 경우 flag을 set하고
                                              // 현재의 mouse pointer위치를 지억시켜둔다.
	m_prevpoint.x=point.x;
	m_prevpoint.y=point.y;

}

void COpenGLWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_lbutton = FALSE;                        // 마우스의 왼쪽버튼이 그만 누를 경우 flag을 reset한다
}

void COpenGLWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_lbutton)							  // 마우스가 움직일 경우 이전에 저장해 두었던 mouse pointer의
		                                      // 위치와 현재의 위치를 비교하여 x축과 y축의 회전울을 계산한다.
											  // 이 값은 OnPaint에서 object에 대한 retation값으로 이용된다.
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
	
	if(cx && cy)                            // 만약 윈도우의 가로나 세로 길이가 0 가 아니라면
	{                                       // 그에 맞춰 OpenGL의 ViewPort를 재정립한다.
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
	return TRUE;                      // 배경화면을 지우지 않는다.
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
	glEnable(GL_DEPTH_TEST);                                      // depth buffering(z-buffering) 가동

	glEnable(GL_NORMALIZE);                                       // 앞으로 쓰게될 vector들을 자동으로 unit vector로 변환함
	glEnable(GL_COLOR_MATERIAL);

//	Obj=gluNewQuadric();

	InitLight();
	InitTexture();
	InitAlphaTest();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	InitShadingModel();

	glMatrixMode(GL_PROJECTION);                                  // projection matrix의 초기화
	glLoadIdentity();

	InitProjection();

	glMatrixMode(GL_MODELVIEW);                                   // modelview matrix의 초기화
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
	glShadeModel(GL_SMOOTH);                                      // Smooth shading을 선택
}
