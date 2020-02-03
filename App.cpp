
#include "App.hpp"
#include "Hero.hpp"
#include "House.hpp"
#include "Rock.hpp"
#include "Emitter.hpp"
//#include "DDSupport.h"
#include "global.h"
#include "debug.h"


Hero *   hero = NULL;
Emitter *volcanoEmitter = NULL;


extern DWORD CreateRegKey(char *keyName);
extern DWORD GetRegString(char *keyName, char *valueName, char *dataOut);
extern DWORD SetRegString(char *keyName, char *valueName, char *dataOut);



#if 0
// Temporary!!  Remove this!!

enum
   {
   SURFACE_BACKGROUND = 0,
   SURFACE_SPRITES,
   SURFACE_HOUSE,
   SURFACE_NUMBERS,
   SURFACE_LAST
   };

SurfaceStruct  surfaceDescr[SURFACE_LAST] =
   {
   640,  480,  "VOLCANO",  NULL, NULL, // SURFACE_BACKGROUND
   80,   112,  "SPRITES",  NULL, NULL, // SURFACE_SPRITES
   192,  256,  "HOUSE",    NULL, NULL, // SURFACE_HOUSE
   416,  90,   "NUMBERS",  NULL, NULL  // SURFACE_NUMBERS
   };

// End of temporaries
#endif



App::App()
   {
   }



App::~App()
   {
   int   count1;
   AliveObject *currentObject;

   while (!mainList.empty())
      {
      currentObject = mainList.front();
      mainList.pop_front();
      delete currentObject;
//      DeleteObject(currentObject);
      }

   if (renderer)
      delete renderer;

   if (volcanoEmitter)
      delete volcanoEmitter;

   CleanupInput();      // clean up DirectInput objects
   DestroySound();

   // don't need to delete basket because it was added to mainList

   for (count1 = 0; count1 < NUM_WIN_SOUNDS; count1++)
      {
      SndObjDestroy(winSound[count1]);
      winSound[count1] = NULL;
      }

   for (count1 = 0; count1 < NUM_LOSE_SOUNDS; count1++)
      {
      SndObjDestroy(loseSound[count1]);
      loseSound[count1] = NULL;
      }

   SndObjDestroy(openingSound);

   // make the cursor visible
   SetCursor(LoadCursor(NULL, IDC_ARROW));
   }




BOOL App::Initialize(HWND hWndMain)
   {
   int      count1;

//   _CrtMemState s1, s2, s3;
//   _CrtMemCheckpoint(&s1);

   for (count1 = 0; count1 < 10; count1++)
      highScores[count1] = 100;

   strcpy(highScoreNames[0], "Sami the Great");
   strcpy(highScoreNames[1], "Amazing Aimee");
   strcpy(highScoreNames[2], "Super Sami");
   strcpy(highScoreNames[3], "Aimster");
   strcpy(highScoreNames[4], "Sami Hammy");
   strcpy(highScoreNames[5], "Aimee the Bee");
   strcpy(highScoreNames[6], "Silly Sami");
   strcpy(highScoreNames[7], "The Incredible Aimee");
   strcpy(highScoreNames[8], "Stunning Sami");
   strcpy(highScoreNames[9], "Lovely Aimee");

   ReadHighScores();

   status = TRUE;
   srand(GetTickCount());
   showFrameCount = TRUE;
   frameTime = timeGetTime();
   frameCount = 0;

   renderer = new Renderer();
   if (renderer == NULL)
      return FALSE;

   if (renderer->Initialize(hWndMain, 640, 480, 8, FALSE))
      {
      dprintf("renderer->Initialize failed!\n");
      return FALSE;
      }
//   renderer->LoadBackgroundFromFile("Art/Volcano.bmp");
//   renderer->LoadBackgroundFromResource("VOLCANO");

   InitializeSound();

   if (!InitInput(hInst, hWndMain))
      {
      dprintf("DirectInput initialization Failed!\n");
      return FALSE;
      }


   hero = new Hero;
   if (!hero)
      {
      dprintf("Out of memory!!\n");
      return FALSE;
      }
   mainList.push_back(hero);

   houseLeft = new House;
   if (!houseLeft)
      {
      dprintf("Out of memory!!\n");
      return FALSE;
      }
   mainList.push_back(houseLeft);
   houseLeft->SetDst(0, 480);
   houseLeft->SetAction(ACTION_IDLE_RIGHT);

   houseRight = new House;
   if (!houseRight)
      {
      dprintf("Out of memory!!\n");
      return FALSE;
      }
   mainList.push_back(houseRight);
   houseRight->SetDst((640 - 16), 480);
   houseRight->SetAction(ACTION_IDLE_LEFT);

   spewRateMin = 1000;
   spewRateMax = 3000;

   volcanoEmitter = new Emitter(400);
   volcanoEmitter->SetPos(225, 320, 120, 120);
   volcanoEmitter->SetAngles(0, 360, 270, 40);
   volcanoEmitter->SetSpeed(3.0, 0.5);
   volcanoEmitter->SetForce(0.0f, 0.04f, 0.0f);
   volcanoEmitter->SetEmits(1, 1);
   volcanoEmitter->SetLife(180, 15);
   volcanoEmitter->SetColors(16 + 7, 2, 16 + 5, 2);


   winSound[0]   = SndObjCreate(ds, "APPLAUSE1", 1);
   winSound[1]   = SndObjCreate(ds, "APPLAUSE2", 1);
   winSound[2]   = SndObjCreate(ds, "CHEERS2", 1);
   winSound[3]   = SndObjCreate(ds, "COOL", 1);
   winSound[4]   = SndObjCreate(ds, "EXCELLENT", 1);
   winSound[5]   = SndObjCreate(ds, "HALLELEUJAI", 1);

   loseSound[0]  = SndObjCreate(ds, "BABY1", 1);
   loseSound[1]  = SndObjCreate(ds, "CRYING", 1);
   loseSound[2]  = SndObjCreate(ds, "LAUGH1", 1);
   loseSound[3]  = SndObjCreate(ds, "LAUGH2", 1);
   loseSound[4]  = SndObjCreate(ds, "NEENER", 1);
   loseSound[5]  = SndObjCreate(ds, "TOADZILLA", 1);

   surfaceNumbersID = renderer->NewSurfaceFromResource("NUMBERS");
//   surfaceNumbersID = renderer->NewSurfaceFromFile("Art/Numbers.bmp");

   ActivateApp(TRUE);

   // Store a 2nd memory checkpoint in s2   
//   _CrtMemCheckpoint(&s2);
//   dprintf("Dump the changes that occurred between two memory checkpoints\n");

//   if (_CrtMemDifference(&s3, &s1, &s2))
//      _CrtMemDumpStatistics(&s3);

   openingSound  = SndObjCreate(ds, "GREATBALLS", 1);
   SndObjPlay(openingSound, 0);
   return TRUE;
   }




BOOL App::RunCycle(void)
   {
   // check for next-level score
//   if ((playerLevel == LEVEL1) && (playerScore > LEVEL2_MIN_SCORE))
//      InitLevel();
//   else if ((playerLevel == LEVEL2) && (playerScore > LEVEL3_MIN_SCORE))
//      InitLevel();

   // check for end-game situation (hero dead or both houses dead)
   if ((hero->GetHealth() == 0) || ((houseLeft->GetHealth() == 0) && (houseRight->GetHealth() == 0)))
      {
      int   whichSound;

      if (playerScore > highScores[9])    // have a new high score?
         {
         whichSound = randInt(0, NUM_WIN_SOUNDS - 1);
         SndObjPlay(winSound[whichSound], 0);
         }
      else     // nope, not a new high score
         {
         whichSound = randInt(0, NUM_LOSE_SOUNDS - 1);
         SndObjPlay(loseSound[whichSound], 0);
         }

      return TRUE;
      }

   UpdateDisplayList();
   DrawDisplayList();
   renderer->FlipBuffers();

   return FALSE;     // still running program
   }





int App::EnterName(void)
   {
   static int  state = 0;
   static int  highScorePos;
   static char userName[40];

   switch (state)
      {
      case 0:              // just ended game, check for high score
         {
         if ((highScorePos = SortHighScores()) != -1)
            {
            state = 1;
            userName[0] = 0;
            return FALSE;
            }
         else
            {
            FlushKeyboardInput();
            return TRUE;   // didn't have high score, so we're finished here
            }
         }

      case 1:              // did achieve high score, get user's name
         {
         RECT  sourceRECT;

         DrawDisplayList();
         sourceRECT.left   = 0;
         sourceRECT.right  = 300;
         sourceRECT.top    = 63;
         sourceRECT.bottom = 90;
         renderer->BltText("New High Score", surfaceNumbersID, 110, 180);
         renderer->Draw(100, 200, surfaceNumbersID, &sourceRECT);
         renderer->BltText(userName, surfaceNumbersID, 110, 206);
         renderer->FlipBuffers();

         if (InputString(userName, 29))
            {
            strcpy(highScoreNames[highScorePos], userName);
            WriteHighScores();
            state = 0;
            return TRUE;   // finished entering name
            }
         else
            {
            return FALSE;  // still entering name
            }
         }

      default:
         {
         state = 0;
         return TRUE;
         }
      }

   // should never get here
   return TRUE;
   }




int App::InputString(char *theString, size_t maxLen)
   {
   int            endStrPos;
   char           input;
   DWORD          rawKey;
   static BOOL    shiftPos = FALSE;

   rawKey = ReadOneKeyboardInput();

   if (((rawKey & 0x000000ff) == DIK_LSHIFT) || ((rawKey & 0x000000ff) == DIK_RSHIFT))
      {
      if (rawKey & 0x10000000)   // it's the upstroke, cancel SHIFT condition
         shiftPos = FALSE;
      else
         shiftPos = TRUE;

      return FALSE;
      }

   if (rawKey & 0x10000000)   // it's the upstroke and it's NOT the shift keys, return
      return FALSE;

   input = DIKCodeToAnsi(rawKey, shiftPos);

   if (input == 0)
      return FALSE;

   if (input == 27)     // Escape key
      {
      theString[0] = 0;
      return TRUE;
      }

   if (input == 13)     // Enter key
      return TRUE;

   if (input == 8)      // Backspace key
      {
      endStrPos = strlen(theString) - 1;
      if (endStrPos >= 0)
         theString[endStrPos] = 0;     // move NULL terminator back one
      return FALSE;
      }

   if (strlen(theString) >= maxLen)
      return FALSE;

   if (((input >= '0') && (input <= '9')) ||
       ((input >= 'A') && (input <= 'Z')) ||
       ((input >= 'a') && (input <= 'z')) ||
        (input == ' '))
      {
      endStrPos = strlen(theString);
      theString[endStrPos] = input;    // append the new char
      theString[endStrPos + 1] = 0;    // move NULL terminator
      }

   return FALSE;
   }



void App::ReadHighScores(void)
   {
   int   count1;
   DWORD dwResult;
   char  keyName[255];
   char  valueName[255];
   char  dataValue[255];

   strcpy(keyName, "Software\\Thoughtwaves\\FireBall\\Scores");

   for (count1 = 0; count1 < 10; count1++)
      {
      sprintf(valueName, "Name %d", count1);
      dwResult = GetRegString(keyName, valueName, dataValue);
      if (dwResult == ERROR_SUCCESS)
         strcpy(highScoreNames[count1], dataValue);

      sprintf(valueName, "Score %d", count1);
      dwResult = GetRegString(keyName, valueName, dataValue);
      if (dwResult == ERROR_SUCCESS)
         highScores[count1] = atoi(dataValue);
      }
   }



void App::WriteHighScores(void)
   {
   int   count1;
   DWORD dwResult;
   char  keyName[255];
   char  valueName[255];
   char  dataValue[255];

   strcpy(keyName, "Software\\Thoughtwaves\\FireBall\\Scores");

   for (count1 = 0; count1 < 10; count1++)
      {
      sprintf(valueName, "Name %d", count1);
      dwResult = SetRegString(keyName, valueName, highScoreNames[count1]);
      if (dwResult != ERROR_SUCCESS)
         dprintf("ERROR in WriteHighScores()\n");

      sprintf(valueName, "Score %d", count1);
      itoa(highScores[count1], dataValue, 10);
      dwResult = SetRegString(keyName, valueName, dataValue);
      if (dwResult != ERROR_SUCCESS)
         dprintf("ERROR in WriteHighScores()\n");
      }
   }


#if 0
void App::DeleteObject(AliveObject *deadObj)
   {
   switch (deadObj->GetType())
      {
      case TYPE_HERO:
         {
         delete (Hero *)deadObj;
         break;
         }

      case TYPE_HOUSE:
         {
         delete (House *)deadObj;
         break;
         }

      case TYPE_ROCK:
         {
         delete (Rock *)deadObj;
         break;
         }

      case TYPE_BASKET:
         {
         delete (Basket *)deadObj;
         break;
         }

      default:
         {
         delete deadObj;
         break;
         }
      }
   }
#endif



void App::Stop()
   {
   status = FALSE;
   }



static char instructions[][50] = 
   {
   "You were in your house when it happened.",
   "The volcano started to erupt!!  You grab",
   "your bucket, run outside, and fill the",
   "bucket with water.  Try to catch all the",
   "fireballs, but make sure you don't get hit",
   "on the head by one.  You must keep your",
   "bucket full of water to exterminate the",
   "flames. It's up to you to save the town.",
   "             GOOD LUCK!",
   " ",
   "             How To Play",
   "To move the person, push the left and right",
   "arrow keys.  Keep the bucket full of water",
   "by standing under the faucets on the house.",
   "If you let the houses burn down, you will",
   "have no water to full your bucket, so",
   "protect the houses!",
   " ",
   "        Push the SPACEBAR to begin"
   };



int App::ShowInstructions(void)
   {
   int            count1;
   DWORD          input;
   static DWORD   endTime = 0;

   if (!endTime)
      {
      renderer->SetPaletteFromResource("VOLCANO");
//      renderer->SetPaletteFromFile("Art/Volcano.bmp");
      renderer->LoadBackgroundFromResource("VOLCANO");
      DrawDisplayList();
      for (count1 = 0; count1 < 20; count1++)
         renderer->BltText(instructions[count1], surfaceNumbersID, 100, 40 + (20 * count1));

      renderer->FlipBuffers();
      endTime = timeGetTime() + SHOW_SPLASH_COUNT;
      FlushKeyboardInput();
      }

/*
   // look for timeout
   if (timeGetTime() > endTime)
      {
//      renderer->EraseAllScreens();
      endTime = 0;
      return TRUE;
      }
*/
   
   input = ReadOneKeyboardInput();
   if (input & 0x10000000)    // ignore upstrokes
      return FALSE;

   if (input)
      return TRUE;

   return FALSE;
   }



int App::ShowHighScores(void)
   {
   int   count1;
   DWORD input;
   char  strOut[100];

   DrawDisplayList();

   for (count1 = 0; count1 < 10; count1++)
      {
      sprintf(strOut, "%s", highScoreNames[count1]);
      renderer->BltText(strOut, surfaceNumbersID, 100, 100 + (20 * count1));
      sprintf(strOut, "%d", highScores[count1]);
      renderer->BltText(strOut, surfaceNumbersID, 100 + (16 * 20), 100 + (20 * count1));
      }

   renderer->FlipBuffers();

   input = ReadOneKeyboardInput();
   if (input & 0x10000000)    // ignore upstrokes
      return FALSE;
   if (input)
      return TRUE;

   return FALSE;
   }



int App::ShowEndingScore(void)
   {
   int   textWidth;
   DWORD input;
   char  strOut[20];

   strcpy(strOut, "Game Over");
   textWidth = strlen(strOut) * 16;
   DrawDisplayList();
   renderer->BltText(strOut, surfaceNumbersID, (SCREEN_WIDTH / 2) - (textWidth / 2), 100);
   renderer->FlipBuffers();

   input = ReadOneKeyboardInput();
   if (input & 0x10000000)    // ignore upstrokes
      return FALSE;
   if (input)
      return TRUE;

   return FALSE;
   }




int App::SortHighScores(void)
   {
   int   count1;

   // we assume that the existing scores are sorted with the highest in position 0
   // (which they will be unless someone's been monkeying with the registry)
   // if we have a new high score, make room for the new one by moving all the 
   // scores that are lower down by one (actually incrementing its index)
   count1 = 9;
   while ((count1 >= 0) && (playerScore > highScores[count1]))
      {
      if (count1 < 9)   // can't move the lowest score down, it's already at the bottom
         {
         highScores[count1 + 1] = highScores[count1];
         strcpy(highScoreNames[count1 + 1], highScoreNames[count1]);
         }

      count1 -= 1;
      }

   if (count1 < 9)   // if count1 still equals 9, we didn't have a new high score
      {
      highScores[count1 + 1] = playerScore;
      strcpy(highScoreNames[count1 + 1], "New High Score");    // this name will be replaced by the user
      return count1 + 1;
      }

   return -1;     // indicates we didn't have a new high score
   }




// stuff to be done once per game
void App::Setup(void)
   {
   playerScore = 0;
   playerLevel = LEVEL1;   
   }



// stuff to be done at the beginning of each level
void App::InitLevel(void)
   {
   AliveListItr   mainListItr;
   AliveObject *  nextObject;

   playerScore = 0;

   if (playerScore > LEVEL3_MIN_SCORE)
      playerLevel = LEVEL3;
   else if (playerScore > LEVEL2_MIN_SCORE)
      playerLevel = LEVEL2;
   else
      playerLevel = LEVEL1;

   // remove any rocks that may be left in the list from the previous level
   mainListItr = mainList.begin();
   while (mainListItr != mainList.end())
      {
      nextObject = *mainListItr;
      if (nextObject->GetType() == TYPE_ROCK)
         {
         delete nextObject;
//         DeleteObject(nextObject);
         mainListItr = mainList.erase(mainListItr);
         }
      else
         {
         ++mainListItr;
         }
      }

   houseLeft->NewLevel(playerLevel);
   houseRight->NewLevel(playerLevel);
   hero->NewLevel(playerLevel);

   nextSpewTime = timeGetTime();
   nextUpdateTime = timeGetTime();
   }



void App::LeftClick(int keysSet, int xPos, int yPos)
   {
#if 0
   BOOL           foundHit;
   AliveObject *  firstObject;
   AliveObject *  currentObject;

   firstObject = (AliveObject *)mainList->GetFirstChild1();
   if (firstObject == NULL)
      return;

   // MK_CONTROL  Set if the CTRL key is down.
   // MK_LBUTTON  Set if the left mouse button is down.
   // MK_MBUTTON  Set if the middle mouse button is down.
   // MK_RBUTTON  Set if the right mouse button is down.
   // MK_SHIFT    Set if the SHIFT key is down.

   foundHit = FALSE;
   currentObject = firstObject;
   do
      {
      if (currentObject->CheckForHit(xPos, yPos))
         {
         PlayPanned(boinkSound, (AliveObject *)currentObject);
         foundHit = TRUE;
         break;
         }
      currentObject = (AliveObject *)mainList->GetNextChild1();
      }
   while (currentObject != firstObject);

   if (foundHit == TRUE)      // clicked on a unit
      {
      foundHit = FALSE;
      firstObject = (AliveObject *)mainList->GetFirstChild1();
      currentObject = firstObject;
      do
         {
         if (keysSet & MK_SHIFT)    // holding SHIFT key down
            {
            if (!currentObject->IsSelected())
               {
               if ((currentObject->CheckForHit(xPos, yPos)) && (!foundHit))
                  {
                  currentObject->SetSelected(TRUE);
                  foundHit = TRUE;
                  }
               }
            }
         else                       // NOT holding SHIFT key down
            {
            if ((currentObject->CheckForHit(xPos, yPos)) && (!foundHit))
               {
               currentObject->SetSelected(TRUE);
               foundHit = TRUE;
               }
            else
               {
               currentObject->SetSelected(FALSE);
               }
            }

         currentObject = (AliveObject *)mainList->GetNextChild1();
         }
      while (currentObject != firstObject);
      }
   else                       // clicked on background
      {
      firstObject = (AliveObject *)mainList->GetFirstChild1();
      currentObject = firstObject;
      do
         {
         if (currentObject->IsSelected())
            {
//            DLList * thePath;

//            screenPtr->SetOccupied(currentObject->GetRect(), FALSE);
//            thePath = screenPtr->FindPath(currentObject->GetXPosition(), currentObject->GetYPosition(), (long)xPos, (long)yPos, currentObject->GetXSize());
//            currentObject->SetPath(thePath);
            }

         currentObject = (AliveObject *)mainList->GetNextChild1();
         }
      while (currentObject != firstObject);
      }
#endif
   }



void App::RightClick(int keysSet, int xPos, int yPos)
   {
#if 0
   AliveObject * firstObject;
   AliveObject * currentObject;

   firstObject = (AliveObject *)mainList->GetFirstChild1();
   if (firstObject == NULL)
      return;

   currentObject = firstObject;
   do
      {
      currentObject->SetSelected(FALSE);
      currentObject = (AliveObject *)mainList->GetNextChild1();
      }
   while (currentObject != firstObject);
#endif
   }




void App::DrawDisplayList(void)
   {
   AliveListItr   mainListItr;
   AliveObject *  nextObject;
   LONG           renderStatus;

//   renderer->EraseBackground();
   renderer->BltBackground();
   volcanoEmitter->Render();
   DisplayFrameRate();
   DisplayPlayerScore();

   if (mainList.empty())
      {
      dprintf("mainList is empty!!\n");
      Stop();
      return;
      }

   mainListItr = mainList.begin();
   while (mainListItr != mainList.end())
      {
      nextObject = *mainListItr;
      renderStatus = nextObject->Draw();
      if (renderStatus)
         {
         dprintf("in DrawDisplayList: ddrval = [%s]...\n",DirectDrawErrorToString(renderStatus));
         Stop();
         return;
         }

      ++mainListItr;
      }
   }





void App::UpdateDisplayList(void)
   {
   DWORD          input;
   Rock *         newRock;
   AliveObject *  nextObject;
   AliveListItr   mainListItr;

   volcanoEmitter->Update();

   if (timeGetTime() >= nextSpewTime)
      {
      newRock = new Rock;
//      dprintf("rock = %x\n", newRock);
      if (!newRock)
         {
         dprintf("Out of memory!!\n");
         Stop();
         }

      mainList.push_back(newRock);
//      PlayPanned(boinkSound, (AliveObject *)newRock);
      nextSpewTime += randDWORD(spewRateMin, spewRateMax);
      }

   input = ReadGameInput();
   if (input & KEY_ESCAPE)
      {
      status = FALSE;
      return;
      }

   if (mainList.empty())
      {
      Stop();
      dprintf("mainList is empty!!\n");
      return;
      }

   mainListItr = mainList.begin();
   while (mainListItr != mainList.end())
      {
      nextObject = *mainListItr;

      if (!nextObject->IsAlive())
         {
//         DeleteObject(nextObject);
         delete nextObject;
         mainListItr = mainList.erase(mainListItr);
         continue;
         }
      else
         {
         nextObject->Move(&mainList);
         ++mainListItr;
         }

      if (!nextObject->IsSelected())
         continue;

      if (input & KEY_SPACE)
         nextObject->FlipDir();

      if (input & KEY_LEFT)
         nextObject->MoveLeft();

      if (input & KEY_RIGHT)
         nextObject->MoveRight();
      }
   }



