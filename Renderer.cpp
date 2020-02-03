
#include "App.hpp"
#include "DirectDraw.h"
#include "Renderer.hpp"
#include "resource.h"
#include "global.h"
#include "debug.h"


/*
// This macro 
#define FIND_XY(x,y) ((y << 9) + (y << 7) + x)
// does the same as this one
// #define FIND_XY(x,y) ((640 * y) + x)
*/


Renderer::Renderer()
   {
   renderStarted = FALSE;
   surfaceLocked = FALSE;
   splashPalette = NULL;
   backgroundSurfaceID = 0;
   }



Renderer::~Renderer()
   {
   directDraw->Close();
   delete directDraw;
   directDraw = NULL;
   }


int Renderer::Initialize(HWND hWndMain, int width, int height, int bpp, bool windowedIn)
   {
   screenWidth    = width;
   screenHeight   = height;
   screenBPP      = bpp;
   windowed       = windowedIn;

   directDraw = new DirectDraw();

   if (directDraw->Init(hWndMain, width, height, bpp, windowed) == FALSE)
      {
      dprintf("DirectDraw_Init Failed!\n");
      return TRUE;
      }

//   BltBackground();         // display the background
//   DirectDraw_SwapBuffers();
//   BltBackground();           // display the background (load on both frames)

//   fillColor = DDColorMatch(frontBuffer, CLR_INVALID);   // get color of pixel at (0, 0) of buffer
   return FALSE;
   }



int Renderer::StartRender(void)
   {
   if (renderStarted)
      return FALSE;

   renderStarted = TRUE;

   directDraw->GetBackBuffer()->GetDC(&bufferDC);
   thePen = CreatePen(PS_DOT, 1, RGB(255,0,0));
   oldPen = (HPEN)SelectObject(bufferDC, thePen);

   return FALSE;
   }




int Renderer::EndRender(void)
   {
   if (!renderStarted)
      return FALSE;

   renderStarted = FALSE;

	SelectObject(bufferDC, oldPen);
	DeleteObject(thePen);
   directDraw->GetBackBuffer()->ReleaseDC(bufferDC);

   return FALSE;
   }



void Renderer::DrawBox(RECT *theBox)
   {
   StartRender();
   MoveToEx(bufferDC, theBox->left, theBox->top, &oldPoint);
   LineTo(bufferDC, theBox->right, theBox->top);
   LineTo(bufferDC, theBox->right, theBox->bottom);
   LineTo(bufferDC, theBox->left, theBox->bottom);
   LineTo(bufferDC, theBox->left, theBox->top);
   EndRender();
   }




void Renderer::LockSurface(void)
   {
   memset(&ddsd, 0, sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);		

   if (directDraw->GetBackBuffer()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
      {
      surfaceLocked = FALSE;
      return;
      }

   surfaceLocked = TRUE;
   }



void Renderer::UnlockSurface(void)
   {
   surfaceLocked = FALSE;
   directDraw->GetBackBuffer()->Unlock(&ddsd);	
   }


UINT32 Renderer::NewSurfaceFromFile(char *bitmapName)
   {
   return directDraw->NewSurfaceFromFile(bitmapName);
   }


UINT32 Renderer::NewSurfaceFromResource(char *resourceName)
   {
   directDraw->SetPaletteFromResource(resourceName);
   return directDraw->NewSurfaceFromResource(resourceName);
   }


bool Renderer::SetPaletteFromResource(char *resourceName)
   {
   return directDraw->SetPaletteFromResource(resourceName);
   }


bool Renderer::SetPaletteFromFile(char *fileName)
   {
   return directDraw->SetPaletteFromFile(fileName);
   }



void Renderer::DrawArray(char *theArray, int xSize, int ySize, int xOffset, int yOffset)
   {
   int            countX;
   int            countY;
   int            paletteIndex;
   char *         lpDDMemory;
   char *         lpDDMemoryNextLine;

   if (!surfaceLocked)
      return;

   if ((xOffset < 0) || (yOffset < 0))
      return;

   lpDDMemory = (char *)ddsd.lpSurface;
   lpDDMemory += yOffset * ddsd.lPitch;            // Move down to correct y co-ord	
//   lpDDMemoryNextLine = lpDDMemory + ddsd.lPitch;  // save the position of the next line
   
   // Move over to correct x co-ord	
   lpDDMemory += xOffset * (ddsd.ddpfPixelFormat.dwRGBBitCount >> 3);

   for (countY = 0; countY < ySize; countY++)
      {
      lpDDMemoryNextLine = lpDDMemory + ddsd.lPitch;
      for (countX = 0; countX < xSize; countX++)
         {
         if (lpDDMemory > (char *)ddsd.lpSurface + (ddsd.dwHeight * ddsd.lPitch))
            {
            directDraw->GetBackBuffer()->Unlock(&ddsd);	
            return;
            }

         if (screenBPP == 8)
            {
            // find the postion in the palette for the next color
            paletteIndex = ((int)(*(theArray + (countY * xSize) + countX)) & 0xff);

            if (paletteIndex > 1)
               *lpDDMemory = paletteIndex;   // set the screen color to the palette color
            }
         else
            {
            // set the screen color to the array color
            *lpDDMemory = *(theArray + (countY * xSize) + countX);
            }

         lpDDMemory += 1;
         }

      lpDDMemory = lpDDMemoryNextLine;
      }
   }



#if 0

void Renderer::DrawArray(char *theArray, int xSize, int ySize, int xOffset, int yOffset)
   {
   int            countX;
   int            countY;
   int            paletteIndex;
   char *         lpDDMemory;
   char *         lpDDMemoryNextLine;

   if (!surfaceLocked)
      return;

   if ((xOffset < 0) || (yOffset < 0))
      return;

   lpDDMemory = (char *)ddsd.lpSurface;
   lpDDMemory += yOffset * ddsd.lPitch;            // Move down to correct y co-ord	
//   lpDDMemoryNextLine = lpDDMemory + ddsd.lPitch;  // save the position of the next line
   lpDDMemory += xOffset;                          // Move over to correct x co-ord	

   for (countY = 0; countY < ySize; countY++)
      {
      lpDDMemoryNextLine = lpDDMemory + ddsd.lPitch;
      for (countX = 0; countX < xSize; countX++)
         {
         if (lpDDMemory > (char *)ddsd.lpSurface + (ddsd.dwHeight * ddsd.lPitch))
            {
            directDraw->GetBackBuffer()->Unlock(&ddsd);	
            return;
            }

         if (ddsd.ddpfPixelFormat.dwRGBBitCount == 8)
            {
            // find the postion in the palette for the next color
            paletteIndex = ((int)(*(theArray + (countY * xSize) + countX)) & 0xff);

            if (paletteIndex > 1)
               *lpDDMemory = paletteIndex;   // set the screen color to the palette color
            }
         else
            {
            // set the screen color to the array color
            *lpDDMemory = *(theArray + (countY * xSize) + countX);
            }

         lpDDMemory += 1;
         }

      lpDDMemory = lpDDMemoryNextLine;
      }
   }

#endif



bool Renderer::Draw(ULONG destLeft, ULONG destTop, UINT32 surfaceID, RECT *sourceRECT)
   {
   return directDraw->Draw(destLeft, destTop, surfaceID, sourceRECT);
   }



bool Renderer::Draw(POINT dest, UINT32 surfaceID, RECT *sourceRECT)
   {
   return Draw(dest.x, dest.y, surfaceID, sourceRECT);
   }




void Renderer::FlipBuffers(void)
   {
   if (renderStarted)
      EndRender();

   directDraw->SwapBuffers();
   }



void Renderer::EraseAllScreens(void)
   {
   EraseBackground();
   directDraw->SwapBuffers();
   EraseBackground();
   }



void Renderer::EraseBackground(void)
   {
   DDBLTFX     ddbltfx;
   HRESULT     ddrval;

   fillColor = 0xff;             // white
//   fillColor = 0x00;             // black

   // Erase the background
   ddbltfx.dwSize = sizeof(ddbltfx);
   ddbltfx.dwFillColor = fillColor;

   while (1)
      {
      ddrval = directDraw->GetBackBuffer()->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);
      if (ddrval == DD_OK)
         break;

      if (ddrval == DDERR_SURFACELOST)
         {
         if (!directDraw->RestoreSurfaces())
            return;
         }

      if (ddrval != DDERR_WASSTILLDRAWING)
         return;
      }
   }



void Renderer::LoadBackgroundFromResource(char *resourceName)
   {
   backgroundSurfaceID = renderer->NewSurfaceFromResource(resourceName);
   directDraw->SetPaletteFromResource(resourceName);
   dprintf("LoadBackgroundFromResource %s = %ld\n", resourceName, backgroundSurfaceID);
   }



void Renderer::LoadBackgroundFromFile(char *fileName)
   {
   backgroundSurfaceID = renderer->NewSurfaceFromFile(fileName);
   directDraw->SetPaletteFromFile(fileName);
   dprintf("LoadBackgroundFromFile %s = %ld\n", fileName, backgroundSurfaceID);
   }



void Renderer::BltBackground(void)
   {
   directDraw->BltSurface(backgroundSurfaceID);
   }





BOOL Renderer::ShowSplash(void)
   {
//   HRESULT        ddrval;
   static DWORD   splashEndTime = 0;

   // initialize the splash screen
   if (!splashEndTime)
      {
      directDraw->SetPaletteFromResource("SPLASH");
      LoadBackgroundFromResource("SPLASH");
      BltBackground();
      directDraw->SwapBuffers();
      BltBackground();
      splashEndTime = timeGetTime() + SHOW_SPLASH_COUNT;
      }

   // look for timeout or key hit to end splash screen
   if ((timeGetTime() > splashEndTime) || (ReadOneKeyboardInput() != 0))
      {
/*
      if (splashPalette)
         {
         splashPalette->Release();
         splashPalette = NULL;
         }
      ddrval = frontBuffer->SetPalette(artPalette);
      if (ddrval != DD_OK)
         {
         dprintf("frontBuffer->SetPalette(artPalette) Failed!\n");
         return 0;
         }
*/   
      EraseAllScreens();
      return 0;      // splash screen timed out
      }

   return 1;      // still not timed out

//   return 0;      // splash screen timed out
   }






void Renderer::BltText(char *strOut, UINT32 textSurfaceID, int x, int y)
   {
   int      i;
   char *   c;
   RECT     src;

   // numbers are 16 x 16
   // alphas are 16 x 20
   for (c = strOut; *c != '\0'; c++)
      {
      if (*c == '-')
         {
         src.top = 0;
         src.left = 160;
         src.bottom = src.top + 15;
         src.right = src.left + 15;
         }
      else if (*c == ' ')
         {
         src.top = 0;
         src.left = 176;
         src.bottom = src.top + 19;
         src.right = src.left + 9;
         }
      else if ((*c >= '0') && (*c <= '9'))
         {
         i = *c - '0';
         src.top = 0;
         src.left = i * 16;
         src.bottom = src.top + 15;
         src.right = src.left + 15;
         }
      else if ((*c >= 'A') && (*c <= 'Z'))
         {
         i = *c - 'A';
         src.top = 20;
         src.left = i * 16;
         src.bottom = src.top + 19;
         src.right = src.left + upperCharWidths[i];
         }
      else if ((*c >= 'a') && (*c <= 'z'))
         {
         i = *c - 'a';
         src.top = 40;
         src.left = i * 16;
         src.bottom = src.top + 19;
         src.right = src.left + lowerCharWidths[i];
         }
      else
         {
         // replace unreadable chars with space
         src.top = 0;
         src.left = 176;
         src.bottom = src.top + 19;
         src.right = src.left + 9;
         }

      if (Draw(x, y, textSurfaceID, &src))
         return;

      x += src.right - src.left + 2;
      }
   }







