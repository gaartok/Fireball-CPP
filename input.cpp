
#include "input.hpp"
#include "resource.h"
#include "global.h"
#include "debug.h"

extern HWND hWndMain;
extern DWORD ReadKeyboardInput(void);
extern DWORD ReadJoystickInput(void);

// allocate external variables
DWORD (*ReadGameInput)(void) = ReadKeyboardInput;

/*
 *  We'll use up to the first ten input devices.
 *
 *  c_cpdiFound is the number of found devices.
 *  g_rgpdiFound[0] is the array of found devices.
 *  g_pdevCurrent is the device that we are using for input.
 */
#define MAX_DINPUT_DEVICES 10
int g_cpdevFound;
LPDIRECTINPUTDEVICE2 g_rgpdevFound[MAX_DINPUT_DEVICES];
LPDIRECTINPUTDEVICE2 g_pdevCurrent;




/*--------------------------------------------------------------------------
| AddInputDevice
|
| Records an input device in the array of found devices.
|
| In addition to stashing it in the array, we also add it to the device
| menu so the user can pick it.
|
*-------------------------------------------------------------------------*/

void AddInputDevice(LPDIRECTINPUTDEVICE pdev, LPCDIDEVICEINSTANCE pdi)
   {
   if (g_cpdevFound < MAX_DINPUT_DEVICES)
      {

      HRESULT hRes;

      /*
       *  Convert it to a Device2 so we can Poll() it.
       */

      //        hRes = pdev->QueryInterface(&IID_IDirectInputDevice2, (LPVOID *)&g_rgpdevFound[g_cpdevFound]);
      hRes = pdev->QueryInterface(IID_IDirectInputDevice2, (LPVOID *)&g_rgpdevFound[g_cpdevFound]);

      if (SUCCEEDED(hRes))
         {

         HMENU hmenu;

         /*
          *  Add its description to the menu.
          */
         hmenu = GetSubMenu(GetMenu(hWndMain), 0);

         InsertMenu(hmenu, g_cpdevFound, MF_BYPOSITION | MF_STRING,
            IDC_DEVICES + g_cpdevFound,
            pdi->tszInstanceName);

         g_cpdevFound++;
         }
      }
   }

/*--------------------------------------------------------------------------
|
| SetDIDwordProperty
|
| Set a DWORD property on a DirectInputDevice.
|
*-------------------------------------------------------------------------*/

HRESULT SetDIDwordProperty(LPDIRECTINPUTDEVICE pdev, REFGUID guidProperty,
                           DWORD dwObject, DWORD dwHow, DWORD dwValue)
   {
   DIPROPDWORD dipdw;

   dipdw.diph.dwSize       = sizeof(dipdw);
   dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
   dipdw.diph.dwObj        = dwObject;
   dipdw.diph.dwHow        = dwHow;
   dipdw.dwData            = dwValue;

   return pdev->SetProperty(guidProperty, &dipdw.diph);
   }

/*--------------------------------------------------------------------------
| InitKeyboardInput
|
| Initializes DirectInput for the keyboard.  Creates a keyboard device,
| sets the data format to our custom format, sets the cooperative level and
| adds it to the menu.
|
*-------------------------------------------------------------------------*/

BOOL InitKeyboardInput(LPDIRECTINPUT pdi)
   {
   LPDIRECTINPUTDEVICE pdev;
   DIDEVICEINSTANCE di;

   // create the DirectInput keyboard device
   //   if (pdi->CreateDevice(&GUID_SysKeyboard, &pdev, NULL) != DI_OK)
   if (pdi->CreateDevice(GUID_SysKeyboard, &pdev, NULL) != DI_OK)
      {
      OutputDebugString("IDirectInput::CreateDevice FAILED\n");
      return FALSE;
      }

   // set keyboard data format
   if (pdev->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetDataFormat FAILED\n");
      pdev->Release();
      return FALSE;
      }

   // set the cooperative level
   if (pdev->SetCooperativeLevel(hWndMain,
      DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetCooperativeLevel FAILED\n");
      pdev->Release();
      return FALSE;
      }

   // set buffer size
   if (SetDIDwordProperty(pdev, DIPROP_BUFFERSIZE, 0, DIPH_DEVICE, KEYBUFSIZE) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_DEVICE) FAILED\n");
      pdev->Release();
      return FALSE;
      }

   //
   // Get the name of the primary keyboard so we can add it to the menu.
   //
   di.dwSize = sizeof(di);
   if (pdev->GetDeviceInfo(&di) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::GetDeviceInfo FAILED\n");
      pdev->Release();
      return FALSE;
      }

   //
   // Add it to our list of devices.  If AddInputDevice succeeds,
   // he will do an AddRef.
   //
   AddInputDevice(pdev, &di);
   pdev->Release();

   return TRUE;
   }

/*--------------------------------------------------------------------------
| InitJoystickInput
|
| Initializes DirectInput for an enumerated joystick device.
| Creates the device, device, sets the standard joystick data format,
| sets the cooperative level and adds it to the menu.
|
| If any step fails, just skip the device and go on to the next one.
|
*-------------------------------------------------------------------------*/

BOOL FAR PASCAL InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef)
   {
   LPDIRECTINPUT pdi = (LPDIRECTINPUT) pvRef;
   LPDIRECTINPUTDEVICE pdev;
   DIPROPRANGE diprg;

   // create the DirectInput joystick device
   //   if (pdi->CreateDevice(&pdinst->guidInstance, &pdev, NULL) != DI_OK)
   if (pdi->CreateDevice(pdinst->guidInstance, &pdev, NULL) != DI_OK)
      {
      OutputDebugString("IDirectInput::CreateDevice FAILED\n");
      return DIENUM_CONTINUE;
      }

   // set joystick data format
   if (pdev->SetDataFormat(&c_dfDIJoystick) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetDataFormat FAILED\n");
      pdev->Release();
      return DIENUM_CONTINUE;
      }

   // set the cooperative level
   if (pdev->SetCooperativeLevel(hWndMain,
      DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetCooperativeLevel FAILED\n");
      pdev->Release();
      return DIENUM_CONTINUE;
      }

   // set X-axis range to (-1000 ... +1000)
   // This lets us test against 0 to see which way the stick is pointed.

   diprg.diph.dwSize       = sizeof(diprg);
   diprg.diph.dwHeaderSize = sizeof(diprg.diph);
   diprg.diph.dwObj        = DIJOFS_X;
   diprg.diph.dwHow        = DIPH_BYOFFSET;
   diprg.lMin              = -1000;
   diprg.lMax              = +1000;

   if (pdev->SetProperty(DIPROP_RANGE, &diprg.diph) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_RANGE) FAILED\n");
      pdev->Release();
      return FALSE;
      }

   //
   // And again for Y-axis range
   //
   diprg.diph.dwObj        = DIJOFS_Y;

   if (pdev->SetProperty(DIPROP_RANGE, &diprg.diph) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_RANGE) FAILED\n");
      pdev->Release();
      return FALSE;
      }

   // set X axis dead zone to 50% (to avoid accidental turning)
   // Units are ten thousandths, so 50% = 5000/10000.
   if (SetDIDwordProperty(pdev, DIPROP_DEADZONE, DIJOFS_X, DIPH_BYOFFSET, 5000) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_DEADZONE) FAILED\n");
      pdev->Release();
      return FALSE;
      }


   // set Y axis dead zone to 50% (to avoid accidental thrust)
   // Units are ten thousandths, so 50% = 5000/10000.
   if (SetDIDwordProperty(pdev, DIPROP_DEADZONE, DIJOFS_Y, DIPH_BYOFFSET, 5000) != DI_OK)
      {
      OutputDebugString("IDirectInputDevice::SetProperty(DIPH_DEADZONE) FAILED\n");
      pdev->Release();
      return FALSE;
      }


   //
   // Add it to our list of devices.  If AddInputDevice succeeds,
   // he will do an AddRef.
   //
   AddInputDevice(pdev, pdinst);
   pdev->Release();

   return DIENUM_CONTINUE;
   }

/*--------------------------------------------------------------------------
| InitInput
|
| Initializes DirectInput for the keyboard and all joysticks.
|
| For each input device, add it to the menu.
|
*-------------------------------------------------------------------------*/

BOOL InitInput(HINSTANCE hInst, HWND hWnd)
   {
   LPDIRECTINPUT pdi;
   BOOL fRc;
//   DIRECTINPUTCREATE    DirectInputCreate  = NULL;

   // Note: Joystick support is a DirectX 5.0 feature.
   // Since we also want to run on DirectX 3.0, we will start out
   // with DirectX 3.0 to make sure that at least we get the keyboard.

   
   // create the DirectInput interface object
   if (DirectInputCreate(hInst, DIRECTINPUT_VERSION, &pdi, NULL) != DI_OK)
//   if (DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pdi, NULL) != DI_OK)
//	if (FAILED(hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pdi, NULL)))
      {
      OutputDebugString("DirectInputCreate 3.0 FAILED\n");
      return FALSE;
      }

   fRc = InitKeyboardInput(pdi);
   pdi->Release();       // Finished with DX 3.0

   if (!fRc)
      {
      return FALSE;
      }

#if 0
   // create the DirectInput 5.0 interface object
   if (DirectInputCreate(hInst, DIRECTINPUT_VERSION, &pdi, NULL) == DI_OK)
      {
      // Enumerate the joystick devices.  If it doesn't work, oh well,
      // at least we got the keyboard.
      pdi->EnumDevices(DIDEVTYPE_JOYSTICK, InitJoystickInput, pdi, DIEDFL_ATTACHEDONLY);
      pdi->Release();    // Finished with DX 5.0.
      }
   else
      {
      OutputDebugString("DirectInputCreate 5.0 FAILED - no joystick support\n");
      }
#endif

   // Default device is the keyboard
   PickInputDevice(0);

   // if we get here, we were successful
   return TRUE;
   }



/*--------------------------------------------------------------------------
| CleanupInput
|
| Cleans up all DirectInput objects.
*-------------------------------------------------------------------------*/
void CleanupInput(void)
   {
   int idev;

   // make sure the device is unacquired
   // it doesn't harm to unacquire a device that isn't acquired

   if (g_pdevCurrent)
      IDirectInputDevice_Unacquire(g_pdevCurrent);

   // release all the devices we created
   for (idev = 0; idev < g_cpdevFound; idev++)
      {
      if (g_rgpdevFound[idev])
         {
         IDirectInputDevice_Release(g_rgpdevFound[idev]);
         g_rgpdevFound[idev] = 0;
         }
      }
   }




/*--------------------------------------------------------------------------
| ReacquireInput
|
| Reacquires the current input device.  If Acquire() returns S_FALSE,
| that means
| that we are already acquired and that DirectInput did nothing.
*-------------------------------------------------------------------------*/
BOOL ReacquireInput(void)
   {
   HRESULT hRes;

   // if we have a current device
   if (g_pdevCurrent)
      {
      // acquire the device
      hRes = IDirectInputDevice_Acquire(g_pdevCurrent);
      if (SUCCEEDED(hRes))
         {
         // acquisition successful
         return TRUE;
         }
      else
         {
         // acquisition failed
         return FALSE;
         }
      }
   else
      {
      // we don't have a current device
      return FALSE;
      }

   }




void FlushKeyboardInput(void)
   {
#if 1
   DWORD                dwEvents;
   HRESULT              hRes;
   DIDEVICEOBJECTDATA   rgKeyData[KEYBUFSIZE];

   dwEvents = KEYBUFSIZE;
   while (dwEvents > 0)
      {
      dwEvents = KEYBUFSIZE;
      hRes = IDirectInputDevice_GetDeviceData(g_pdevCurrent, sizeof(DIDEVICEOBJECTDATA), rgKeyData, &dwEvents, 0);
      if (hRes == DIERR_INPUTLOST)
         {
         ReacquireInput();
         dwEvents = KEYBUFSIZE;
         }
      }
#endif
   }



DWORD ReadOneKeyboardInput(void)
   {
   DIDEVICEOBJECTDATA      rgKeyData[1];
   DWORD                   dwEvents;
   static DWORD            dwKeyState = 0;
   HRESULT                 hRes;

   // get data from the keyboard
   dwEvents = 1;
   hRes = IDirectInputDevice_GetDeviceData(g_pdevCurrent, sizeof(DIDEVICEOBJECTDATA), rgKeyData, &dwEvents, 0);

   if (hRes != DI_OK)
      {
      // did the read fail because we lost input for some reason?
      // if so, then attempt to reacquire.  If the second acquire
      // fails, then the error from GetDeviceData will be
      // DIERR_NOTACQUIRED, so we won't get stuck an infinite loop.
      if (hRes == DIERR_INPUTLOST)
         ReacquireInput();

      // return the fact that we did not read any data
      return 0;
      }

   if (dwEvents <= 0)
      return 0;
   
   // we have a keystroke...
   if (rgKeyData[0].dwData & 0x80)
      return(rgKeyData[0].dwOfs);                  // it's the downstroke
   else
      return(rgKeyData[0].dwOfs | 0x10000000);     // it's the upstroke
   }



/*--------------------------------------------------------------------------
| ReadKeyboardInput
|
| Requests keyboard data and performs any needed processing.
*-------------------------------------------------------------------------*/
DWORD ReadKeyboardInput(void)
   {
   DIDEVICEOBJECTDATA      rgKeyData[KEYBUFSIZE];
   DWORD                   dwEvents;
   DWORD                   dw;
   static DWORD            dwKeyState = 0;
   HRESULT                 hRes;

   // get data from the keyboard
   dwEvents = KEYBUFSIZE;
   hRes = IDirectInputDevice_GetDeviceData(g_pdevCurrent, sizeof(DIDEVICEOBJECTDATA), rgKeyData, &dwEvents, 0);

   if (hRes != DI_OK)
      {
      // did the read fail because we lost input for some reason?
      // if so, then attempt to reacquire.  If the second acquire
      // fails, then the error from GetDeviceData will be
      // DIERR_NOTACQUIRED, so we won't get stuck an infinite loop.
      if (hRes == DIERR_INPUTLOST)
         ReacquireInput();

      // return the fact that we did not read any data
      return 0;
      }

   // process the data
   for (dw = 0; dw < dwEvents; dw++)
      {
      switch (rgKeyData[dw].dwOfs)
         {
         case DIK_ESCAPE:
            {
            if (rgKeyData[dw].dwData & 0x80)
               dwKeyState |= KEY_ESCAPE;
            else
               dwKeyState &= (DWORD)~KEY_ESCAPE;
            break;
            }

         case DIK_SPACE:
            {
            if (rgKeyData[dw].dwData & 0x80)
               dwKeyState |= KEY_SPACE;
            else
               dwKeyState &= (DWORD)~KEY_SPACE;
            break;
            }

         case DIK_NUMPAD5:
            {
            if (rgKeyData[dw].dwData & 0x80)
               dwKeyState |= KEY_STOP;
            else
               dwKeyState &= ~KEY_STOP;
            break;
            }

         case DIK_NUMPAD7:
            {
            if (rgKeyData[dw].dwData & 0x80)
               dwKeyState |= KEY_SHIELD;
            else
               dwKeyState &= ~KEY_SHIELD;
            break;
            }

         case DIK_UP:
         case DIK_NUMPAD8:
            {
            if (rgKeyData[dw].dwData & 0x80)
               dwKeyState |= KEY_UP;
            else
               dwKeyState &= ~KEY_UP;
            break;
            }

         case DIK_DOWN:
         case DIK_NUMPAD2:
            {
            if (rgKeyData[dw].dwData & 0x80)
               dwKeyState |= KEY_DOWN;
            else
               dwKeyState &= ~KEY_DOWN;
            break;
            }

         case DIK_LEFT:
         case DIK_NUMPAD4:
            {
            if (rgKeyData[dw].dwData & 0x80)
               dwKeyState |= KEY_LEFT;
            else
               dwKeyState &= ~KEY_LEFT;
            break;
            }

         case DIK_RIGHT:
         case DIK_NUMPAD6:
            {
            if (rgKeyData[dw].dwData & 0x80)
               dwKeyState |= KEY_RIGHT;
            else
               dwKeyState &= ~KEY_RIGHT;
            break;
            }
         }
      }

   // return the state of the keys to the caller
   return dwKeyState;
   }




/*--------------------------------------------------------------------------
| ReadJoystickInput
|
| Requests joystick data and performs any needed processing.
|
*-------------------------------------------------------------------------*/
DWORD ReadJoystickInput(void)
   {
   DWORD                   dwKeyState;
   HRESULT                 hRes;
   DIJOYSTATE              js;

   // poll the joystick to read the current state
   hRes = IDirectInputDevice2_Poll(g_pdevCurrent);

   // get data from the joystick
   hRes = IDirectInputDevice_GetDeviceState(g_pdevCurrent,
      sizeof(DIJOYSTATE), &js);

   if (hRes != DI_OK)
      {
      // did the read fail because we lost input for some reason?
      // if so, then attempt to reacquire.  If the second acquire
      // fails, then the error from GetDeviceData will be
      // DIERR_NOTACQUIRED, so we won't get stuck an infinite loop.
      if (hRes == DIERR_INPUTLOST)
         ReacquireInput();

      // return the fact that we did not read any data
      return 0;
      }

   // Now study the position of the stick and the buttons.
   dwKeyState = 0;

   if (js.lX < 0)
      dwKeyState |= KEY_LEFT;
   else if (js.lX > 0)
      dwKeyState |= KEY_RIGHT;

   if (js.lY < 0)
      dwKeyState |= KEY_UP;
   else if (js.lY > 0)
      dwKeyState |= KEY_DOWN;

   if (js.rgbButtons[0] & 0x80)
      dwKeyState |= KEY_SPACE;      // KEY_FIRE

   if (js.rgbButtons[1] & 0x80)
      dwKeyState |= KEY_SHIELD;

   if (js.rgbButtons[2] & 0x80)
      dwKeyState |= KEY_STOP;

   return dwKeyState;
   }



/*--------------------------------------------------------------------------
| PickInputDevice
|
| Make the n'th input device the one that we will use for game play.
|
*-------------------------------------------------------------------------*/

BOOL PickInputDevice(int n)
   {
   if (n < g_cpdevFound)
      {
      // Unacquire the old device.
      if (g_pdevCurrent)
         IDirectInputDevice_Unacquire(g_pdevCurrent);

      // Move to the new device.
      g_pdevCurrent = g_rgpdevFound[n];

      // Set ReadGameInput to the appropriate handler.
      if (n == 0)
         ReadGameInput = ReadKeyboardInput;
      else
         ReadGameInput = ReadJoystickInput;

      CheckMenuRadioItem(GetSubMenu(GetMenu(hWndMain), 0),
                  IDC_DEVICES, IDC_DEVICES + g_cpdevFound - 1,
                  IDC_DEVICES + n, MF_BYCOMMAND);

      ReacquireInput();
      return TRUE;
      }
   else
      {
      return FALSE;
      }
   }




char DIKCodeToAnsi(DWORD DIK_CODE, BOOL shiftPos)
   {
   if (shiftPos)
      {
      switch (DIK_CODE & 0x000000ff)   // if shift is active, return caps
         {
         case 0:
            return 0;
         case DIK_ESCAPE:
            return 27;
         case DIK_1:
            return '!';
         case DIK_2:
            return '@';
         case DIK_3:
            return '#';
         case DIK_4:
            return '$';
         case DIK_5:
            return '%';
         case DIK_6:
            return '^';
         case DIK_7:
            return '&';
         case DIK_8:
            return '*';
         case DIK_9:
            return '(';
         case DIK_0:
            return ')';
         case DIK_MINUS:
            return '_';
         case DIK_EQUALS:
            return '+';
         case DIK_BACK:
            return 8;
         case DIK_TAB:
            return 9;
         case DIK_Q:
            return 'Q';
         case DIK_W:
            return 'W';
         case DIK_E:
            return 'E';
         case DIK_R:
            return 'R';
         case DIK_T:
            return 'T';
         case DIK_Y:
            return 'Y';
         case DIK_U:
            return 'U';
         case DIK_I:
            return 'I';
         case DIK_O:
            return 'O';
         case DIK_P:
            return 'P';
         case DIK_LBRACKET:
            return '{';
         case DIK_RBRACKET:
            return '}';
         case DIK_RETURN:
            return 13;
         case DIK_A:
            return 'A';
         case DIK_S:
            return 'S';
         case DIK_D:
            return 'D';
         case DIK_F:
            return 'F';
         case DIK_G:
            return 'G';
         case DIK_H:
            return 'H';
         case DIK_J:
            return 'J';
         case DIK_K:
            return 'K';
         case DIK_L:
            return 'L';
         case DIK_SEMICOLON:
            return ':';
         case DIK_APOSTROPHE:
            return '"';
         case DIK_GRAVE:
            return '~';
         case DIK_BACKSLASH:
            return '|';
         case DIK_Z:
            return 'Z';
         case DIK_X:
            return 'X';
         case DIK_C:
            return 'C';
         case DIK_V:
            return 'V';
         case DIK_B:
            return 'B';
         case DIK_N:
            return 'N';
         case DIK_M:
            return 'M';
         case DIK_COMMA:
            return '<';
         case DIK_PERIOD:
            return '>';
         case DIK_SLASH:
            return '?';
         case DIK_MULTIPLY:
            return '*';
         case DIK_SPACE:
            return ' ';
         case DIK_NUMPAD7:
            return '7';
         case DIK_NUMPAD8:
            return '8';
         case DIK_NUMPAD9:
            return '9';
         case DIK_SUBTRACT:
            return '-';
         case DIK_NUMPAD4:
            return '4';
         case DIK_NUMPAD5:
            return '5';
         case DIK_NUMPAD6:
            return '6';
         case DIK_ADD:
            return '+';
         case DIK_NUMPAD1:
            return '1';
         case DIK_NUMPAD2:
            return '2';
         case DIK_NUMPAD3:
            return '3';
         case DIK_NUMPAD0:
            return '0';
         case DIK_DECIMAL:
            return '.';
         case DIK_NUMPADEQUALS:
            return '=';
         case DIK_CIRCUMFLEX:
            return '~';
         case DIK_COLON:
            return ':';
         case DIK_UNDERLINE:
            return '_';
         case DIK_NUMPADENTER:
            return 13;
         case DIK_NUMPADCOMMA:
            return ',';
         case DIK_DIVIDE:
            return '/';
         case DIK_HOME:
            return 0;
         case DIK_UP:
            return 0;
         case DIK_LEFT:
            return 0;
         case DIK_RIGHT:
            return 0;
         case DIK_END:
            return 0;
         case DIK_DOWN:
            return 0;
         }
      }
   else
      {
      switch (DIK_CODE & 0x000000ff)
         {
         case 0:
            return 0;
         case DIK_ESCAPE:
            return 27;
         case DIK_1:
            return '1';
         case DIK_2:
            return '2';
         case DIK_3:
            return '3';
         case DIK_4:
            return '4';
         case DIK_5:
            return '5';
         case DIK_6:
            return '6';
         case DIK_7:
            return '7';
         case DIK_8:
            return '8';
         case DIK_9:
            return '9';
         case DIK_0:
            return '0';
         case DIK_MINUS:
            return '-';
         case DIK_EQUALS:
            return '=';
         case DIK_BACK:
            return 8;
         case DIK_TAB:
            return 9;
         case DIK_Q:
            return 'q';
         case DIK_W:
            return 'w';
         case DIK_E:
            return 'e';
         case DIK_R:
            return 'r';
         case DIK_T:
            return 't';
         case DIK_Y:
            return 'y';
         case DIK_U:
            return 'u';
         case DIK_I:
            return 'i';
         case DIK_O:
            return 'o';
         case DIK_P:
            return 'p';
         case DIK_LBRACKET:
            return '[';
         case DIK_RBRACKET:
            return ']';
         case DIK_RETURN:
            return 13;
         case DIK_A:
            return 'a';
         case DIK_S:
            return 's';
         case DIK_D:
            return 'd';
         case DIK_F:
            return 'f';
         case DIK_G:
            return 'g';
         case DIK_H:
            return 'h';
         case DIK_J:
            return 'j';
         case DIK_K:
            return 'k';
         case DIK_L:
            return 'l';
         case DIK_SEMICOLON:
            return ';';
         case DIK_APOSTROPHE:
            return 39;
         case DIK_GRAVE:
            return '`';
         case DIK_BACKSLASH:
            return '\\';
         case DIK_Z:
            return 'z';
         case DIK_X:
            return 'x';
         case DIK_C:
            return 'c';
         case DIK_V:
            return 'v';
         case DIK_B:
            return 'b';
         case DIK_N:
            return 'n';
         case DIK_M:
            return 'm';
         case DIK_COMMA:
            return ',';
         case DIK_PERIOD:
            return '.';
         case DIK_SLASH:
            return '/';
         case DIK_MULTIPLY:
            return '*';
         case DIK_SPACE:
            return ' ';
         case DIK_NUMPAD7:
            return '7';
         case DIK_NUMPAD8:
            return '8';
         case DIK_NUMPAD9:
            return '9';
         case DIK_SUBTRACT:
            return '-';
         case DIK_NUMPAD4:
            return '4';
         case DIK_NUMPAD5:
            return '5';
         case DIK_NUMPAD6:
            return '6';
         case DIK_ADD:
            return '+';
         case DIK_NUMPAD1:
            return '1';
         case DIK_NUMPAD2:
            return '2';
         case DIK_NUMPAD3:
            return '3';
         case DIK_NUMPAD0:
            return '0';
         case DIK_DECIMAL:
            return '.';
         case DIK_NUMPADEQUALS:
            return '=';
         case DIK_CIRCUMFLEX:
            return '~';
         case DIK_COLON:
            return ':';
         case DIK_UNDERLINE:
            return '_';
         case DIK_NUMPADENTER:
            return 13;
         case DIK_NUMPADCOMMA:
            return ',';
         case DIK_DIVIDE:
            return '/';
         case DIK_HOME:
            return 0;
         case DIK_UP:
            return 0;
         case DIK_LEFT:
            return 0;
         case DIK_RIGHT:
            return 0;
         case DIK_END:
            return 0;
         case DIK_DOWN:
            return 0;
         }
      }

   return 0;
   }
