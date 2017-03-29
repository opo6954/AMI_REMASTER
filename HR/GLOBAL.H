/***************************************************************
 	Copyright (C) 2000 AI&MM LAB, EECS, KAIST

	Author	: yhseo
	Section : 
	Description
		Global Definition

*****************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H

#define COMPORT_1	3
#define COMPORT_2	4
#define COMPORT_3	1
#define COMPORT_4	2

/////////////////////////////////////////////////////////
#define MOBILE
#define SENSOR
#define HEAD
#define ARM
#define TTS

#define NEW_SENSOR

//#define JOYSTICK
/////////////////////////////////////////////////////////

#define	MOBILE_MODE	0
#define	CAMERA_MODE	1

/////////////////////////////////////////////////////////
char sensorFile[50]="c:\\robot\\196kit\\hr\\sensor\\sense.hex";
char headFile[50]="c:\\robot\\196kit\\hr\\head\\head.hex";
char armFile[50]="c:\\robot\\196kit\\hr\\arm\\arm.hex";

char *downloadFile;

DWORD	temp;
BYTE	tempBuffer[MAX_BUFFER];
/////////////////////////////////////////////////////////
/*------------------*/
/* Head Part		*/
/*------------------*/
/* -6500 < Y <0		*/
/* -4500 < X < 4500 */
/*
	66:1 x 100P x 4 = 26400 pulse/rotation
*/

#define	HEAD_DATA_SIZE	(8+5)

LONG	headPosX=0,headPosY=-4000;
LONG	R_headPosX=0,R_headPosY=0;

/*--------------------------------------*/
/* Face Part							*/
/*--------------------------------------*/
/* 0x8 ~0x10~ 0x18						*/
/* 0=LEFT EYELID , 1=RIGHT EYELID		*/ 
/* 2=LEFT EYE , 3=RIGHT EYE , 4=MOUSE	*/
/* 0 => 6,9,10 , 1 => 15,13,C			*/
/* 2,3 => 15,10,B						*/
/* 4 =>	15,13,10,E						*/

BYTE	pwm[6]={0x06,0x15,0x10,0x10,0x15,0x10};


/*--------------------------------------*/
/* Left & Right Hand Part				*/
/*--------------------------------------*/
/*
	      --+--
 		  1   0
  --+--	
   3 2    5	  4
		  --+--
			
	[right hand ]
	open ~ close 
	0 => 7,B,10 
	1 => 9,D,10
	
	2 => 17,11,E
	3 => 15,13,10

	4 => 17,13,10
	5 => 17,12,F
	
	[left hand ]
	open ~ close 

	0 => 17,13,F 
	1 => 15,10,D
	
	2 => 9,C,F
	3 => 9,?,A

	4 => 7,C,F
	5 => 7,C,F
*/
BYTE	RightHand[6]={0x7,0x9,0x17,0x10,0x17,0x17};
BYTE	LeftHand[6]={0x17,0x15,0x9,0xA,0x7,0x7};

/*--------------------------*/
/* Mobile Part				*/
/*--------------------------*/
/*	[ ]	 [ ]  [ ]			*/
/*   G (Go) => 8,2,4,6,5	*/
/*	 P (Ping)				*/
/*   S (SPEED)				*/
#define	ALL			'0'

#define	FRONT		'8'
#define	BACK		'2'
#define	LEFT		'4'
#define	RIGHT		'6'
#define	STOP		'5'

#define	UP			'8'
#define	DOWN		'2'
#define	LEFT		'4'
#define	RIGHT		'6'
#define	CENTER		'5'

#define MAX_ACC				100
#define MAX_ROTATION_ACC	50

BYTE	mobile[3];
_int16	encoder1,encoder2;

/*--------------------------*/
/* Sensor Part				*/
/*--------------------------*/
/*	
	Infra Sensor Data 
		xxxx 11 10 9 8 | 7 6 5 4 3 2 1 

	UltraSonic Sensor Data
		40~185 (0=Not sensed,Too far!)
		max range : 30~250cm: 1m=74, 2.5m=185  Value * 0.74= cm Value 
*/
#ifdef NEW_SENSOR
#define MAX_SERIAL_RETRY_COUNT	15000
#else
#define MAX_SERIAL_RETRY_COUNT	5000
#endif

#define	MAX_SONAR_CHANNEL	18
#define	SENSOR_DATA_SIZE	25	//(2+18+5)
#define MOBILE_DATA_SIZE	9

WORD	infra;
BYTE	ultra[MAX_SONAR_CHANNEL];
BYTE	ultra_enable=0;
/*--------------------------*/
/* Arm Part					*/
/*--------------------------*/
/*	ARM: 100:1 500P x 4 = 200000 pulse/rotation
	WAIST: 50:1 200P x 4 = 40000 pulse/rotation 

	Right part				
  		0:   25000 ~ -45000
  		1:  -4700  ~ 49000
  		2:   50000 ~ -50000
  		3:  -60000 ~ 0
  		4:   85000 ~ -85000
 	Left part				
  		0:  -25000 ~ 45000
  		1:   4700  ~ -49000  
  		2:  -50000 ~ 50000
  		3:   60000 ~ 0
  		4:  -85000 ~ 85000
	(5)waist:	-5000  ~ 5000
	(순서 : 뒤,안,안,위,안,왼이 먼저)
*/	


//ARM2,3: 134:1 180P x 4 = 96480 pulse/rotation
//계산은 200000 pulse/rotation으로 하고 아래 factor를 곱해 수정
#define M_ARM2 0.4824
#define M_ARM3 0.4824

#define	ARM_DATA_SIZE	(24+5)



#define F_PI                ((double)3.141592)
#define F_PI2               ((double)1.570796)
#define F_2PI               ((double)6.283184)

/*	math.h	*/

#define rad2deg(c)	(double)((c) * (double)57.2958)
#define deg2rad(c)	(double)((c) / (double)57.2958)
//#define min(a,b)	((a) < (b) ? (a) : (b))
//#define max(a,b)	((a) > (b) ? (a) : (b))
#define ABS(a)		((a) < 0 ? (-(a)) : (a))
#define sign(a)		((a) > 0 ? 1 : ((a) == 0 ? 0 : -1))
#define sqr(a)		((a) * (a))


#define	ADJUST_360(a)	if (a > F_PI)	a -= F_2PI; else if (a < -F_PI) a += F_2PI
#define	ADJUST_180(a)	if (a > F_PI2)	a -= F_PI; else if (a < -F_PI2) a += F_PI

/*	relation between pulse and distance , CAIR-3 */
#define Lp2cm(p)		((double)(p) * RfCM_P_L)
#define Rp2cm(p)		((double)(p) * RfCM_P_R)

#define Lcm2p(c)		((double)(c) * RfP_CM_L)
#define Rcm2p(c)		((double)(c) * RfP_CM_R)

double	RfP_CM_L, RfCM_P_L, RfP_CM_R, RfCM_P_R;
double	PPR_WHEEL = 3434;
/************* Local data, types, fns and macros ****************/
#define WHEEL		7.62		/*  Radius of CAIR-3's driving wheel  	*/
#define WHEEL_L		7.62	
#define WHEEL_R		7.62
#define ROBOT_BASE	41		/*  Distance Between wheels	of CAIR-3			*/

/*
typedef struct {
	double	v,  w;	
	int		v1, v2;	
} SPEED;

typedef struct {
	double	x, y, q;
} Posture;
*/

Posture	JtRobot;
Posture	JtObject;
Speed	JtSpeed;

BYTE bCritical_Sensing=FALSE;
BYTE dir=STOP;
int  timer0_Count=0;



typedef struct {
    int x,y;
} PV;                   /* Position Vector in Image   */

struct VISION_DATA {
	int		bx,by;          // FACE x,y.
	int		minx,miny;
	int		maxx,maxy;
	int BX,BY,TX,TY;      /* Minimum box containing region */
	unsigned char empty;
	int			area;		/* Mass of Area */
	PV			MC;			/* Center of Mass  */
	short int 	BL;			/* Contour Length */
	int 		count;		/* count of processing similiar to above */
	double 		sum_dis; 	/* Sum of Distance from MC to boundary */
	double 		mean_dis;	/* Mean of Distance from MC to boundary */
	double 		variance_dis;/* Variance of Distance from MC to boundary */
	double 		mean_var;	/* mean / sqrt of variance */
	double 		periment;	/* Square of periment / Area */
	double		eccentricity;/* max_distance / min_distance */

	int 		ellipse_success; /* whether ellipse or not */
	double 		moment_rr;	/* moment rr */
	double		moment_rc;	/* moment rc */
	double		moment_cc;	/* moment cc */
	double		major_axis;	/* major axis length */
	double		minor_axis;	/* minor axis length */
	double		ellipse_area; /* area of ellipse */
	double 		ratio;		/* area / ellipse */
	double		orientation;/* orientation of major axis */
	double		dist;
};

typedef struct {
	int		v[3];		/* validity : left, right, target */
	int		x[2], y[2];	/* (x,y) coord. in image (number of pixel) */
	double	ix, iy, iz;	/* (x,y,z) in camera's coord. (cm) */
	double	rx, ry, rz;	/* (x,y,z) in robot's coord. (cm) */
	double	wx, wy, wz;	/* (x,y,z) in world coord. (cm) */
	double	th;			/* angle (rad) */
} RTarget;


VISION_DATA Vstereo[2];// OBJECT 인식 DATA
RTarget sp;
int globalTimer;



#endif