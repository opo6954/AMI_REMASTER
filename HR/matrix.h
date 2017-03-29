/* matrix.h - Copyright 1993 VR Lab, CS Dept, KAIST
	
   - history 
		9/18/92	created by ujsung
		4/11/99 modified by hjson
*/

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <math.h>

// ------------------------------------------------------------
// 4x4 matrix : 6DOF
// ------------------------------------------------------------

#ifndef X
#define X	0
#define Y	1
#define Z	2
#endif

#ifndef	_ABS
#define	_ABS(x)  ((x) < 0 ? -(x) : (x))
#endif	/* _ABS */

#ifndef M_PI
#define M_PI 3.141592
#endif

typedef float REAL;

class Posn3d {
protected:
	REAL m_vec[3];
public:
	// constructors, setting value
	void Set(REAL x,REAL y,REAL z) { m_vec[X]=x,m_vec[Y]=y,m_vec[Z]=z; }
	void Init() { Set(0,0,0); }
	Posn3d() { Set(0,0,0); }
	Posn3d(REAL x,REAL y,REAL z) { Set(x,y,z); }

	const REAL&	operator [](int idx) const { return m_vec[idx]; }
	REAL&	operator [](int idx) { return m_vec[idx]; }
	const Posn3d&	operator +=(const Posn3d& a); // Posn3d addition
	const Posn3d	operator +(const Posn3d& a) const; // Posn3d addition
	const Posn3d&	operator -=(const Posn3d& a); // Posn3d subtraction
	const Posn3d	operator -(const Posn3d& a) const; // Posn3d subtraction
};

class Matrix4 {
protected:
/* coloum-major matrix for OpenGL
        0  4  8 12
        1  5  9 13 
        2  6 10 14
        3  7 11 15
*/
	REAL m_el[16];

public:
	inline void Set(REAL e00,REAL e01,REAL e02, REAL e03,
		REAL e10,REAL e11,REAL e12, REAL e13,
		REAL e20,REAL e21,REAL e22, REAL e23,
		REAL e30,REAL e31,REAL e32, REAL e33) {
		m_el[0]=e00,m_el[4]=e01,m_el[8]=e02,m_el[12]=e03,
		m_el[1]=e10,m_el[5]=e11,m_el[9]=e12,m_el[13]=e13,
		m_el[2]=e20,m_el[6]=e21,m_el[10]=e22,m_el[14]=e23,
		m_el[3]=e30,m_el[7]=e31,m_el[11]=e32,m_el[15]=e33;
	}
	inline	Matrix4& Identity(); 
	Matrix4() { Set(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1); }
	Matrix4(
		REAL e00,REAL e01,REAL e02, REAL e03,
		REAL e10,REAL e11,REAL e12, REAL e13,
		REAL e20,REAL e21,REAL e22, REAL e23,
		REAL e30,REAL e31,REAL e32, REAL e33) {
		Set(e00,e01,e02,e03,e10,e11,e12,e13,
			e20,e21,e22,e23,e30,e31,e32,e33);
	}

	const	REAL&	operator [](int idx) const {return m_el[idx];}
	REAL&	operator [](int idx) {return m_el[idx];}
	
	inline void FromPosition(const Posn3d& p);
	inline void FromPosition(REAL x,REAL y,REAL z);
	inline void FromScale(const Posn3d& p); // Added by likidas
	inline void FromScale(REAL sx,REAL sy,REAL sz); // Added by hjson
	inline void FromAngle(REAL angle,int rx,int ry,int z); // angle is Euler, added by hjson

	Matrix4(const Posn3d& p) { FromPosition(p); }

	const Matrix4	Transpose(); // transpose, non-destructive, return separate copy
	float Determinant();
	const Matrix4	InverseTR(); // inverse of TR matrix
	const Matrix4&	operator +=(const Matrix4& a); // matrix addition
	const Matrix4&	operator -=(const Matrix4& a); // matrix subtraction
	const Matrix4&	operator *=(const Matrix4& a); // matrix multiplication
	const Matrix4	operator +(const Matrix4& a) const; // matrix addition
	const Matrix4	operator -(const Matrix4& a) const; // matrix subtraction
	const Matrix4	operator *(const Matrix4& a) const; // matrix multiplication
	const Posn3d	operator *=(const Posn3d& a) const;
	const Posn3d	operator *(const Posn3d& a) const;

	operator	REAL*() { return m_el; }
	void operator=(REAL s) { for(int i=0;i<16;i++) m_el[i] = s; }

	#define ELEM4(i,j) m_el[((i)|(j)<<2)]
};

// Posn3d

inline const Posn3d& Posn3d::operator+=(const Posn3d& a)
{
	Set(m_vec[X]+a[X],m_vec[Y]+a[Y],m_vec[Z]+a[Z]);
	return *this;
}

inline const Posn3d Posn3d::operator+(const Posn3d& a) const
{
	return Posn3d(*this)+=a;
}

inline const Posn3d& Posn3d::operator-=(const Posn3d& a)
{
	Set(m_vec[X]-a[X],m_vec[Y]-a[Y],m_vec[Z]-a[Z]);
	return *this;
}

inline const Posn3d Posn3d::operator-(const Posn3d& a) const
{
	return Posn3d(*this)-=a;
}

// Matrix4

inline Matrix4& Matrix4::Identity()
{
	m_el[0]=m_el[5]=m_el[10]=m_el[15]=1;
	m_el[1]=m_el[2]=m_el[3]=m_el[4]=
	m_el[6]=m_el[7]=m_el[8]=m_el[9]=
	m_el[11]=m_el[12]=m_el[13]=m_el[14]=0;
	return *this;
}

inline const Matrix4 Matrix4::Transpose()
{
	return Matrix4(m_el[0], m_el[1], m_el[ 2], m_el[3],
		m_el[4], m_el[5], m_el[6], m_el[7],
		m_el[8], m_el[9], m_el[10], m_el[11],
		m_el[12], m_el[13], m_el[14], m_el[15]);
}

inline float Matrix4::Determinant()
{
        return (
     - ELEM4(0,0)*ELEM4(1,1)*ELEM4(2,2)*ELEM4(3,3) +
ELEM4(0,0)*ELEM4(1,1)*ELEM4(2,3)*ELEM4(3,2) +
ELEM4(0,0)*ELEM4(2,1)*ELEM4(1,2)*ELEM4(3,3)
     - ELEM4(0,0)*ELEM4(2,1)*ELEM4(1,3)*ELEM4(3,2) -
ELEM4(0,0)*ELEM4(3,1)*ELEM4(1,2)*ELEM4(2,3) +
ELEM4(0,0)*ELEM4(3,1)*ELEM4(1,3)*ELEM4(2,2)
     + ELEM4(1,0)*ELEM4(0,1)*ELEM4(2,2)*ELEM4(3,3) -
ELEM4(1,0)*ELEM4(0,1)*ELEM4(2,3)*ELEM4(3,2) -
ELEM4(1,0)*ELEM4(2,1)*ELEM4(0,2)*ELEM4(3,3)
     + ELEM4(1,0)*ELEM4(2,1)*ELEM4(0,3)*ELEM4(3,2) +
ELEM4(1,0)*ELEM4(3,1)*ELEM4(0,2)*ELEM4(2,3) -
ELEM4(1,0)*ELEM4(3,1)*ELEM4(0,3)*ELEM4(2,2)
     - ELEM4(2,0)*ELEM4(0,1)*ELEM4(1,2)*ELEM4(3,3) +
ELEM4(2,0)*ELEM4(0,1)*ELEM4(1,3)*ELEM4(3,2) +
ELEM4(2,0)*ELEM4(1,1)*ELEM4(0,2)*ELEM4(3,3)
     - ELEM4(2,0)*ELEM4(1,1)*ELEM4(0,3)*ELEM4(3,2) -
ELEM4(2,0)*ELEM4(3,1)*ELEM4(0,2)*ELEM4(1,3) +
ELEM4(2,0)*ELEM4(3,1)*ELEM4(0,3)*ELEM4(1,2)
     + ELEM4(3,0)*ELEM4(0,1)*ELEM4(1,2)*ELEM4(2,3) -
ELEM4(3,0)*ELEM4(0,1)*ELEM4(1,3)*ELEM4(2,2) -
ELEM4(3,0)*ELEM4(1,1)*ELEM4(0,2)*ELEM4(2,3)
     + ELEM4(3,0)*ELEM4(1,1)*ELEM4(0,3)*ELEM4(2,2) +
ELEM4(3,0)*ELEM4(2,1)*ELEM4(0,2)*ELEM4(1,3) -
ELEM4(3,0)*ELEM4(2,1)*ELEM4(0,3)*ELEM4(1,2)
    );
}

inline const Matrix4 Matrix4::InverseTR()
{
        float determinant = Determinant();
        Matrix4 inverseMatrix;
 
        inverseMatrix[0] = - ( ELEM4(1,1)*ELEM4(2,2)*ELEM4(3,3) -
ELEM4(1,1)*ELEM4(2,3)*ELEM4(3,2) - ELEM4(2,1)*ELEM4(1,2)*ELEM4(3,3) +
ELEM4(2,1)*ELEM4(1,3)*ELEM4(3,2) + ELEM4(3,1)*ELEM4(1,2)*ELEM4(2,3) -
ELEM4(3,1)*ELEM4(1,3)*ELEM4(2,2))/determinant;
        inverseMatrix[4] =   ( ELEM4(0,1)*ELEM4(2,2)*ELEM4(3,3) -
ELEM4(0,1)*ELEM4(2,3)*ELEM4(3,2) - ELEM4(2,1)*ELEM4(0,2)*ELEM4(3,3) +
ELEM4(2,1)*ELEM4(0,3)*ELEM4(3,2) + ELEM4(3,1)*ELEM4(0,2)*ELEM4(2,3) -
ELEM4(3,1)*ELEM4(0,3)*ELEM4(2,2))/determinant;
        inverseMatrix[8] = - ( ELEM4(0,1)*ELEM4(1,2)*ELEM4(3,3) -
ELEM4(0,1)*ELEM4(1,3)*ELEM4(3,2) - ELEM4(1,1)*ELEM4(0,2)*ELEM4(3,3) +
ELEM4(1,1)*ELEM4(0,3)*ELEM4(3,2) + ELEM4(3,1)*ELEM4(0,2)*ELEM4(1,3) -
ELEM4(3,1)*ELEM4(0,3)*ELEM4(1,2))/determinant;
        inverseMatrix[12] =   ( ELEM4(0,1)*ELEM4(1,2)*ELEM4(2,3) -
ELEM4(0,1)*ELEM4(1,3)*ELEM4(2,2) - ELEM4(1,1)*ELEM4(0,2)*ELEM4(2,3) +
ELEM4(1,1)*ELEM4(0,3)*ELEM4(2,2) + ELEM4(2,1)*ELEM4(0,2)*ELEM4(1,3) -
ELEM4(2,1)*ELEM4(0,3)*ELEM4(1,2))/determinant;
 
        inverseMatrix[1] =   ( ELEM4(1,0)*ELEM4(2,2)*ELEM4(3,3) -
ELEM4(1,0)*ELEM4(2,3)*ELEM4(3,2) - ELEM4(2,0)*ELEM4(1,2)*ELEM4(3,3) +
ELEM4(2,0)*ELEM4(1,3)*ELEM4(3,2) + ELEM4(3,0)*ELEM4(1,2)*ELEM4(2,3) -
ELEM4(3,0)*ELEM4(1,3)*ELEM4(2,2))/determinant;
        inverseMatrix[5] = - ( ELEM4(0,0)*ELEM4(2,2)*ELEM4(3,3) -
ELEM4(0,0)*ELEM4(2,3)*ELEM4(3,2) - ELEM4(2,0)*ELEM4(0,2)*ELEM4(3,3) +
ELEM4(2,0)*ELEM4(0,3)*ELEM4(3,2) + ELEM4(3,0)*ELEM4(0,2)*ELEM4(2,3) -
ELEM4(3,0)*ELEM4(0,3)*ELEM4(2,2))/determinant;
        inverseMatrix[9] =   ( ELEM4(0,0)*ELEM4(1,2)*ELEM4(3,3) -
ELEM4(0,0)*ELEM4(1,3)*ELEM4(3,2) - ELEM4(1,0)*ELEM4(0,2)*ELEM4(3,3) +
ELEM4(1,0)*ELEM4(0,3)*ELEM4(3,2) + ELEM4(3,0)*ELEM4(0,2)*ELEM4(1,3) -
ELEM4(3,0)*ELEM4(0,3)*ELEM4(1,2))/determinant;
        inverseMatrix[13] = - ( ELEM4(0,0)*ELEM4(1,2)*ELEM4(2,3) -
ELEM4(0,0)*ELEM4(1,3)*ELEM4(2,2) - ELEM4(1,0)*ELEM4(0,2)*ELEM4(2,3) +
ELEM4(1,0)*ELEM4(0,3)*ELEM4(2,2) + ELEM4(2,0)*ELEM4(0,2)*ELEM4(1,3) -
ELEM4(2,0)*ELEM4(0,3)*ELEM4(1,2))/determinant;
 
        inverseMatrix[2] = - ( ELEM4(1,0)*ELEM4(2,1)*ELEM4(3,3) -
ELEM4(1,0)*ELEM4(2,3)*ELEM4(3,1) - ELEM4(2,0)*ELEM4(1,1)*ELEM4(3,3) +
ELEM4(2,0)*ELEM4(1,3)*ELEM4(3,1) + ELEM4(3,0)*ELEM4(1,1)*ELEM4(2,3) -
ELEM4(3,0)*ELEM4(1,3)*ELEM4(2,1))/determinant;
        inverseMatrix[6] = - (-ELEM4(0,0)*ELEM4(2,1)*ELEM4(3,3) +
ELEM4(0,0)*ELEM4(2,3)*ELEM4(3,1) + ELEM4(2,0)*ELEM4(0,1)*ELEM4(3,3) -
ELEM4(2,0)*ELEM4(0,3)*ELEM4(3,1) - ELEM4(3,0)*ELEM4(0,1)*ELEM4(2,3) +
ELEM4(3,0)*ELEM4(0,3)*ELEM4(2,1))/determinant;
        inverseMatrix[10] =   (-ELEM4(0,0)*ELEM4(1,1)*ELEM4(3,3) +
ELEM4(0,0)*ELEM4(1,3)*ELEM4(3,1) + ELEM4(1,0)*ELEM4(0,1)*ELEM4(3,3) -
ELEM4(1,0)*ELEM4(0,3)*ELEM4(3,1) - ELEM4(3,0)*ELEM4(0,1)*ELEM4(1,3) +
ELEM4(3,0)*ELEM4(0,3)*ELEM4(1,1))/determinant;
        inverseMatrix[14] = - (-ELEM4(0,0)*ELEM4(1,1)*ELEM4(2,3) +
ELEM4(0,0)*ELEM4(1,3)*ELEM4(2,1) + ELEM4(1,0)*ELEM4(0,1)*ELEM4(2,3) -
ELEM4(1,0)*ELEM4(0,3)*ELEM4(2,1) - ELEM4(2,0)*ELEM4(0,1)*ELEM4(1,3) +
ELEM4(2,0)*ELEM4(0,3)*ELEM4(1,1))/determinant;
 
        inverseMatrix[3] =   ( ELEM4(1,0)*ELEM4(2,1)*ELEM4(3,2) -
ELEM4(1,0)*ELEM4(2,2)*ELEM4(3,1) - ELEM4(2,0)*ELEM4(1,1)*ELEM4(3,2) +
ELEM4(2,0)*ELEM4(1,2)*ELEM4(3,1) + ELEM4(3,0)*ELEM4(1,1)*ELEM4(2,2) -
ELEM4(3,0)*ELEM4(1,2)*ELEM4(2,1))/determinant;
        inverseMatrix[7] =   ( ELEM4(2,0)*ELEM4(0,1)*ELEM4(3,2) +
ELEM4(0,0)*ELEM4(2,2)*ELEM4(3,1) - ELEM4(2,0)*ELEM4(0,2)*ELEM4(3,1) -
ELEM4(0,0)*ELEM4(2,1)*ELEM4(3,2) + ELEM4(3,0)*ELEM4(0,2)*ELEM4(2,1) -
ELEM4(3,0)*ELEM4(0,1)*ELEM4(2,2))/determinant;
        inverseMatrix[11] = - (-ELEM4(0,0)*ELEM4(1,1)*ELEM4(3,2) +
ELEM4(0,0)*ELEM4(1,2)*ELEM4(3,1) + ELEM4(1,0)*ELEM4(0,1)*ELEM4(3,2) -
ELEM4(1,0)*ELEM4(0,2)*ELEM4(3,1) - ELEM4(3,0)*ELEM4(0,1)*ELEM4(1,2) +
ELEM4(3,0)*ELEM4(0,2)*ELEM4(1,1))/determinant;
        inverseMatrix[15] =   (-ELEM4(0,0)*ELEM4(1,1)*ELEM4(2,2) +
ELEM4(0,0)*ELEM4(1,2)*ELEM4(2,1) + ELEM4(1,0)*ELEM4(0,1)*ELEM4(2,2) -
ELEM4(1,0)*ELEM4(0,2)*ELEM4(2,1) - ELEM4(2,0)*ELEM4(0,1)*ELEM4(1,2) +
ELEM4(2,0)*ELEM4(0,2)*ELEM4(1,1))/determinant;
 
        return inverseMatrix;
/*	return Matrix4(m_el[0],m_el[1],m_el[2],-(m_el[0]*m_el[12]+m_el[1]*m_el[13]+m_el[2]*m_el[14]),
		m_el[4],m_el[5],m_el[6],-(m_el[4]*m_el[12]+m_el[5]*m_el[13]+m_el[6]*m_el[14]),
		m_el[8],m_el[9],m_el[10],-(m_el[8]*m_el[12]+m_el[9]*m_el[13]+m_el[10]*m_el[14]),
		0,0,0,1);*/
}

inline const Matrix4& Matrix4::operator+=(const Matrix4& a)
{
	Set(m_el[0]+a.m_el[0],m_el[4]+a.m_el[4],m_el[ 8]+a.m_el[ 8],m_el[12]+a.m_el[12],
		m_el[1]+a.m_el[1],m_el[5]+a.m_el[5],m_el[ 9]+a.m_el[ 9],m_el[13]+a.m_el[13],
		m_el[2]+a.m_el[2],m_el[6]+a.m_el[6],m_el[10]+a.m_el[10],m_el[14]+a.m_el[14],
		m_el[3]+a.m_el[3],m_el[7]+a.m_el[7],m_el[11]+a.m_el[11],m_el[15]+a.m_el[15]);
	return *this;
}

inline const Matrix4 Matrix4::operator+(const Matrix4& a) const
{
	return Matrix4(*this)+=a;
}

inline const Matrix4& Matrix4::operator-=(const Matrix4& a)
{
	Set(m_el[0]-a.m_el[0],m_el[4]-a.m_el[4],m_el[ 8]-a.m_el[ 8],m_el[12]-a.m_el[12],
		m_el[1]-a.m_el[1],m_el[5]-a.m_el[5],m_el[ 9]-a.m_el[ 9],m_el[13]-a.m_el[13],
		m_el[2]-a.m_el[2],m_el[6]-a.m_el[6],m_el[10]-a.m_el[10],m_el[14]-a.m_el[14],
		m_el[3]-a.m_el[3],m_el[7]-a.m_el[7],m_el[11]-a.m_el[11],m_el[15]-a.m_el[15]);
	return *this;
}

inline const Matrix4 Matrix4::operator-(const Matrix4& a) const
{
	return Matrix4(*this)-=a;
}

inline const Matrix4& Matrix4::operator*=(const Matrix4& a)
{
	int i,j,k,l;
	REAL elem[16];
	REAL sum;

	for(l=k=0;k<4;k++)
		for(j=0;j<4;j++) {
			for(sum=0,i=0;i<4;i++) {
				sum += ELEM4(j,i) * a.ELEM4(i,k);
			}
			elem[l++] = sum;
		}
	for(i=0;i<16;i++) m_el[i] = elem[i];

	return *this;
}

inline const Matrix4 Matrix4::operator*(const Matrix4& a) const
{
	return Matrix4(*this)*=a;
}

inline const Posn3d Matrix4::operator*=(const Posn3d& a) const
{
	return Posn3d(m_el[0]*a[X]+m_el[4]*a[Y]+m_el[8]*a[Z]+m_el[12],
		m_el[1]*a[X]+m_el[5]*a[Y]+m_el[9]*a[Z]+m_el[13],
		m_el[2]*a[X]+m_el[6]*a[Y]+m_el[10]*a[Z]+m_el[14]);
}

inline const Posn3d Matrix4::operator*(const Posn3d& a) const // v' = M x v
{ 
	//return Matrix4(*this)*=a;
	return Posn3d(m_el[0]*a[X]+m_el[4]*a[Y]+m_el[8]*a[Z]+m_el[12],
	m_el[1]*a[X]+m_el[5]*a[Y]+m_el[9]*a[Z]+m_el[13],
	m_el[2]*a[X]+m_el[6]*a[Y]+m_el[10]*a[Z]+m_el[14]);
}

inline void Matrix4::FromPosition(const Posn3d& p)
{
	Identity();
	m_el[12] = p[X],m_el[13] = p[Y],m_el[14] = p[Z];
}

inline void Matrix4::FromPosition(REAL x,REAL y,REAL z)
{
	Identity();
	m_el[12] = x, m_el[13] = y, m_el[14] = z;
}

inline void Matrix4::FromScale(const Posn3d& p)
{
	Identity();
	m_el[0] = p[0],m_el[5] = p[1],m_el[10] = p[2];
}

inline void Matrix4::FromScale(REAL sx,REAL sy,REAL sz)
{
	Identity();
	m_el[0] = sx, m_el[5] = sy, m_el[10] = sz;
}

inline void Matrix4::FromAngle(REAL angle,int x,int y,int z)
{
	double rad = M_PI*angle/(double)180.0;
	REAL cosine,sine;

	Identity();
	cosine = (REAL)cos(rad); sine = (REAL)sin(rad);
	if(z) {
		m_el[0] = m_el[5] = cosine;
		m_el[1] = sine; m_el[4] = -sine;
	} else if(x) {
		m_el[5] = m_el[10] = cosine;
		m_el[6] = sine; m_el[9] = -sine;
	} else {
		m_el[0] = m_el[10] = cosine;
		m_el[2] = -sine; m_el[8] = sine;
	}
}

#endif

