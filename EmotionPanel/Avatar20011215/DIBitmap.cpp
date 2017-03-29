//DIBitmap.CPP BMP 화일을 출력하는 클래스

#include "stdafx.h"
#include "DIBitmap.h"

CDIBitmap::CDIBitmap()
{
}

CDIBitmap::~CDIBitmap()
{
}



void CDIBitmap::output(char* filename)
{
	int i,j;

	FILE *stream;
	
	stream = fopen(filename, "w");

	fprintf(stream,"%d\n",cnp.width);
	fprintf(stream,"%d\n",cnp.height);

	for(j=0; j<cnp.height; j++)
		for(i=0; i<cnp.width; i++)
		{
			fprintf(stream, "%c%c%c",cnp.bitmap[j*cnp.width+i].r,
									cnp.bitmap[j*cnp.width+i].g,
									cnp.bitmap[j*cnp.width+i].b);
		}

	fclose(stream);
}

void CDIBitmap::parse(char* filename)
{
	BYTE _rgb[3];
	
	int s, t;
//	COLORREF C;

	HANDLE hFile;
	unsigned long nTemp;

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(hFile, &cnp.width, sizeof(int), &nTemp, NULL);
	ReadFile(hFile, &cnp.height, sizeof(int), &nTemp, NULL);

	cnp.bitmap = (rgb*)malloc(sizeof(rgb)*cnp.width*cnp.height);

	for(t = 0; t < cnp.height; t++)
	{
		for(s = 0; s < cnp.width; s++)
		{
			ReadFile(hFile, _rgb, 3, &nTemp, NULL);
//			C = RGB( _rgb[0], _rgb[1], _rgb[2]);
			cnp.bitmap[t*cnp.width+s].r= _rgb[0];
			cnp.bitmap[t*cnp.width+s].g= _rgb[1];
			cnp.bitmap[t*cnp.width+s].b= _rgb[2];
//			SetPixel(MemDC, s, t, C);
		}
	}
	CloseHandle(hFile);

/*
	int i,j;

	FILE *stream;

	char temp[81];
	
	stream = fopen(filename, "r");

	fgets(temp,80,stream);
	sscanf(temp,"%d",&cnp.width);

	fgets(temp,80,stream);
	sscanf(temp,"%d",&cnp.height);

	cnp.bitmap = (rgb*)malloc(sizeof(rgb)*cnp.width*cnp.height);

	for(j=0; j<cnp.height; j++)
		for(i=0; i<cnp.width; i++)
		{
			cnp.bitmap[j*cnp.width+i].r= (unsigned char) fgetc(stream);
			cnp.bitmap[j*cnp.width+i].g= (unsigned char) fgetc(stream);
			cnp.bitmap[j*cnp.width+i].b= (unsigned char) fgetc(stream);
		}

	fclose(stream);
*/
/*	
	
	printf("x:%d   y:%d \n",cnp.width, cnp.height);
	for(j=0; j<cnp.height; j++)
		for(i=0; i<cnp.width; i++)
		{
			printf("%d %d %d \n",cnp.bitmap[j*cnp.width+i].r,
								cnp.bitmap[j*cnp.width+i].g,
								cnp.bitmap[j*cnp.width+i].b);
		}
*/
}

unsigned char* CDIBitmap::bFindDib()
{
	return &cnp.bitmap[0].r;
}

int CDIBitmap::GetXSize()
{
	return cnp.width;
}

int CDIBitmap::GetYSize()
{
	return cnp.height;
}
