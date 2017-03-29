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
	numberimage = 0;
	kmphimage = 0;
	graphimage = 0;
	titleimage = 0;
	winimage = 0;
	lostimage = 0;

	state = 0;

	sorrow_joy = 0.5f;
	anger = 0.0f;

	xx = yy = zz = 0.0f;
	HeartBeatRate = 30;
	HeartBeatCount = 0;
	HeartSize = 1.0f;


	theta = 0.0f;
	scalex = 1.0f;
	scaley = 1.0f;
	transx = 0.0f;
	transy = 0.0f;

	map_alpha = 0.5f;

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
}

void CHUD::init()
{
	unsigned char *temp2;
	unsigned char *temp3;
	int i;

	mapimage = new CImage("maps/map2.png");
	numberimage = new CImage("maps/numbers.png");
	kmphimage = new CImage("maps/kmPH2.png");
	graphimage = new CImage("maps/graph.png");

	titleimage = new CImage("maps/TDF.png");
	winimage = new CImage("maps/win.png");
	lostimage = new CImage("maps/lost.png");

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
	
	
////////////////////////// numbers image
	temp2 = numberimage->GetRawImage();
	temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)numberimage->GetWidth()*(GLint)numberimage->GetHeight()*4);

	for(i=0;i<(GLint)numberimage->GetWidth()*(GLint)numberimage->GetHeight();i++)
	{
		temp3[i*4] = 255;
		temp3[i*4+1] = 255;
		temp3[i*4+2] = 255;
		if(temp2[i*3]<20)
			temp3[i*4+3] = 0;
		else
			temp3[i*4+3] = temp2[i*3];
	}
	glGenTextures(1, &(texnames[1]));
	glBindTexture(GL_TEXTURE_2D, texnames[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)numberimage->GetWidth(), (GLint)numberimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);
	
	free(temp3);

	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
/////////////////////// kmph image	

	temp2 = kmphimage->GetRawImage();
	temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)kmphimage->GetWidth()*(GLint)kmphimage->GetHeight()*4);

	for(i=0;i<(GLint)kmphimage->GetWidth()*(GLint)kmphimage->GetHeight();i++)
	{
		temp3[i*4] = 255;
		temp3[i*4+1] = 255;
		temp3[i*4+2] = 255;
		if(temp2[i*3]<15)
			temp3[i*4+3] = 0;
		else
			temp3[i*4+3] = temp2[i*3];
	}
	glGenTextures(1, &(texnames[2]));
	glBindTexture(GL_TEXTURE_2D, texnames[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)kmphimage->GetWidth(), (GLint)kmphimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);
	
	free(temp3);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

/////////////////////// graph image	

	temp2 = graphimage->GetRawImage();
	temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)graphimage->GetWidth()*(GLint)graphimage->GetHeight()*4);

	for(i=0;i<(GLint)graphimage->GetWidth()*(GLint)graphimage->GetHeight();i++)
	{
		temp3[i*4] = temp2[i*3];
		temp3[i*4+1] = temp2[i*3+1];
		temp3[i*4+2] = temp2[i*3+2];
		temp3[i*4+3] = temp2[i*3]*100/255+150;
	}
	glGenTextures(1, &(texnames[3]));
	glBindTexture(GL_TEXTURE_2D, texnames[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)graphimage->GetWidth(), (GLint)graphimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);

	free(temp3);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

/////////////////////// title image	

	temp2 = titleimage->GetRawImage();
	temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)titleimage->GetWidth()*(GLint)titleimage->GetHeight()*4);

	for(i=0;i<(GLint)titleimage->GetWidth()*(GLint)titleimage->GetHeight();i++)
	{
		temp3[i*4] = temp2[i*3];
		temp3[i*4+1] = temp2[i*3+1];
		temp3[i*4+2] = temp2[i*3+2];
		temp3[i*4+3] = 255;
	}
	glGenTextures(1, &(texnames[4]));
	glBindTexture(GL_TEXTURE_2D, texnames[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)titleimage->GetWidth(), (GLint)titleimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);

	free(temp3);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

/////////////////////// win image	

	temp2 = winimage->GetRawImage();
	temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)winimage->GetWidth()*(GLint)winimage->GetHeight()*4);

	for(i=0;i<(GLint)winimage->GetWidth()*(GLint)winimage->GetHeight();i++)
	{
		temp3[i*4] = 200;
		temp3[i*4+1] = 255;
		temp3[i*4+2] = 200;
		if(temp2[i*3]<20)
			temp3[i*4+3] = 0;
		else
			temp3[i*4+3] = temp2[i*3];
	}
	glGenTextures(1, &(texnames[5]));
	glBindTexture(GL_TEXTURE_2D, texnames[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)winimage->GetWidth(), (GLint)winimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);

	free(temp3);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

/////////////////////// lost image	

	temp2 = lostimage->GetRawImage();
	temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)lostimage->GetWidth()*(GLint)lostimage->GetHeight()*4);

	for(i=0;i<(GLint)lostimage->GetWidth()*(GLint)lostimage->GetHeight();i++)
	{
		temp3[i*4] = 255;
		temp3[i*4+1] = 200;
		temp3[i*4+2] = 200;
		if(temp2[i*3]<20)
			temp3[i*4+3] = 0;
		else
			temp3[i*4+3] = temp2[i*3];
	}
	glGenTextures(1, &(texnames[6]));
	glBindTexture(GL_TEXTURE_2D, texnames[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)lostimage->GetWidth(), (GLint)lostimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);

	free(temp3);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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


/*
	static CTank *tank;
	static CCannon *cannon;

	for(i=0;i<TDFgame.m_cannonCount;i++)
	{
		cannon = TDFgame.m_cannon[i];

		glPointSize(5.0f);
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glBegin(GL_POINTS);
			glVertex3f((cannon->pos[2]+25.0f)/50.0f,(cannon->pos[0]+25.0f)/50.0f,0.0f);
		glEnd();
	}

	for(i=0;i<TDFgame.m_tankCount;i++)
	{
		tank = TDFgame.m_tank[i];

		glPointSize(7.0f);
		glColor4f(0.0f,1.0f,0.0f,1.0f);
		glBegin(GL_POINTS);
			glVertex3f((tank->pos[2]+25.0f)/50.0f,(tank->pos[0]+25.0f)/50.0f,0.0f);
		glEnd();
		glLineWidth(3.0f);
		glColor4f(0.5f,1.0f,0.0f,1.0f);
		glBegin(GL_LINES);
			glVertex3f((tank->pos[2]+25.0f)/50.0f,(tank->pos[0]+25.0f)/50.0f,0.0f);
			glVertex3f((tank->pos[2]+25.0f)/50.0f+sin(tank->ori[0])*0.035f,(tank->pos[0]+25.0f)/50.0f+cos(tank->ori[0])*0.035f,0.0f);
		glEnd();
	}

	glPointSize(7.0f);
	count = (++count)%20;
	if(count>12)
		glColor4f(1.0f,0.0f,0.0f,1.0f);
	else
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	glBegin(GL_POINTS);
	glVertex3f(posx,posy,0.0f);
	glEnd();

	glPointSize(1.0f);
//	DrawPath();

*/
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
}

void CHUD::DrawTitle()
{
	if(state==0)
		return;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
//	glDisable(GL_CULL_FACE);

	switch(state)
	{
	case 1:
		glBindTexture(GL_TEXTURE_2D,texnames[4]);
		break;
	case 2:
		glBindTexture(GL_TEXTURE_2D,texnames[5]);
		break;
	case 3:
		glBindTexture(GL_TEXTURE_2D,texnames[6]);
		break;
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

//	glTranslatef(0.65f,0.6f,0.0f);
//	glScalef(0.35f,0.4f,1.0f);
	glColor4f(1.0f,1.0f,1.0f,1.0f);

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


void CHUD::DrawSpeed()
{
	static int offset2;
	static int offset1;
	static int offset0;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
//	glDisable(GL_CULL_FACE);

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texnames[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(0.5f,0.02f,0.0f);

	glScalef(0.115f,0.20f,1.0f);

#define UNIT 0.081f

////////////// 10^2
//	offset2 = ((speed/100)%10);

	if(offset2)
	{

		glBegin(GL_QUADS);
		glTexCoord2f(offset2*UNIT,0.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glTexCoord2f(offset2*UNIT + UNIT,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glTexCoord2f(offset2*UNIT + UNIT,1.0f);
		glVertex3f(1.2f,1.0f,0.0f);
		glTexCoord2f(offset2*UNIT,1.0f);
		glVertex3f(0.2f,1.0f,0.0f);
		glEnd();
	}

////////////// 10^1
	glTranslatef(0.95f,0.0f,0.0f);

//	offset = 5*UNIT;
//	offset1 = (((speed-offset2*100)/10)%10);

	if(offset1 || offset2)
	{

		glBegin(GL_QUADS);
		glTexCoord2f(offset1*UNIT,0.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glTexCoord2f(offset1*UNIT+UNIT,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glTexCoord2f(offset1*UNIT+UNIT,1.0f);
		glVertex3f(1.2f,1.0f,0.0f);
		glTexCoord2f(offset1*UNIT,1.0f);
		glVertex3f(0.2f,1.0f,0.0f);
		glEnd();
	}

////////////// 10^0
	glTranslatef(0.95f,0.0f,0.0f);

//	offset0 = (speed%10);

	glBegin(GL_QUADS);
	glTexCoord2f(offset0*UNIT,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(offset0*UNIT+UNIT,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glTexCoord2f(offset0*UNIT+UNIT,1.0f);
	glVertex3f(1.2f,1.0f,0.0f);
	glTexCoord2f(offset0*UNIT,1.0f);
	glVertex3f(0.2f,1.0f,0.0f);
	glEnd();
/////////////// kmPH

	glTranslatef(0.95f,0.0f,0.0f);

	glScalef(1.2f,0.5f,1.0f);

	glBindTexture(GL_TEXTURE_2D,texnames[2]);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(0.371381f,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glTexCoord2f(0.371381f,1.0f);
	glVertex3f(1.14f,1.0f,0.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(0.14f,1.0f,0.0f);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
}


void CHUD::DrawRPM()
{
	static float value;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);

//	glDisable(GL_CULL_FACE);

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texnames[3]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(0.175f,0.05f,0.0f);

	glScalef(0.07f,0.13f,1.0f);

////////////// graph
//	glDisable(GL_BLEND);

//	if(rpm>100)
		value = 1.0f;
//	else
//		value = (float)(rpm)/110.0f+0.16f;
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(value,0.0f);
	glVertex3f(value*5.0f,0.0f,0.0f);
	glTexCoord2f(value,1.0f);
	glVertex3f(value*5.0f,0.5f,0.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(0.0f,0.5f,0.0f);
	glEnd();

/////////////// rpm

	glTranslatef(-2.55f,0.0f,0.0f);

	glScalef(2.2f,0.5f,1.0f);

	glBindTexture(GL_TEXTURE_2D,texnames[2]);

	glBegin(GL_QUADS);
	glTexCoord2f(0.371381f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(0.64421f,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glTexCoord2f(0.64421f,1.0f);
	glVertex3f(1.14f,1.0f,0.0f);
	glTexCoord2f(0.371381f,1.0f);
	glVertex3f(0.14f,1.0f,0.0f);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
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

	DrawSJA();
	DrawHeart();

	DrawAcceleration(sorrow_joy,anger);
	DrawMap();

//	DrawGear();
//	DrawRPM();
//	DrawSpeed();

//	DrawTitle();
}

void CHUD::DrawGear()
{
/*
	static float offset;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
//	glDisable(GL_CULL_FACE);

	glColor4f(1.0f,1.0f,0.75f,1.0f);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,1.0f,0.0f,1.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	
	glLineWidth(1.0f);
	
	glBegin(GL_LINES);

	glColor4f(1.0f,1.0f,1.0f,0.8f);
	glVertex3f(0.5f,0.15f,0.0f);
	glVertex3f(0.5f,0.65f,0.0f);
	glEnd();

	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(0.35f,0.5f,0.0f);
	glVertex3f(0.65f,0.5f,0.0f);

	glColor4f(1.0f,0.7f,0.5f,0.8f);
	glVertex3f(0.425f,0.5f-angle/1.2f,0.0f);
	glVertex3f(0.575f,0.5f-angle/1.2f,0.0f);

	glColor4f(0.2f,0.3f,1.0f,0.8f);
	glVertex3f(0.5f,0.5f,0.0f);
	glVertex3f(0.5f,0.5f-angle/1.2f,0.0f);

	glEnd();

	glLineWidth(4.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f,0.3f,0.4f,1.0f);
	glVertex3f(0.2f,0.15f,0.0f);
	glVertex3f(0.2f+cos(angle)*0.16f,0.15f+sin(angle)*0.2f,0.0f);

	glColor4f(0.3f,0.8f,1.00f,1.0f);
	glVertex3f(0.2f,0.15f,0.0f);
	glVertex3f(0.2f+cos(lastangle)*0.16f,0.15f+sin(lastangle)*0.2f,0.0f);

	glEnd();

	glLineWidth(1.0f);

	glTranslatef(0.165f,0.14f,0.0f);

	glScalef(0.07f,0.13f,1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texnames[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
*/
////////////// 
/*
	if(gear==0)
		offset = 10.1f*UNIT;
	else if(gear==4)
		offset = 11.3f*UNIT;
	else
		offset = gear*UNIT;

	glBegin(GL_QUADS);
	glTexCoord2f(offset,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(offset + UNIT,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glTexCoord2f(offset + UNIT,1.0f);
	glVertex3f(1.2f,1.0f,0.0f);
	glTexCoord2f(offset,1.0f);
	glVertex3f(0.2f,1.0f,0.0f);
	glEnd();
*/
/////////////// gear
/*
	glTranslatef(-2.35f,0.0f,0.0f);
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glScalef(2.2f,0.5f,1.0f);

	glBindTexture(GL_TEXTURE_2D,texnames[2]);

	glBegin(GL_QUADS);
	glTexCoord2f(0.64421f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.14f,1.0f,0.0f);
	glTexCoord2f(0.64421f,1.0f);
	glVertex3f(0.14f,1.0f,0.0f);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
*/
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
	xx += 0.3f;
	yy += 0.2f;
	zz += 0.1f;

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

void CHUD::DrawHeart()
{
	static float hs[50] = {1.0f};

	for(int i=1;i<50;i++)
		hs[i] = hs[i-1];

	hs[0] = HeartSize;

	
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

		glRotatef(xx,1.0f,0.0f,0.0f);
		glRotatef(yy,0.0f,1.0f,0.0f);
		glRotatef(zz,0.0f,0.0f,1.0f);

		glPushMatrix();


			glColor4f(0.9f,0.5f,0.5f,1.0f);
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
