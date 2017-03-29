// Parser.cpp: implementation of the CParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Parser.h"
/*
#include "MainFrm.h"
#include "map1doc.h"
#include "map1view.h"
*/
//#include "stdio.h"
//#include "string.h"

//#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParser::CParser()
{
	parseready = FALSE;

	vertexcount=0;

	firstcome=TRUE;
}

CParser::~CParser()
{
	CleanUpHeap();
}

/////////////////////////////////////////////////////////////////////////////
// CParser commands
int CParser::parseline(char *singleline)
{
	if(!strnicmp(singleline,"v",1))
	{
		node(singleline);
		return 0;
	}
	else if(!strnicmp(singleline,"f",1) && parseface)
	{
		face(singleline);
		return 0;
	}
	return -1;
}

int CParser::parse(char *filename)
{
	FILE *stream;
	
	char singleline[100];
	int eof=FALSE;
	int i=0;

	parseface=TRUE;
	
	if(filename[0]!='@')
	{
		if( (stream  = fopen( filename, "r" )) == NULL )
		{
			AfxMessageBox( filename );
			return -1;
		}
	}

//	CMainFrame* pFrame=(CMainFrame*) AfxGetMainWnd();
//	CMap1View* pView=(CMap1View*) pFrame->GetActiveView();

	InitDB();

	do
	{
		i++;
		fgets(singleline,100,stream);
		if(strnicmp(singleline,"#",1)&&strlen(singleline)>3)
		{
			parseline(singleline);
		}
		if(!strnicmp(singleline,"END",3))
			eof=TRUE;
	}while(!feof(stream)&&!eof);

	TRACE("PARSE END\n");

	parseready = TRUE;

	return 0;
}

int CParser::parsevertex(char *filename)
{
	FILE *stream;
	
	char singleline[100];
	int eof=FALSE;
	int i=0;

	parseface=FALSE;
	
	InitDB();

	if( (stream  = fopen( filename, "r" )) == NULL )
	{
		AfxMessageBox( filename );
		return -1;
	}
	
	do
	{
		i++;
		fgets(singleline,100,stream);
		if(strnicmp(singleline,"#",1)&&strlen(singleline)>3)
		{
			parseline(singleline);
		}
		if(!strnicmp(singleline,"END",3))
			eof=TRUE;
	}while(!feof(stream)&&!eof);

	TRACE("PARSE END\n");

	MakeAdjacencyMatrix();

	parseready = TRUE;

	return 0;
}

int CParser::InitDB(void)
{
	flag_AdjacencyMatrix=FALSE;
	group = 0;
	CleanUpHeap();

//	glFeedbackBuffer(1000, GL_3D, feedback_buffer);
	return 0;
}

void CParser::node(char * singleline)
{
	float x, y, z;

//	token=strtok(singleline+1,"}");

	sscanf(singleline+1,"%f %f %f",&x,&y,&z);

//	TRACE("%f %f %f\n",x,y,z);

	InsertNode(x,y,z);
}

void CParser::InsertNode(float x, float y, float z)
{
	vertexnode = new CVertexNode;

	vertexnode->x=x;
	vertexnode->y=y;
	vertexnode->z=z;
	
	vertexlist.AddTail( vertexnode );

	vertexcount++;
}

bool* CParser::GetAdjacencyMatrix()
{
	return AdjacencyMatrix;
}

int CParser::GetVertexCount()
{
	return vertexcount;
}

CObList* CParser::GetVertexList()
{
	return &vertexlist;
}

CObList* CParser::GetFaceList()
{
	return &facelist;
}

void CParser::CleanUpHeap()
{
	POSITION pos;
	POSITION vertexpos;

    if(vertexcount)
	{
		for( pos = vertexlist.GetHeadPosition(); pos != NULL; vertexlist.GetNext( pos ))
		{
			vertexnode = (CVertexNode*) vertexlist.GetAt( pos );
			delete vertexnode;
		}
		vertexlist.RemoveAll();

		if(flag_AdjacencyMatrix)
			delete AdjacencyMatrix;
	}

	if(parseface)
	{

		for( pos = facelist.GetHeadPosition(); pos != NULL; facelist.GetNext( pos ))
		{
			facenode = (CFaceNode*) facelist.GetAt( pos );
			
			for( vertexpos = facenode->VertexIndexList->GetHeadPosition(); vertexpos != NULL; facenode->VertexIndexList->GetNext( vertexpos ) )
			{
				sequence = (CVertexIndexNode*) facenode->VertexIndexList->GetAt( vertexpos );
				
//				TRACE("[%d]  ",sequence->index);

				delete sequence;
			}
//			TRACE("\n");
			
			facenode->VertexIndexList->RemoveAll();

			delete facenode->VertexIndexList;

			delete facenode;
		}
		facelist.RemoveAll();
	}

	vertexcount=0;

	parseready = FALSE;

	firstcome=TRUE;

}

void CParser::face(char *singleline)
{
	char *token;
	int index;
	POSITION pos;
	CObList* VertexIndexList;

	VertexIndexList = new CObList;

	token=strtok(singleline," ");

	while(token=strtok(NULL," "))
	{
		sscanf(token,"%d",&index);

		sequence = new CVertexIndexNode;

		sequence->index = index; 
		
		VertexIndexList->AddTail( sequence );
	}

	for(pos=VertexIndexList->GetHeadPosition(); pos != NULL; VertexIndexList->GetNext( pos ))
	{
		sequence = (CVertexIndexNode*) VertexIndexList->GetAt( pos );
	}

	InsertFace( group, VertexIndexList );
}

void CParser::InsertFace(int group, CObList* VertexIndexList)
{
	facenode = new CFaceNode;

	facenode->group = group;
	facenode->VertexIndexList = VertexIndexList;

	POSITION vertexpos;

	for( vertexpos = facenode->VertexIndexList->GetHeadPosition(); vertexpos != NULL; facenode->VertexIndexList->GetNext( vertexpos ) )
	{
		sequence = (CVertexIndexNode*) facenode->VertexIndexList->GetAt( vertexpos );
		
//		TRACE("/%d/  ",sequence->index);
	}
//	TRACE("\n");

	facelist.AddTail( facenode );
}

void CParser::MakeAdjacencyMatrix()
{
	POSITION pos;
	POSITION vertexpos;

	CFaceNode *FaceNode;
	CVertexIndexNode *sequence1;
	CVertexIndexNode *sequence2;

	AdjacencyMatrix = new bool[vertexcount*vertexcount];

	for(int i=0; i<vertexcount*vertexcount; i++)
		AdjacencyMatrix[i]=FALSE;

	for( pos = facelist.GetHeadPosition(); pos != NULL; facelist.GetNext( pos ))
	{
		FaceNode = (CFaceNode*) facelist.GetAt( pos );

		for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
		{
			sequence1 = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			FaceNode->VertexIndexList->GetNext( vertexpos );
			if(vertexpos)
				sequence2 = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			else
				sequence2 = (CVertexIndexNode*) FaceNode->VertexIndexList->GetHead();

			AdjacencyMatrix[sequence1->index-1 * vertexcount + sequence2->index-1] = TRUE;
		}
	}
	flag_AdjacencyMatrix=TRUE;
}



/*	POSITION pos;
	POSITION indexpos;
	POSITION vertexpos;

	CInitialVertexNode *VertexNode;

	CFaceNode *FaceNode;
	CVertexIndexNode *sequence;

	for( pos = InitialFace->GetHeadPosition(); pos != NULL; InitialFace->GetNext( pos ))
	{
		FaceNode = (CFaceNode*) InitialFace->GetAt( pos );

		for( vertexpos = FaceNode->VertexIndexList->GetHeadPosition(); vertexpos != NULL;  )
		{
			sequence = (CVertexIndexNode*) FaceNode->VertexIndexList->GetAt( vertexpos );
			indexpos = InitialVertexList.FindIndex( sequence->index-1 );
			VertexNode = (CInitialVertexNode*) InitialVertexList.GetAt( indexpos );
			
			FaceNode->VertexIndexList->GetNext( vertexpos );

			glTexCoord2f(VertexNode->backupx*0.1379031f+0.498328f, VertexNode->backupy*0.1285902f+0.2105264f);
			glVertex3f(VertexNode->x, VertexNode->y, VertexNode->z);
		}
	}
*/