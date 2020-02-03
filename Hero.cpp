
#include "Hero.hpp"
#include "Misc.hpp"
#include "global.h"
#include "debug.h"



Hero :: Hero()
   {
//   int   count1;

   type   = TYPE_HERO;
   action = ACTION_IDLE_RIGHT;

   velocity.x        = 0;
   velocity.y        = 0;
   minVelocity.x     = 1;
   minVelocity.y     = 0;
   maxVelocity.x     = 5;
   maxVelocity.y     = 0;

//   surfaceID = renderer->NewSurfaceFromFile("Art/Hero.bmp");
   surfaceID = renderer->NewSurfaceFromResource("HERO");

//   videoSurface[ACTION_IDLE_RIGHT]  = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[ACTION_IDLE_RIGHT]    = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_IDLE_RIGHT].x   = 1;
   numFrames[ACTION_IDLE_RIGHT].y   = 1;
   size[ACTION_IDLE_RIGHT].x        = 16;
   size[ACTION_IDLE_RIGHT].y        = 32;
   bmpOffset[ACTION_IDLE_RIGHT].x   = 0;
   bmpOffset[ACTION_IDLE_RIGHT].y   = 0;

//   videoSurface[ACTION_IDLE_LEFT]   = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[ACTION_IDLE_LEFT]     = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_IDLE_LEFT].x    = 1;
   numFrames[ACTION_IDLE_LEFT].y    = 1;
   size[ACTION_IDLE_LEFT].x         = 16;
   size[ACTION_IDLE_LEFT].y         = 32;
   bmpOffset[ACTION_IDLE_LEFT].x    = 0;
   bmpOffset[ACTION_IDLE_LEFT].y    = 32;

//   videoSurface[ACTION_WALK_RIGHT]  = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[ACTION_WALK_RIGHT]    = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_WALK_RIGHT].x   = 4;
   numFrames[ACTION_WALK_RIGHT].y   = 1;
   size[ACTION_WALK_RIGHT].x        = 16;
   size[ACTION_WALK_RIGHT].y        = 32;
   bmpOffset[ACTION_WALK_RIGHT].x   = 0;
   bmpOffset[ACTION_WALK_RIGHT].y   = 0;

//   videoSurface[ACTION_WALK_LEFT]   = surfaceDescr[SURFACE_SPRITES].videoSurface;
//   memSurface[ACTION_WALK_LEFT]     = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_WALK_LEFT].x    = 4;
   numFrames[ACTION_WALK_LEFT].y    = 1;
   size[ACTION_WALK_LEFT].x         = 16;
   size[ACTION_WALK_LEFT].y         = 32;
   bmpOffset[ACTION_WALK_LEFT].x    = 0;
   bmpOffset[ACTION_WALK_LEFT].y    = 32;

   Initialize();

   basket = new Basket;
//   dprintf("basket = %x\n", basket);
   if (!basket)
      {
      dprintf("Out of memory!!\n");
      theApp->Stop();
      }
   mainList.push_back(basket);

   moveBounds.left   = 32 + basket->GetXSize();                // 32 = width of house
   moveBounds.right  = SCREEN_WIDTH - 32 - basket->GetXSize();

   SetDst(320, 480);
   SetSelected(TRUE);

   nextFlipTime = 0;
   MoveBasket();

   hitSound[0]   = SndObjCreate(ds, "ARGH",  1);
   hitSound[1]   = SndObjCreate(ds, "AAAH",  1);
   hitSound[2]   = SndObjCreate(ds, "AGONY",  1);
   hitSound[3]   = SndObjCreate(ds, "AHHHHHHH",  1);
   hitSound[4]   = SndObjCreate(ds, "BONK",  1);
   hitSound[5]   = SndObjCreate(ds, "JIBBERISH",  1);
   hitSound[6]   = SndObjCreate(ds, "KINISON",  1);
   hitSound[7]   = SndObjCreate(ds, "KOOK",  1);
   hitSound[8]   = SndObjCreate(ds, "MWPUNCH",  1);
   hitSound[9]   = SndObjCreate(ds, "NI",  1);
   hitSound[10]  = SndObjCreate(ds, "OUCH",  1);
   hitSound[11]  = SndObjCreate(ds, "POP",  1);
   hitSound[12]  = SndObjCreate(ds, "POP1",  1);
   hitSound[13]  = SndObjCreate(ds, "POP2",  1);
   hitSound[14]  = SndObjCreate(ds, "SCREAM",  1);
   hitSound[15]  = SndObjCreate(ds, "WOOW1",  1);
   hitSound[16]  = SndObjCreate(ds, "YELLING",  1);

//   for (count1 = 0; count1 < NUM_HERO_HIT_SOUNDS; count1++)
//      {
//      if (!hitSound[count1])
//         dprintf("Bad hero hit-sound init for #%d\n", count1);
//      }

#ifdef SHOW_HEALTH
   memset(healthBitmap, 42, 50);                   // blue
#endif
   }



Hero :: ~Hero()
   {
   int   count1;

   // don't need to delete basket because it was added to mainList

   for (count1 = 0; count1 < NUM_HERO_HIT_SOUNDS; count1++)
      {
      SndObjDestroy(hitSound[count1]);
      hitSound[count1] = NULL;
      }
   }



void Hero :: Hit(int damage)
   {
   int   whichSound;

   (AliveObject::Hit(damage));

#ifdef SHOW_HEALTH
   // red (grows from bottom)
   memset(healthBitmap + 50 - ((100 - health) >> 1), 249, (100 - health) >> 1);
#endif

   whichSound = randInt(0, NUM_HERO_HIT_SOUNDS - 1);
   SndObjPlay(hitSound[whichSound], 0);
   }




void Hero :: NewLevel(int level)
   {
   action = ACTION_IDLE_RIGHT;
   velocity.x        = 0;
   velocity.y        = 0;
   SetDst(320, 480);
   SetHealth(100);

#ifdef SHOW_HEALTH
   memset(healthBitmap, 42, 50);    // blue
#endif

   basket->NewLevel(level);
   MoveBasket();
   }



void Hero :: MoveLeft(void)
   {
   SetDestination(objCenter.x - 5, objCenter.y);

//   if (velocity.x < 1)
//      SetDestination(objCenter.x - velocity.x, objCenter.y);
//   else
//      SetDestination(objCenter.x - 1, objCenter.y);
   }



void Hero :: MoveRight(void)
   {
   SetDestination(objCenter.x + 5, objCenter.y);

//   if (velocity.x > 1)
//      SetDestination(objCenter.x + velocity.x, objCenter.y);
//   else
//      SetDestination(objCenter.x + 1, objCenter.y);
   }




void Hero :: FlipDir(void)
   {
   if (timeGetTime() < nextFlipTime)
      return;     // not enough time elapsed from last flip
   nextFlipTime = timeGetTime() + 300;

   switch (action)
      {
      case ACTION_WALK_LEFT:
         {
         action = ACTION_WALK_RIGHT;
         break;
         }

      case ACTION_WALK_RIGHT:
         {
         action = ACTION_WALK_LEFT;
         break;
         }

      case ACTION_IDLE_LEFT:
         {
         action = ACTION_IDLE_RIGHT;
         break;
         }

      case ACTION_IDLE_RIGHT:
         {
         action = ACTION_IDLE_LEFT;
         break;
         }
      }

   MoveBasket();
   GetNextFrame();
   }



RECT * Hero :: Move(AliveList *collideableList)
   {
   int   dirX;
//   int   dirY;

   if (timeGetTime() < nextMoveTime)
      return NULL;   // not time to move yet
   nextMoveTime = timeGetTime() + 10;

   if ((action == ACTION_IDLE_LEFT) || (action == ACTION_IDLE_RIGHT))
      {
      if (++frameCount > 3)
         {
         GetNextFrame();
         frameCount = 0;
         }
      return NULL;   // not moving
      }

   if (objCenter.x < destination.x)    // moving to right
      {
      if (objCenter.x + velocity.x + 1 > destination.x)
         {
         objCenter.x = destination.x;
         dirX = 0;         // at destination X location
         velocity.x = 0;
         }
      else
         {
         dirX = 1;
         velocity.x += 1;
         action = ACTION_WALK_RIGHT;
         }
      }
   else if (objCenter.x > destination.x)  // moving to left
      {
      if (objCenter.x + velocity.x - 1 < destination.x)
         {
         objCenter.x = destination.x;
         dirX = 0;         // at destination X location
         velocity.x = 0;
         }
      else
         {
         dirX = -1;        // moving to the left
         velocity.x -= 1;
         action = ACTION_WALK_LEFT;
         }
      }
   else
      {
      dirX = 0;         // at destination X location
      velocity.x = 0;
      }

   // have we arrived at our destination?
   if (dirX == 0)
      {
      switch (type)
         {
         case TYPE_HERO:
            {
            if (action == ACTION_WALK_LEFT)
               action = ACTION_IDLE_LEFT;
            else
               action = ACTION_IDLE_RIGHT;
            break;
            }

         case TYPE_ROCK:
            {
            action = ACTION_DEAD;
            break;
            }

         default:
            {
            action = ACTION_DEAD;
            break;
            }
         }

      frameCount = 3;      // force a new frame
//      GetNextFrame();
//      MoveBasket();
//      return NULL;
      }

   if (++frameCount > 3)
      {
      GetNextFrame();
      frameCount = 0;
      }

   if (velocity.x > maxVelocity.x)
      velocity.x = maxVelocity.x;
   else if (velocity.x < -maxVelocity.x)
      velocity.x = -maxVelocity.x;

   // move the object to its new position
   objCenter.x += velocity.x;

   // switched X directions?
   if (((objCenter.x < destination.x) && (dirX == -1)) || ((objCenter.x > destination.x) && (dirX == 1)))
      velocity.x = 0;

   if (objCenter.x > moveBounds.right - halfSize[action].x)
      objCenter.x = moveBounds.right - halfSize[action].x;
   if (objCenter.x < moveBounds.left + halfSize[action].x)
      objCenter.x = moveBounds.left + halfSize[action].x;

   objRect.left   = objCenter.x - halfSize[action].x;
   objRect.right  = objCenter.x + halfSize[action].x;
   objRect.top    = objCenter.y - halfSize[action].y;
   objRect.bottom = objCenter.y + halfSize[action].y;

   MoveBasket();
   return &objRect;
   }



   
void Hero :: MoveBasket(void)
   {
   if ((action == ACTION_IDLE_LEFT) || (action == ACTION_WALK_LEFT))
      basket->SetDst(objRect.left - (basket->GetXSize() >> 1), objCenter.y);
   else
      basket->SetDst(objRect.right + (basket->GetXSize() >> 1), objCenter.y);

/*
   if ((action == ACTION_IDLE_LEFT) || (action == ACTION_WALK_LEFT))
      basket->SetDst(objRect.left - (basket->GetXSize() >> 1), objCenter.y);
   else
      basket->SetDst(objRect.right + (basket->GetXSize() >> 1), objCenter.y);
*/
   }



HRESULT Hero :: Draw(void)
   {
   HRESULT  returnVal;

   returnVal = (AliveObject::Draw());

   DrawHealthLevel();
   return returnVal;
   }



void Hero::DrawHealthLevel(void)
   {
#ifdef SHOW_HEALTH
   renderer->LockSurface();
   if ((action == ACTION_IDLE_LEFT) || (action == ACTION_WALK_LEFT))
      renderer->DrawArray((char *)healthBitmap, 2, 25, objRect.right - 2, objRect.top + 7);
   else
      renderer->DrawArray((char *)healthBitmap, 2, 25, objRect.left, objRect.top + 7);

   renderer->UnlockSurface();
#endif
   }
