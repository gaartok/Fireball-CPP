
#ifndef __Basket_hpp__
#define __Basket_hpp__


#include "AliveObject.hpp"
#include "Emitter.hpp"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <dsound.h>
#include "dsutil.h"


#define SHOW_SPLASH



class Basket : public AliveObject
   {
private:
   int      waterLevel;
   char     waterBitmap[50][50];
   DWORD    underFaucetTime;
   HSNDOBJ  hitSound;
   HSNDOBJ  fillSound;
   Emitter *waterOverflow;

#ifdef SHOW_SPLASH
   DWORD    splashEndTime;
   Emitter *waterSplash;
#endif

   void     GetNextFrame(void);
   void     DrawWaterLevel(void);
   void     UpdateWaterLevel(void);


public:

            Basket();
            ~Basket();
   HRESULT  Draw(void);
   void     Hit(int damage);
   int      GetWaterLevel(void);
   void     NewLevel(int level);
   };



#endif      // #ifndef __Basket_hpp__
