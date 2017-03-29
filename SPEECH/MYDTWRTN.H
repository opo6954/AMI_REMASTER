#include "stdafx.h"
#define MAX_REAL    ((double)(1e+308))
#define MIN_REAL    ((double)(1e-308))

void **freeMatrix1(int**p);
void **freeMatrix2(double **p);
void **mallocMatrix(int nItemSize,int nRow,int nCol);
double DoMyDTW(int *piPath,double **pprX,double **pprY,int iDelta,int nX,int nY,int nVector,int nOption);