
#ifndef __Hero_hpp__
#define __Hero_hpp__


#include "AliveObject.hpp"
#include "Basket.hpp"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <dsound.h>
#include "dsutil.h"
#include "global.h"
#include <list>

#pragma warning(disable : 4786) //* disable the C4786 truncating warning.

using namespace std;


#define SHOW_HEALTH


#define NUM_HERO_HIT_SOUNDS   17


class Hero : public AliveObject
   {
private:
   Basket *       basket;
   DWORD          nextFlipTime;
   HSNDOBJ        hitSound[NUM_HERO_HIT_SOUNDS];
#ifdef SHOW_HEALTH
   char           healthBitmap[50];
#endif

   void           MoveBasket(void);
   void           DrawHealthLevel(void);

public:

                  Hero();
                 ~Hero();
   RECT *         Move(AliveList *collideableList);
   void           FlipDir(void);
   void           MoveLeft(void);
   void           MoveRight(void);
   void           NewLevel(int level);
   void           Hit(int damage);
   HRESULT        Draw(void);
   };



#endif      // #ifndef __Hero_hpp__
