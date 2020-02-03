
#include "Emitter.hpp"
#include "Misc.hpp"
#include "global.h"
#include "debug.h"
#include <time.h>


const int HALF_RAND = (RAND_MAX / 2);

char particleArray[2][2];


float RandomNum()
   {
	int rn;
	rn = rand();
	return ((float)(rn - HALF_RAND) / (float)HALF_RAND);
   }


Emitter :: Emitter(int numParticles)
   {
	int loop;

//   sizeof(tParticle) = 60
//   dprintf("sizeof(tParticle) = %d\n", sizeof(tParticle));
//   dprintf("sizeof(particleArray) = %d\n", sizeof(particleArray));

   deadParticles = (tParticle *)malloc(numParticles * sizeof(tParticle));
   if (!deadParticles)
      {
      dprintf("Out of memory!!\n");
      theApp->Stop();
      }

   firstParticle = deadParticles;

	// this is a linked list of particles, so i need to establish links
   for (loop = 1; loop < numParticles - 1; loop++)
      {
      deadParticles[loop].prev = &deadParticles[loop - 1];
      deadParticles[loop].next = &deadParticles[loop + 1];
      }

   deadParticles[0].prev = NULL;
   deadParticles[0].next = &deadParticles[1];
   deadParticles[numParticles - 1].prev = &deadParticles[numParticles - 2];
   deadParticles[numParticles - 1].next = NULL;

   totalParticles = numParticles;
	particleCount	= 0;
   SetDefault();

   aliveParticles = NULL;     // start with no alive particles
   }



Emitter :: ~Emitter()
   {
	delete firstParticle;
   }



// Function:	RotationToDirection
// Purpose:		Convert a Yaw and Pitch to a direction vector
void Emitter :: RotationToDirection(float pitch, float yaw, tVector *direction)
   {
	direction->x = (float)(-sin(yaw) * cos(pitch));
	direction->y = (float)sin(pitch);
	direction->z = (float)(cos(pitch) * cos(yaw));
   }



void Emitter :: SetPos(RECT *posRect)
   {
	posUL.x = (float)posRect->left;
	posUL.y = (float)posRect->top;
	posUL.z = 0;
	posLR.x = (float)posRect->right;
	posLR.y = (float)posRect->bottom;
	posLR.z = 0;
   }


void Emitter :: SetPos(int left, int right, int top, int bottom)
   {
	posUL.x = (float)left;
	posUL.y = (float)top;
	posUL.z = 0;
	posLR.x = (float)right;
	posLR.y = (float)bottom;
	posLR.z = 0;
   }


void Emitter :: SetAngles(float yawAngle, float yawVarAngle, float pitchAngle, float pitchVarAngle)
   {
	yaw      = DEGTORAD(yawAngle);
	yawVar   = DEGTORAD(yawVarAngle);
	pitch    = DEGTORAD(pitchAngle);
	pitchVar = DEGTORAD(pitchVarAngle);
   }



void Emitter :: SetSpeed(float newSpeed, float newSpeedVar)
   {
	speed    = newSpeed;
	speedVar = newSpeedVar;
   }


void Emitter :: SetForce(float forceX, float forceY, float forceZ)
   {
	force.x  = forceX;
	force.y  = forceY;
	force.z  = forceZ;
   }



void Emitter :: SetEmits(int newEmits, int newEmitsVar)
   {
   emitsPerFrame  = newEmits;
   emitVar        = newEmitsVar;
   }



void Emitter :: SetLife(int newLife, int newLifeVar)
   {
   life     = newLife;
   lifeVar  = newLifeVar;
   }



void Emitter :: SetColors(int newStartColor, int newStartColorVar, int newEndColor, int newEndColorVar)
   {
	startColor     = (float)newStartColor;
	startColorVar  = (float)newStartColorVar;
	endColor       = (float)newEndColor;
	endColorVar    = (float)newEndColorVar;
   }


void Emitter :: SetDefault(void)
   {
	posUL.x = 225.0f; // XYZ POSITION Upper Left
	posUL.y = 120.0f; // XYZ POSITION Upper Left
	posUL.z = 0.0f;   // XYZ POSITION Upper Left
	posLR.x = 320.0f; // XYZ POSITION Upper Left
	posLR.y = 120.0f; // XYZ POSITION Upper Left
	posLR.z = 50.0f;  // XYZ POSITION Upper Left

	yaw      = DEGTORAD(0.0f);
	yawVar   = DEGTORAD(360.0f);
	pitch    = DEGTORAD(270.0f);
	pitchVar = DEGTORAD(40.0f);

	speed    = 3.0f;
	speedVar = 0.5f;

	force.x        = 0.000f;
	force.y        = 0.04f;
	force.z        = 0.0f;

	emitsPerFrame	= 2;
	emitVar	      = 3;

	life           = 180;
	lifeVar        = 15;

	startColor     = 7.0f;
	startColorVar  = 1.0f;
	endColor       = 1.0f;
	endColorVar    = 1.0f;
   }




BOOL Emitter :: AddParticle(void)
   {
	tParticle * thisParticle;
	float       thisStart;
   float       thisEnd;
	float       thisYaw;
   float       thisPitch;
   float       thisSpeed;

	if ((deadParticles != NULL) && (particleCount < totalParticles) && (life > 0))
	   {
      thisParticle  = deadParticles;           // the current particle 
      deadParticles = deadParticles->next;     // fix the pool pointers
      if (deadParticles != NULL)
         deadParticles->prev = NULL;

      if (aliveParticles != NULL)
         aliveParticles->prev = thisParticle;   // set back link

      thisParticle->next = aliveParticles;   // set its next pointer
      thisParticle->prev = NULL;             // it has no back pointer
      aliveParticles = thisParticle;         // set it in the emitter

      thisParticle->pos.x = randFloat(posUL.x, posLR.x);
      thisParticle->pos.y = randFloat(posUL.y, posLR.y);
      thisParticle->pos.z = randFloat(posUL.z, posLR.z);

      thisParticle->prevPos.x = thisParticle->pos.x;
      thisParticle->prevPos.y = thisParticle->pos.y;
      thisParticle->prevPos.z = thisParticle->pos.z;

      // calculate the starting direction vector
      thisYaw = yaw + (yawVar * RandomNum());
      thisPitch = pitch + (pitchVar * RandomNum());

      // convert the rotations to a vector
      RotationToDirection(thisPitch, thisYaw, &thisParticle->dir);
		
      // multiply in the speed factor
      thisSpeed = speed + (speedVar * RandomNum());
      thisParticle->dir.x *= thisSpeed;
      thisParticle->dir.y *= thisSpeed;
      thisParticle->dir.z *= thisSpeed;

      thisStart = startColor + (startColorVar * randFloat(0, 1));
      thisEnd   = endColor   + (endColorVar   * randFloat(0, 1));
      thisParticle->color = thisStart;
      thisParticle->life = life + (int)((float)lifeVar * RandomNum());
      thisParticle->deltaColor = (thisEnd - thisStart) / (float)thisParticle->life;
      particleCount++;     // a new particle is born

      return TRUE;
      }

	return FALSE;
   }




///////////////////////////////////////////////////////////////////////////////
// Function:	updateParticle
// Purpose:		updateParticle settings
// Arguments:	The particle to update and the emitter it came from
///////////////////////////////////////////////////////////////////////////////
BOOL Emitter :: UpdateParticle(tParticle *thisParticle)
   {
   // IF THIS IS AN VALID PARTICLE
	if ((thisParticle != NULL) && (thisParticle->life > 0))
	   {
		// SAVE ITS OLD POS FOR ANTI ALIASING
		thisParticle->prevPos.x = thisParticle->pos.x;
		thisParticle->prevPos.y = thisParticle->pos.y;
		thisParticle->prevPos.z = thisParticle->pos.z;

		// CALCULATE THE NEW
		thisParticle->pos.x += thisParticle->dir.x;
		thisParticle->pos.y += thisParticle->dir.y;
		thisParticle->pos.z += thisParticle->dir.z;

		// APPLY GLOBAL FORCE TO DIRECTION
		thisParticle->dir.x += force.x;
		thisParticle->dir.y += force.y;
		thisParticle->dir.z += force.z;

		// SAVE THE OLD COLOR
		thisParticle->prevColor = thisParticle->color;

		// GET THE NEW COLOR
		thisParticle->color += thisParticle->deltaColor;

		thisParticle->life--;	// IT IS A CYCLE OLDER
		return TRUE;
	   }
	else if (thisParticle != NULL && thisParticle->life == 0)  // free this sucker up back to the main pool
	   {
		if (thisParticle->prev != NULL)
			thisParticle->prev->next = thisParticle->next;
      else  
         aliveParticles = thisParticle->next;

		if (thisParticle->next != NULL)
			thisParticle->next->prev = thisParticle->prev;

      thisParticle->next = deadParticles;
      thisParticle->prev = NULL;

      if (deadParticles != NULL)
         deadParticles->prev = thisParticle;

      deadParticles = thisParticle;  // new pool pointer

      if (particleCount > 0)
         particleCount--;           // add one to pool
	   }

	return FALSE;
   }




///////////////////////////////////////////////////////////////////////////////
// Function:	updateEmitter
// Purpose:		updateEmitter setting
// Arguments:	The Emitter to update
// Notes:		This is called once per frame to update the emitter
///////////////////////////////////////////////////////////////////////////////
void Emitter :: Update(void)
   {
	int         loop;
   int         emits;
	tParticle * thisParticle;
   tParticle * next;

   if (aliveParticles != NULL)
      {
      // GO THROUGH THE PARTICLES AND UPDATE THEM
      thisParticle = aliveParticles;
      while (thisParticle)
         {
         next = thisParticle->next;
         UpdateParticle(thisParticle);
         thisParticle = next;
         }
      }

   // don't add new particles if life == 0
   if (life == 0)
      return;

   // EMIT PARTICLES FOR THIS FRAME
   emits = emitsPerFrame + (int)((float)emitVar * RandomNum());
   for (loop = 0; loop < emits; loop++)
      AddParticle();
 
   }





///////////////////////////////////////////////////////////////////////////////
// Function:	renderEmitter
// Purpose:		render particle system
// Arguments:	The Emitter to render
// Notes:		This is called once per frame to render the emitter
///////////////////////////////////////////////////////////////////////////////
void Emitter :: Render(void)
   {
	tParticle * thisParticle;

   if (aliveParticles != NULL)
      {
      thisParticle = aliveParticles;

      renderer->LockSurface();
      while (thisParticle)
         {
//         if (antiAlias)
//            {
//            glColor3f(particle->prevColor.r, particle->prevColor.g, particle->prevColor.b);
//            glVertex3f(particle->prevPos.x,particle->prevPos.y,particle->prevPos.z);
//            }

         memset(particleArray, (int)thisParticle->color, sizeof(particleArray));
         renderer->DrawArray((char *)particleArray, 2, 2, (int)thisParticle->pos.x, (int)thisParticle->pos.y);
         thisParticle = thisParticle->next;
         }

      renderer->UnlockSurface();
      }
   }
