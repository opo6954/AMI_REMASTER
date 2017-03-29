// Eyes.cpp: implementation of the CEyes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Avatar.h"
#include "Mydib.h"
#include "Eyes.h"

#include "gl/gl.h"
#include "gl/glu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEyes::CEyes()
{
	EyeLeft.SetEyeOri( 0.0f, 0.0f );
	EyeRight.SetEyeOri( 0.0f, 0.0f );

	EyeScale = 1.0f;
	EyeDistance = 3.0f;

	EyeList=-999;
}

CEyes::~CEyes()
{
	delete EyeImage;

}

void CEyes::SetEyesOri(float hori, float vert)
{
	HoriDegree = hori;
	VertDegree = vert;

//	EyeLeft.SetEyeOri( hori+20.0f, vert );
//	EyeRight.SetEyeOri( hori-20.0f, vert );
	EyeLeft.SetEyeOri( hori, vert );
	EyeRight.SetEyeOri( hori, vert );

}

void CEyes::Draw()
{
	if(EyeList==-999)
	{
		EyeImage = new CMyDib("facedata\\eye2.bmp");

//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBindTexture(GL_TEXTURE_2D, 1);

		unsigned char *temp2;
		unsigned char *temp3;

		temp2 = (unsigned char *)EyeImage->bFindDib();
		temp3 = (unsigned char*)malloc(sizeof(unsigned char)*(GLint)EyeImage->GetXSize()*(GLint)EyeImage->GetYSize()*4);

		for(int i=0;i<(GLint)EyeImage->GetXSize()*(GLint)EyeImage->GetYSize();i++)
		{
			temp3[i*4] = temp2[i*3];
			temp3[i*4+1] = temp2[i*3+1];
			temp3[i*4+2] = temp2[i*3+2];
			temp3[i*4+3] = temp2[i*3];
			if (temp2[i*3+1]==255)
				temp3[i*4+3] = 0;
		}
//		glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLint)circleimage->GetWidth(), (GLint)circleimage->GetHeight(),0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)temp3);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, (GLint)EyeImage->GetXSize(), (GLint)EyeImage->GetYSize(),
				  GL_RGBA, GL_UNSIGNED_BYTE, (const void * )temp3);

		free(temp3);

//		EyeList = glGenLists(1);
//		glNewList(EyeList, GL_COMPILE);
//		glEndList();

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		EyeList=0;
	}
	else
//		glCallList(EyeList);
		glBindTexture(GL_TEXTURE_2D, 1);

	glEnable(GL_BLEND);
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glPushMatrix();
	glTranslatef(EyeDistance/2.0f,0.0f,0.0f);
	glScalef(EyeScale, EyeScale, EyeScale);
	EyeLeft.DrawEye();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-EyeDistance/2.0f,0.0f,0.0f);
	glScalef(EyeScale, EyeScale, EyeScale);
	EyeRight.DrawEye();
	glPopMatrix();

}

void CEyes::Draw0()
{

}

void CEyes::SetEyeDistance(float distance)
{
	EyeDistance = distance;
}

void CEyes::SetEyeScale(float scale)
{

	EyeScale = scale;

}
