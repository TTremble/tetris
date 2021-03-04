/*************************************************************************/
/*                                                                       */
/*                           TETRIS IN A NUTCHELL                        */
/*                                                                       */
/*************************************************************************/

/* Headers */

#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<SDL2/SDL.h>

/* a premiere vue, je dirai qu'il faut une classe pour le plateau de jeu,
une classe pour les blocs, et plein de fonction pour rendre tout ça dynamique*/

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
const SDL_Rect TILE_ROUGE = {0,0,27,27};
const SDL_Rect TILE_BLEU = {27,0,27,27};
const SDL_Rect TILE_ORANGE = {54,0,27,27};
const SDL_Rect TILE_JAUNE = {81,0,27,27};
const SDL_Rect TILE_VIOLET = {108,0,27,27};
const SDL_Rect TILE_CYAN = {135,0,27,27};
const SDL_Rect TILE_VERT = {162,0,27,27};

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
    SDL_Window *window=SDL_CreateWindow("Tetris",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,320,613,0);
    bool keep_window_open = true;
    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    SDL_Surface *tiles = SDL_LoadBMP("Tiles.bmp");
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    //Gestion fenêtre
    while(keep_window_open)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0)
        {
            switch(e.type){
                case SDL_QUIT:
                    keep_window_open=false;
                    break;
            }
            SDL_BlitSurface(image, NULL, window_surface, NULL);
            int n=5;
            for(int i=0;i<4;i++){
                a[i].x = blocks[n].cases[i]%2;
                a[i].y = blocks[n].cases[i]/2;
            }
            for(int i=0;i<4;i++){
                SDL_Rect POS = {(a[i].x+1)*27,(a[i].y)*27,27,27};
                SDL_BlitSurface(tiles,&blocks[n].couleur,window_surface,&POS);
            }
            SDL_UpdateWindowSurface(window);
        }
    }
}