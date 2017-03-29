#include "stdafx.h"
#include "mydtwrtn.h"

void **mallocMatrix(int nItemSize,int nRow,int nCol)
{
    char    **p;
    int     i;

    p=(char **)malloc(sizeof(char *)*nRow);
    *p=(char *)malloc(nItemSize*nRow*nCol);
    nItemSize*=nCol;
    for(i=1;i<nRow;i++) p[i]=p[i-1]+nItemSize;
    return((void **)p);
}

void **freeMatrix1(int **p)
{
    free(*p);
    free(p);
    return(NULL);
}

void **freeMatrix2(double **p)
{
    free(*p);
    free(p);
    return(NULL);    
}

double DoMyDTW(int *piPath,double **pprX,double **pprY,int iDelta,int nX,int nY,int nVector,int nOption)
{
	int     i,i_1,j,j_1,k,iStart,iEnd,iT,iPath,jPath,iPathCnt;
    int     nY_Delta,nX_Delta,nX_1,nY_1;        
    int     *piX1,*piX2,**ppiPath;
    double    **pprD,**pprSum,rMin,rSum,rSub;

    nX_1=nX-1;  nY_1=nY-1;
    nY_Delta=nY_1-iDelta;   nX_Delta=nX_1-iDelta;
    if(nY<nX/2.8||nY>nX*2.8) return(MAX_REAL);

    pprD=(double **)mallocMatrix(sizeof(double),nX,nY);
    pprSum=(double **)mallocMatrix(sizeof(double),nX,nY);
    ppiPath=(int **)mallocMatrix(sizeof(int),nX+2,nY);

    piX1=ppiPath[nX];
    piX2=piX1+nY;

    memset(piX1,0,sizeof(int)*nY<<1);
    /*********************************************
    * first calculate distances of every point   *
    *   |  /..../                                *
    *   | /..../                                 *
    *   |/..../                                  *
    *   |..../                                   *
    *   |___/_____                               *
    * interior point is of interest              *
    **********************************************/
    /* find the region of interest */
    for(j=0;j<=nY_1;j++)
    {
        /* find start point of j */
        iStart=3*(j-nY_1)+nX_Delta;						// main  slope
        if(iStart<(j-iDelta)/3) iStart=(j-iDelta)/3;	// start slope
        if(iStart<0)    iStart=0;
    
        /* find end point of j */
        iEnd=3*j+iDelta;
        if(iEnd>(j-nY_Delta)/3+nX_1)  iEnd=(j-nY_Delta)/3+nX_1;
        if(iEnd>nX_1)   iEnd=nX_1;
        
        piX1[j]=iStart; piX2[j]=iEnd;
        for(i=iStart;i<=iEnd;i++)
        {
            for(rSum=0.0,k=0;k<nVector;k++)
            {
                rSub=pprX[i][k]-pprY[j][k];
                rSum+=rSub*rSub;
            }
            pprD[i][j]=rSum/nVector;
        }            
    }
    /* now start weight summing at first line */
    for(i=piX1[0];i<=piX2[0];i++)    pprSum[i][0]=pprD[i][0];
    for(j=1;j<nY;j++)
    {
        if(piX1[j]>0)   break;
        pprSum[0][j]=pprD[0][j];
    }
    /****************************
    *  calc all line            *
    * 0.75 /~~/|                *
    *     /  / |                *
    *       1  / 0.75           *
    *         /                 *
    ****************************/
    for(j=1;j<nY;j++)
    {
        j_1=j-1;
        if(piX1[j]<=1)  i=1;
        else            i=piX1[j];
        for(;i<=piX2[j];i++)
        {
            i_1=i-1;
            /* Left Path */
            if(piX1[j_1]<=i-3&&piX2[j_1]>=i-3&&piX1[j]<=i-2&&piX2[j]>=i-2&&piX1[j]<=i_1&&piX2[j]>=i_1)
                rMin=(pprSum[i-3][j_1]+2.*pprD[i-2][j]+pprD[i_1][j]+pprD[i][j]),iPath=1;
            else    rMin=MAX_REAL;

            /* Left Path */
            if(piX1[j_1]<=i-2&&piX2[j_1]>=i-2&&piX1[j]<=i_1&&piX2[j]>=i_1)
			{
                rMin=pprSum[i-2][j_1]+2.*pprD[i_1][j]+pprD[i][j];
                if(rSum<rMin)   rMin=rSum,iPath=2;
			}
            /* Middle Path */
            if(piX1[j_1]<=i_1&&piX2[j_1]>=i_1)
            {   
                rSum=pprSum[i_1][j_1]+2.*pprD[i][j];
                if(rSum<rMin)   rMin=rSum,iPath=3;
            }
            /* Right Path */
            if(j>=2&&piX1[j-2]<=i_1&&piX2[j-2]>=i_1&&piX1[j_1]<=i&&piX2[j_1]>=i)
            {
                rSum=pprSum[i_1][j-2]+2.*pprD[i][j_1]+pprD[i][j];
                if(rSum<rMin)   rMin=rSum,iPath=4;
            }
            /* Right Path2 */
            if(j>=3&&j>=2&&piX1[j-3]<=i_1&&piX2[j-3]>=i_1&&piX1[j-2]<=i&&piX2[j-2]>=i&&piX1[j_1]<=i&&piX2[j_1]>=i)
            {
                rSum=pprSum[i_1][j-3]+2.*pprD[i][j-2]+pprD[i][j_1]+pprD[i][j];
                if(rSum<rMin)   rMin=rSum,iPath=5;
            }
			if(rMin==MAX_REAL)	return(MAX_REAL);
				// printf("rMin=MAX_REAL\n");
            pprSum[i][j]=rMin;
            ppiPath[i][j]=iPath;
        }
    }
    /* Now find the minimum value */
    rMin=MAX_REAL;
    for(i=piX1[nY_1];i<piX2[nY_1];i++)
    {
        /* normalizing */
        rSum=pprSum[i][nY_1]/(i+nY_1);
        if(rSum<rMin)   rMin=rSum,iPath=i,jPath=nY_1;
    }
    for(j=nY_1;j>=0;j--)
    {
        if(piX2[j]<nX_1)    break; 
        rSum=pprSum[nX_1][j]/(nX_1+j);
        /* normalizing */        
        if(rSum<rMin)   rMin=rSum,jPath=j,iPath=nX_1;
    }   

	if(!nOption) return(rMin);    
    /* Back Tracking */

    piPath[i=2]=jPath;
    piPath[i+1]=iPath;
    iPathCnt=1;
    while(iPath!=0&&jPath!=0)
    {                           
        iT=ppiPath[iPath][jPath];
        if(iT==1)
        {                      
            iPathCnt+=2;
            piPath[i+=2]=jPath,
            piPath[i+1]=--iPath;
            piPath[i+=2]=jPath,
            piPath[i+1]=--iPath;
        } 
		else if(iT==2)
		{
            iPathCnt++;
            piPath[i+=2]=jPath,
            piPath[i+1]=--iPath;
		}
        else if(iT==4)            
        {                     
            iPathCnt++;
            piPath[i+=2]=--jPath,
            piPath[i+1]=iPath;
        }            
        else if(iT==5) 
        {                     
            iPathCnt+=2;
            piPath[i+=2]=--jPath,
            piPath[i+1]=iPath;
            piPath[i+=2]=--jPath,
            piPath[i+1]=iPath;
        }            
        iPathCnt++;
        piPath[i+=2]=--jPath;
        piPath[i+1]=--iPath;
    }
    /* Revese Retrieve every points */
    iPath=(iPathCnt<<1)+2;
    for(i=0;i<iPathCnt;i++)
    {
        iT=piPath[--iPath];
        piPath[iPath]=piPath[i+2];
        piPath[i+2]=iT;
    }       
    piPath[0]=iPathCnt++; /* # of points  */
    piPath[1]=iPathCnt*sizeof(int)*2;    /* # of memsize */
                        
    freeMatrix1(ppiPath);
    freeMatrix2(pprSum);
    freeMatrix2(pprD);
    return(rMin);
}