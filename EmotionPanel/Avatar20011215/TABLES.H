/*
    table...ks...kssm
*/

unsigned char table_ks[]={
#include "tblks.h"
};
unsigned char table_kssm[] = {
#include "tblkssm.h"
};

struct SpecialChar
{
    unsigned char start, width;
};

SpecialChar ksSpHdr=    {   0xa1, 12};
SpecialChar ksSpBody=   {   0xa1, 0xfe-0xa1};
SpecialChar kssmSpHdr=  {   0xd9, 6};
SpecialChar kssmSpBody[2]= {
	{   0x31, 0xa0-0x31},
	{   0xa1, 0xfe-0xa1}
	};
SpecialChar ksHanjaHdr=     {   0xca, 0xfe-0xca};
SpecialChar kssmHanjaHdr=  	{	0xe0, 0xf9-0xe0};


