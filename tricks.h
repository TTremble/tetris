#ifndef _TRICKSFILE_H
#define _TRICKSFILE_H

#include <math.h>

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
    
    float x;
    float y;

    point(float _x = 0.0f, float _y = 0.0f)
        : x(_x), y(_y) {}

    float MagnitudeSqr() {
        return x*x + y*y;
    }

    float Magnitude() {
        return (float)sqrt(x*x+y*y);
    }

    point Normalized() {

        float mag = Magnitude();
        return point(x /mag, y / mag);
    }

};

struct shape{
    int couleur;
    SDL_Rect sprite;
    int cases[7];
    double x,y;
};


#endif