// Parser.h: interface for the CParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSER_H__663425D4_F07C_11D1_BDA4_00A0C98A1BF2__INCLUDED_)
#define AFX_PARSER_H__663425D4_F07C_11D1_BDA4_00A0C98A1BF2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CParser  
{
public:

	#include "PDef.h"

	CParser();
	virtual ~CParser();

public:
	bool flag_AdjacencyMatrix;
	void MakeAdjacencyMatrix();
	int parse(char *filename);
	int parsevertex(char *filename);

	CObList* GetVertexList();
	CObList* GetFaceList();
	int GetVertexCount();
	
	bool* GetAdjacencyMatrix();
	
	void InsertNode(float x, float y, float z);
	void InsertFace(int group,CObList* VertexIndexList);

	// Generated message map functions
protected:
	int group;

	bool parseready;
	int firstcome;
	bool parseface;

	bool *AdjacencyMatrix;

	int vertexcount;

	CObList vertexlist;
	CObList facelist;
	CFaceNode* facenode;
	CVertexNode* vertexnode;
	CVertexIndexNode* sequence;

	int parseline(char *singleline);
	void face(char *singleline);
	void node(char * singleline);

	int InitDB(void);
	
	void CleanUpHeap();

};

#endif // !defined(AFX_PARSER_H__663425D4_F07C_11D1_BDA4_00A0C98A1BF2__INCLUDED_)
