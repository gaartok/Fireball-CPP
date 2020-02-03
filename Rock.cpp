
#include "Rock.hpp"
#include "Hero.hpp"
#include "Basket.hpp"
#include "Misc.hpp"
#include "global.h"
#include "debug.h"



#define BOUNCE_Y_COORD  460

Rock :: Rock()
   {
   type   = TYPE_ROCK;
   action = ACTION_IDLE_RIGHT;

   minVelocity.x     = 1;
   minVelocity.y     = 1;
   maxVelocity.x     = 1;
//   maxVelocity.y     = 4;
   maxVelocity.y     = 15;

//   surfaceID = renderer->NewSurface(416, 90, "Rock");
   surfaceID = renderer->NewSurfaceFromResource("ROCK");

//   videoSurface[ACTION_WALK_UP]     = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[ACTION_WALK_UP]       = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_WALK_UP].x      = 3;
   numFrames[ACTION_WALK_UP].y      = 1;
   size[ACTION_WALK_UP].x           = 8;
   size[ACTION_WALK_UP].y           = 16;
   bmpOffset[ACTION_WALK_UP].x      = 0;
   bmpOffset[ACTION_WALK_UP].y      = 0;

//   videoSurface[ACTION_WALK_DOWN]   = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[ACTION_WALK_DOWN]     = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_WALK_DOWN].x    = 3;
   numFrames[ACTION_WALK_DOWN].y    = 1;
   size[ACTION_WALK_DOWN].x         = 8;
   size[ACTION_WALK_DOWN].y         = 16;
   bmpOffset[ACTION_WALK_DOWN].x    = 24;
   bmpOffset[ACTION_WALK_DOWN].y    = 0;

//   videoSurface[ACTION_WALK_LEFT]   = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[ACTION_WALK_LEFT]     = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_WALK_LEFT].x    = 3;
   numFrames[ACTION_WALK_LEFT].y    = 1;
   size[ACTION_WALK_LEFT].x         = 16;
   size[ACTION_WALK_LEFT].y         = 8;
   bmpOffset[ACTION_WALK_LEFT].x    = 0;
   bmpOffset[ACTION_WALK_LEFT].y    = 16;

//   videoSurface[ACTION_WALK_RIGHT]  = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[ACTION_WALK_RIGHT]    = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_WALK_RIGHT].x   = 3;
   numFrames[ACTION_WALK_RIGHT].y   = 1;
   size[ACTION_WALK_RIGHT].x        = 16;
   size[ACTION_WALK_RIGHT].y        = 8;
   bmpOffset[ACTION_WALK_RIGHT].x   = 0;
   bmpOffset[ACTION_WALK_RIGHT].y   = 24;

   SetDst(randInt(225, 320), 120);
   SetDestination(randInt(100, 540), BOUNCE_Y_COORD);

   velocity.y = -1 * randInt(3, 5);

   if (objCenter.x < destination.x)
      velocity.x = 1;
   else if (objCenter.x > destination.x)
      velocity.x = -1;
   else
      velocity.x = 0;

   bounceCount = 0;

   tail = new Emitter(20);
//   dprintf("tail = %x\n", tail);
   if (!tail)
      {
      dprintf("Out of memory!!\n");
      theApp->Stop();
      }
   tail->SetSpeed(0.50f, 0.2f);
   tail->SetEmits(1, 1);
   tail->SetLife(10, 3);
   tail->SetColors(34, 3, 37, 2);
//   tail->SetAngles(0, 1, 0, 0);
//   tail->SetAngles(270, 10, 0, 0);
//   tail->SetAngles(0, 360, 270, 40);
   AimTail();

   Initialize();

   nextAccelTime = timeGetTime() + 200;
   }



void Rock :: AimTail()
   {
   RECT  tailPos;


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

   switch (action)
      {
      case ACTION_WALK_RIGHT:
         {
         tail->SetAngles(90, 5, 0, 5);     // left
//         tail->SetAngles(270, 10, 0, 0);
         tailPos.left = objRect.left + 5;
         tailPos.right = tailPos.left;
         tailPos.top = objRect.top;
         tailPos.bottom = objRect.bottom;
         tail->SetForce(-0.10f, 0.0f, 0.0f);
         break;
         }

      case ACTION_WALK_LEFT:
         {
         tail->SetAngles(270, 10, 0, 0);   // right
//         tail->SetAngles(90, 10, 0, 0);
         tailPos.left = objRect.right - 5;
         tailPos.right = tailPos.left;
         tailPos.top = objRect.top;
         tailPos.bottom = objRect.bottom;
         tail->SetForce(0.10f, 0.0f, 0.0f);
         break;
         }

      case ACTION_WALK_UP:
         {
         tail->SetAngles(0, 5, 90, 5);     // down
//         tail->SetAngles(0, 10, 270, 40);
         tailPos.left = objRect.left;
         tailPos.right = objRect.right;
         tailPos.top = objRect.bottom - 5;
         tailPos.bottom = tailPos.top;
         tail->SetForce(0.0f, 0.10f, 0.0f);
         break;
         }

      case ACTION_WALK_DOWN:
         {
//         tail->SetAngles(180, 10, 0, 0);
//         tail->SetForce(0.0f, -0.01f, 0.0f);

         tail->SetAngles(0, 5, 270, 5);    // up
//         tail->SetAngles(0, 20, 90, 20);
         tailPos.left = objRect.left;
         tailPos.right = objRect.right;
         tailPos.top = objRect.top + 5;
         tailPos.bottom = tailPos.top;
         tail->SetForce(0.0f, -0.10f, 0.0f);
         break;
         }
      }

   tail->SetPos(&tailPos);
   }




Rock :: ~Rock()
   {
   if (tail)
      delete tail;
   }




void Rock :: GetNextFrame(void)
   {
   if (velocity.x == 0)
      {
      if (velocity.y > 0)
         action = ACTION_WALK_DOWN;
      else
         action = ACTION_WALK_UP;
      }
   else if (velocity.x > 0)
      {
      if (velocity.y >= 3)
         action = ACTION_WALK_DOWN;
      else if (velocity.y <= -3)
         action = ACTION_WALK_UP;
      else
         action = ACTION_WALK_RIGHT;
      }
   else
      {
      if (velocity.y >= 3)
         action = ACTION_WALK_DOWN;
      else if (velocity.y <= -3)
         action = ACTION_WALK_UP;
      else
         action = ACTION_WALK_LEFT;
      }

   frameIndex.x += 1;
   if (frameIndex.x >= numFrames[action].x)
      {
      frameIndex.x = 0;
      frameIndex.y += 1;
      if (frameIndex.y >= numFrames[action].y)
         frameIndex.y = 0;
      }

   nextFrame         = firstFrame[action];
   nextFrame.top    += size[action].y * frameIndex.y;
   nextFrame.bottom += size[action].y * frameIndex.y;
   nextFrame.left   += size[action].x * frameIndex.x;
   nextFrame.right  += size[action].x * frameIndex.x;

   AimTail();
   }




RECT * Rock :: Move(AliveList *collideableList)
   {
   int            dirX;
   int            dirY;
   AliveObject *  overlappedObj;

   tail->Update();

   if (timeGetTime() < nextMoveTime)
      return NULL;   // not time to move yet
   nextMoveTime = timeGetTime() + 20;
//   nextMoveTime = timeGetTime() + 2;

   if (++frameCount > 3)
      {
      GetNextFrame();
      frameCount = 0;
      }

   if (objCenter.x < destination.x)
      dirX = 1;         // moving to the right
   else if (objCenter.x > destination.x)
      dirX = -1;        // moving to the left
   else
      dirX = 0;         // at destination X location

   if (objCenter.y < destination.y)
      dirY = 1;         // moving down
   else if (objCenter.y > destination.y)
      dirY = -1;        // moving up
   else
      dirY = 0;         // at destination Y location

   if (timeGetTime() > nextAccelTime)
      {
      if (dirX == 1)
         velocity.x += 1;
      else if (dirX == -1)
         velocity.x -= 1;
      else
         velocity.x = 0;

      if (dirY == 1)
         velocity.y += 1;
      else if (dirY == -1)
         velocity.y -= 1;

      nextAccelTime = timeGetTime() + 200;
      }

   if (velocity.x > maxVelocity.x)
      velocity.x = maxVelocity.x;
   else if (velocity.x < -maxVelocity.x)
      velocity.x = -maxVelocity.x;

   if (velocity.y > maxVelocity.y)
      velocity.y = maxVelocity.y;
   else if (velocity.y < -maxVelocity.y)
      velocity.y = -maxVelocity.y;

   // move the object to its new position
   objCenter.x += velocity.x;
   objCenter.y += velocity.y;

   // switched X directions?
   if (((objCenter.x < destination.x) && (dirX == -1)) || ((objCenter.x > destination.x) && (dirX == 1)))
      velocity.x = 0;

   // switched Y directions?
   if (((objCenter.y < destination.y) && (dirY == -1)) || ((objCenter.y > destination.y) && (dirY == 1)))
      velocity.y = 0;

   // have we arrived at our destination?
   if (objCenter.y >= destination.y)
      {
      if (bounceCount > 3)
         {
         action = ACTION_DEAD;
         return NULL;
         }

      maxVelocity.x = 2;
      objCenter.y = destination.y - 1;
      if (bounceCount == 0)
         {
         if (velocity.x < 0)
            {
            SetDestination(0, BOUNCE_Y_COORD);
            }
         else if (velocity.x > 0)
            {
            SetDestination(SCREEN_WIDTH, BOUNCE_Y_COORD);
            }
         else
            {
            if (objCenter.x < SCREEN_WIDTH / 2)
               SetDestination(0, BOUNCE_Y_COORD);
            else
               SetDestination(SCREEN_WIDTH, BOUNCE_Y_COORD);
            }
         }

      velocity.y = (3 - bounceCount) * -2;
//      velocity.y = (4 - bounceCount) * -1;
      bounceCount++;

      if (velocity.y > -2)
         velocity.y = -2;
      velocity.x = 0;
      }

   if (objCenter.x > moveBounds.right)
      objCenter.x = moveBounds.right;
   if (objCenter.x < moveBounds.left)
      objCenter.x = moveBounds.left;

   objRect.left   = objCenter.x - halfSize[action].x;
   objRect.right  = objCenter.x + halfSize[action].x;
   objRect.top    = objCenter.y - halfSize[action].y;
   objRect.bottom = objCenter.y + halfSize[action].y;

   // check for collisions
   overlappedObj = CheckOverlapped(collideableList);
   if (overlappedObj)
      {
      switch (overlappedObj->GetType())
         {
         case TYPE_HOUSE:
            {
            if (((House *)overlappedObj)->GetHealth() > 0)
               {
               playerScore -= 10;
               ((House *)overlappedObj)->Hit(25);
               }

            action = ACTION_DEAD;
            return NULL;
            }

         case TYPE_BASKET:
            {
            if (((Basket *)overlappedObj)->GetWaterLevel() > 0)
               {
               ((Basket *)overlappedObj)->Hit(10);
               playerScore += 25;
               action = ACTION_DEAD;
               }
//            else
//               {
//               }
            return NULL;
            }

         case TYPE_HERO:
            {
//            dprintf("Hit hero!\n");
            ((Hero *)overlappedObj)->Hit(10);
            playerScore -= 15;
            action = ACTION_DEAD;
            return NULL;
            }

         default:
            {
//            velocity.x = -velocity.x;
//            if (velocity.x < 0)
//               SetDestination(0, BOUNCE_Y_COORD);
//            else
//               SetDestination(SCREEN_WIDTH, BOUNCE_Y_COORD);
//            dprintf("Hit something!\n");
            break;
            }
         }
      }


   return &objRect;
   }



HRESULT Rock :: Draw(void)
   {
    HRESULT     returnVal;

   returnVal = (AliveObject::Draw());
   tail->Render();

   return returnVal;
   }
