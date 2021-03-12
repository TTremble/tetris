#ifndef _TETRIS_H
#define _TETRIS_H

#include "graphic.h"

class GameManager {

    public:
        static GameManager* Instance();
        static void Release();
        void Run();

    private:
        static GameManager* sInstance;

        const int FRAME_RATE = 120;

        bool mQuit;
        Graphics* mGraphics;
        SDL_Event mEvents;

        GameManager();
        ~GameManager();

};


#endif