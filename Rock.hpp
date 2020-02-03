
#ifndef __Rock_hpp__
#define __Rock_hpp__


#include "AliveObject.hpp"
#include "Emitter.hpp"


class Rock : public AliveObject
   {
private:
   UINT           bounceCount;
   DWORD          nextAccelTime;
   Emitter *      tail;

   void           GetNextFrame(void);
   void           AimTail(void);

public:

                  Rock();
                  ~Rock();
   RECT *         Move(AliveList *collideableList);
   HRESULT        Draw(void);
   };



#endif      // #ifndef __Rock_hpp__
