
#ifndef  DIRECTDRAW_VERSION
#define   DIRECTDRAW_VERSION 0x0700
#endif

#include <ddraw.h>
//#include <d3dtypes.h>
#include "DirectDraw.h"
#include "debug.h"



DirectDraw::DirectDraw()
   {
   nextSurface    = 1;
   bpp            = 8;
   width          = 640;
   height         = 480;
   windowed       = FALSE;
   directDraw     = NULL;
   frontBuffer    = NULL;
   backBuffer     = NULL;
   artPalette     = NULL;
   }


DirectDraw::~DirectDraw()
   {
   if (directDraw)
      Close();
   }


void DirectDraw::Close(void)
   {
   SurfaceStruct *   nextSurface;
   SurfaceStructItr  surfaceItr;

   if (bpp == 8)
      {
      // free the palette
      if (artPalette)
         {
         artPalette->Release();
         artPalette=NULL;
         }
      }


#ifdef _DEBUG
   if (windowed)
      {
      // if we are windowed then the back surface is a normal surface that is not 
      // attached to the primary so it needs to be freed
      if (backBuffer)
         {
         backBuffer->Release();
         backBuffer = NULL;
         }
      }
#endif

   surfaceItr = surfaceMap.begin();
   while (surfaceItr != surfaceMap.end())
      {
      nextSurface = &(surfaceItr->second);

      if (nextSurface->videoSurface)
         {
         nextSurface->videoSurface->Release();
         nextSurface->videoSurface = NULL;
         }

//      if (nextSurface->memSurface)
//         {
//         nextSurface->memSurface->Release();
//         nextSurface->memSurface = NULL;
//         }

      surfaceMap.erase(surfaceItr);
      surfaceItr = surfaceMap.begin();
      }

   if (frontBuffer)
      {
      frontBuffer->Release();
      frontBuffer = NULL;
      }

   if (directDraw)
      {
      directDraw->Release();
      directDraw = NULL;
      }
   }




bool DirectDraw::Init(HWND hWndMain, int widthIn, int heightIn, int bppIn, bool windowedIn)
   {
//   int            count1;
   HRESULT        ddrval;
   DDSURFACEDESC  ddsd;
   DDSCAPS        ddscaps;
   HDC            hdc;

   ASSERTMSG(directDraw == NULL, "DirectDraw is already initialised");

   if ((ddrval = DirectDrawCreate(NULL, &directDraw, NULL))!=DD_OK)
      return(FALSE);

   width    = widthIn;
   height   = heightIn;
   bpp      = bppIn;
   windowed = windowedIn;

   if (windowed)
      {
      ddrval=directDraw->SetCooperativeLevel(hWndMain, DDSCL_NORMAL);
      if (ddrval != DD_OK )
         {
         dprintf("SetCooperativeLevel failed [%s]...\n",DirectDrawErrorToString(ddrval));
         return(FALSE);
         }

      SetWindowPos(hWndMain, NULL, 0, 0, width, height, SWP_NOMOVE);

      ddsd.dwFlags = DDSD_CAPS;
      ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
      ddsd.dwSize = sizeof(ddsd);

      ddrval = directDraw->CreateSurface(&ddsd, &frontBuffer, NULL);
      if (ddrval != DD_OK)
         return(FALSE);

      memset(&ddsd, 0, sizeof(ddsd));
      ddsd.dwSize = sizeof(ddsd);
      ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
      ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE | DDSCAPS_SYSTEMMEMORY;
//      ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE | DDSCAPS_VIDEOMEMORY;
      ddsd.dwWidth = width;
      ddsd.dwHeight = height;

      ddrval = directDraw->CreateSurface(&ddsd, &backBuffer, NULL);
      if (ddrval != DD_OK)
         {
         dprintf("CreateSurface failed [%s]...\n",DirectDrawErrorToString(ddrval));
         frontBuffer->Release();
         return(FALSE);
         }

      ClearBackBuffer();

      // get the actual bit depth of the GDI screen
      hdc = GetDC(NULL);
      bpp = GetDeviceCaps(hdc, PLANES) * GetDeviceCaps(hdc, bpp);
      ReleaseDC(NULL, hdc);
      }
   else
      {
      ddrval = directDraw->SetCooperativeLevel(hWndMain, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX);
      if(ddrval != DD_OK )
         {
         dprintf("SetCooperativeLevel failed [%s]...\n",DirectDrawErrorToString(ddrval));
         return(FALSE);
         }

      ddrval = directDraw->SetDisplayMode(width, height, bpp);
      if( ddrval != DD_OK )
         {
         directDraw->SetCooperativeLevel(hWndMain, DDSCL_NORMAL);
         dprintf("SetDisplayMode failed [%s]...\n",DirectDrawErrorToString(ddrval));
         return(FALSE);
         }

      // Create the primary surface with the required back buffers
      ddsd.dwSize = sizeof(ddsd);
      ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
//      ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE | DDSCAPS_MODEX | DDSCAPS_SYSTEMMEMORY;
      ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE | DDSCAPS_MODEX | DDSCAPS_VIDEOMEMORY;
      ddsd.dwBackBufferCount = 1;
      ddrval=directDraw->CreateSurface(&ddsd, &frontBuffer, NULL);
      if (ddrval != DD_OK)
         {
         directDraw->SetCooperativeLevel(hWndMain, DDSCL_NORMAL);
         directDraw->RestoreDisplayMode();
         dprintf("CreateSurface failed [%s]...\n", DirectDrawErrorToString(ddrval));
         return(FALSE);
         }

      // Get a pointer to the back buffer interface
      ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
      ddrval = frontBuffer->GetAttachedSurface(&ddscaps, &backBuffer);
      if (ddrval != DD_OK)
         {
         directDraw->SetCooperativeLevel(hWndMain, DDSCL_NORMAL);
         directDraw->RestoreDisplayMode();
         dprintf("GetAttachedSurface failed [%s]...\n", DirectDrawErrorToString(ddrval));
         return(FALSE);
         }

      // ensure that both of the buffers are clear
      ClearBackBuffer();
      SwapBuffers();
      ClearBackBuffer();
      }

//   if (!RestoreSurfaces())
//      {
//      dprintf("RestoreSurfaces Failed!\n");
//      return FALSE;
//      }

   return TRUE;
   }



bool DirectDraw::SetPaletteFromResource(char *resourceName)
   {
   // palettes are required only for bpp of 8
   if (bpp != 8)
      return FALSE;

   if (artPalette)
      {
      artPalette->Release();
      artPalette = NULL;
      }

   // Create and set the palette for the art bitmap
   artPalette = LoadPalette(resourceName);
   if (!artPalette)
      {
      dprintf("LoadPalette Failed!\n");
      return TRUE;
      }

   frontBuffer->SetPalette(artPalette);
//   backBuffer->SetPalette(artPalette);
   return FALSE;
   }



bool DirectDraw::SetPaletteFromFile(char *fileName)
   {
   // palettes are required only for bpp of 8
   if (bpp != 8)
      return FALSE;

   if (artPalette)
      {
      artPalette->Release();
      artPalette = NULL;
      }

   ReadBMPPalette(fileName, artPalette);
   frontBuffer->SetPalette(artPalette);
   backBuffer->SetPalette(artPalette);

   return TRUE;
   }




UINT32 DirectDraw::NewSurfaceFromResource(char *resourceName)
   {
   int               surfaceWidth;
   int               surfaceHeight;
   int               surfaceBPP;
   SurfaceStruct     newSurface;
   SurfaceStruct    *currentSurface;
   SurfaceStructItr  surfaceItr;

   surfaceItr = surfaceMap.begin();
   while (surfaceItr != surfaceMap.end())
      {
      currentSurface = &(surfaceItr->second);
      if (!strcmpi(resourceName, currentSurface->bitmapName))
         return surfaceItr->first;

      ++surfaceItr;
      }

   newSurface.videoSurface = LoadBitmapFromResource(resourceName, &surfaceWidth, &surfaceHeight, &surfaceBPP);
   if (newSurface.videoSurface == NULL)
      return 0;

   newSurface.width  = surfaceWidth;
   newSurface.height = surfaceHeight;
   newSurface.surfaceSource = SURFACE_RESOURCE;
   strcpy(newSurface.bitmapName, resourceName);

   nextSurface += 1;
   surfaceMap.insert(make_pair(nextSurface, newSurface));

   return nextSurface;
   }



UINT32 DirectDraw::NewSurfaceFromFile(char *bitmapName)
   {
   int            surfaceWidth;
   int            surfaceHeight;
   int            surfaceBPP;
   SurfaceStruct  newSurface;

   newSurface.videoSurface = LoadBitmapFromFile(bitmapName, &surfaceWidth, &surfaceHeight, &surfaceBPP);
   if (newSurface.videoSurface == NULL)
      return 0;

   newSurface.width  = surfaceWidth;
   newSurface.height = surfaceHeight;
   newSurface.surfaceSource = SURFACE_BITMAP;
   strcpy(newSurface.bitmapName, bitmapName);

   nextSurface += 1;
   surfaceMap.insert(make_pair(nextSurface, newSurface));

   return nextSurface;
   }



void DirectDraw::ClearBackBuffer(void)
   {
   DDBLTFX     ddbltfx;
   HRESULT     res;


   ddbltfx.dwSize = sizeof(ddbltfx);
   ddbltfx.dwFillColor = 0;

   while(1)
      {
      if ((res = backBuffer->Blt(NULL,NULL,NULL,DDBLT_COLORFILL,&ddbltfx)) == DD_OK)
         break;

      if (res == DDERR_SURFACELOST)
         {
         RestorePrimarySurfaces();
         break;
         }

      if (res != DDERR_WASSTILLDRAWING)
         {
         DEBUGSTR("EraseRect: \n");
         break;
         }
      }
   }



void DirectDraw::RestorePrimarySurfaces(void)
   {
   HRESULT  res;

   // this will restores any surfaces that are in the flipping chain
   while (1)
      {
      res = frontBuffer->Restore();
      if (res==DD_OK)
         break;               // fall through and free the next

      if (res == DDERR_WASSTILLDRAWING)
         continue;

      break;
      }


#ifdef _DEBUG
   // this loop will fail if the back surface is attached to the primary surface,
   // such is the case when in full screen and we are page flipping.
   // this is required becasue in windows the back surface is just a normal surface that can be lost
   while (1)
      {
      res = backBuffer->Restore();
      if (res == DD_OK)
         return;

      if (res == DDERR_WASSTILLDRAWING)
         continue;

      break;
      }
#endif
   }



void DirectDraw::SwapBuffers(void)
   {
   HRESULT  ddrval;

   if (windowed)
      {
      ddrval=frontBuffer->BltFast(0, 0, backBuffer, NULL, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
//      ddrval=frontBuffer->BltFast(0, 0, backBuffer, NULL, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
      if (ddrval == DDERR_SURFACELOST)
         RestorePrimarySurfaces();
      }
   else
      {
      while(1)
         {
         ddrval=frontBuffer->Flip(NULL, 0);
         if (ddrval==DD_OK )
            {
            break;
            }

         if (ddrval==DDERR_SURFACELOST )
            {
            RestorePrimarySurfaces();
            break;
            }

         if (ddrval!=DDERR_WASSTILLDRAWING)
            {
            DEBUGSTR("FlipSurfaces: \n");
            break;
            }
         }
      }
   }


bool DirectDraw::RestoreSurfaces(void)
   {
   HRESULT  ddrval;
   HBITMAP  hbm;
   SurfaceStruct *   nextSurface;
   SurfaceStructItr  surfaceItr;

   if (frontBuffer)
      {
      ddrval = frontBuffer->Restore();
      if (ddrval != DD_OK)
         {
         dprintf("frontBuffer->Restore Failed!: ddrval = [%s]...\n", DirectDrawErrorToString(ddrval));
         return FALSE;
         }
      }

   surfaceItr = surfaceMap.begin();
   while (surfaceItr != surfaceMap.end())
      {
      nextSurface = &(surfaceItr->second);

      if (nextSurface->videoSurface)
         {
         ddrval = nextSurface->videoSurface->Restore();
         if (ddrval != DD_OK)
            {
            dprintf("surfaceArray.videoSurface->Restore Failed!: ddrval = [%s]...\n", DirectDrawErrorToString(ddrval));
            return FALSE;
            }

         hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), nextSurface->bitmapName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
         if (!hbm)
            {
            dprintf("LoadImage videoSurface Failed!: ddrval = [%s]...\n", DirectDrawErrorToString(ddrval));
            return FALSE;
            }

         ddrval = CopyBitmap(nextSurface->videoSurface, hbm, 0, 0, nextSurface->width, nextSurface->height);
         if (ddrval != DD_OK)
            {
            DeleteObject(hbm);
            dprintf("CopyBitmap videoSurface Surface Failed!: ddrval = [%s]...\n", DirectDrawErrorToString(ddrval));
            return FALSE;
            }

         DeleteObject(hbm);
         }
      }

   return TRUE;
   }



bool DirectDraw::GetBitmapDetails(char *fileName, int *sizeX, int *sizeY, int *bpp)
   {
   HANDLE               hFile;
   DWORD                actualRead;
   BITMAPFILEHEADER     BMPFileHead;
   BITMAPINFOHEADER     BMPFileInfo;

   hFile = OpenBMP(fileName);
   if (hFile == INVALID_HANDLE_VALUE)
      {
      dprintf("hFile == INVALID_HANDLE_VALUE\n");
      return TRUE;
      }

   if (!ReadFile(hFile, &BMPFileHead, sizeof(BMPFileHead), &actualRead, NULL))
      {
      dprintf("ReadFile Failed on BMPFileHead!\n");
      CloseHandle(hFile);
      return TRUE;
      }

   if (!ReadFile(hFile, &BMPFileInfo, sizeof(BMPFileInfo), &actualRead, NULL))
      {
      dprintf("ReadFile Failed on BMPFileInfo!\n");
      CloseHandle(hFile);
      return TRUE;
      }

   *sizeX = BMPFileInfo.biWidth;
   *sizeY = BMPFileInfo.biHeight;
   *bpp   = BMPFileInfo.biBitCount;
   CloseHandle(hFile);

   return FALSE;
   }


bool DirectDraw::ReadBMPPalette(char *fileName, IDirectDrawPalette * ddpal)
   {
   int                  i;
//   LPBYTE               Image;
   HANDLE               hFile;
   BITMAPFILEHEADER     BMPFileHead;
   BITMAPINFOHEADER     BMPFileInfo;
   RGBQUAD              Palette[256];
   DWORD                actualRead;
   PALETTEENTRY         pe[256];

   // palettes are required only for bpp of 8
   if (bpp != 8)
      return FALSE;

//   if (!ddpal)
//      return TRUE;
   
/*
   Image = (LPBYTE)LocalAlloc(LPTR, 640 * 480);
   if (Image == NULL)
      {
      dprintf("Image == NULL\n");
      return TRUE;
      }
*/

   hFile = OpenBMP(fileName);
   if (hFile == INVALID_HANDLE_VALUE)
      {
      dprintf("hFile == INVALID_HANDLE_VALUE\n");
      return TRUE;
      }

   if (!ReadFile(hFile, &BMPFileHead, sizeof(BMPFileHead), &actualRead, NULL))
      {
      dprintf("ReadFile Failed on BMPFileHead!\n");
      CloseHandle(hFile);
      return TRUE;
      }

   if (!ReadFile(hFile, &BMPFileInfo, sizeof(BMPFileInfo), &actualRead, NULL))
      {
      dprintf("ReadFile Failed on BMPFileInfo!\n");
      CloseHandle(hFile);
      return TRUE;
      }
   
/*
   if ((BMPFileInfo.biWidth != 640) || (BMPFileInfo.biHeight != 480) || (BMPFileInfo.biBitCount != 8))
      {
      dprintf("Dimensions don't match!\n");
      CloseHandle(hFile);
      return TRUE;
      }
*/

   if (!ReadFile(hFile, Palette, sizeof(Palette), &actualRead, NULL))
      {
      dprintf("ReadFile Failed on Palette!\n");
      CloseHandle(hFile);
      return TRUE;
      }
/*
   if (!ReadFile(hFile, Image, 640 * 480, &actualRead, NULL))
      {
      dprintf("ReadFile Failed on Image!\n");
      CloseHandle(hFile);
      return TRUE;
      }
*/

   CloseHandle(hFile);

   for (i = 0; i < 256; i++)
      {
      pe[i].peRed    = Palette[i].rgbRed;
      pe[i].peGreen  = Palette[i].rgbGreen;
      pe[i].peBlue   = Palette[i].rgbBlue;
//      pe[i].peFlags  = D3DPAL_READONLY;
      }

   directDraw->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE, pe, &ddpal, NULL);

/*
   if (ddpal->SetEntries(0, 0, 256, pe) != DD_OK)
      {
      dprintf("SetEntries Failed!\n");
      return TRUE;
      }
*/

//   LocalFree(Image);
   return FALSE;
   }


IDirectDrawPalette * DirectDraw::LoadPalette(LPCSTR resourceName)
   {
   IDirectDrawPalette* ddpal;
   int                 i;
   int                 n;
   int                 fh;
   HRSRC               h;
   LPBITMAPINFOHEADER  lpbi;
   PALETTEENTRY        ape[256];
   RGBQUAD *           prgb;

   // palettes are required only for bpp of 8
   if (bpp != 8)
      return FALSE;

//   dprintf("Setting palette to file %s\n", resourceName);

   // build a 332 palette as the default.
   for (i = 0; i < 256; i++)
      {
      ape[i].peRed   = (BYTE)(((i >> 5) & 0x07) * 255 / 7);
      ape[i].peGreen = (BYTE)(((i >> 2) & 0x07) * 255 / 7);
      ape[i].peBlue  = (BYTE)(((i >> 0) & 0x03) * 255 / 3);
      ape[i].peFlags = (BYTE)0;
      }

   // get a pointer to the bitmap resource.
   if (resourceName && (h = FindResource(NULL, resourceName, RT_BITMAP)))
      {
      lpbi = (LPBITMAPINFOHEADER)LockResource(LoadResource(NULL, h));
      if (!lpbi)
         dprintf("lock resource failed\n");

      prgb = (RGBQUAD*)((BYTE*)lpbi + lpbi->biSize);

      if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
         n = 0;
      else if (lpbi->biBitCount > 8)
         n = 0;
      else if (lpbi->biClrUsed == 0)
         n = 1 << lpbi->biBitCount;
      else
         n = lpbi->biClrUsed;

      //  a DIB color table has its colors stored BGR not RGB
      //  so flip them around.
//      for (i = 1; i < 255; i++)
      for (i = 0; i < n; i++)
         {
         ape[i].peRed   = prgb[i].rgbRed;
         ape[i].peGreen = prgb[i].rgbGreen;
         ape[i].peBlue  = prgb[i].rgbBlue;
         ape[i].peFlags = (BYTE)0;
         }
      }
   else if (resourceName && (fh = _lopen(resourceName, OF_READ)) != -1)
      {
      BITMAPFILEHEADER bf;
      BITMAPINFOHEADER bi;

      _lread(fh, &bf, sizeof(bf));
      _lread(fh, &bi, sizeof(bi));
      _lread(fh, ape, sizeof(ape));
      _lclose(fh);

      if (bi.biSize != sizeof(BITMAPINFOHEADER))
         n = 0;
      else if (bi.biBitCount > 8)
         n = 0;
      else if (bi.biClrUsed == 0)
         n = 1 << bi.biBitCount;
      else
         n = bi.biClrUsed;

      //  a DIB color table has its colors stored BGR not RGB
      //  so flip them around.
      for (i = 0; i < n; i++)
         {
         BYTE r = ape[i].peRed;
         ape[i].peRed  = ape[i].peBlue;
         ape[i].peBlue = r;
         }
      }

   directDraw->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE, ape, &ddpal, NULL);

//   dprintf("ape[254].peBlue = %d\n", ape[254].peBlue);
//   for (i = 0; i < 256; i++)
//      dprintf("ape[%d].peBlue = %d\n", i, ape[i].peBlue);

   return ddpal;
   }



IDirectDrawSurface * DirectDraw::LoadBitmapFromResource(char *resourceName, int *width, int *height, int *bpp)
   {
   HBITMAP             hbm;
   BITMAP              bm;
   DDSURFACEDESC       ddsd;
   IDirectDrawSurface *videoSurface;

   hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), resourceName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if (hbm == NULL)
      return NULL;

   // get size of the bitmap
   GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

   // create a DirectDrawSurface for this bitmap
   ZeroMemory(&ddsd, sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);
   ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
   ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
//   ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
   ddsd.dwWidth = bm.bmWidth;
   ddsd.dwHeight = bm.bmHeight;
   *width = bm.bmWidth;
   *height = bm.bmHeight;
   *bpp = bm.bmBitsPixel;

   if (directDraw->CreateSurface(&ddsd, &videoSurface, NULL) != DD_OK)
      return NULL;

   DDCOLORKEY  ddck;
   ddck.dwColorSpaceLowValue  = (DWORD)1;
   ddck.dwColorSpaceHighValue = (DWORD)7;
   if (videoSurface->SetColorKey(DDCKEY_SRCBLT, &ddck) != DD_OK)
      {
//      dprintf("SetColorKey Failed on surface!: ddrval = [%s]...\n", DirectDrawErrorToString(ddrval));
      return NULL;
      }

   CopyBitmap(videoSurface, hbm, 0, 0, 0, 0);
   DeleteObject(hbm);
   return videoSurface;
   }



IDirectDrawSurface * DirectDraw::LoadBitmapFromFile(LPCSTR szBitmap, int *width, int *height, int *bpp)
   {
   HBITMAP             hbm;
   BITMAP              bm;
   DDSURFACEDESC       ddsd;
   IDirectDrawSurface *videoSurface;

   //  try to load the bitmap as a resource, if that fails, try it as a file
   hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
//   hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_DEFAULTCOLOR);

   if (hbm == NULL)
//      hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE | LR_DEFAULTCOLOR);
      hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

   if (hbm == NULL)
      return NULL;

   // get size of the bitmap
   GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap

   // create a DirectDrawSurface for this bitmap
   ZeroMemory(&ddsd, sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);
   ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
   ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
//   ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
   ddsd.dwWidth = bm.bmWidth;
   ddsd.dwHeight = bm.bmHeight;
   *width = bm.bmWidth;
   *height = bm.bmHeight;
   *bpp = bm.bmBitsPixel;

   if (directDraw->CreateSurface(&ddsd, &videoSurface, NULL) != DD_OK)
      return NULL;

   DDCOLORKEY  ddck;
   ddck.dwColorSpaceLowValue  = (DWORD)1;
   ddck.dwColorSpaceHighValue = (DWORD)7;
   if (videoSurface->SetColorKey(DDCKEY_SRCBLT, &ddck) != DD_OK)
      {
//      dprintf("SetColorKey Failed on surface!: ddrval = [%s]...\n", DirectDrawErrorToString(ddrval));
      return NULL;
      }

   CopyBitmap(videoSurface, hbm, 0, 0, 0, 0);
   DeleteObject(hbm);
   return videoSurface;
   }


HRESULT DirectDraw::ReLoadBitmap(IDirectDrawSurface *pdds, LPCSTR szBitmap)
   {
   HBITMAP             hbm;
   HRESULT             hr;

   //  try to load the bitmap as a resource, if that fails, try it as a file
   hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

   if (hbm == NULL)
      hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

   if (hbm == NULL)
      {
      OutputDebugString("handle is null\n");
      return E_FAIL;
      }

   hr = CopyBitmap(pdds, hbm, 0, 0, 0, 0);
   if (hr != DD_OK)
      {
      dprintf("CopyBitmap failed\n");
      }


   DeleteObject(hbm);
   return hr;
   }


HRESULT DirectDraw::CopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, 
                                        int y, int dx, int dy)
   {
   HDC                 hdcImage;
   HDC                 hdc;
   BITMAP              bm;
   DDSURFACEDESC       ddsd;
   HRESULT             hr;

   if (hbm == NULL || pdds == NULL)
      return E_FAIL;

   // make sure this surface is restored.
   pdds->Restore();

   //  select bitmap into a memoryDC so we can use it.
   hdcImage = CreateCompatibleDC(NULL);
   if (!hdcImage)
      OutputDebugString("createcompatible dc failed\n");
   SelectObject(hdcImage, hbm);

   // get size of the bitmap
   GetObject(hbm, sizeof(bm), &bm);    // get size of bitmap
   dx = dx == 0 ? bm.bmWidth  : dx;    // use the passed size, unless zero
   dy = dy == 0 ? bm.bmHeight : dy;

   // get size of surface.
   ddsd.dwSize = sizeof(ddsd);
   ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
   pdds->GetSurfaceDesc(&ddsd);

   if ((hr = pdds->GetDC(&hdc)) == DD_OK)
      {
      BitBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, SRCCOPY);
//      StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
      pdds->ReleaseDC(hdc);
      }

   DeleteDC(hdcImage);

   return hr;
   }


DWORD DirectDraw::ColorMatch(IDirectDrawSurface *pdds, COLORREF rgb)
   {
   COLORREF rgbT;
   HDC hdc;
   DWORD dw = CLR_INVALID;
   DDSURFACEDESC ddsd;
   HRESULT hres;

   //  use GDI SetPixel to color match for us
   if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
      {
      rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
      SetPixel(hdc, 0, 0, rgb);               // set our value
      pdds->ReleaseDC(hdc);
      }

   // now lock the surface so we can read back the converted color
   ddsd.dwSize = sizeof(ddsd);
   while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
      ;

   if (hres == DD_OK)
      {
      dw  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
      if(ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
         dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
      pdds->Unlock(NULL);
      }

   //  now put the color that was there back.
   if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
      {
      SetPixel(hdc, 0, 0, rgbT);
      pdds->ReleaseDC(hdc);
      }

   return dw;
   }


HRESULT DirectDraw::SetColorKey(IDirectDrawSurface *pdds, COLORREF rgb)
   {
   DDCOLORKEY          ddck;

   ddck.dwColorSpaceLowValue  = ColorMatch(pdds, rgb);
   ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
   return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
   }



HANDLE DirectDraw::OpenBMP(char *fileName)
   {
   HANDLE   hFile;

   hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
   if (hFile != INVALID_HANDLE_VALUE)
      return hFile;

   return INVALID_HANDLE_VALUE;
   }



void DirectDraw::BltSurface(UINT32 surfaceID)
   {
   RECT              rcRect;
   HRESULT           ddrval;
   SurfaceStruct *   theSurface;
   SurfaceStructItr  surfaceItr;

   surfaceItr = surfaceMap.find(surfaceID);
   if (surfaceItr == surfaceMap.end())
      return;

   theSurface = &(surfaceItr->second);
   rcRect.left = 0;
   rcRect.top = 0;
   rcRect.right = theSurface->width;
   rcRect.bottom = theSurface->height;

   while (1)
      {
      ddrval = backBuffer->BltFast(0, 0, theSurface->videoSurface, &rcRect, DDBLTFAST_NOCOLORKEY);
//      ddrval = backBuffer->BltFast(0, 0, theSurface->videoSurface, &rcRect, DDBLTFAST_SRCCOLORKEY);

      if (ddrval == DD_OK)
         break;

      if (ddrval == DDERR_SURFACELOST)
         {
         if (!RestoreSurfaces())
            {
            dprintf("in DirectDraw::BltSurface  DDERR_SURFACELOST!: ddrval = [%s]...\n", DirectDrawErrorToString(ddrval));
            return;
            }
         }

      if (ddrval != DDERR_WASSTILLDRAWING)
         {
         dprintf("in DirectDraw::BltSurface   ddrval = [%s]...\n",DirectDrawErrorToString(ddrval));
         return;
         }
      }
   }



bool DirectDraw::Draw(ULONG destLeft, ULONG destTop, UINT32 surfaceID, RECT *sourceRECT)
   {
   LONG  ddrval;
   SurfaceStruct *   theSurface;
   SurfaceStructItr  surfaceItr;

   surfaceItr = surfaceMap.find(surfaceID);
   if (surfaceItr == surfaceMap.end())
      return TRUE;

   theSurface = &(surfaceItr->second);

   while (TRUE)
      {
      ddrval = backBuffer->BltFast(destLeft, destTop, theSurface->videoSurface, sourceRECT, DDBLTFAST_SRCCOLORKEY);

      if (ddrval == DD_OK)
         return FALSE;

      if (ddrval == DDERR_SURFACELOST)
         {
         if (!RestoreSurfaces())
            {
            dprintf("in DirectDraw::Draw  DDERR_SURFACELOST!: ddrval = [%s]...\n", DirectDrawErrorToString(ddrval));
            return FALSE;
            }
         }

      if (ddrval != DDERR_WASSTILLDRAWING)
         {
         dprintf("in DirectDraw::Draw   ddrval = [%s]...\n",DirectDrawErrorToString(ddrval));
         return FALSE;
         }
      }
   }


/*
HRESULT DirectDraw::GetDC(HDC *bufferDC)
   {
   return backBuffer->GetDC(bufferDC);
   }



HRESULT DirectDraw::ReleaseDC(HDC *bufferDC)
   {
   return backBuffer->ReleaseDC(bufferDC);
   }
*/