/*----------------------------------------------------
   DEBUG SUPPORT FOR MEMORY ALLOCATIONS
   this file should be included in every source file 
   so that it can redirect any memory allocations
   to the debug library.
------------------------------------------------------*/

#ifndef _h_debug
#define _h_debug

// windows runtime debug header
#include "crtdbg.h"


#ifdef   _DEBUG

static char __debug__text__[16];

// replace the standard memory functions with macros to call the debug build version...

extern _CrtMemState memState;

#if 0    // use these for really verbose memory dumps

#define malloc(x)	(_malloc_dbg((x),_NORMAL_BLOCK,__FILE__,__LINE__));\
                  dprintf("malloc in file %s at line %d\n", __FILE__, __LINE__);\
                  _CrtMemDumpAllObjectsSince(&memState);\
                  _CrtMemCheckpoint(&memState)

#define realloc(x,s) (_realloc_dbg((x),(s),_NORMAL_BLOCK,__FILE__,__LINE__));\
                  dprintf("realloc in file %s at line %d\n", __FILE__, __LINE__);\
                  _CrtMemDumpAllObjectsSince(&memState);\
                  _CrtMemCheckpoint(&memState)

#define calloc(t,x) (_calloc_dbg((t),(x),_NORMAL_BLOCK,__FILE__,__LINE__));\
                  dprintf("calloc in file %s at line %d\n", __FILE__, __LINE__);\
                  _CrtMemDumpAllObjectsSince(&memState);\
                  _CrtMemCheckpoint(&memState)

#define expand(x,s) (_expand_dbg((x),(s),_NORMAL_BLOCK,__FILE__,__LINE__));\
                  dprintf("expand in file %s at line %d\n", __FILE__, __LINE__);\
                  _CrtMemDumpAllObjectsSince(&memState);\
                  _CrtMemCheckpoint(&memState)

#else

#define malloc(x)	(_malloc_dbg((x),_NORMAL_BLOCK,__FILE__,__LINE__))
#define realloc(x,s) (_realloc_dbg((x),(s),_NORMAL_BLOCK,__FILE__,__LINE__))
#define calloc(t,x) (_calloc_dbg((t),(x),_NORMAL_BLOCK,__FILE__,__LINE__))
#define expand(x,s) (_expand_dbg((x),(s),_NORMAL_BLOCK,__FILE__,__LINE__))

#endif


#define free(x) (_free_dbg((x),_NORMAL_BLOCK))
#define msize(x) _msize_dbg((x),_NORMAL_BLOCK)


// macro to set and clear individual debug flags

#define  SET_CRT_DEBUG_FIELD(a)     _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a)   _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

// dump out all currently allocated memory blocks

#define DEBUG_MEM_DUMP \
			OutputDebugString(__FILE__);\
			OutputDebugString("(");\
			OutputDebugString(itoa(__LINE__,__debug__text__,10));\
			OutputDebugString("):  Memory Dump Requested\n");\
			_CrtMemDumpAllObjectsSince(NULL);\
			OutputDebugString("\n")


// check current memory heap for damage and unfreed blocks.
// do this at end of program to check for lost blocks.
#define DEBUG_MEM_CHECK \
			OutputDebugString(__FILE__);\
			OutputDebugString("(");\
			OutputDebugString(itoa(__LINE__,__debug__text__,10));\
			OutputDebugString("):  Memory Check Requested\n");\
			_CrtCheckMemory();\
			OutputDebugString("\n")

// send all debug warnings/errors and assetions to the output window
// displaying a window is of no use while in full screen mode..
#define DEBUG_MEM_INIT \
			SET_CRT_DEBUG_FIELD(_CRTDBG_LEAK_CHECK_DF);\
			SET_CRT_DEBUG_FIELD( _CRTDBG_DELAY_FREE_MEM_DF );\
		   _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );\
		   _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );\
		   _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG )


// send a message to the output window
#define DEBUGSTR(a) OutputDebugString((char*)(a));

// jump into the debugger
#define DEBUG_BREAK	_asm{int 3}

// an assertion that will exit the program after diplaying the line and file info along with the specified message
#define ASSERTMSG(x,msg) if (x) {} else { dprintf("ASSERTION FAILED in '%s' at line: %d\nREASON:%s\n",__FILE__,__LINE__,(char*)(msg)); exit(-1); }

// a function to convert DirectX return codes into error messages
extern char* DirectDrawErrorToString(HRESULT ddrval);


#else //_DEBUG


// in non debug builds all macros become nothing, this will force the real memory funcitons to be called and all other things will
// be removed..


#define SET_CRT_DEBUG_FIELD(a)
#define CLEAR_CRT_DEBUG_FIELD(a)
#define DEBUG_MEM_DUMP
#define DEBUG_MEM_CHECK
#define DEBUG_MEM_INIT
#define DEBUG_BREAK
#define ASSERTMSG(x,msg)
#define DEBUGSTR(msg)
#define DirectDrawErrorToString(x)	(NULL)
#endif //_DEBUG


// a printf style function that sends text to the output console window
// inside the debugger.
// this is always defined but is an empty function in the debug build.
extern void __cdecl dprintf(char*,...);


#endif //_