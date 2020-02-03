
#ifndef MATHSTUFF_H__INCLUDED_
#define MATHSTUFF_H__INCLUDED_


#define M_PI      3.14159265358979323846
#define HALF_PI	1.57079632679489661923

#define DEGTORAD(d)	((d * (float)M_PI) / 180.0f);
#define RADTODEG(r)	((r * 180.0f) /(float)M_PI);




typedef struct
   {
	float x;
   float y;
   float z;
   } tVector;



// NOT DECLARED AS float[4][4] BECAUSE OPENGL ACCESSES THIS STRANGLY
typedef struct
   {
	float m[16];
   } tMatrix;



// SOME STRUCTURES TO HELP ME ACCESS VERTEX DATA IN AN ARRAY
typedef struct
   {
	float r,g,b;
	float x,y,z;
   } tColoredVertex;



typedef struct
   {
	float u,v;
	float x,y,z;
   } tTexturedVertex;



typedef struct
   {
	float u,v;
	float r,g,b;
	float x,y,z;
   } tTexturedColoredVertex;


/// Quaternion Definitions ////////////////////////////////////////////////////
typedef struct
   {
	float x,y,z,w;
   } tQuaternion;



#endif // !defined(MATH_H__INCLUDED_)

