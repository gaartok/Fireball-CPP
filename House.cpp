
#include "House.hpp"
#include "Misc.hpp"
#include "global.h"
#include "debug.h"



House :: House()
   {
   type   = TYPE_HOUSE;
   action = ACTION_IDLE_RIGHT;

   velocity.x        = 0;
   velocity.y        = 0;
   minVelocity.x     = 0;
   minVelocity.y     = 0;
   maxVelocity.x     = 0;
   maxVelocity.y     = 0;

//   surfaceID = renderer->NewSurface(192, 256, "House");
   surfaceID = renderer->NewSurfaceFromResource("HOUSE");

//   videoSurface[ACTION_IDLE_RIGHT]     = surfaceDescr[SURFACE_HOUSE].videoSurface;
//   memSurface[ACTION_IDLE_RIGHT]       = surfaceDescr[SURFACE_HOUSE].memSurface;
   numFrames[ACTION_IDLE_RIGHT].x      = 5;
   numFrames[ACTION_IDLE_RIGHT].y      = 1;
   size[ACTION_IDLE_RIGHT].x           = 32;
   size[ACTION_IDLE_RIGHT].y           = 128;
   bmpOffset[ACTION_IDLE_RIGHT].x      = 0;
   bmpOffset[ACTION_IDLE_RIGHT].y      = 0;

//   videoSurface[ACTION_IDLE_LEFT]      = surfaceDescr[SURFACE_HOUSE].videoSurface;
//   memSurface[ACTION_IDLE_LEFT]        = surfaceDescr[SURFACE_HOUSE].memSurface;
   numFrames[ACTION_IDLE_LEFT].x       = 5;
   numFrames[ACTION_IDLE_LEFT].y       = 1;
   size[ACTION_IDLE_LEFT].x            = 32;
   size[ACTION_IDLE_LEFT].y            = 128;
   bmpOffset[ACTION_IDLE_LEFT].x       = 0;
   bmpOffset[ACTION_IDLE_LEFT].y       = 128;

   memset(fireArray, 1, sizeof(fireArray));
//   memset(fireArray, 1 << 4, sizeof(fireArray));
   nextFlickerTime = timeGetTime() + 20;
   lastHealth = 100;
   Initialize();

   faucet = new Emitter(30);
//   dprintf("faucet = %x\n", faucet);
   if (!faucet)
      {
      dprintf("Out of memory!!\n");
      theApp->Stop();
      }

//   faucet->SetAngles(90, 10, 0, 0);    // left
//   faucet->SetAngles(0, 1, 0, 0);      // down
//   faucet->SetAngles(270, 10, 0, 0);   // right
//   faucet->SetAngles(0, 5, 90, 5);     // down
//   faucet->SetAngles(90, 5, 0, 5);     // left
//   faucet->SetAngles(0, 5, 0, 5);      // blobs around position
//   faucet->SetAngles(180, 5, 0, 5);    // blobs around position
//   faucet->SetAngles(90, 5, 90, 5);    // down
//   faucet->SetAngles(45, 5, 45, 5);    // south-west
//   faucet->SetAngles(90, 5, 45, 5);    // south-west
//   faucet->SetAngles(0, 5, 270, 5);    // up

   faucet->SetAngles(90, 1, 90, 1);    // down
   faucet->SetSpeed(1.0f, 0.1f);
   faucet->SetForce(0.0f, 0.0f, 0.0f);
//   faucet->SetForce(0.0f, 0.01f, 0.0f);
   faucet->SetEmits(1, 1);
   faucet->SetLife(30, 3);
   faucet->SetColors(48, 0, 48, 0);

   hitSound[0]   = SndObjCreate(ds, "BANG",  1);
   hitSound[1]   = SndObjCreate(ds, "BOOM",  1);
   hitSound[2]   = SndObjCreate(ds, "EXPLOSINO",  1);
   hitSound[3]   = SndObjCreate(ds, "GLASS",  1);
   hitSound[4]   = SndObjCreate(ds, "GLASSBRK",  1);
   }



House :: ~House()
   {
   int   count1;

   PutOutFires();

   if (faucet)
      delete faucet;

   for (count1 = 0; count1 < NUM_HOUSE_HIT_SOUNDS; count1++)
      {
      SndObjDestroy(hitSound[count1]);
      hitSound[count1] = NULL;
      }
   }



void House :: Hit(int damage)
   {
   int   whichSound;

   AliveObject::Hit(damage);

   whichSound = randInt(0, NUM_HOUSE_HIT_SOUNDS - 1);
   SndObjPlay(hitSound[whichSound], 0);
   }




void House :: NewLevel(int level)
   {
   SetHealth(100);
   PutOutFires();
   }



void House :: PutOutFires(void)
   {
   Emitter *   currentObject;

   while (!flameList.empty())
      {
      currentObject = (Emitter *)flameList.front();
      flameList.pop_front();
      delete currentObject;
      }
   }




void House :: SetHealth(int newHealth)
   {
   if ((newHealth >= 0) && (newHealth <= 100))
      {
      health = newHealth;
      lastHealth = health;
      }
   }




void House :: GetNextFrame(void)
   {
   frameIndex.x = 4 - (health / 25);

   nextFrame         = firstFrame[action];
   nextFrame.top    += size[action].y * frameIndex.y;
   nextFrame.bottom += size[action].y * frameIndex.y;
   nextFrame.left   += size[action].x * frameIndex.x;
   nextFrame.right  += size[action].x * frameIndex.x;
   }




RECT * House :: Move(AliveList *collideableList)
   {
   list<Emitter *>::iterator flameListItr;

   GetNextFrame();
   faucet->Update();

   flameListItr = flameList.begin();
   while (flameListItr != flameList.end())
      {
      (*flameListItr)->Update();
      ++flameListItr;
      }

   return NULL;   // not moving
   }



void House :: TurnOnFaucet(void)
   {
   if (health > 0)
      faucet->SetLife(30, 3);
   }



void House :: TurnOffFaucet(void)
   {
   faucet->SetLife(0, 0);
   }



void House :: BlockFaucet(void)
   {
   if (health > 0)
      faucet->SetLife(5, 0);
   }



void House :: DrawFire(int addFlamePoints)
   {
   int   countX;
   int   countY;
   int   avgFire;

   if (addFlamePoints)
      {
      for (countX = 0; countX < addFlamePoints; countX++)
         {
         int   xPos;
         int   yPos;

         xPos = randInt(3, 28);
         yPos = randInt(10, 126);
         fireArray[yPos][xPos] = 16 + 7;    // set to max fire value
         fireArray[yPos][xPos + 1] = 16 + 7;    // set to max fire value
         fireArray[yPos][xPos - 1] = 16 + 7;    // set to max fire value
         fireArray[yPos + 1][xPos] = 16 + 7;    // set to max fire value
         fireArray[yPos - 1][xPos] = 16 + 7;    // set to max fire value

//         fireArray[yPos][xPos] = 7 << 4;    // set to max fire value
//         fireArray[yPos][xPos + 1] = 7 << 4;    // set to max fire value
//         fireArray[yPos][xPos - 1] = 7 << 4;    // set to max fire value
//         fireArray[yPos + 1][xPos] = 7 << 4;    // set to max fire value
//         fireArray[yPos - 1][xPos] = 7 << 4;    // set to max fire value
         }
      }

   if (timeGetTime() > nextFlickerTime)
      {
      for (countY = 126; countY > 0; countY--)
         {
         for (countX = 2; countX < 30; countX++)
            {
            avgFire = AverageFire(countX, countY);
            if (avgFire > fireArray[countY - 1][countX])
               fireArray[countY - 1][countX] = avgFire;
            }
         }
      nextFlickerTime = timeGetTime() + 200;
      }

   renderer->LockSurface();
   renderer->DrawArray((char *)fireArray, 32, 128, objRect.left, objRect.top);
   renderer->UnlockSurface();
   }





int House :: AverageFire(int xPos, int yPos)
   {
   int   averageVal;

   averageVal  = fireArray[yPos][xPos];
   averageVal += fireArray[yPos][xPos + 1];
   averageVal += fireArray[yPos][xPos - 1];
   averageVal += fireArray[yPos][xPos + 2];
   averageVal += fireArray[yPos][xPos - 2];

   averageVal += fireArray[yPos + 1][xPos];
   averageVal += fireArray[yPos + 1][xPos + 1];
   averageVal += fireArray[yPos + 1][xPos - 1];

   averageVal += fireArray[yPos - 1][xPos];
   averageVal += fireArray[yPos - 1][xPos + 1];
   averageVal += fireArray[yPos - 1][xPos - 1];

   averageVal = averageVal / 11;

   if (averageVal < 1)
      averageVal = 1;
   if (averageVal > 7)
      averageVal = 7;

//   if (averageVal < 1 << 4)
//      averageVal = 1 << 4;
//   if (averageVal > 7 << 4)
//      averageVal = 7 << 4;

   return averageVal;
   }



HRESULT House :: Draw(void)
   {
   int         count1;
   Emitter *   nextFlame;
   HRESULT     returnVal;
   list<Emitter *>::iterator flameListItr;

   returnVal = (AliveObject::Draw());

   if (action == ACTION_IDLE_RIGHT)
      faucet->SetPos(objRect.right + 3, objRect.right + 6, objRect.bottom - 30, objRect.bottom - 30);
   else
      faucet->SetPos(objRect.left - 4, objRect.left - 7, objRect.bottom - 30, objRect.bottom - 30);

   faucet->Render();

   flameListItr = flameList.begin();
   while (flameListItr != flameList.end())
      {
      nextFlame = *flameListItr;
      nextFlame->Render();
      ++flameListItr;
      }


   if (lastHealth != health)
      {
      for (count1 = 0; count1 < 2; count1++)
         {
         RECT     flamePos;
         Emitter *flame;

         flame = new Emitter(20);
//         dprintf("flame = %x\n", flame);
         if (!flame)
            {
            dprintf("Out of memory!!\n");
            theApp->Stop();
            }

         flame->SetAngles(0, 10, 10, 3);
         flame->SetSpeed(0.50f, 0.1f);
         flame->SetForce(0.0f, -0.10f, 0.0f);
         flame->SetEmits(1, 1);
         flame->SetLife(20, 10);
         flame->SetColors(16 + 7, 2, 14, 2);
         flamePos.left = objRect.left + randInt(5, size[action].x - 5);
         flamePos.right = flamePos.left + randInt(3, 5);
         flamePos.top = objRect.top + randInt(10, size[action].y);
         flamePos.bottom = flamePos.top;
         flame->SetPos(&flamePos);
         flameList.push_back(flame);
         }

      if (health == 0)
         TurnOffFaucet();
      }


//   if (lastHealth != health)
//      DrawFire(2);
//   else
//      DrawFire(0);
   
   lastHealth = health;
   return returnVal;
   }
