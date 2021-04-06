#ifndef _TETRIS_H
#define _TETRIS_H

#include "graphic.h"
#include "timer.h"
//#include "InputManager.h"
#include "GameEntity.h"

class GameManager {

    public:
        static GameManager* Instance();
        static void Release();
        void Run();

        static GameManager* sInstance;

        const int FRAME_RATE = 120;

        bool mQuit;
        Graphics* mGraphics;
        SDL_Event mEvents;

        Timer* mTimer;

        GameEntity* mParent;
        GameEntity* mChild;

        //InputManager* mInputMngr;

        GameManager();
        ~GameManager();

};


#endif