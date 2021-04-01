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

const int SPEED=300;
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
SDL_Rect tab[7]= {TILE_ROUGE,TILE_BLEU,TILE_ORANGE,TILE_JAUNE,TILE_VIOLET,TILE_CYAN,TILE_VERT};

struct point{
    int x,y;
} a[4], b[4];

struct shape{
    int couleur;
    SDL_Rect sprite;
    int cases[7];
    double x,y;
};

shape blocks[7]={
    {ROUGE,TILE_ROUGE,{1,3,5,7},5,4}, // I
    {BLEU,TILE_BLEU,{2,3,5,7},5,4}, // L
    {ORANGE,TILE_ORANGE,{3,5,7,6},5,4}, // J
    {JAUNE,TILE_JAUNE,{2,3,4,5},5,4}, // O
    {VIOLET,TILE_VIOLET,{3,5,4,6},5,4}, // S
    {CYAN,TILE_CYAN,{3,5,4,7},5,4}, // T
    {VERT,TILE_VERT,{2,4,5,7},5,4}  // Z
};

bool check(){
    for(int i=0;i<4;i++){
        if(a[i].x>=N||a[i].x<0||a[i].y==M+1){
            return false;
        }
        if(field[a[i].y][a[i].x]!=0){
            return false;
        }
    }
    return true;
}

int main()
{
    //Initialisation de la fenêtre de jeu
    SDL_Window *window=SDL_CreateWindow("Tetris",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,384,736,0);
    bool keep_window_open = true;
    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    SDL_Surface *tiles = SDL_LoadBMP("Tiles.bmp");
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    //Quelques variables
    int counter=0;
    int SPEED=300;
    int speed_ini=SPEED;
    int tps_actuel=0;
    int tps_prec=0;
    int n=rand()%7;
    int dx=0;
    bool rotate=0;
    int colorNum=1;
    bool reset=true;
    unsigned int delay=0;
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
                if(e.key.keysym.sym==SDLK_LEFT){
                    dx=-1;
                }
                else if(e.key.keysym.sym==SDLK_RIGHT){
                    dx=+1;
                }
                else if(e.key.keysym.sym==SDLK_UP){
                    rotate=true;
                }
                else if(e.key.keysym.sym==SDLK_DOWN){
                    SPEED=SPEED/10;
                }
                else if(e.key.keysym.sym=SDLK_SPACE){
                    SPEED=1;
                }
            }
        }
        SDL_BlitSurface(image, NULL, window_surface, NULL);
        //Movement
        for(int i=0;i<4;i++){
            b[i]=a[i];
            a[i].x +=dx;
        }
        if(!check()){
            for(int i=0;i<4;i++){
                a[i]=b[i];
            }
        }
        //Rotation
        if(rotate){
            point p= a[1]; //Centre de rotation
            for (int i=0;i<4;i++){
                b[i]=a[i];
                int x=a[i].y-p.y;
                int y=a[i].x-p.x;
                a[i].x = p.x-x;
                a[i].y = p.y+y;
            }
        }
        if(!check()){
            for(int i=0;i<4;i++){
                a[i]=b[i];
            }
        }
        //Ini
        if(reset){
            for(int i=0;i<4;i++){
                a[i].x = blocks[n].cases[i]%2;
                a[i].y = blocks[n].cases[i]/2;
            }
        }
        //Game
        tps_actuel=SDL_GetTicks();
        if(tps_actuel-tps_prec>SPEED){
            for(int i=0;i<4;i++){
                b[i]=a[i];
                a[i].y++;
            }
            tps_prec=tps_actuel;
            if(!check()){
                for(int i=0;i<4;i++){
                    field[b[i].y][b[i].x]=blocks[n].couleur;
                }
                n=rand()%7;
                for(int i=0;i<4;i++){
                    a[i].x = blocks[n].cases[i]%2;
                    a[i].y = blocks[n].cases[i]/2;
                }
            }
        }
        //Accélération
        if(SDL_GetTicks()%5000==0){
            speed_ini+=-1;
        }
        //Draw
        for(int i =0;i<M;i++){
            for(int j=0;j<N;j++){
                if(field[i][j]!=0){
                    SDL_Rect PLACE = {(j+1)*TILESIZE,(i+1)*TILESIZE,TILESIZE,TILESIZE};
                    SDL_BlitSurface(tiles,&tab[field[i][j]-1],window_surface,&PLACE);
                }
            }
        }
        //Check Lines
        int k=M-1;
        for(int i=M-1;i>0;i--){
            int count=0;
            for(int j=0;j<N;j++){
                if(field[i][j]){
                    count++;
                }
                field[k][j]=field[i][j];
            }
            if(count<N){
                k--;
            }
        }

        //Fin
        reset=false;
        SPEED=speed_ini;
        dx=0;
        rotate=0;
        for(int i=0;i<4;i++){
            SDL_Rect POS = {(a[i].x+1)*TILESIZE,(a[i].y)*TILESIZE,TILESIZE,TILESIZE};
            SDL_BlitSurface(tiles,&blocks[n].sprite,window_surface,&POS);
        }
        SDL_UpdateWindowSurface(window);
    }
}