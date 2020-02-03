
#ifndef __House_hpp__
#define __House_hpp__


#include "AliveObject.hpp"
#include "Emitter.hpp"

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <dsound.h>
#include "dsutil.h"



#define NUM_HOUSE_HIT_SOUNDS     5



class House : public AliveObject
   {
private:
   char     fireArray[128][32];
   int      lastHealth;
   DWORD    nextFlickerTime;
   list<Emitter *> flameList;
   Emitter *faucet;
   HSNDOBJ  hitSound[NUM_HOUSE_HIT_SOUNDS];

   void     GetNextFrame(void);
   void     DrawFire(int addFlamePoints);
   int      AverageFire(int xPos, int yPos);


public:

            House();
            ~House();
   RECT *   Move(AliveList *collideableList);
   HRESULT  Draw(void);
   void     TurnOnFaucet(void);
   void     TurnOffFaucet(void);
   void     BlockFaucet(void);
   void     PutOutFires(void);
   void     SetHealth(int newHealth);
   void     NewLevel(int level);
   void     Hit(int damage);
   };



#endif      // #ifndef __House_hpp__
