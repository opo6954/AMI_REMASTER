// MorphObj.cpp: implementation of the CMorphObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyDib.h"
#include "PDef.h"
#include "Avatar.h"
#include "MorphObj.h"

#include "gl/gl.h"
#include "gl/glu.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMyDib *m_pBitmap;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMorphObj::CMorphObj()
{
}

CMorphObj::CMorphObj( CObList *l1, CObList *fl1)
{
	POSITION pos;
	CVertexNode *VertexNode;

    for( pos = l1->GetHeadPosition(); pos != NULL; l1->GetNext( pos ))
    {
		VertexNode = (CVertexNode*)l1->GetAt( pos );

		InitialVertexNode = new CInitialVertexNode;
		
		InitialVertexNode->backupx = VertexNode->x;
		InitialVertexNode->backupy = VertexNode->y;
		InitialVertexNode->backupz = VertexNode->z;

		InitialVertexList.AddTail( InitialVertexNode );
    }

	ResetInitialVertexList();

	InitialFace = fl1;
	VertexCount = InitialVertexList.GetCount();
	FaceFlag=TRUE;
}

CMorphObj::CMorphObj( CObList *l1, CObList *fl1, CObList *ml)
{
	POSITION pos;
	CVertexNode *VertexNode;

    for( pos = l1->GetHeadPosition(); pos != NULL; l1->GetNext( pos ))
    {
		VertexNode = (CVertexNode*)l1->GetAt( pos );

		InitialVertexNode = new CInitialVertexNode;
		
		InitialVertexNode->backupx = VertexNode->x;
		InitialVertexNode->backupy = VertexNode->y;
		InitialVertexNode->backupz = VertexNode->z;

		InitialVertexList.AddTail( InitialVertexNode );
    }

	ResetInitialVertexList();

	InitialFace = fl1;
	VertexCount = InitialVertexList.GetCount();
	FaceFlag=TRUE;

	MapVertexList = ml;
}

CMorphObj::~CMorphObj()
{
	POSITION pos;

    for( pos = InitialVertexList.GetHeadPosition(); pos != NULL; InitialVertexList.GetNext( pos ))
    {
		InitialVertexNode = (CInitialVertexNode*)InitialVertexList.GetAt( pos );

		delete InitialVertexNode;
    }
	InitialVertexList.RemoveAll();
}

void CMorphObj::DrawInitialObj()
{
	POSITION pos;

	glBegin(GL_POINTS);

    for( pos = InitialVertexList.GetHeadPosition(); pos != NULL; InitialVertexList.GetNext( pos ))
    {
		InitialVertexNode = (CInitialVertexNode*)InitialVertexList.GetAt( pos );

		glVertex3f(InitialVertexNode->x, InitialVertexNode->y, InitialVertexNode->z);
    }

	glEnd();
}

void CMorphObj::DrawInitialFaceObj()
{
	POSITION pos;
	POSITION indexpos;
	POSITION indexpos2;
	POSITION vertexpos;

//	CInitialVertexNode *prevVertexNode;
	CInitialVertexNode *VertexNode;
	CVertexNode *MapVertexNode;

	CFaceNode *FaceNode;
	CVertexIndexNode *sequence;

//	GLfloat v1[3],v2[3];

	MapVertexList->GetHeadPosition();

	for( pos = InitialFace->GetHeadPosition(); pos != NULL; InitialFace->GetNext( pos ))
	{
		FaceNode = (CFaceNode*) InitialFace->GetAt( pos );

		glBegin(GL_POLYGON);
		
		for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
		{
			sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			
			indexpos = InitialVertexList.FindIndex( sequence->index-1 );
			VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );

			indexpos2 = MapVertexList->FindIndex( sequence->index-1 );
			MapVertexNode = (CVertexNode*) MapVertexList->GetAt( indexpos2 );
			
			FaceNode->VertexIndexList->GetNext( vertexpos );

			glTexCoord2f((MapVertexNode->x+2.583966f)/5.167932f, 1.0f-(MapVertexNode->y+1.34541f)/6.041671f);
			glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);
		}
		glEnd();
	}
}

void CMorphObj::DrawInitialFaceObjLine()
{
	POSITION pos;
	POSITION indexpos;
	POSITION indexpos2;
	POSITION vertexpos;

//	CInitialVertexNode *prevVertexNode;
	CInitialVertexNode *VertexNode;
	CVertexNode *MapVertexNode;

	CFaceNode *FaceNode;
	CVertexIndexNode *sequence;

//	GLfloat v1[3],v2[3];

	glColor3f(1.0,1.0,1.0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	MapVertexList->GetHeadPosition();

	for( pos = InitialFace->GetHeadPosition(); pos != NULL; InitialFace->GetNext( pos ))
	{
		FaceNode = (CFaceNode*) InitialFace->GetAt( pos );

		glBegin(GL_LINE_STRIP);
		
		for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
		{
			sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			
			indexpos = InitialVertexList.FindIndex( sequence->index-1 );
			VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );

			FaceNode->VertexIndexList->GetNext( vertexpos );

			glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);
		}
		
		vertexpos = FaceNode->VertexIndexList->GetHeadPosition();
		sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
		indexpos = InitialVertexList.FindIndex( sequence->index-1 );
		VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );

		glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);

		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

}
/*
void CMorphObj::DrawFinalObj()
{
	POSITION pos;
	CVertexNode *VertexNode;

	glBegin(GL_POINTS);

    for( pos = Final->GetHeadPosition(); pos != NULL; Final->GetNext( pos ))
    {
		VertexNode = (CVertexNode*)Final->GetAt( pos );

		glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);
    }

	glEnd();
}

void CMorphObj::DrawFinalFaceObj()
{
	if(FaceFlag)
	{
		POSITION pos;
		POSITION indexpos;
		POSITION vertexpos;
		CVertexNode *VertexNode;
		CFaceNode *FaceNode;
		CVertexIndexNode *sequence;

		for( pos = FinalFace->GetHeadPosition(); pos != NULL; FinalFace->GetNext( pos ))
		{
			FaceNode = (CFaceNode*) FinalFace->GetAt( pos );

			glBegin(GL_POLYGON);
			
			for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL; FaceNode->VertexIndexList->GetNext( vertexpos ) )
			{
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );

				indexpos = Initial->FindIndex( sequence->index-1 );
				VertexNode = (CVertexNode*) Initial->GetAt( indexpos );

				glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);

				TRACE("[[[%f,%f,%f]]]",VertexNode->x, VertexNode->y, VertexNode->z);
			}
			TRACE("\n");
			glEnd();
		}
	}
}
*/

void CMorphObj::DrawInitialFaceObjTexture()
{
	POSITION pos;
	POSITION indexpos;
	POSITION indexpos2;
	POSITION vertexpos;

	CInitialVertexNode *prevVertexNode;
	CInitialVertexNode *VertexNode;
	CInitialVertexNode *nextVertexNode;

	CVertexNode *MapVertexNode;
	CFaceNode *FaceNode;
	CVertexIndexNode *sequence;
	CVertexIndexNode *sequence2;

	GLfloat v1[3],v2[3];

//	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	MapVertexList->GetHeadPosition();

	for( pos = InitialFace->GetHeadPosition(); pos != NULL; InitialFace->GetNext( pos ))
	{
		FaceNode = (CFaceNode*) InitialFace->GetAt( pos );

		glBegin(GL_POLYGON);
		
		for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
		{

///////////////////////////////////////////////////////////////////////////////////////////////
			FaceNode->VertexIndexList->GetPrev( vertexpos );
			if(!vertexpos)
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetTail();
			else
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			
			indexpos = InitialVertexList.FindIndex( sequence->index-1 );
			prevVertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );
			
			if(vertexpos)
				FaceNode->VertexIndexList->GetNext( vertexpos );
			else
			{
				FaceNode->VertexIndexList->GetHead();
				vertexpos=FaceNode->VertexIndexList->GetHeadPosition();
			}
///////////////////////////////////////////////////////////////////////////////////////////////

			sequence2 = sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			indexpos = InitialVertexList.FindIndex( sequence->index-1 );
			VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );

///////////////////////////////////////////////////////////////////////////////////////////////
			
			FaceNode->VertexIndexList->GetNext( vertexpos );
			if(!vertexpos)
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetHead();
			else
				sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			
			indexpos = InitialVertexList.FindIndex( sequence->index-1 );
			nextVertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );
			
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

			indexpos2 = MapVertexList->FindIndex( sequence2->index-1 );
			MapVertexNode = (CVertexNode*) MapVertexList->GetAt( indexpos2 );


			glTexCoord2f((MapVertexNode->x+2.543794f)/5.144124f, (MapVertexNode->y+4.97173f)/8.7630677f);
			glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);

		}
		glEnd();

	}

}

void CMorphObj::DrawInitialFaceObjSmooth()
{
	POSITION pos;
	POSITION indexpos;
	POSITION indexpos2;
	POSITION vertexpos;

	CInitialVertexNode *prevVertexNode;
	CInitialVertexNode *VertexNode;
	CInitialVertexNode *nextVertexNode;
	CVertexNode *MapVertexNode;

	CFaceNode *FaceNode;
	CVertexIndexNode *sequence;

	float Mag;

	int i;

	GLfloat v1[3],v2[3];

//	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// normal vector를 계산화기위해 0.0으로 초기화 함.
	
	for( pos = InitialVertexList.GetHeadPosition(); pos != NULL; InitialVertexList.GetNext( pos ) )
	{
		VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( pos );

		VertexNode->NormalVector.x = 0.0;
		VertexNode->NormalVector.y = 0.0;
		VertexNode->NormalVector.z = 0.0;

		VertexNode->VectorCount=0;
	}

	i=0;


	for( pos = InitialFace->GetHeadPosition(); pos != NULL; InitialFace->GetNext( pos ))
	{
		FaceNode = (CFaceNode*) InitialFace->GetAt( pos );

		vertexpos = FaceNode->VertexIndexList->GetHeadPosition();

///////////////////////////////////////////////////////////////////////////////////////////////
		FaceNode->VertexIndexList->GetAt( vertexpos );
		sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
		indexpos = InitialVertexList.FindIndex( sequence->index-1 );
		prevVertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );
		
		FaceNode->VertexIndexList->GetNext( vertexpos );
		sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
		indexpos = InitialVertexList.FindIndex( sequence->index-1 );
		VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );

		FaceNode->VertexIndexList->GetNext( vertexpos );
		sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
		indexpos = InitialVertexList.FindIndex( sequence->index-1 );
		nextVertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );
			
///////////////////////////////////////////////////////////////////////////////////////////////
		v1[0]=prevVertexNode->x-VertexNode->x;
		v1[1]=prevVertexNode->y-VertexNode->y;
		v1[2]=prevVertexNode->z-VertexNode->z;
		v2[0]=VertexNode->x-nextVertexNode->x;
		v2[1]=VertexNode->y-nextVertexNode->y;
		v2[2]=VertexNode->z-nextVertexNode->z;

		FaceNode->NormalVector.x = v1[1]*v2[2]-v1[2]*v2[1];
		FaceNode->NormalVector.y = v1[2]*v2[0]-v1[0]*v2[2];
		FaceNode->NormalVector.z = v1[0]*v2[1]-v1[1]*v2[0];

		Mag = (float)sqrt( (FaceNode->NormalVector.x)*(FaceNode->NormalVector.x) +
						(FaceNode->NormalVector.y)*(FaceNode->NormalVector.y) +
						(FaceNode->NormalVector.z)*(FaceNode->NormalVector.z) );
		
		FaceNode->NormalVector.x /= Mag;
		FaceNode->NormalVector.y /= Mag;
		FaceNode->NormalVector.z /= Mag;


		for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
		{
			sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			
			indexpos = InitialVertexList.FindIndex( sequence->index-1 );
			VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );

			VertexNode->NormalVector.x += (VertexNode->NormalVector.x*VertexNode->VectorCount + FaceNode->NormalVector.x)/(VertexNode->VectorCount + 1);
			VertexNode->NormalVector.y += (VertexNode->NormalVector.y*VertexNode->VectorCount + FaceNode->NormalVector.y)/(VertexNode->VectorCount + 1);
			VertexNode->NormalVector.z += (VertexNode->NormalVector.z*VertexNode->VectorCount + FaceNode->NormalVector.z)/(VertexNode->VectorCount + 1);

			VertexNode->VectorCount++;

			FaceNode->VertexIndexList->GetNext( vertexpos );
		}
	}

	for( pos = InitialFace->GetHeadPosition(); pos != NULL; InitialFace->GetNext( pos ))
	{
		FaceNode = (CFaceNode*) InitialFace->GetAt( pos );

		glBegin(GL_POLYGON);
		
		for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
		{
			sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			
			indexpos = InitialVertexList.FindIndex( sequence->index-1 );
			VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );

			indexpos2 = MapVertexList->FindIndex( sequence->index-1 );
			MapVertexNode = (CVertexNode*) MapVertexList->GetAt( indexpos2 );

			FaceNode->VertexIndexList->GetNext( vertexpos );
/*

			Mag = sqrt( (VertexNode->NormalVector.x)*(VertexNode->NormalVector.x) +
						(VertexNode->NormalVector.y)*(VertexNode->NormalVector.y) +
						(VertexNode->NormalVector.z)*(VertexNode->NormalVector.z) );
		
			VertexNode->NormalVector.x /= Mag;
			VertexNode->NormalVector.y /= Mag;
			VertexNode->NormalVector.z /= Mag;
*/

//			glTexCoord2f((MapVertexNode->x+2.543794f)/5.144124f, (MapVertexNode->y+4.97173f)/8.7630677f);
			glTexCoord2f((MapVertexNode->x+2.583966f)/5.167932f, 1.0f-(MapVertexNode->y+1.34541f)/6.041671f);
			glNormal3f(VertexNode->NormalVector.x, VertexNode->NormalVector.y, VertexNode->NormalVector.z);
			glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);
		}
		glEnd();
	}

//	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

}


void CMorphObj::DrawObj(float degree)
{
/*	POSITION Ipos;
	POSITION Fpos;
	CVertexNode *IVertexNode;
	CVertexNode *FVertexNode;

	int i;

    Ipos = Initial->GetHeadPosition();
    Fpos = Final->GetHeadPosition();

	glBegin(GL_POINTS);

    for( i = 0; i < VertexCount; i++)
    {
		IVertexNode = (CVertexNode*)Initial->GetAt( Ipos );
		FVertexNode = (CVertexNode*)Final->GetAt( Fpos );

		glVertex3f( (IVertexNode->x*(100.0f-degree) + FVertexNode->x*degree)/100.0f,
					(IVertexNode->y*(100.0f-degree) + FVertexNode->y*degree)/100.0f,
					(IVertexNode->z*(100.0f-degree) + FVertexNode->z*degree)/100.0f);

		Initial->GetNext( Ipos );
		Final->GetNext( Fpos );
    }

	glEnd();
*/
}

void CMorphObj::DrawObj(int degree)
{
/*
	DrawObj((float)degree);
*/
}

void CMorphObj::CalculateMorph(CObList * VectorList, float influence)
{
	POSITION Ipos;
	POSITION Vectorpos;
	CVertexNode *FVertexNode;

	int i;

    Ipos = InitialVertexList.GetHeadPosition();
    Vectorpos = VectorList->GetHeadPosition();

    for( i = 0; i < VertexCount; i++)
    {
		InitialVertexNode = (CInitialVertexNode*)InitialVertexList.GetAt( Ipos );
		FVertexNode = (CVertexNode*)VectorList->GetAt( Vectorpos );

		InitialVertexNode->x += FVertexNode->x * influence;
		InitialVertexNode->y += FVertexNode->y * influence;
		InitialVertexNode->z += FVertexNode->z * influence;

		InitialVertexList.GetNext( Ipos );
		VectorList->GetNext( Vectorpos );
    }
}

void CMorphObj::ResetInitialVertexList()
{
	POSITION pos;

    for( pos = InitialVertexList.GetHeadPosition(); pos != NULL; InitialVertexList.GetNext( pos ))
    {
		InitialVertexNode = (CInitialVertexNode*)InitialVertexList.GetAt( pos );
	
		InitialVertexNode->x = InitialVertexNode->backupx;
		InitialVertexNode->y = InitialVertexNode->backupy;
		InitialVertexNode->z = InitialVertexNode->backupz;
    }
}
