
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#define INITGUID
//#include <windows.h>
#include "App.hpp"
#include "Misc.hpp"
#include "Global.h"
#include "resource.h"
#include "global.h"
#include "debug.h"


void  UpdateFrame(void);
//long FAR PASCAL MainWndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



static int     programState;
static DWORD   stateTimer;
HWND    hWndMain;




int MakeWindow(HINSTANCE hInstance, int nCmdShow, WNDPROC callBackFunc)
   {
   WNDCLASS    wc;
   BOOL        rc;

   wc.style = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc = callBackFunc;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = hInstance;
   wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(APP_ICON));
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//   wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
   wc.hbrBackground = NULL;
   wc.lpszMenuName =  MAKEINTRESOURCE(APP_MENU);
   wc.lpszMenuName =  NULL;
   wc.lpszClassName = "AppClass";
   rc = RegisterClass(&wc);
   if (!rc)
      {
      LPVOID   lpMsgBuf;

      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf, 0, NULL);
      dprintf("RegisterClass failed - %s", lpMsgBuf);
      LocalFree(lpMsgBuf);
      return FALSE;
      }

   hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(APP_ACCEL));
   if (!hAccel)
      return FALSE;

/*
   hWndMain = CreateWindowEx( 
    0,                      // no extended styles           
    "MainWClass",           // class name                   
    "Main Window",          // window name                  
    WS_OVERLAPPEDWINDOW |   // overlapped window            
        WS_HSCROLL |        // horizontal scroll bar        
        WS_VSCROLL,         // vertical scroll bar          
    CW_USEDEFAULT,          // default horizontal position  
    CW_USEDEFAULT,          // default vertical position    
    CW_USEDEFAULT,          // default width                
    CW_USEDEFAULT,          // default height               
    (HWND) NULL,            // no parent or owner window    
    (HMENU) NULL,           // class menu used              
    hInstance,              // instance handle              
    NULL);                  // no window creation data      
*/ 

   hWndMain = CreateWindowEx(0,
               "AppClass",
               "App",
               WS_POPUP,    // non-app window
               0,0,
               SCREEN_WIDTH,
               SCREEN_HEIGHT,
               NULL,
               NULL,
               hInstance,
               NULL);

   if (!hWndMain)
      return FALSE;

   ShowWindow(hWndMain, nCmdShow);
   UpdateWindow(hWndMain);
   SetFocus(hWndMain);

   return TRUE;
   }




long FAR PASCAL MainWndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
   {
   PAINTSTRUCT ps;
   HDC         hdc;
//   UINT        cmd;

   switch(message)
      {
      case WM_LBUTTONDOWN:
         {
//       fwKeys = wParam;        // key flags
                                 // MK_CONTROL  Set if the CTRL key is down.
                                 // MK_LBUTTON  Set if the left mouse button is down.
                                 // MK_MBUTTON  Set if the middle mouse button is down.
                                 // MK_RBUTTON  Set if the right mouse button is down.
                                 // MK_SHIFT    Set if the SHIFT key is down.
//       xPos = LOWORD(lParam);  // horizontal position of cursor
//       yPos = HIWORD(lParam);  // vertical position of cursor
         if (theApp)
            theApp->LeftClick(wParam, LOWORD(lParam), HIWORD(lParam));
         break;
         }

      case WM_RBUTTONDOWN:
         {
         if (theApp)
            theApp->RightClick(wParam, LOWORD(lParam), HIWORD(lParam));
         }

      case WM_ACTIVATEAPP:
         {
         if (theApp)
            theApp->ActivateApp((BOOL)wParam);
         break;
         }

      case WM_CREATE:
         {
         break;
         }

      case WM_SETCURSOR:
         {
         if (theApp)
            theApp->SetAppCursor();
         return TRUE;
         }

      case WM_COMMAND:
         {
         break;
         }

      case WM_INITMENU:
         {
         break;
         }

      case WM_ERASEBKGND:
         {
         return 1;
         }

      case WM_PAINT:
         {
         hdc = BeginPaint(hWnd, &ps);
         EndPaint(hWnd, &ps);
         return 1;
         }

      case WM_DESTROY:
         {
         PostQuitMessage(0);
         break;
         }

      case WM_ENTERMENULOOP:
         {
//         if (theApp)
//            theApp->Pause();
         break;
         }

      case WM_EXITMENULOOP:
         {
//         if (theApp)
//            theApp->Unpause();
         break;
         }

      default:
         {
         break;
         }
      }

   return DefWindowProc(hWnd, message, wParam, lParam);
   }






void UpdateFrame(void)
   {
   if (theApp == NULL)
      return;

   switch(programState)
      {
      case PS_SPLASH:
         {
         if (!renderer->ShowSplash())
            {
            FlushKeyboardInput();
            theApp->Setup();
            programState = PS_INSTRUCTIONS;
            }
         break;
         }

      case PS_INSTRUCTIONS:
         {
         if (theApp->ShowInstructions())
            {
            FlushKeyboardInput();
            programState = PS_SHOW_SCORES;
            }
         break;
         }

      case PS_SHOW_SCORES:
         {
         if (theApp->ShowHighScores())
            {
            FlushKeyboardInput();
            programState = PS_BEGIN_LEVEL;
            }
         break;
         }

      case PS_BEGIN_LEVEL:
         {
         FlushKeyboardInput();
         theApp->InitLevel();
         programState = PS_ACTIVE;
         break;
         }

      case PS_ACTIVE:
         {
         if (theApp->RunCycle())
            {
            FlushKeyboardInput();
            programState = PS_ENDGAME;
            }

         if (!theApp->GetStatus())
            programState = PS_FINISHED;
         break;
         }

      case PS_ENDGAME:
         {
         if (theApp->EnterName())
            {
            FlushKeyboardInput();
            programState = PS_ENTER_NAME;
            }
         break;
         }

      case PS_ENTER_NAME:
         {
         if (theApp->ShowEndingScore())
            {
            FlushKeyboardInput();
            programState = PS_SHOW_SCORES;
            }
         break;
         }

      case PS_FINISHED:
         {
         PostMessage(hWndMain, WM_CLOSE, 0, 0);
         programState = PS_IDLE;
         break;
         }

      case PS_IDLE:
         {
         break;
         }

      default:
         {
         programState = PS_SPLASH;
         break;
         }
      }
   }



int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
   {
   MSG     msg;

   DEBUG_MEM_INIT;

   // save off application instance
   hInst = hInstance;

   // parse the command line
   while (lpCmdLine[0] == '-')
      {
      lpCmdLine++;

      switch (*lpCmdLine++)
         {
         case 'e':
            {
            useEmulation = TRUE;
            break;
            }

         case 't':
            {
            break;
            }

         case 'x':
            {
            break;
            }
         }

      while (IS_SPACE(*lpCmdLine))
         {
         lpCmdLine++;
         }
      }

//   ScreenX = (long)getint(&lpCmdLine, 640);
//   ScreenY = (long)getint(&lpCmdLine, 480);
//   ScreenBpp = getint(&lpCmdLine, 8);

   if (!MakeWindow(hInstance, nCmdShow, MainWndproc))
      return FALSE;

   theApp = new App();
   if (!theApp)
      return FALSE;

   if (!theApp->Initialize(hWndMain))
      {
      DestroyWindow(hWndMain);
      delete theApp;
      theApp = NULL;
      return FALSE;
      }


   programState = PS_SPLASH;
//   programState = PS_BEGIN_LEVEL;
//   programState = PS_SHOW_SCORES;

   FlushKeyboardInput();

   // the main FOREVER loop
   while (1)
      {
      if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
         {
         if (!GetMessage(&msg, NULL, 0, 0))
            {
            if (theApp)
               {
               delete theApp;
               theApp = NULL;
               }
            
            return msg.wParam;
            }

         if (!TranslateAccelerator(hWndMain, hAccel, &msg))
            {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            }
         }
      else if (theApp->IsActive())
         UpdateFrame();
      else
         WaitMessage();
      }


   DEBUG_MEM_CHECK;
   }


