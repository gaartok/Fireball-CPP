
#include <stdlib.h>
#include <math.h>
#include "Misc.hpp"
#include "global.h"
#include "debug.h"





int randInt(int low, int high)
   {
   int range = high - low;
   int num = rand() % range;
   return(num + low);
   }


DWORD randDWORD(DWORD low, DWORD high)
   {
   DWORD range = high - low;
   DWORD num = (DWORD)rand() % range;

   return(num + low);
   }


double randDouble(double low, double high)
   {
   double range = high - low;
   double num = range * (double)rand() / (double)RAND_MAX;
   return(num + low);
   }



float randFloat(float low, float high)
   {
   float range = high - low;
   float num = range * (float)rand() / (float)RAND_MAX;
   return(num + low);
   }



int getint(char**p, int def)
   {
   int i=0;


   while (IS_SPACE(**p))
      (*p)++;

   if (!IS_NUM(**p))
      return def;

   while (IS_NUM(**p))
      i = i*10 + *(*p)++ - '0';

   while (IS_SPACE(**p))
      (*p)++;

   return i;
   }



BOOL GetSign(long numIn)
   {
   if (numIn > 0)
      return TRUE;
   else
      return FALSE;
   }


