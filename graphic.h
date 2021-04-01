#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Graphics {

    public:

        const int SCREEN_WIDTH = 384;
        const int SCREEN_HEIGHT = 736;
        const char* WINDOW_TITLE = "Tetris";

        static Graphics* Instance();    
        static void Release();
        static bool Initialized();
        void ShowGame(SDL_Surface* image, SDL_Surface* windowSurf);

    private :

        static Graphics* sInstance;
        static bool sInitialized;

        SDL_Window* mWindow;

        Graphics();
        ~Graphics();

        bool Init();


};

#endif