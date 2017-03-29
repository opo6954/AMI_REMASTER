// ToothObj.cpp: implementation of the CToothObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDef.h"
#include "Avatar.h"
#include "mydib.h"
#include "ToothObj.h"

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

CToothObj::CToothObj( CObList *l1, CObList *fl1, CObList *ml)
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

	ToothList=-999;
}

CToothObj::~CToothObj()
{
	POSITION pos;

    for( pos = InitialVertexList.GetHeadPosition(); pos != NULL; InitialVertexList.GetNext( pos ))
    {
		InitialVertexNode = (CInitialVertexNode*)InitialVertexList.GetAt( pos );

		delete InitialVertexNode;
    }
	InitialVertexList.RemoveAll();

	delete ToothImage;

}

void CToothObj::Draw()
{
	if(ToothList==-999)
	{
		ToothImage = new CMyDib("facedata\\Teeth.bmp");

//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBindTexture(GL_TEXTURE_2D, 3);

//		EyeList = glGenLists(1);
//		glNewList(EyeList, GL_COMPILE);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, (GLint)ToothImage->GetXSize(), (GLint)ToothImage->GetYSize(),
				  GL_RGB, GL_UNSIGNED_BYTE, (const void * )ToothImage->bFindDib());
//		glEndList();

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glEnable(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		ToothList=0;
	}
	else
		glBindTexture(GL_TEXTURE_2D, 3);

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

			glTexCoord2f((MapVertexNode->x+1.128375f)/2.25675f, (MapVertexNode->y+0.438563f)/0.872401f);
			glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);
		}
		glEnd();
	}

	TRACE("~~~~~~~~~~~~ %d\n",InitialFace->GetCount());
}
