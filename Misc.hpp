
#ifndef __Misc_hpp__
#define __Misc_hpp__

//#include <ddraw.h>


#define TARGET_FPS      30       // the frames-per-second we're shooting for

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_BPP      8        // bits-per-pixel (number of colors)


#define IS_NUM(c)     ((c) >= '0' && (c) <= '9')
#define IS_SPACE(c)   ((c) == ' ' || (c) == '\r' || (c) == '\n' || (c) == '\t' || (c) == 'x')


#define SHOW_SPLASH_COUNT  10000 // length of time to display splash screen in milliSeconds


/*
enum
   {
   SURFACE_BACKGROUND = 0,
   SURFACE_SPRITES,
   SURFACE_HOUSE,
   SURFACE_NUMBERS,
   SURFACE_LAST
   };
*/


// action types
enum
   {
   ACTION_UNKNOWN = 0,
   ACTION_IDLE_LEFT,
   ACTION_IDLE_RIGHT,
   ACTION_WALK_LEFT,
   ACTION_WALK_RIGHT,
   ACTION_WALK_UP,
   ACTION_WALK_DOWN,
   ACTION_FIDGET1,
   ACTION_FIDGET2,
   ACTION_FIDGET3,
   ACTION_DEAD,
   ACTION_LAST
   };

#define MAX_ACTIONS  ACTION_LAST



enum
   {
   LEVEL1 = 1,
   LEVEL2,
   LEVEL3
   };


#define LEVEL1_MIN_SCORE   0
#define LEVEL2_MIN_SCORE   100
#define LEVEL3_MIN_SCORE   300



// program states
enum
   {
   PS_SPLASH = 0,
   PS_INSTRUCTIONS,
   PS_BEGIN_LEVEL,
   PS_ACTIVE,
   PS_ENDGAME,
   PS_SHOW_SCORES,
   PS_ENTER_NAME,
   PS_FINISHED,
   PS_IDLE
   };


// item types
enum
   {
   TYPE_UNKNOWN = 0,
   TYPE_HERO,
   TYPE_BASKET,
   TYPE_ROCK,
   TYPE_HOUSE,
   TYPE_LAST
   };



#endif   // #ifndef __Misc_hpp__
