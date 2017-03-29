// ShapeArray.cpp: implementation of the CShapeArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pDef.h"
#include "Avatar.h"
#include "ShapeArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShapeArray::CShapeArray()
{
	LoadShapes();
}

CShapeArray::~CShapeArray()
{

}

void CShapeArray::LoadShapes()
{
	char singleline[200];

	FILE *stream;

	int eof=FALSE;
	int ShapeNumber;

	if( (stream  = fopen( "FaceData//shapes.shp", "r" )) == NULL )
	{
		AfxMessageBox( "can not load shape data");
		return;
	}

	do
	{
		fgets(singleline,100,stream);
		if(strnicmp(singleline,"#",1)&&strlen(singleline)>3)
		{
			if(!strnicmp(singleline,"shape",5))
			{
				sscanf(singleline, "shape:%d\n", &ShapeNumber);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].mouth_open);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].liptail_up);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].liptail_down);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].lip_longer);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].lip_tighten_o);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].lip_tighten_u);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].lip_open);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].eyetail_up);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].eye_close);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].eyebrow_up);
				fgets(singleline,200,stream);
				sscanf(singleline, "%f\n", &Shapes[ShapeNumber].eyebrow_glower);
			}
		}
		if(!strnicmp(singleline,"END",3))
			eof=TRUE;
	}while(!feof(stream)&&!eof);

	fclose( stream );

}

SliderStatus* CShapeArray::GetShape(int index)
{
	return &Shapes[index];
}
