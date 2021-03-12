/*************************************************************************/
/*                                                                       */
/*                           TETRIS IN A NUTCHELL                        */
/*                                                                       */
/*************************************************************************/

/* Headers */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <SDL2/SDL.h>

// #include "timer.h"

////// CONSTANTES


const int TILESIZE=32;

const int M=20;
const int N=10;
int field[M][N]={0};

// COULEURS
const int ROUGE=1;
const int BLEU=2;
const int ORANGE=3;
const int JAUNE=4;
const int VIOLET=5;
const int CYAN=6;
const int VERT=7;

// TILES
const SDL_Rect TILE_ROUGE = {0,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_BLEU = {TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_ORANGE = {2*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_JAUNE = {3*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_VIOLET = {4*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_CYAN = {5*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_VERT = {6*TILESIZE,0,TILESIZE,TILESIZE};

struct point{
    int x,y;
} a[4], b[4];

struct shape{
    SDL_Rect couleur;
    int cases[7];
    double x,y;
};

shape blocks[7]={
    {TILE_ROUGE,{1,3,5,7},5,4}, // I
    {TILE_BLEU,{2,3,5,7},5,4}, // L
    {TILE_ORANGE,{3,5,7,6},5,4}, // J
    {TILE_JAUNE,{2,3,4,5},5,4}, // O
    {TILE_VIOLET,{3,5,4,6},5,4}, // S
    {TILE_CYAN,{3,5,4,7},5,4}, // T
    {TILE_VERT,{2,4,5,7},5,4}  // Z
};

int main()
{
    //Initialisation de la fenêtre de jeu
    SDL_Window *window=SDL_CreateWindow("Tetris",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,384,736,0);
    bool keep_window_open = true;
    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    SDL_Surface *tiles = SDL_LoadBMP("Tiles.bmp");
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    //Quelques variables
    int dx=0;
    bool rotate=0;
    int colorNum=1;
    bool reset=true;
    unsigned int delay=1000;
    //Ecrire un truc ici
    while(keep_window_open)
    {
        //Events
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0)
        {
            //closing window
            switch(e.type){
                case SDL_QUIT:
                    keep_window_open=false;
                    break;
            }

            SDL_BlitSurface(image, NULL, window_surface, NULL);

            SDL_UpdateWindowSurface(window);
        }
    }
}
