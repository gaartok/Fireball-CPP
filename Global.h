#ifndef __Global_H__
#define __Global_H__


//#ifndef  DIRECTDRAW_VERSION
//#define   DIRECTDRAW_VERSION 0x0300
//#endif

//#include "Windows.h"
//#include <mmsystem.h>
//#include <ddraw.h>
//#include <dsound.h>
#include "App.hpp"
//#include "Screen.hpp"
#include "Renderer.hpp"
#include "House.hpp"
#include "misc.hpp"
//#include <list>


//#pragma warning(disable : 4786) //* disable the C4786 truncating warning.


//using namespace std;


//extern Screen *   screenPtr;
extern App *      theApp;
extern House *    houseLeft;
extern House *    houseRight;
extern Renderer * renderer;
extern AliveList  mainList;
extern HINSTANCE  hInst;
//extern HWND       hWndMain;
extern HACCEL     hAccel;
extern HINSTANCE  hInst;
extern BOOL       useEmulation;
extern double     Dirx[40];
extern double     Diry[40];
extern int        playerScore;
extern int        playerLevel;
extern BOOL       soundEnabled;


//extern LPDIRECTDRAW           lpDD;

// bitmap surfaces
//extern LPDIRECTDRAWSURFACE    frontBuffer;
//extern LPDIRECTDRAWSURFACE    backBuffer;
//extern LPDIRECTDRAWPALETTE    artPalette;


//extern LPDIRECTDRAWSURFACE    surfaceArray[SURFACE_LAST];
//extern SURFACE_DESCR          surfaceDescr[SURFACE_LAST];
extern LPDIRECTSOUND          ds;

                  
extern DWORD      fillColor;
extern BYTE       bDDbpp;
extern DWORD      dwDDInitFlags;


extern int        upperCharWidths[];
extern int        lowerCharWidths[];


// global functions
extern double     randDouble(double low, double high);
extern int        randInt(int low, int high);
extern DWORD      randDWORD(DWORD low, DWORD high);
extern float      randFloat(float low, float high);
extern int        getint(char**p, int def);
extern BOOL       GetSign(long numIn);


#endif
