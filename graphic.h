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
        
        SDL_Texture* LoadTexture(std::string path);
        SDL_Texture* CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color);
		void ClearBackBuffer();
		void DrawTexture(SDL_Texture* tex, SDL_Rect* clip = NULL, SDL_Rect* rend = NULL, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);


//        void Render();

    private :

        static Graphics* sInstance;
        static bool sInitialized;

        SDL_Window* mWindow;
//		SDL_Renderer* mRenderer;

        Graphics();
        ~Graphics();

        bool Init();


};

#endif