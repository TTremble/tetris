
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


int main() {

    SDL_Color color={0,0,0};
    SDL_Surface *text_surface = NULL;
    TTF_Font *police = NULL;

    if ( TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    police = TTF_OpenFont("SilverMedal.ttf", 25);

    if ( police == NULL ){
        fprintf(stderr, "Erreur de lecture de la police: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    text_surface = TTF_RenderText_Solid(police, "Next Block", color);
    SDL_Window *window = SDL_CreateWindow("Tetris",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1152,736,0);
    bool keep_window_open = true;
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!(text_surface=TTF_RenderText_Solid(police, "Next Block",color))) {
        //handle error here, perhaps print TTF_GetError at least
    } 
    else {
        SDL_BlitSurface(text_surface,NULL,window_surface,NULL);
        //perhaps we can reuse it, but I assume not for simplicity.
        SDL_FreeSurface(text_surface);
    }
    return 0;
}