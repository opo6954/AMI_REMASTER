//DIBitmap.H 

typedef struct _rgb
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb;

typedef struct _Cnp
{
	int width;
	int height;
	rgb *bitmap;
} Cnp;

class CDIBitmap
{

public:
	CDIBitmap();
	~CDIBitmap();

	void parse(char* filename);
	void output(char* filename);

	int GetYSize();
	int GetXSize();
	unsigned char* bFindDib();
	HGLOBAL hDIB;

	Cnp cnp;
};