
#include "global.h"
#include "misc.hpp"
#include "debug.h"

#ifdef _DEBUG
_CrtMemState memState;
#endif


//Screen *             screenPtr = NULL;
App *                theApp = NULL;
Renderer *           renderer = NULL;
AliveList            mainList;
House *              houseLeft;
House *              houseRight;


HINSTANCE            hInst;
HACCEL               hAccel;
BOOL                 useEmulation;

//LPDIRECTDRAWSURFACE  surfaceArray[SURFACE_LAST];

LPDIRECTSOUND        ds             = NULL;

DWORD                fillColor;

int                  playerScore = 0;
int                  playerLevel = LEVEL1;
BOOL                 soundEnabled = FALSE;




int upperCharWidths[] =
   {
   14,      // A
   12,      // B
   13,      // C
   12,      // D
   11,      // E
   10,      // F
   14,      // G
   12,      // H
   11,      // I
   10,      // J
   12,      // K
   10,      // L
   15,      // M
   12,      // N
   14,      // O
   11,      // P
   15,      // Q
   11,      // R
   12,      // S
   11,      // T
   12,      // U
   13,      // V
   16,      // W
   12,      // X
   13,      // Y
   12       // Z
   };



int lowerCharWidths[] =
   {
   11,      // a
   11,      // b
   10,      // c
   11,      // d
   10,      // e
    7,      // f
   11,      // g
   11,      // h
    3,      // i
    4,      // j
   10,      // k
    3,      // l
   16,      // m
   11,      // n
   11,      // o
   11,      // p
   11,      // q
    7,      // r
   10,      // s
    6,      // t
   11,      // u
   11,      // v
   15,      // w
   10,      // x
   11,      // y
    9       // z
   };

