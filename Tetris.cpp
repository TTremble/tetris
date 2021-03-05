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
            //Key presses
            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_LEFT&&(a[0].x!=0&&a[1].x!=0&&a[2].x!=0&&a[3].x!=0)){
                    dx=-1;
                }
                else if(e.key.keysym.sym==SDLK_RIGHT&&(a[0].x!=9&&a[1].x!=9&&a[2].x!=9&&a[3].x!=9)){
                    dx=+1;
                }
                else if(e.key.keysym.sym==SDLK_UP){
                    rotate=true;
                }
            }
            SDL_BlitSurface(image, NULL, window_surface, NULL);
            //Movement
            for(int i=0;i<4;i++){
                a[i].x +=dx;
            }
            //Rotation
            if(rotate){
                point p= a[1]; //Centre de rotation
                for (int i=0;i<4;i++){
                    int x=a[i].y-p.y;
                    int y=a[i].x-p.x;
                    a[i].x = p.x-x;
                    a[i].y = p.y+y;
                }
            }
            //Game
            if(SDL_GetTicks()>delay){
                delay+=1000;
                for(int i=0;i<4;i++){
                    a[i].y++;
                }
            }

            int n=5;
            if(reset){
                for(int i=0;i<4;i++){
                    a[i].x = blocks[n].cases[i]%2;
                    a[i].y = blocks[n].cases[i]/2;
                }
            }
            reset=false;
            dx=0;
            rotate=0;
            for(int i=0;i<4;i++){
                SDL_Rect POS = {(a[i].x+1)*TILESIZE,(a[i].y)*TILESIZE,TILESIZE,TILESIZE};
                SDL_BlitSurface(tiles,&blocks[n].couleur,window_surface,&POS);
            }
            SDL_UpdateWindowSurface(window);
        }
    }
}