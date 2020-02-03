
#include "Flame.hpp"
#include "Misc.hpp"
#include "global.h"
#include "debug.h"



Flame :: Flame()
   {
   type   = TYPE_FLAME;
   action = ACTION_IDLE_RIGHT;

   velocity.x        = 0;
   velocity.y        = 0;
   minVelocity.x     = 0;
   minVelocity.y     = 0;
   maxVelocity.x     = 0;
   maxVelocity.y     = 0;

   videoSurface[ACTION_IDLE_RIGHT]     = surfaceDescr[SURFACE_SPRITES].videoSurface;
   memSurface[ACTION_IDLE_RIGHT]       = surfaceDescr[SURFACE_SPRITES].memSurface;
   numFrames[ACTION_IDLE_RIGHT].x      = 5;
   numFrames[ACTION_IDLE_RIGHT].y      = 1;
   size[ACTION_IDLE_RIGHT].x           = 16;
   size[ACTION_IDLE_RIGHT].y           = 16;
   bmpOffset[ACTION_IDLE_RIGHT].x      = 0;
   bmpOffset[ACTION_IDLE_RIGHT].y      = 0;

   Initialize();
   }



Flame :: ~Flame()
   {
   }



void Flame :: GetNextFrame(void)
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

