// Heart.cpp: implementation of the CHeart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "Heart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeart::CHeart()
{
}

void CHeart::Init()
{
	int i;
	FILE *fp;

	fp = fopen("data/heart.obj","rt");

	for(i=0;i<162;i++)
	{
		fscanf(fp,"v %f %f %f\n",&vertex[i][0],&vertex[i][1],&vertex[i][2]);
	}
	for(i=0;i<162;i++)
	{
		fscanf(fp,"vn %f %f %f\n",&vertexnormal[i][0],&vertexnormal[i][1],&vertexnormal[i][2]);
	}
	for(i=0;i<320;i++)
	{
		fscanf(fp,"f %d//%d %d//%d %d//%d\n",&triangle[i][0],&triangle[i][3],&triangle[i][1],&triangle[i][4],&triangle[i][2],&triangle[i][5]);
	}
	fclose(fp);


	for(i=0;i<162;i++)
	{
		brokenheart[i][0] = ((float)rand())/355.350f;
		brokenheart[i][1] = ((float)rand())/355.350f;
		brokenheart[i][2] = ((float)rand())/355.350f;
	}
}

CHeart::~CHeart()
{

}

void CHeart::Draw()
{
	register int i;

	glBegin(GL_TRIANGLES);

	for(i=0;i<320;i++)
	{
		glNormal3f(vertexnormal[triangle[i][3]-1][0],vertexnormal[triangle[i][3]-1][1],vertexnormal[triangle[i][3]-1][2]);
		glVertex3f(vertex[triangle[i][0]-1][0],vertex[triangle[i][0]-1][1],vertex[triangle[i][0]-1][2]);
		glNormal3f(vertexnormal[triangle[i][4]-1][0],vertexnormal[triangle[i][4]-1][1],vertexnormal[triangle[i][4]-1][2]);
		glVertex3f(vertex[triangle[i][1]-1][0],vertex[triangle[i][1]-1][1],vertex[triangle[i][1]-1][2]);
		glNormal3f(vertexnormal[triangle[i][5]-1][0],vertexnormal[triangle[i][5]-1][1],vertexnormal[triangle[i][5]-1][2]);
		glVertex3f(vertex[triangle[i][2]-1][0],vertex[triangle[i][2]-1][1],vertex[triangle[i][2]-1][2]);
	}

	glEnd();
}

void CHeart::DrawBrokenHeart(float weight)
{
	register int i;

	glBegin(GL_TRIANGLES);

	for(i=0;i<320;i++)
	{
		glNormal3f(vertexnormal[triangle[i][3]-1][0],vertexnormal[triangle[i][3]-1][1],vertexnormal[triangle[i][3]-1][2]);
		glVertex3f(vertex[triangle[i][0]-1][0]+brokenheart[triangle[i][0]-1][0]*weight,vertex[triangle[i][0]-1][1]+brokenheart[triangle[i][0]-1][1]*weight,vertex[triangle[i][0]-1][2]+brokenheart[triangle[i][0]-1][2]*weight);

		glNormal3f(vertexnormal[triangle[i][4]-1][0],vertexnormal[triangle[i][4]-1][1],vertexnormal[triangle[i][4]-1][2]);
		glVertex3f(vertex[triangle[i][1]-1][0]+brokenheart[triangle[i][1]-1][0]*weight,vertex[triangle[i][1]-1][1]+brokenheart[triangle[i][1]-1][1]*weight,vertex[triangle[i][1]-1][2]+brokenheart[triangle[i][1]-1][2]*weight);

		glNormal3f(vertexnormal[triangle[i][5]-1][0],vertexnormal[triangle[i][5]-1][1],vertexnormal[triangle[i][5]-1][2]);
		glVertex3f(vertex[triangle[i][2]-1][0]+brokenheart[triangle[i][2]-1][0]*weight,vertex[triangle[i][2]-1][1]+brokenheart[triangle[i][2]-1][1]*weight,vertex[triangle[i][2]-1][2]+brokenheart[triangle[i][2]-1][2]*weight);
	}

	glEnd();
}
