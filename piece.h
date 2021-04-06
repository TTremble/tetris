#ifndef _PIECE_H
#define _PIECE_H

#include "graphic.h"

class Piece {
    
    public:
        static Piece Instance();
        static void Release();
        bool check();

    private:

        static Piece* sInstance;

        shape blocks[7]={
            {ROUGE,TILE_ROUGE,{1,3,5,7},5,4}, // I
            {BLEU,TILE_BLEU,{2,3,5,7},5,4}, // L
            {ORANGE,TILE_ORANGE,{3,5,7,6},5,4}, // J
            {JAUNE,TILE_JAUNE,{2,3,4,5},5,4}, // O
            {VIOLET,TILE_VIOLET,{3,5,4,6},5,4}, // S
            {CYAN,TILE_CYAN,{3,5,4,7},5,4}, // T
            {VERT,TILE_VERT,{2,4,5,7},5,4}  // Z
        };

        const int SPEED=300;
        bool rotate=0;
        int n=rand()%7;
        int dx=0;

        Piece();
        ~Piece();

} 



#endif