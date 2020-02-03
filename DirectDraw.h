
#ifndef __DirectDraw_h__
#define __DirectDraw_h__

#include <windows.h>
#include <map>


#pragma warning(disable : 4786) //* disable the C4786 truncating warning.

using namespace std;


// forward declarations so we don't have to include ddraw.h everywhere.
struct IDirectDraw;
struct IDirectDrawPalette;
struct IDirectDrawSurface;


enum SurfaceSource
   {
   SURVACE_INVALID = 0,
   SURFACE_BITMAP,
   SURFACE_RESOURCE,
   SURFACE_LAST
   };


typedef struct SurfaceStruct 
   {
   int                  width;
   int                  height;
   SurfaceSource        surfaceSource;
   char                 bitmapName[21];
   IDirectDrawSurface * videoSurface;
//   IDirectDrawSurface * memSurface;
   } SurfaceStruct;


typedef map<UINT32, SurfaceStruct>  SurfaceStructMap;
typedef SurfaceStructMap::iterator  SurfaceStructItr;


class DirectDraw
   {
public:

                           DirectDraw();
                          ~DirectDraw();
   bool                    Init(HWND hWndMain, int width, int height, int bpp, bool windowed);
   void                    Close(void);

   void                    ClearBackBuffer(void);
   void                    SwapBuffers(void);
   bool                    GetBitmapDetails(char *fileName, int *sizeX, int *sizeY, int *bpp);
   bool                    ReadBMPPalette(char *fileName, IDirectDrawPalette * ddpal);
   void                    BltSurface(UINT32 surfaceID);
   bool                    Draw(ULONG destLeft, ULONG destTop, UINT32 surfaceID, RECT *sourceRECT);
   bool                    RestoreSurfaces(void);
   bool                    SetPaletteFromResource(char *resourceName);
   bool                    SetPaletteFromFile(char *fileName);
   // returns 0 on failure, otherwise returns new surface's identifier
   UINT32                  NewSurfaceFromFile(char *bitmapName);
   UINT32                  NewSurfaceFromResource(char *resourceName);
   inline IDirectDrawSurface * GetBackBuffer(void) { return backBuffer; };


private:
   SurfaceStructMap        surfaceMap;
   UINT32                  nextSurface;
   int                     width;
   int                     height;
   int                     bpp;
   bool                    windowed;
   IDirectDraw *           directDraw;
   IDirectDrawSurface *    frontBuffer;
   IDirectDrawSurface *    backBuffer;
   IDirectDrawPalette *    artPalette;

   HANDLE                  OpenBMP(char *fileName);
   IDirectDrawPalette *    LoadPalette(LPCSTR szBitmap);
   IDirectDrawSurface *    LoadBitmapFromFile(LPCSTR szBitmap, int *width, int *height, int *bpp);
   IDirectDrawSurface *    LoadBitmapFromResource(char *resourceName, int *width, int *height, int *bpp);
//   IDirectDraw *           GetDDObject(void);
   HRESULT                 ReLoadBitmap(IDirectDrawSurface *pdds, LPCSTR szBitmap);
   HRESULT                 CopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, 
                                        int y, int dx, int dy);
   DWORD                   ColorMatch(IDirectDrawSurface *pdds, COLORREF rgb);
   HRESULT                 SetColorKey(IDirectDrawSurface *pdds, COLORREF rgb);
   void                    RestorePrimarySurfaces(void);
   };

#endif
