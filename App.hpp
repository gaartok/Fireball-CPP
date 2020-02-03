/*==========================================================================
 *
 *  Copyright (C) 1998 David Durant
 *
 *  File:       App.hpp
 *  Content:    main include file
 *
 *
 ***************************************************************************/
#ifndef __App_hpp__
#define __App_hpp__


#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "dsutil.h"
#include "input.hpp"
#include "misc.hpp"
#include "AliveObject.hpp"



#define NUM_WIN_SOUNDS     6
#define NUM_LOSE_SOUNDS    6



class App
   {
private:
   BOOL                    status;
   BOOL                    active;
   DWORD                   frameCount;
   DWORD                   frameTime;
   DWORD                   frames;
   DWORD                   framesLast;
   DWORD                   transType;
   BOOL                    showFrameCount;
   BOOL                    mouseVisible;
//   HSNDOBJ                 boinkSound;
   DWORD                   nextUpdateTime;   // when we should do our next screen update
   DWORD                   nextSpewTime;     // when we should spew another rock out the volcano
   DWORD                   spewRateMin;      // rate at which to spew rocks in milliseconds (min time between)
   DWORD                   spewRateMax;      // rate at which to spew rocks in milliseconds (max time between)
   HSNDOBJ                 openingSound;
   HSNDOBJ                 winSound[NUM_WIN_SOUNDS];
   HSNDOBJ                 loseSound[NUM_LOSE_SOUNDS];
   int                     highScores[10];
   char                    highScoreNames[10][31];
   UINT32                  surfaceNumbersID;

public:
            App();
            ~App();
//   void     Pause(void);
//   void     Unpause(void);
   void     Setup(void);
   void     CheckMenuItems(HWND hwnd);
   void     DestroySound(void);
   void     DisplayLevel(void);
   void     DrawDisplayList(void);
   void     EraseAllScreens(void);
   void     InitializeSound(void);
   void     UpdateDisplayList(void);
   void     InitLevel(void);
   void     DisplayFrameRate(void);
   void     DisplayPlayerScore(void);
   void     CheckOneMenuItem(HMENU hmenu, UINT idc, BOOL fCheck);
   BOOL     IsActive(void);
   void     ToggleFrameRateDisplay(void);
   void     ToggleSound(void);
   void     ActivateApp(BOOL newValue);
   void     SetAppCursor(void);
   BOOL     Initialize(HWND hWndMain);
   BOOL     RunCycle(void);
   BOOL     GetStatus(void);
   void     LeftClick(int keysSet, int xPos, int yPos);
   void     RightClick(int keysSet, int xPos, int yPos);
//   void     PlayPanned(HSNDOBJ hSO, AliveObject *object);
   void     Stop(void);
   int      ShowEndingScore(void);
//   void     DeleteObject(AliveObject *deadObj);
   int      ShowInstructions(void);
   void     ReadHighScores(void);
   void     WriteHighScores(void);
   int      SortHighScores(void);
   int      ShowHighScores(void);
   int      EnterName(void);
   int      InputString(char *theString, size_t maxLen);
   };


#endif      // #ifndef __App_hpp__


