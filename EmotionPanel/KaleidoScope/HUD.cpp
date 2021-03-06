//
//  2000.5.25
//  daniel park
//
// HUD.cpp: implementation of the CHUD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "HUD.h"

#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define UNIT 0.081f


#define X .525731112119133606
#define Z .850650808352039932

#define DIFF3(_a,_b,_c) { \
    (_c)[0] = (_a)[0] - (_b)[0]; \
    (_c)[1] = (_a)[1] - (_b)[1]; \
    (_c)[2] = (_a)[2] - (_b)[2]; \
}

static void
crossprod(GLfloat v1[3], GLfloat v2[3], GLfloat prod[3])
{
  GLfloat p[3];         /* in case prod == v1 or v2 */

  p[0] = v1[1] * v2[2] - v2[1] * v1[2];
  p[1] = v1[2] * v2[0] - v2[2] * v1[0];
  p[2] = v1[0] * v2[1] - v2[0] * v1[1];
  prod[0] = p[0];
  prod[1] = p[1];
  prod[2] = p[2];
}

static void
normalize(GLfloat v[3])
{
  GLfloat d;

  d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  if (d == 0.0) {
//    __glutWarning("normalize: zero length vector");
    v[0] = d = 1.0;
  }
  d = 1 / d;
  v[0] *= d;
  v[1] *= d;
  v[2] *= d;
}

static void
recorditem(GLfloat * n1, GLfloat * n2, GLfloat * n3,
  GLenum shadeType)
{
  GLfloat q0[3], q1[3];

  DIFF3(n1, n2, q0);
  DIFF3(n2, n3, q1);
  crossprod(q0, q1, q1);
  normalize(q1);

  glBegin(shadeType);
  glNormal3fv(q1);
  glVertex3fv(n1);
  glVertex3fv(n2);
  glVertex3fv(n3);
  glEnd();
}

static void
subdivide(GLfloat * v0, GLfloat * v1, GLfloat * v2,
  GLenum shadeType)
{
  int depth;
  GLfloat w0[3], w1[3], w2[3];
  GLfloat l;
  int i, j, k, n;

  depth = 1;
  for (i = 0; i < depth; i++) {
    for (j = 0; i + j < depth; j++) {
      k = depth - i - j;
      for (n = 0; n < 3; n++) {
        w0[n] = (i * v0[n] + j * v1[n] + k * v2[n]) / depth;
        w1[n] = ((i + 1) * v0[n] + j * v1[n] + (k - 1) * v2[n])
          / depth;
        w2[n] = (i * v0[n] + (j + 1) * v1[n] + (k - 1) * v2[n])
          / depth;
      }
      l = sqrt(w0[0] * w0[0] + w0[1] * w0[1] + w0[2] * w0[2]);
      w0[0] /= l;
      w0[1] /= l;
      w0[2] /= l;
      l = sqrt(w1[0] * w1[0] + w1[1] * w1[1] + w1[2] * w1[2]);
      w1[0] /= l;
      w1[1] /= l;
      w1[2] /= l;
      l = sqrt(w2[0] * w2[0] + w2[1] * w2[1] + w2[2] * w2[2]);
      w2[0] /= l;
      w2[1] /= l;
      w2[2] /= l;
      recorditem(w1, w0, w2, shadeType);
    }
  }
}

static void
drawtriangle(int i, GLfloat data[][3], int ndx[][3],
  GLenum shadeType)
{
  GLfloat *x0, *x1, *x2;

  x0 = data[ndx[i][0]];
  x1 = data[ndx[i][1]];
  x2 = data[ndx[i][2]];
  subdivide(x0, x1, x2, shadeType);
}

static GLfloat idata[12][3] =
{
  {-X, 0, Z},
  {X, 0, Z},
  {-X, 0, -Z},
  {X, 0, -Z},
  {0, Z, X},
  {0, Z, -X},
  {0, -Z, X},
  {0, -Z, -X},
  {Z, X, 0},
  {-Z, X, 0},
  {Z, -X, 0},
  {-Z, -X, 0}
};

static int index[20][3] =
{
  {0, 4, 1},
  {0, 9, 4},
  {9, 5, 4},
  {4, 5, 8},
  {4, 8, 1},
  {8, 10, 1},
  {8, 3, 10},
  {5, 3, 8},
  {5, 2, 3},
  {2, 7, 3},
  {7, 10, 3},
  {7, 6, 10},
  {7, 11, 6},
  {11, 0, 6},
  {0, 1, 6},
  {6, 1, 10},
  {9, 0, 11},
  {9, 11, 2},
  {9, 2, 5},
  {7, 2, 11},
};

void CHUD::DrawIcosahedron()
{
	int i;

	for (i = 19; i >= 0; i--)
	{
		drawtriangle(i, idata, index, GL_TRIANGLES);
	}
}

void CHUD::DrawIcosahedronWire()
{
	int i;

	for (i = 19; i >= 0; i--)
	{
		drawtriangle(i, idata, index, GL_LINE_LOOP);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHUD::CHUD()
{
	mapimage = 0;
	circleimage = 0;
/*
	numberimage = 0;
	kmphimage = 0;
	graphimage = 0;
	titleimage = 0;
	winimage = 0;
	lostimage = 0;
*/
	state = 0;

	sorrow_joy = 0.5f;
	anger = 0.0f;
	sorrow_joy_tobe = 0.5f;
	anger_tobe = 0.0f;

	duration = 0;

	xx = yy = zz = 0.0f;
	HeartBeatRate = 30;
	HeartBeatCount = 0;
	HeartSize = 1.0f;


	theta = 0.0f;
	scalex = 0.4f;
	scaley = 0.4f;
	transx = 0.0f;
	transy = 0.0f;

	map_alpha = 0.25f;

	init();
}

CHUD::~CHUD()
{
	if(mapimage)
	{
		delete mapimage;
		mapimage = 0;
		glDeleteTextures(1,&(texnames[0]));
	}
	if(circleimage)
	{
		delete circleimage;
		circleimage = 0;
		glDeleteTextures(1,&(texnames[1]));
	}

	delete heart;
/*
	if(numberimage)
	{
		delete numberimage;
		numberimage = 0;
		glDeleteTextures(1,&(texnames[1]));
	}
	if(kmphimage)
	{
		delete kmphimage;
		kmphimage = 0;
		glDeleteTextures(1,&(texnames[2]));
	}
	if(graphimage)
	{
		delete graphimage;
		graphimage = 0;
		glDeleteTextures(1,&(texnames[3]));
	}
	if(titleimage)
	{
		delete titleimage;
		titleimage = 0;
		glDeleteTextures(1,&(texnames[4]));
	}
	if(winimage)
	{
		delete winimage;
		winimage = 0;
		glDeleteTextures(1,&(texnames[5]));
	}
	if(lostimage)
	{
		delete lostimage;
		lostimage = 0;
		glDeleteTextures(1,&(texnames[6]));
	}
*/
}

void CHUD::init()
{
	unsigned char *temp2;
	unsigned char *temp3;
	int i;

	heart = new CHeart;
	heart->Init();

	mapimage = new CImage("maps/map3small.png");
	circleimage = new CImage("maps/circle.png");
//	numberimage = new CImage("maps/numbers.png");
//	kmphimage = new CImage("maps/kmPH2.png");
//	graphimage = new CImage("maps/graph.png");

//	titleimage = new CImage("maps/TDF.png");
//	winimage = new CImage("maps/win.png");
//	lostimage = new CImage("maps/lost.png");

/////////////////////////// map image
	temp2 = mapimage->GetRawImage();
	temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)mapimage->GetWidth()*(GLint)mapimage->GetHeight()*4);

	for(i=0;i<(GLint)mapimage->GetWidth()*(GLint)mapimage->GetHeight();i++)
	{
		temp3[i*4] = temp2[i*3];
		temp3[i*4+1] = temp2[i*3+1];
		temp3[i*4+2] = temp2[i*3+2];
		temp3[i*4+3] = temp2[i*3]*100/255+50;
	}
	glGenTextures(1, &(texnames[0]));

	glBindTexture(GL_TEXTURE_2D, texnames[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)mapimage->GetWidth(), (GLint)mapimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);

	free(temp3);

	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
/////////////////////////// circle image
	temp2 = circleimage->GetRawImage();
	temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)circleimage->GetWidth()*(GLint)circleimage->GetHeight()*4);

	for(i=0;i<(GLint)circleimage->GetWidth()*(GLint)circleimage->GetHeight();i++)
	{
		temp3[i*4] = 255;
		temp3[i*4+1] = 255;
		temp3[i*4+2] = 255;
		temp3[i*4+3] = temp2[i*3];
		if (temp3[i*4+3]<35)
			temp3[i*4+3] = 0;
	}
	glGenTextures(1, &(texnames[1]));

	glBindTexture(GL_TEXTURE_2D, texnames[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)circleimage->GetWidth(), (GLint)circleimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);

	free(temp3);

	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
}

void CHUD::DrawMap()
{
	static int count=0;
	static int i;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
//	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_2D,texnames[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
/*
	glTranslatef(0.65f,0.6f,0.0f);
	glScalef(0.35f,0.4f,1.0f);
*/
	glColor4f(1.0f,1.0f,1.0f,map_alpha);

	glMatrixMode(GL_TEXTURE);

	glLoadIdentity();

	glTranslatef(transx,transy,0.0f);
	glScalef(scalex,scaley,1.0f);
	glRotatef(theta,0.0f,0.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
}

void CHUD::DrawMap2()
{
	static int count=0;
	static int i;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D,texnames[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-0.16f,1.16f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glMatrixMode(GL_TEXTURE);

	glLoadIdentity();

	glTranslatef(transx,transy,0.0f);
	glScalef(scalex,scaley,1.0f);
	glRotatef(theta,0.0f,0.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);

	for(float dd=0.0f;dd<360.0f;dd+=40.0f)
	{
		glLoadIdentity();
		glTranslatef(0.5f,0.5f,0.0f);
		glRotatef(dd,0.0f,0.0f,1.0f);
		glScalef(1.6f,1.6f,1.0f);
		glTranslatef(-0.5f,-0.5f,0.0f);

		glBegin(GL_TRIANGLES);
		glTexCoord2f(0.7f,1.0f);
		glVertex3f(0.68395f,0.0f,0.0f);
		glTexCoord2f(0.3f,1.0f);
		glVertex3f(0.5f,0.0f,0.0f);
		glTexCoord2f(0.5f,0.5f);
		glVertex3f(0.5f,0.5f,0.0f);

		glTexCoord2f(0.3f,1.0f);
		glVertex3f(0.5f,0.0f,0.0f);
		glTexCoord2f(0.7f,1.0f);
		glVertex3f(0.32f,0.0f,0.0f);
		glTexCoord2f(0.5f,0.5f);
		glVertex3f(0.5f,0.5f,0.0f);
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void CHUD::DrawFace()
{
	static float th = 0.0f;

	th+=0.01f;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
//	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_2D,texnames[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
//	glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_TEXTURE);

	glLoadIdentity();
/*
	glTranslatef(0.65f,0.6f,0.0f);
	glScalef(0.35f,0.4f,1.0f);
*/
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(0.1f*sin(th),0.1f*cos(th*0.745f)-0.35f+sorrow_joy*0.4f,0.0f);
//	glTranslatef(0.5f,0.5f,0.0f);
	glRotatef(cos(th*0.352f)*3.5f,0.0f,0.0f,1.0f);
	glScalef(1.0f+0.1f*cos(th*0.32),1.0f+0.1f*cos(th*0.81),1.0f);
//	glTranslatef(-0.5f,-0.5f,0.0f);

	glColor4f(1.0f-anger/2.0f,1.0f-sorrow_joy*0.5f,0.5f+sorrow_joy*0.5f,0.84f);

///////////////////////////// left eye brow
	glPushMatrix();
	glTranslatef(0.5f,0.35f,0.0f);
	glRotatef(anger*50.0f-5.0f,0.0f,0.0f,1.0f);
	glScalef(0.8f+anger*0.5f,0.15f,1.0f);

	DrawCircle((sorrow_joy-0.5f)*2.0f);

	glPopMatrix();

///////////////////////////// right eye brow
	glPushMatrix();
	glTranslatef(-0.5f,0.35f,0.0f);
	glRotatef(-anger*50.0f+5.0f,0.0f,0.0f,1.0f);
	glScalef(0.8f+anger*0.5f,0.15f,1.0f);

	DrawCircle((sorrow_joy-0.5f)*2.0f);

	glPopMatrix();

///////////////////////////// mouth
	glPushMatrix();
	glTranslatef(0.0f,-0.45f,0.0f);
	glScalef(1.2f-sorrow_joy*0.5f,0.15f,1.0f);
//	glRotatef(theta,0.0f,0.0f,1.0f);

	DrawCircle((sorrow_joy-0.5f)*-3.0f-0.3f);

	glPopMatrix();


	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);


}

void CHUD::DrawCircle(float a)
{
	register int i, j;

	glBegin(GL_QUADS);
	for(i=0;i<3;i++)
		for(j=0;j<10;j++)
		{
			glTexCoord2f((float)j/10.0f,(float)(i+1)/3.0f);
			glVertex3f((float)j/10.0f-0.5f,(float)(i+1)/3.0f-0.5f + a*sin(((float)j/10.0f)*3.141592f),0.0f);

			glTexCoord2f((float)j/10.0f+0.1f,(float)(i+1)/3.0f);
			glVertex3f((float)j/10.0f+0.1f-0.5f,(float)(i+1)/3.0f-0.5f + a*sin(((float)(j+1)/10.0f)*3.141592f),0.0f);

			glTexCoord2f((float)j/10.0f+0.1f,(float)(i)/3.0f);
			glVertex3f((float)j/10.0f+0.1f-0.5f,(float)(i)/3.0f-0.5f + a*sin(((float)(j+1)/10.0f)*3.141592f),0.0f);

			glTexCoord2f((float)j/10.0f,(float)(i)/3.0f);
			glVertex3f((float)j/10.0f-0.5f,(float)(i)/3.0f-0.5f + a*sin(((float)j/10.0f)*3.141592f),0.0f);
		}
	glEnd();

}

void CHUD::DrawTitle()
{
}


void CHUD::DrawSpeed()
{
}


void CHUD::DrawRPM()
{
}

void CHUD::DrawSJA()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

//	glTranslatef(0.33f,0.3f,0.0f);
//	glScalef(0.30f,0.38f,1.0f);
//	glColor4f(0.5f,0.5f,0.5f,0.6f);
	glColor4f(anger,sorrow_joy,1.0f-sorrow_joy,0.6f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}


void CHUD::Draw()
{

//	DrawSJA();
//	DrawHeart();
//	DrawHeart2();

//	DrawAcceleration(sorrow_joy,anger);
	DrawMap2();
//	DrawFace();

//	DrawGear();
//	DrawRPM();
//	DrawSpeed();

//	DrawTitle();
}

void CHUD::DrawGear()
{
}


void CHUD::DrawAcceleration(float ax, float ay)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(0.05f,0.1f,0.0f);
	glScalef(0.14f,0.18f,1.0f);
	glColor4f(0.2f,0.0f,0.3f,0.6f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glEnd();

	glLineWidth(5.0f);
	glBegin(GL_LINES);
	glColor4f(1.0f,0.5f,0.7f,1.0f);
	glVertex3f(0.5f,0.0f,0.0f);
	glVertex3f(0.5f,ay,0.0f);

	glColor4f(0.5f,0.5f,1.0f,1.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,1.0f-ax,0.0f);

	glColor4f(0.5f,1.0f,0.5f,1.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glVertex3f(1.0f,ax,0.0f);

	glEnd();

	glLineWidth(1.0f);
	glBegin(GL_LINES);

	glColor4f(1.0f,0.0f,0.0f,1.0f);
	glVertex3f(0.0f,ay,0.0f);
	glVertex3f(1.0f,ay,0.0f);

//	glColor4f(1.0f,0.0f,0.0f,1.0f);
//	glVertex3f(ax,0.0f,0.0f);
//	glVertex3f(ax,1.0f,0.0f);

	glColor4f(0.0f,1.0f,0.0f,1.0f);
	glVertex3f(0.0f,1.0f-ax,0.0f);
	glVertex3f(1.0f,ax,0.0f);

	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void CHUD::ProcessHeart()
{
	xx += 0.3f*(anger*5.0f+1.0f);
	yy += 0.2f*(anger*55.0f+1.0f);
	zz += 0.1f*(anger*5.0f+1.0f);

	xx2 = 10.0f*sin(xx*0.001) + 7.0f*cos(yy*0.001);
	yy2 = 10.0f*sin(yy*0.001) + 7.0f*cos(zz*0.001);
	zz2 = 10.0f*sin(zz*0.001) + 7.0f*cos(xx*0.001);

	HeartBeatRate = (int)((1.0f-anger*0.5f)*30.0f);

	HeartSize = HeartSize*0.95f + 0.05f;

	HeartBeatCount++;

	if(HeartBeatCount > HeartBeatRate-3)
		HeartSize *= 1.1f;
	if(HeartBeatCount >= HeartBeatRate)
	{
		HeartSize = 1.4f;
		HeartBeatCount = 0;
	}
}

void CHUD::DrawHeart2()
{
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(30.0,1.3333,0.5,10.0);
	gluLookAt(0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

	glColor4f(anger+0.35f,anger*0.5f+0.2f+sorrow_joy*0.5f,anger*0.5f+0.2f+sorrow_joy*0.5f,0.8f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		glTranslatef(0.0f,-0.2f,0.0f);

		glScalef((anger*0.35f+0.4f),(anger*0.35f+0.4f),(anger*0.35+0.4f));
		glScalef(0.02f*HeartSize,0.02f*HeartSize,0.02f*HeartSize);

		glRotatef(xx2,1.0f,0.0f,0.0f);
		glRotatef(yy2,0.0f,1.0f,0.0f);
		glRotatef(zz2,0.0f,0.0f,1.0f);

		glPushMatrix();


//			glColor4f(0.9f,0.5f,0.5f,1.0f);
		heart->Draw();

		glPopMatrix();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_DEPTH_TEST);

}

void CHUD::DrawHeart()
{
	static float hs[50] = {1.0f};
	static int co = 0;

	for(register i=1;i<50;i++)
		hs[i] = hs[i-1];

	hs[0] = HeartSize;

	co++;

	
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(30.0,1.3333,0.5,10.0);
	gluLookAt(0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		glScalef((anger*1.0f+0.5f),(anger*1.0f+0.5f),(anger*1.0f+0.5f));

		glRotatef(xx,1.0f,0.0f,0.0f);
		glRotatef(yy,0.0f,1.0f,0.0f);
		glRotatef(zz,0.0f,0.0f,1.0f);

		glPushMatrix();


//			glColor4f(0.9f,0.5f,0.5f,1.0f);
			if(co%5>2)
				glColor4f(0.0f,0.0f,0.0f,1.0f);
			else
				glColor4f(1.0f,1.0f,1.0f,1.0f);
			glPushMatrix();
				glScalef(hs[0],hs[0],hs[0]);
				glScalef(0.3f,0.3f,0.3f);
				DrawIcosahedron();
			glPopMatrix();

			glColor4f(0.7f,0.8f,0.5f,0.3f);
			glPushMatrix();
				glScalef(hs[10],hs[10],hs[10]);
				glScalef(0.6f,0.6f,0.6f);
				DrawIcosahedron();
			glPopMatrix();

			glColor4f(1.0f,0.5f,0.5f,0.2f);
			glPushMatrix();
				glScalef(hs[20],hs[20],hs[20]);
				glScalef(0.8f,0.8f,0.8f);
				DrawIcosahedron();
			glPopMatrix();

			glColor4f(0.0f,0.0f,0.0f,0.8f);
			glPushMatrix();
				glScalef(hs[30],hs[30],hs[30]);
				glScalef(0.9f,0.9f,0.9f);
				DrawIcosahedronWire();
			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_DEPTH_TEST);
}

void CHUD::DrawPath()
{
	//path의 length만큼  loop을 돌면서 선을 긋는다.
	//CPath가 좌표값을 테이블로 가지고 있고, 노드 번호를 주면, x,y값을 넘겨주는 함수하나
	//구현하면...끝
/*
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CRideSimulatorView *pView = (CRideSimulatorView*)pMain->GetActiveView();
	int i;

	CPoint* point;

	glColor3f(1.0f,0.2f,0.2f);
	glLineWidth(3.0f);

	glBegin(GL_LINE_STRIP);
	
	for (i=0; i<(pView->SP->length); i++) {

		point = pView->PF.getPoint(pView->SP->path[i]);
		glVertex3f((float)(point->x-165)/670.0f,1.0f-(float)point->y/700.0f,0.0f);
	}

	glEnd();

	glLineWidth(1.0f);
*/
}
