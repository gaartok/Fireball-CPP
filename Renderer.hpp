
#ifndef __Renderer_hpp__
#define __Renderer_hpp__


#include <windows.h>
//#define DIRECTINPUT_VERSION 0x0300      /* Remain DX3-compatible */
//#include <dinput.h>
//#include <mmsystem.h>
#include <ddraw.h>


class DirectDraw;


class Renderer  
   {
private:
   int                     screenWidth;
   int                     screenHeight;
   int                     screenBPP;
   bool                    windowed;
   int                     renderStarted;
   int                     surfaceLocked;
   UINT32                  backgroundSurfaceID;
   HDC                     bufferDC;
   POINT                   oldPoint;
   HPEN                    thePen;
	HPEN                    oldPen;
   LPDIRECTDRAWPALETTE     splashPalette;
   DDSURFACEDESC           ddsd;
   DirectDraw *            directDraw;

public:
	         Renderer();
	        ~Renderer();
   int      Initialize(HWND hWndMain, int width, int height, int bpp, bool windowed);
   void     EraseBackground(void);
   void     EraseAllScreens(void);
   void     BltBackground(void);
   void     LoadBackgroundFromFile(char *fileName);
   void     LoadBackgroundFromResource(char *resourceName);
   BOOL     ShowSplash(void);
   void     BltText(char *strOut, UINT32 textSurfaceID, int x, int y);
   int      StartRender(void);
   int      EndRender(void);
   void     FlipBuffers(void);
   bool     Draw(POINT dest, UINT32 surfaceID, RECT *sourceRECT);
   bool     Draw(ULONG destLeft, ULONG destTop, UINT32 surfaceID, RECT *sourceRECT);
   void     DrawBox(RECT *theBox);
   void     DrawArray(char *theArray, int xSize, int ySize, int xOffset, int yOffset);
   void     LockSurface(void);
   void     UnlockSurface(void);
   bool     SetPaletteFromResource(char *resourceName);
   bool     SetPaletteFromFile(char *fileName);

   // returns 0 on failure, otherwise returns new surface's identifier
//   UINT32   NewSurface(int surfaceWidth, int surfaceHeight, char *bitmapName);
   UINT32   NewSurfaceFromFile(char *bitmapName);
   UINT32   NewSurfaceFromResource(char *resourceName);
   };



#endif      // #ifndef _
