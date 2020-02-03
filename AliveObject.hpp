
#ifndef __AliveObject_hpp__
#define __AliveObject_hpp__

#include <windows.h>
#include <stdio.h>
#include "Misc.hpp"
//#include "Types.h"
#include <list>

#pragma warning(disable : 4786) //* disable the C4786 truncating warning.

using namespace std;


class AliveObject;

typedef list<AliveObject *>             AliveList;
typedef list<AliveObject *>::iterator   AliveListItr;


class AliveObject
   {
protected:
   UINT                 type;                      // type of object
   UINT32               surfaceID;
   UINT                 action;
   POINT                objCenter;                 // the object's center point
   RECT                 objRect;                   // the object's position RECT
   UINT                 frame[MAX_ACTIONS];
   POINT                numFrames[MAX_ACTIONS];
   POINT                frameIndex;
   POINT                velocity;
   POINT                minVelocity;
   POINT                maxVelocity;
   POINT                size[MAX_ACTIONS];
   POINT                halfSize[MAX_ACTIONS];
   RECT                 firstFrame[MAX_ACTIONS];   // first frame for each action
   RECT                 nextFrame;                 // the next frame to be displayed
   UINT                 frameCount;                // the number of times the current frame has been displayed
//   LPDIRECTDRAWSURFACE  videoSurface[MAX_ACTIONS]; // the bitmaps for the object (in video memory)
//   LPDIRECTDRAWSURFACE  memSurface[MAX_ACTIONS];   // the bitmaps for the object (in system memory)
   POINT                bmpOffset[MAX_ACTIONS];    // offsets within the .BMP for each action
   BOOL                 selected;
   POINT                finalDestination;
   POINT                destination;               // where the unit is headed on the map
   RECT                 moveBounds;
   DWORD                nextMoveTime;
   int                  health;

   void              Initialize(void);
   void              DeleteMyPath(void);
   virtual void      GetNextFrame(void);

public:
                     AliveObject();
   virtual           ~AliveObject();
   RECT *            SetDst(long xDst, long yDst);
   long              GetXSize(void);
   long              GetYSize(void);
   void              SetSelected(BOOL newValue);
   BOOL              IsSelected(void);
   BOOL              CheckForHit(int xPos, int yPos);
   void              SetDestination(long xPos, long yPos);
   POINT             GetDestination(void);
   POINT             GetObjCenter(void);
   RECT *            GetRect(void);
   BOOL              Overlapped(RECT *testRect);
   BOOL              Overlapped(AliveObject *testObj);
   long              GetXPosition(void);
   long              GetYPosition(void);
   void              SetAction(int newAction);
   void              SetFinalDest(long finalX, long finalY);
   BOOL              IsAlive(void);
   UINT              GetType(void);
   BOOL              CheckPixelCollision(AliveObject * theObject);
   int               GetHealth(void);
   AliveObject *     CheckOverlapped(AliveList *theList);
   virtual void      SetHealth(int newHealth);
   virtual HRESULT   Draw(void);
   virtual RECT *    Move(AliveList *collideableList);
   virtual void      Hit(int damage);
   virtual void      FlipDir(void);
   virtual void      MoveLeft(void);
   virtual void      MoveRight(void);
   virtual void      NewLevel(int level);
   };



#endif      // #ifndef __AliveObject_hpp__
