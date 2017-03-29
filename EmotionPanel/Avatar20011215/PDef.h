class CVertexNode : public CObject
{
public:
	float x;
	float y;
	float z;
};

class CInitialVertexNode : public CObject
{
public:
	float x;
	float y;
	float z;

	CVertexNode NormalVector;
	
	int VectorCount;

	float backupx;
	float backupy;
	float backupz;
};

class CVertexIndexNode : public CObject
{
public:
	int index;
};

class CFaceNode : public CObject
{
public:
   int group;
   CObList* VertexIndexList;
	CVertexNode NormalVector;
};

struct SliderStatus
{
	float mouth_open;
	float liptail_up;
	float liptail_down;
	float lip_longer;
	float lip_tighten_o;
	float lip_tighten_u;
	float lip_open;
	float eyetail_up;
	float eye_close;
	float eyebrow_up;
	float eyebrow_glower;
};

class CFrameData : public CObject
{
public:
	int framenumber;

	float mouth_open;
	float liptail_up;
	float liptail_down;
	float lip_longer;
	float lip_tighten_o;
	float lip_tighten_u;
	float lip_open;
	float eyetail_up;
	float eye_close;
	float eyebrow_up;
	float eyebrow_glower;

	float xRot;
	float yRot;
	float scale;

	float xPos;
	float yPos;
};



