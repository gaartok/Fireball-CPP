
#include "Basket.hpp"
#include "Misc.hpp"
#include "global.h"
#include "debug.h"



Basket :: Basket()
   {
   type   = TYPE_BASKET;
   action = LEVEL1;

   velocity.x        = 0;
   velocity.y        = 0;
   minVelocity.x     = 0;
   minVelocity.y     = 0;
   maxVelocity.x     = 0;
   maxVelocity.y     = 0;

//   surfaceID = renderer->NewSurface(416, 90, "Basket");
   surfaceID = renderer->NewSurfaceFromResource("BASKET");

//   videoSurface[LEVEL1]     = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[LEVEL1]       = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[LEVEL1].x      = 1;
   numFrames[LEVEL1].y      = 1;
   size[LEVEL1].x           = 24;
   size[LEVEL1].y           = 16;
   bmpOffset[LEVEL1].x      = 0;
   bmpOffset[LEVEL1].y      = 0;

//   videoSurface[LEVEL2]      = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[LEVEL2]        = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[LEVEL2].x       = 1;
   numFrames[LEVEL2].y       = 1;
   size[LEVEL2].x            = 20;
   size[LEVEL2].y            = 16;
   bmpOffset[LEVEL2].x       = 24;
   bmpOffset[LEVEL2].y       = 0;

//   videoSurface[LEVEL3]      = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[LEVEL3]        = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[LEVEL3].x       = 1;
   numFrames[LEVEL3].y       = 1;
   size[LEVEL3].x            = 16;
   size[LEVEL3].y            = 16;
   bmpOffset[LEVEL3].x       = 44;
   bmpOffset[LEVEL3].y       = 0;

   Initialize();

   waterOverflow = new Emitter(50);
   if (!waterOverflow)
      {
      dprintf("Out of memory!!\n");
      theApp->Stop();
      }

   waterOverflow->SetAngles(90, 1, 90, 1);    // down
   waterOverflow->SetSpeed(1.0f, 0.1f);
   waterOverflow->SetForce(0.0f, 0.0f, 0.0f);
   waterOverflow->SetEmits(2, 1);
   waterOverflow->SetLife(0, 0);
   waterOverflow->SetColors(48, 0, 48, 0);
   waterOverflow->SetPos(objRect.left, objRect.right, objRect.top, objRect.top);

#ifdef SHOW_SPLASH
   splashEndTime = timeGetTime() - 2;
   waterSplash = new Emitter(50);
   if (!waterSplash)
      {
      dprintf("Out of memory!!\n");
      theApp->Stop();
      }

   waterSplash->SetAngles(0, 360, 270, 40);
   waterSplash->SetSpeed(1.0, 2.0f);
   waterSplash->SetForce(0.0f, 0.2f, 0.0f);
   waterSplash->SetEmits(15, 1);
   waterSplash->SetLife(0, 0);
   waterSplash->SetColors(48, 0, 48, 0);
   waterSplash->SetPos(objRect.left, objRect.right, objRect.top, objRect.top);
#endif

   waterLevel = 50;
   UpdateWaterLevel();

   hitSound  = SndObjCreate(ds, "SPLASH",  1);
   fillSound = SndObjCreate(ds, "WATER",  1);
   }




Basket :: ~Basket()
   {
   SndObjDestroy(hitSound);
   SndObjDestroy(fillSound);
   hitSound = NULL;
   fillSound = NULL;

   delete waterOverflow;

#ifdef SHOW_SPLASH
   delete waterSplash;
#endif
   }



void Basket :: NewLevel(int level)
   {
   SetHealth(100);

   if (waterLevel < 50)
      waterLevel = 50;

   action = level;
   GetNextFrame();
   UpdateWaterLevel();
   }



void Basket :: UpdateWaterLevel(void)
   {
   int   levelIndex;

   levelIndex = 0;
   while (levelIndex < 100 - waterLevel)  // fill in empty area (at top)
      {
      memset(waterBitmap[levelIndex >> 1], 37, 50);
      levelIndex += 2;
      }
   while (levelIndex < 100)               // fill in full area (at bottom)
      {
      memset(waterBitmap[levelIndex >> 1], 66, 50);
      levelIndex += 2;
      }
   }



void Basket :: DrawWaterLevel(void)
   {
   renderer->LockSurface();
   renderer->DrawArray((char *)waterBitmap, 50, 50, 25, 50);
   renderer->UnlockSurface();
   }



void Basket :: GetNextFrame(void)
   {
   nextFrame         = firstFrame[playerLevel];
   nextFrame.top    += size[playerLevel].y * frameIndex.y;
   nextFrame.bottom += size[playerLevel].y * frameIndex.y;
   nextFrame.left   += size[playerLevel].x * frameIndex.x;
   nextFrame.right  += size[playerLevel].x * frameIndex.x;
   }



HRESULT Basket :: Draw(void)
   {
   int      underFaucet;
   HRESULT  returnVal;

   returnVal = (AliveObject::Draw());

   underFaucet = FALSE;

   // are we under the left faucet?
   if ((objRect.left <= 32 + 4) && (houseLeft->GetHealth() > 0))     // 32 = width of house  4 = distance faucet is from house
      {
      houseLeft->BlockFaucet();
      underFaucet = TRUE;
      }
   else
      {
      houseLeft->TurnOnFaucet();
      }

   // are we under the right faucet?
   if ((objRect.right >= SCREEN_WIDTH - 32 - 4) && (houseRight->GetHealth() > 0))
      {
      houseRight->BlockFaucet();
      underFaucet = TRUE;
      }
   else
      {
      houseRight->TurnOnFaucet();
      }

   if (underFaucet)
      {
      SndObjPlay(fillSound, DSBPLAY_LOOPING);
      if (waterLevel == 100)
         waterOverflow->SetLife(20, 3);

      if (underFaucetTime == 0)  // if we just got here, start the timer
         {
         underFaucetTime = timeGetTime();
         }
      else     // we've been here a while already
         {
         if (timeGetTime() > underFaucetTime + 200)
            {
            waterLevel += 10;
            if (waterLevel > 100)
               waterLevel = 100;

            UpdateWaterLevel();
            underFaucetTime = timeGetTime(); // restart the timer for another gulp
            }
         }
      }
   else
      {
      SndObjStop(fillSound);
      underFaucetTime = 0;
      waterOverflow->SetLife(0, 0);
      }

   waterOverflow->SetPos(objRect.left, objRect.right, objRect.top, objRect.top);
   waterOverflow->Update();
   waterOverflow->Render();

#ifdef SHOW_SPLASH
   if (timeGetTime() > splashEndTime)
      waterSplash->SetLife(0, 0);

   waterSplash->SetPos(objCenter.x - 4, objCenter.x + 4, objRect.top, objRect.top);
   waterSplash->Update();
   waterSplash->Render();
#endif

   DrawWaterLevel();

   return returnVal;
   }



void Basket :: Hit(int damage)
   {
//   (AliveObject::Hit(damage));

   waterLevel -= damage;
   if (waterLevel < 0)
      waterLevel = 0;
   if (waterLevel > 100)
      waterLevel = 100;

   SndObjPlay(hitSound, 0);

#ifdef SHOW_SPLASH
   waterSplash->SetPos(objCenter.x - 4, objCenter.x + 4, objRect.top, objRect.top);
   waterSplash->SetLife(50, 3);
   splashEndTime = timeGetTime() + 100;
#endif

   UpdateWaterLevel();
   }



int Basket :: GetWaterLevel(void)
   {
   return waterLevel;
   }

