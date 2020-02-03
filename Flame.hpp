
#ifndef __Flame_hpp__
#define __Flame_hpp__


#include "AliveObject.hpp"


class Flame : public AliveObject
   {
private:
   void     GetNextFrame(void);


public:

            Flame();
            ~Flame();
   };



#endif      // #ifndef __Flame_hpp__
