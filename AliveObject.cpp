
#include "App.hpp"
#include "AliveObject.hpp"
#include "Misc.hpp"
#include "global.h"
#include "debug.h"


extern App *      theApp;



AliveObject :: AliveObject()
   {
   nextFrame.left    = 0;
   nextFrame.right   = 0;
   nextFrame.top     = 0;
   nextFrame.bottom  = 0;
   selected          = FALSE;
   objCenter.x       = 0;
   objCenter.y       = 0;

   moveBounds.left   = 0;
   moveBounds.right  = SCREEN_WIDTH;
   moveBounds.top    = 0;
   moveBounds.bottom = SCREEN_HEIGHT;

   nextMoveTime = 0;
   frameCount   = 0;

   health = 100;
   }




AliveObject :: ~AliveObject()
   {
   }



void AliveObject :: NewLevel(int level)
   {
   SetHealth(100);
   }




int AliveObject :: GetHealth(void)
   {
   return health;
   }



void AliveObject :: SetHealth(int newHealth)
   {
   if ((newHealth >= 0) && (newHealth <= 100))
      health = newHealth;
   }




void AliveObject :: MoveLeft(void)
   {
   }



void AliveObject :: MoveRight(void)
   {
   }



void AliveObject :: FlipDir(void)
   {
   }


void AliveObject :: Hit(int damage)
   {
   health -= damage;
   if (health < 0)
      health = 0;
   }



BOOL AliveObject :: IsAlive(void)
   {
   if (action == ACTION_DEAD)
      return FALSE;

   return TRUE;
   }



void AliveObject :: SetFinalDest(long finalX, long finalY)
   {
   finalDestination.x = finalX;
   finalDestination.y = finalY;
   }



void AliveObject :: Initialize()
   {
   int count1;

   for (count1 = 0; count1 < MAX_ACTIONS; count1++)
      {
      firstFrame[count1].left    = bmpOffset[count1].x;
      firstFrame[count1].top     = bmpOffset[count1].y;
      firstFrame[count1].right   = firstFrame[count1].left + size[count1].x;
      firstFrame[count1].bottom  = firstFrame[count1].top + size[count1].y;
      halfSize[count1].x         = size[count1].x >> 1;
      halfSize[count1].y         = size[count1].y >> 1;
      }

   frameIndex.x = 0;
   frameIndex.y = 0;

   objRect.left   = objCenter.x - halfSize[action].x;
   objRect.right  = objCenter.x + halfSize[action].x;
   objRect.top    = objCenter.y - halfSize[action].y;
   objRect.bottom = objCenter.y + halfSize[action].y;

   frameCount = 0;
   GetNextFrame();
   }



long AliveObject :: GetXSize(void)
   {
   return size[action].x;
   }


long AliveObject :: GetYSize(void)
   {
   return size[action].y;
   }


long AliveObject :: GetXPosition(void)
   {
   return objCenter.x;
   }


long AliveObject :: GetYPosition(void)
   {
   return objCenter.y;
   }



BOOL AliveObject :: CheckForHit(int xPos, int yPos)
   {
   if ((xPos > objRect.left) && (xPos < objRect.right) && (yPos > objRect.top) && (yPos < objRect.bottom))
      return TRUE;

   return FALSE;
   }




POINT AliveObject :: GetObjCenter(void)
   {
   return objCenter;
   }



RECT * AliveObject :: SetDst(long xDst, long yDst)
   {
   int   inBounds;

   objCenter.x = xDst;
   objCenter.y = yDst;

   objRect.left   = objCenter.x - halfSize[action].x;
   objRect.right  = objCenter.x + halfSize[action].x;
   objRect.top    = objCenter.y - halfSize[action].y;
   objRect.bottom = objCenter.y + halfSize[action].y;

   inBounds = FALSE;
   while (!inBounds)
      {
      inBounds = TRUE;

      if (objRect.left < moveBounds.left)
         {
         objCenter.x = moveBounds.left + halfSize[action].x;
         inBounds = FALSE;
         }

      if (objRect.right > moveBounds.right)
         {
         objCenter.x = moveBounds.right - halfSize[action].x;
         inBounds = FALSE;
         }

      if (objRect.top < moveBounds.top)
         {
         objCenter.y = moveBounds.top + halfSize[action].y;
         inBounds = FALSE;
         }

      if (objRect.bottom > moveBounds.bottom)
         {
         objCenter.y = moveBounds.bottom - halfSize[action].y;
         inBounds = FALSE;
         }

      if (!inBounds)
         {
         objRect.left   = objCenter.x - halfSize[action].x;
         objRect.right  = objCenter.x + halfSize[action].x;
         objRect.top    = objCenter.y - halfSize[action].y;
         objRect.bottom = objCenter.y + halfSize[action].y;
         }
      }

   destination.x = xDst;
   destination.y = yDst;

   return &objRect;
   }



RECT * AliveObject :: GetRect(void)
   {
   return &objRect;
   }



void AliveObject :: GetNextFrame(void)
   {
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
   }


void AliveObject :: SetAction(int newAction)
   {
   action = newAction;
   GetNextFrame();
   }




RECT * AliveObject :: Move(AliveList *collideableList)
   {
   return NULL;
   }




UINT AliveObject :: GetType(void)
   {
   return type;
   }



void AliveObject :: SetDestination(long xPos, long yPos)
   {
//   static DWORD nextSettableTime = 0;
//   if (timeGetTime() < nextSettableTime)
//      return;
//   nextSettableTime = timeGetTime() + 300;

   // velocity is zero in X and Y when coming from a dead stop
   if ((action == ACTION_IDLE_LEFT) || (action == ACTION_IDLE_RIGHT))    
      {
      velocity.x = 0;
      velocity.y = 0;
      }


// NOTE: this is causing problems in App also!   
//   else
//      {
//      if (GetSign(objCenter.x - destination.x) != GetSign(objCenter.x - xPos))
//         velocity.x = 0;
//      if (GetSign(objCenter.y - destination.y) != GetSign(objCenter.y - yPos))
//         velocity.y = 0;
//      }

   if ((xPos <= moveBounds.right) && (xPos >= moveBounds.left))
      {
      destination.x = xPos;
      }
   else
      {
      if (xPos > moveBounds.right)
         objCenter.x = moveBounds.right;
      else
         objCenter.x = moveBounds.left;
      }

   if ((yPos <= moveBounds.bottom) && (yPos >= moveBounds.top))
      {
      destination.y = yPos;
      }
   else
      {
      if (yPos > moveBounds.bottom)
         objCenter.x = moveBounds.bottom;
      else
         objCenter.y = moveBounds.top;
      }

   if (objCenter.x < destination.x)
      action = ACTION_WALK_RIGHT;
   else if (objCenter.x > destination.x)
      action = ACTION_WALK_LEFT;

   destination.x = xPos;
   destination.y = yPos;
   }



POINT AliveObject :: GetDestination(void)
   {
   return destination;
   }


void AliveObject :: SetSelected(BOOL newValue)
   {
   selected = newValue;
   }


BOOL AliveObject :: IsSelected(void)
   {
   return selected;
   }



HRESULT AliveObject :: Draw(void)
   {
   if (action == ACTION_DEAD)    // if we're dead, don't draw
      return DD_OK;

   // if it's completely off the screen, don't draw anything
   if ((objRect.right <= 0) || (objRect.left >= (long)SCREEN_WIDTH) || (objRect.bottom <= 0) || (objRect.top >= (long)SCREEN_HEIGHT))
      return DD_OK;

   // if it's completely on the screen, draw the whole thing
   if ((objRect.right < (long)SCREEN_WIDTH) && (objRect.left >= 0) && (objRect.bottom < (long)SCREEN_HEIGHT) && (objRect.top >= 0))
      return renderer->Draw(objRect.left, objRect.top, surfaceID, &nextFrame);

   // it's partially on the screen, must clip
   long  xPos;
   long  yPos;
   RECT  newSrc;

   newSrc = nextFrame;

   if (objRect.left >= 0)
      {
      xPos = objRect.left;
      }
   else
      {
      xPos = 0;
      newSrc.left -= objRect.left;
      }

   if (objRect.top >= 0)
      {
      yPos = objRect.top;
      }
   else
      {
      yPos = 0;
      newSrc.top -= objRect.top;
      }

   if (objRect.right > SCREEN_WIDTH)
      newSrc.right -= objRect.right - SCREEN_WIDTH;

   if (objRect.bottom > SCREEN_HEIGHT)
      newSrc.bottom -= objRect.bottom - SCREEN_HEIGHT;

   if (selected)
      return renderer->Draw(xPos, yPos, surfaceID, &newSrc);
   else
      return renderer->Draw(xPos, yPos, surfaceID, &newSrc);
   }





BOOL AliveObject :: Overlapped(AliveObject *testObj)
   {
   return Overlapped(&(testObj->objRect));
   }



BOOL AliveObject :: Overlapped(RECT *testRect)
   {
   // it's easier to test if their NOT overlapped, and invert it
   return (!(testRect->right  <= objRect.left   ||
             testRect->left   >= objRect.right  ||
             testRect->top    >= objRect.bottom ||
             testRect->bottom <= objRect.top));
   }



BOOL AliveObject :: CheckPixelCollision(AliveObject * otherObj)
   {
   return TRUE;

#if 0
   int      countX;
   int      countY;
   int      sizeX;
   int      sizeY;
   int      colorArrayIndex;
   HDC      hdc;
   RECT     overlapThis;      // overlap area for this object
   RECT     overlapOther;     // overlap area for theObject
   COLORREF *thisColorArray;
   COLORREF *otherColorArray;

   // First we find the RECT which describes the collision area for each of the objects.
   // The size of these RECTs will be the same.
   if (objRect.left > otherObj->objRect.left)
      {
      overlapThis.left  = 0;
      overlapOther.left = objRect.left - otherObj->objRect.left;
      }
   else
      {
      overlapThis.left  = otherObj->objRect.left - objRect.left;
      overlapOther.left = 0;
      }

   if (objRect.right > otherObj->objRect.right)
      {
      overlapThis.right  = otherObj->objRect.right - objRect.left;
      overlapOther.right = otherObj->objRect.right - otherObj->objRect.left;
      }
   else
      {
      overlapThis.right  = objRect.right - objRect.left;
      overlapOther.right = objRect.right - otherObj->objRect.left;
      }

   if (objRect.top > otherObj->objRect.top)
      {
      overlapThis.top  = otherObj->objRect.top - objRect.top;
      overlapOther.top = 0;
      }
   else
      {
      overlapThis.top   = 0;
      overlapOther.top  = objRect.top - otherObj->objRect.top;
      }

   if (objRect.bottom > otherObj->objRect.bottom)
      {
      overlapThis.bottom  = otherObj->objRect.bottom - objRect.top;
      overlapOther.bottom = otherObj->objRect.bottom - otherObj->objRect.top;
      }
   else
      {
      overlapThis.bottom  = objRect.bottom - objRect.top;
      overlapOther.bottom = objRect.bottom - otherObj->objRect.top;
      }

   // Now add in the frame information
   overlapThis.top     += nextFrame.top;
   overlapThis.bottom  += nextFrame.bottom;
   overlapThis.left    += nextFrame.left;
   overlapThis.right   += nextFrame.right;
   overlapOther.top    += otherObj->nextFrame.top;
   overlapOther.bottom += otherObj->nextFrame.bottom;
   overlapOther.left   += otherObj->nextFrame.left;
   overlapOther.right  += otherObj->nextFrame.right;

   // We now have two RECTs which point at the correct spot in their surface.

   sizeX = overlapThis.right - overlapThis.left;
   sizeY = overlapThis.bottom - overlapThis.top;

   dprintf("mallocing %d bytes each\n", sizeof(COLORREF) * sizeX * sizeY);

   thisColorArray = (COLORREF *)malloc(sizeof(COLORREF) * sizeX * sizeY);
   otherColorArray = (COLORREF *)malloc(sizeof(COLORREF) * sizeX * sizeY);

   // 
   if (memSurface[action]->GetDC(&hdc) != DD_OK)
      {
      dprintf("Couldn't get DC on this memSurface!  action = %d  type = %d\n", action, type);
      theApp->Stop();
      free(thisColorArray);
      free(otherColorArray);
      return FALSE;
      }
   
   colorArrayIndex = 0;
   for (countY = 0; countY < sizeY; countY++)
      for (countX = 0; countX < sizeX; countX++)
         thisColorArray[colorArrayIndex++] = GetPixel(hdc, countX, countY);

   memSurface[action]->ReleaseDC(hdc);

   if (otherObj->memSurface[otherObj->action]->GetDC(&hdc) != DD_OK)
      {
      dprintf("Couldn't get DC on other memSurface!  action = %d  type = %d\n", otherObj->action, otherObj->type);
      theApp->Stop();
      free(thisColorArray);
      free(otherColorArray);
      return FALSE;
      }

   colorArrayIndex = 0;
   for (countY = 0; countY < sizeY; countY++)
      for (countX = 0; countX < sizeX; countX++)
         otherColorArray[colorArrayIndex++] = GetPixel(hdc, countX, countY);

   otherObj->memSurface[action]->ReleaseDC(hdc);

   for (colorArrayIndex = 0; colorArrayIndex < sizeX * sizeY; colorArrayIndex++)
      {
      if ((thisColorArray[colorArrayIndex] != 1) && (otherColorArray[colorArrayIndex] != 1))
         {
         free(thisColorArray);
         free(otherColorArray);
         return TRUE;
         }
      }

   free(thisColorArray);
   free(otherColorArray);
   return FALSE;
#endif
   }



AliveObject * AliveObject :: CheckOverlapped(AliveList *theList)
   {
   AliveObject *  nextObject;
   AliveListItr   listItr;

   listItr = theList->begin();
   while (listItr != theList->end())
      {
      nextObject = *listItr;

      // skip over this object
      if (nextObject != this)
         {
         // if bounding rectangles overlap...
         if (Overlapped(nextObject))
            {
            // check for pixel collision
            if (CheckPixelCollision(nextObject))
               return nextObject;
            }
         }

      ++listItr;
      }

   return NULL;
   }
