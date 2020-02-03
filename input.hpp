/*==========================================================================
 *
 *  Copyright (C) 1995-1997 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       input.h
 *
 *
 ***************************************************************************/

#ifndef _INPUT_H
#define _INPUT_H

#define DIRECTINPUT_VERSION  0x0500

#include <dinput.h>

//--------------------------------------------------------------------------


// keyboard buffer size
#define KEYBUFSIZE 32

/*
 * keyboard commands
 */
#define KEY_STOP   0x00000001l
#define KEY_DOWN   0x00000002l
#define KEY_LEFT   0x00000004l
#define KEY_RIGHT  0x00000008l
#define KEY_UP     0x00000010l
#define KEY_SPACE  0x00000020l
#define KEY_THROW  0x00000040l
#define KEY_SHIELD 0x00000080l
#define KEY_ESCAPE 0x00000100l


//--------------------------------------------------------------------------

// external variables
extern BOOL bKeyboardAcquired;

extern DWORD (*ReadGameInput)(void);

//--------------------------------------------------------------------------

// prototypes
BOOL InitInput(HINSTANCE hInst, HWND hWnd);
void CleanupInput(void);
BOOL ReacquireInput(void);
BOOL PickInputDevice(int);
DWORD ReadOneKeyboardInput(void);
void FlushKeyboardInput(void);
char DIKCodeToAnsi(DWORD DIK_CODE, BOOL shiftPos);

//--------------------------------------------------------------------------
#endif // _INPUT_H








