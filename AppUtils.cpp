
#include "App.hpp"
#include "resource.h"
#include "global.h"
#include "debug.h"

extern HWND    hWndMain;


BOOL App :: GetStatus(void)
   {
   return status;
   }



void App :: InitializeSound(void)
   {
   soundEnabled = FALSE;

   if (SUCCEEDED(DirectSoundCreate(NULL, &ds, NULL)))
      {
      if (SUCCEEDED(ds->SetCooperativeLevel(hWndMain, DSSCL_NORMAL)))
         {
         soundEnabled = TRUE;
         }
      else
         {
         dprintf("ds->SetCooperativeLevel failed!!\n");
         ds->Release();
         ds = NULL;
         }
      }
   else
      dprintf("DirectSoundCreate failed!!\n");

//   dprintf("soundEnabled = %d\n", soundEnabled);
   }



void App :: DestroySound(void)
   {
   soundEnabled = FALSE;

   if (ds)
      {
//      SndObjDestroy(boinkSound);
//      boinkSound = NULL;

      ds->Release();
      ds = NULL;
      }
   }




void App :: SetAppCursor(void)
   {
   if (!mouseVisible)
      SetCursor(NULL);
   else
      SetCursor(LoadCursor(NULL, IDC_ARROW));
   }



void App :: ToggleSound(void)
   {
//   if (soundEnabled)
//      DestroySound();
//   else
//      InitializeSound();
   }



BOOL App :: IsActive(void)
   {
   return active;
   }




void App :: ActivateApp(BOOL newValue)
   {
   active = newValue;

   if (active)
      {
      mouseVisible = FALSE;
//      mouseVisible = TRUE;
      // we are active, need to reacquire the keyboard
      ReacquireInput();
      }
   else
      {
      mouseVisible = TRUE;
      // DirectInput automatically unacquires for us in FOREGROUND mode
      }
   }


#if 0

void App :: Pause(void)
   {
   lpDD->FlipToGDISurface();
   DrawMenuBar(hWndMain);
   RedrawWindow(hWndMain, NULL, NULL, RDW_FRAME);
   }



/*
 *  AppUnpause
 *
 *  Reset the various time counters so the donuts don't suddenly
 *  jump halfways across the screen and so the frame rate remains accurate.
 */
void App :: Unpause(void)
   {
//   lastTickCount = frameTime = timeGetTime();
   }

#endif



/*
 *  CheckOneMenuItem
 *
 *  Helper function that checks a single item in a menu.
 */
void App :: CheckOneMenuItem(HMENU hmenu, UINT idc, BOOL fCheck)
   {
   CheckMenuItem(hmenu, idc, fCheck ? (MF_BYCOMMAND | MF_CHECKED) : (MF_BYCOMMAND | MF_UNCHECKED));
   }



/*
 *  CheckMenuItems
 *
 *  Sync the menu checkmarks with our internal variables
 */
void App :: CheckMenuItems(HWND hwnd)
   {
   HMENU hmenu = GetMenu(hwnd);

//   CheckOneMenuItem(hmenu, IDC_TRAILS, bSpecialEffects);
//   CheckOneMenuItem(hmenu, IDC_AUDIO, bWantSound && soundEnabled);
   CheckOneMenuItem(hmenu, IDC_FRAMERATE, showFrameCount);
   }



void App :: ToggleFrameRateDisplay()
   {
   showFrameCount = !showFrameCount;

   if (showFrameCount)
      {
      frameCount = 0;
      frameTime = timeGetTime();
      }
   }




void App :: DisplayPlayerScore(void)
   {
   int   textWidth;
   char  strOut[16];

   sprintf(strOut, "%d", playerScore);
   textWidth = strlen(strOut) * 16;
   renderer->BltText(strOut, surfaceNumbersID, SCREEN_WIDTH / 2 - (textWidth / 2), 10);
   }




void App :: DisplayFrameRate(void)
   {
   DWORD time2;
   char  buff[6];

   frameCount++;
   time2 = timeGetTime() - frameTime;

   if (time2 > 1000)
      {
      frames = (frameCount * 1000) / time2;
      frameTime = timeGetTime();
      frameCount = 0;
      }

   if (frames == 0)
      return;

   if (frames != framesLast)
      framesLast = frames;

   sprintf(buff, "%d", frames);
   renderer->BltText(buff, surfaceNumbersID, 10, 10);
   }



