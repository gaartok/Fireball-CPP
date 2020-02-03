#ifndef __Emitter_hpp__
#define __Emitter_hpp__

#include "Windows.h"
#include <math.h>
#include "MathStuff.h"           // get the type for math structures


//#define MAX_PARTICLES		1000  // maximum number of particles



typedef struct tParticle
   {
   tParticle   *prev;
   tParticle   *next;
   tVector	   pos;              // current position
   tVector	   prevPos;          // previous position
   tVector	   dir;              // current direction with speed
   int		   life;             // how long it will last
   float       color;            // current color of particle
   float       prevColor;        // last color of particle
   float       deltaColor;       // change of color
   } tParticle;




class Emitter
	{
private:
   tParticle * firstParticle;
   tParticle * deadParticles;    // pool to pull particles from
   tParticle * aliveParticles;   // active particles
	tVector		posUL;            // XYZ position Upper Left
	tVector		posLR;            // XYZ position Lower Right
   float		   yaw;              // yaw and variation
   float		   yawVar;           // yaw and variation
   float		   pitch;            // pitch and variation
   float		   pitchVar;         // pitch and variation
   float		   speed;
   float		   speedVar;
   int         totalParticles;   // total emitted at any time
   int         particleCount;    // total emitted right now
   int         emitsPerFrame;    // emits per frame and variation
   int         emitVar;          // emits per frame and variation
   int         life;             // life count and variation
   int         lifeVar;          // life count and variation
   float       startColor;       // current color of particle
   float       startColorVar;    // current color of particle
   float       endColor;         // current color of particle
   float       endColorVar;      // current color of particle
   tVector		force;
   BOOL        antiAlias;

   void        RotationToDirection(float pitch, float yaw, tVector *direction);
   BOOL        AddParticle(void);
   BOOL        UpdateParticle(tParticle *particle);

public:
               Emitter(int numParticles);
               ~Emitter();
   void        Render(void);
   void        SetDefault(void);
   void        Update(void);
   void        SetPos(RECT *posRect);
   void        SetPos(int left, int right, int top, int bottom);
   void        SetAngles(float yawAngle, float yawVarAngle, float pitchAngle, float pitchVarAngle);
   void        SetSpeed(float newSpeed, float newSpeedVar);
   void        SetForce(float forceX, float forceY, float forceZ);
   void        SetEmits(int newEmits, int newEmitsVar);
   void        SetLife(int newLife, int newLifeVar);
   void        SetColors(int newStartColor, int newStartColorVar, int newEndColor, int newEndColorVar);
   };


#endif      // #ifndef __Emitter_hpp__
