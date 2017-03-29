// Eye.cpp: implementation of the CEye class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Avatar.h"
#include "Eye.h"

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

CEye::CEye()
{
	parse("facedata\\PupilCenter.obj");

	HorizontalDegree=0.0f;
	VerticalDegree=0.0f;
}

CEye::~CEye()
{

}

int CEye::DrawEye()
{
	POSITION pos;
	POSITION indexpos;
	POSITION vertexpos;

	CVertexNode *VertexNode;

	CFaceNode *FaceNode;
	CVertexIndexNode *sequence;

	static bool flag=FALSE;

	glPushMatrix();

	glRotatef((GLfloat)HorizontalDegree, 0.0f, 1.0f, 0.0f);
	glRotatef((GLfloat)VerticalDegree, 1.0f, 0.0f, 0.0f);

//	glTranslatef(0.0f,0.0f,2.0f);

	if(!flag)
	{
		glNewList(1,GL_COMPILE_AND_EXECUTE);
		for( pos = facelist.GetHeadPosition(); pos != NULL; facelist.GetNext( pos ))
		{
			FaceNode = (CFaceNode*) facelist.GetAt( pos );

			glBegin(GL_POLYGON);
			
			for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
			{
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
				indexpos = vertexlist.FindIndex( sequence->index-1 );
				VertexNode = (CVertexNode*) vertexlist.GetAt( indexpos );

				glTexCoord2f((VertexNode->x+0.9f)/1.8f, (VertexNode->y+0.9f)/1.8f);
				glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);

				FaceNode->VertexIndexList->GetNext( vertexpos );
			}
			glEnd();
		}
		glEndList();
		flag = true;
	}
	else
		glCallList(1);
	glPopMatrix();

	return 0;
}

int CEye::DrawPolygonEye()
{
	POSITION pos;
	POSITION indexpos;
	POSITION vertexpos;

	CVertexNode *prevVertexNode;
	CVertexNode *VertexNode;
	CVertexNode *nextVertexNode;

	CFaceNode *FaceNode;
	CVertexIndexNode *sequence;

	GLfloat v1[3],v2[3];

	glPushMatrix();

	glRotatef((GLfloat)HorizontalDegree, 0.0f, 1.0f, 0.0f);
	glRotatef((GLfloat)VerticalDegree, 1.0f, 0.0f, 0.0f);

	for( pos = facelist.GetHeadPosition(); pos != NULL; facelist.GetNext( pos ))
	{
		FaceNode = (CFaceNode*) facelist.GetAt( pos );

		glBegin(GL_POLYGON);
		
		for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
		{

///////////////////////////////////////////////////////////////////////////////////////////////
			FaceNode->VertexIndexList->GetPrev( vertexpos );
			if(!vertexpos)
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetTail();
			else
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			
			indexpos = vertexlist.FindIndex( sequence->index-1 );
			prevVertexNode = (CVertexNode*) vertexlist.GetAt( indexpos );
			
			if(vertexpos)
				FaceNode->VertexIndexList->GetNext( vertexpos );
			else
			{
				FaceNode->VertexIndexList->GetHead();
				vertexpos=FaceNode->VertexIndexList->GetHeadPosition();
			}
///////////////////////////////////////////////////////////////////////////////////////////////

			sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			indexpos = vertexlist.FindIndex( sequence->index-1 );
			VertexNode = (CVertexNode*) vertexlist.GetAt( indexpos );

///////////////////////////////////////////////////////////////////////////////////////////////
			
			FaceNode->VertexIndexList->GetNext( vertexpos );

			if(!vertexpos)
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetHead();
			else
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			
			indexpos = vertexlist.FindIndex( sequence->index-1 );
			nextVertexNode = (CVertexNode*) vertexlist.GetAt( indexpos );
			
///////////////////////////////////////////////////////////////////////////////////////////////
			v1[0]=prevVertexNode->x-VertexNode->x;
			v1[1]=prevVertexNode->y-VertexNode->y;
			v1[2]=prevVertexNode->z-VertexNode->z;
			v2[0]=VertexNode->x-nextVertexNode->x;
			v2[1]=VertexNode->y-nextVertexNode->y;
			v2[2]=VertexNode->z-nextVertexNode->z;

			glNormal3f ((v1[1]*v2[2]-v1[2]*v2[1]),
						(v1[2]*v2[0]-v1[0]*v2[2]),
						(v1[0]*v2[1]-v1[1]*v2[0]));

//		    glTexCoord2f(VertexNode->backupx*0.1379031f+0.498328f, VertexNode->backupy*0.1285902f+0.2105264f);
			glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);

//			TRACE("[[[%f,%f,%f]]]",VertexNode->x, VertexNode->y, VertexNode->z);
		}
//		TRACE("\n");
		glEnd();
	}
	glPopMatrix();

	return 0;
}

void CEye::SetEyeOri(float hori, float vert)
{

	HorizontalDegree = hori;
	VerticalDegree = vert;

}
