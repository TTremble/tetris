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
int field2[M][N]={0};

// COULEURS
const int ROUGE=1;
const int BLEU=2;
const int ORANGE=3;
const int JAUNE=4;
const int VIOLET=5;
const int CYAN=6;
const int VERT=7;
const int NOIR=8;
const int BLANC=9;

// TILES
const SDL_Rect TILE_ROUGE = {0,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_BLEU = {TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_ORANGE = {2*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_JAUNE = {3*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_VIOLET = {4*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_CYAN = {5*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_VERT = {6*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_NOIR = {7*TILESIZE,0,TILESIZE,TILESIZE};
const SDL_Rect TILE_BLANC = {8*TILESIZE,0,TILESIZE,TILESIZE};
SDL_Rect tab[9]= {TILE_ROUGE,TILE_BLEU,TILE_ORANGE,TILE_JAUNE,TILE_VIOLET,TILE_CYAN,TILE_VERT,TILE_NOIR,TILE_BLANC};

struct point{
    int x,y;
} a[4], b[4], a2[4], b2[4];

struct shape{
    int couleur;
    SDL_Rect sprite;
    int cases[7];
    double x,y;
};

shape blocks[9]={
    {ROUGE,TILE_ROUGE,{1,3,5,7},5,4}, // I
    {BLEU,TILE_BLEU,{2,3,5,7},5,4}, // L
    {ORANGE,TILE_ORANGE,{3,5,7,6},5,4}, // J
    {JAUNE,TILE_JAUNE,{2,3,4,5},5,4}, // O
    {VIOLET,TILE_VIOLET,{3,5,4,6},5,4}, // S
    {CYAN,TILE_CYAN,{3,5,4,7},5,4}, // T
    {VERT,TILE_VERT,{2,4,5,7},5,4},  // Z
    {NOIR,TILE_NOIR,{1,2,3,4},5,4}, // PLACEHOLDER
    {BLANC,TILE_BLANC,{1,2,3,4},5,4} //Utile uniquement pour la couleur
};

bool check(){
    for(int i=0;i<4;i++){
        if(a[i].x>=N||a[i].x<0||a[i].y==M){
            return false;
        }
        if(field[a[i].y][a[i].x]!=0){
            return false;
        }
    }
    return true;
}

bool check2(){
    for(int i=0;i<4;i++){
        if(a2[i].x>=N||a2[i].x<0||a2[i].y==M){
            return false;
        }
        if(field2[a2[i].y][a2[i].x]!=0){
            return false;
        }
    }
    return true;
}

void give_player1(int mult){
    for(int i=0;i<M-mult;i++){
        for(int j=0;j<N;j++){
            field[i][j]=field[i+mult][j];
        }
    }
    int except=rand()%9;
    for(int i=0;i<mult;i++){
        for(int j=0;j<N;j++){
            field[M-i-1][j]=9;
            field[M-i-1][except]=0;
        }
    }
}

void give_player2(int mult){
    for(int i=0;i<M-mult;i++){
        for(int j=0;j<N;j++){
            field2[i][j]=field2[i+mult][j];
        }
    }
    int except=rand()%9;
    for(int i=0;i<mult;i++){
        for(int j=0;j<N;j++){
            field2[M-i-1][j]=9;
            field2[M-i-1][except]=0;
        }
    }
}

int main()
{
    //Initialisation de la fenêtre de jeu
    SDL_Window *window=SDL_CreateWindow("Tetris",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1152,736,0);
    bool keep_window_open = true;
    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    SDL_Surface *tiles = SDL_LoadBMP("Tiles.bmp");
    SDL_Surface *cadre = SDL_LoadBMP("cadre.bmp");
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    //Quelques variables
    int score=0;
    int score2=0;
    int counter=0;
    int lcount=0;
    int lcount2=0;
    int level=1;
    int level2=1;
    int SPEED=1000;
    int SPEED2=1000;
    int speed_ini=SPEED;
    int tps_actuel=0;
    int tps_prec=0;
    int tps_prec2=0;
    int next=rand()%7;
    int n=rand()%7;
    int next2=rand()%7;
    int n2=rand()%7;
    int dx=0;
    int dx2=0;
    bool space=false;
    bool space2=false;
    bool rotate=false;
    bool rotate2=false;
    bool hold=false;
    bool hold2=false;
    int held=-1;
    int held2=-1;
    bool used=false;
    bool used2=false;
    int colorNum=1;
    bool reset=true;
    unsigned int delay=0;
    //Ecrire un truc ici
    while(keep_window_open)
    {
        SDL_FillRect(window_surface, NULL, 0x000000);
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
                else if(e.key.keysym.sym==SDLK_SPACE){
                    space=true;
                }
                else if(e.key.keysym.sym==SDLK_c){
                    hold=true;
                }
                if(e.key.keysym.sym==SDLK_q){
                    dx2=-1;
                }
                else if(e.key.keysym.sym==SDLK_d){
                    dx2=+1;
                }
                else if(e.key.keysym.sym==SDLK_z){
                    rotate2=true;
                }
                else if(e.key.keysym.sym==SDLK_s){
                    SPEED2=SPEED2/10;
                }
                else if(e.key.keysym.sym==SDLK_a){
                    space2=true;
                }
                else if(e.key.keysym.sym==SDLK_e){
                    hold2=true;
                }
            }
        }
        SDL_BlitSurface(image, NULL, window_surface, NULL);
        SDL_Rect Joueur2 = {24*TILESIZE,0,12*TILESIZE,22*TILESIZE};
        SDL_BlitSurface(image,NULL, window_surface, &Joueur2);
        SDL_Rect CADRE1 = {12*TILESIZE,3*TILESIZE,6*TILESIZE,6*TILESIZE};
        SDL_Rect CADRE2 = {12*TILESIZE,12*TILESIZE,6*TILESIZE,6*TILESIZE};
        SDL_Rect CADRE3 = {18*TILESIZE,3*TILESIZE,6*TILESIZE,6*TILESIZE};
        SDL_Rect CADRE4 = {18*TILESIZE,12*TILESIZE,6*TILESIZE,6*TILESIZE};
        SDL_BlitSurface(cadre,NULL,window_surface,&CADRE1);
        SDL_BlitSurface(cadre,NULL,window_surface,&CADRE2);
        SDL_BlitSurface(cadre,NULL,window_surface,&CADRE3);
        SDL_BlitSurface(cadre,NULL,window_surface,&CADRE4);
        //Movement
        for(int i=0;i<4;i++){
            b[i]=a[i];
            a[i].x +=dx;
            b2[i]=a2[i];
            a2[i].x +=dx2;
        }
        if(!check()){
            for(int i=0;i<4;i++){
                a[i]=b[i];
            }
        }
        if(!check2()){
            for(int i=0;i<4;i++){
                a2[i]=b2[i];
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
        if(rotate2){
            point p= a2[1]; //Centre de rotation
            for (int i=0;i<4;i++){
                b2[i]=a2[i];
                int x=a2[i].y-p.y;
                int y=a2[i].x-p.x;
                a2[i].x = p.x-x;
                a2[i].y = p.y+y;
            }
        }
        if(!check2()){
            for(int i=0;i<4;i++){
                a2[i]=b2[i];
            }
        }
        //Ini
        if(reset){
            for(int i=0;i<4;i++){
                a[i].x = blocks[n].cases[i]%2 + N/2 -1;
                a[i].y = blocks[n].cases[i]/2 -1;
                a2[i].x = blocks[n2].cases[i]%2 + N/2 -1;
                a2[i].y = blocks[n2].cases[i]/2 -1;
            }
        }
        //Aterrissage
        if(space){
            int diff=100;
            for(int i=0;i<4;i++){
                int j=a[i].y;
                while(j!=M&&field[j][a[i].x]==0){
                    j++;
                }
                if(j-a[i].y<diff){
                    diff=j-a[i].y;
                }
            }
            for(int i=0;i<4;i++){
                a[i].y=a[i].y+diff-1;
            }
        }
        if(space2){
            int diff=100;
            for(int i=0;i<4;i++){
                int j=a2[i].y;
                while(j!=M&&field2[j][a2[i].x]==0){
                    j++;
                }
                if(j-a2[i].y<diff){
                    diff=j-a2[i].y;
                }
            }
            for(int i=0;i<4;i++){
                a2[i].y=a2[i].y+diff-1;
            }
        }
        //Hold
        if(hold&&!used){
            if(held==-1){
                held=n;
                n=next;
                next=rand()%7;
            }
            else{
                int t=held;
                held=n;
                n=t;
            }
            for (int i=0;i<4;i++){
                    a[i].x = blocks[n].cases[i]%2 + N/2 -1;
                    a[i].y = blocks[n].cases[i]/2-1;
                }
            used=true;
        }
        if(hold2&&!used2){
            if(held2==-1){
                held2=n2;
                n2=next2;
                next2=rand()%7;
            }
            else{
                int t2=held2;
                held2=n2;
                n2=t2;
            }
            for (int i=0;i<4;i++){
                    a2[i].x = blocks[n2].cases[i]%2 + N/2 -1;
                    a2[i].y = blocks[n2].cases[i]/2-1;
                }
            used2=true;
        }
        //Avancée
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
                used=false;
                n=next;
                next=rand()%7;
                for(int i=0;i<4;i++){
                    if(a[i].y<2){
                        keep_window_open=false;
                    }
                    a[i].x = blocks[n].cases[i]%2 + N/2 -1;
                    a[i].y = blocks[n].cases[i]/2-1;
                }
            }
        }
        tps_actuel=SDL_GetTicks();
        if(tps_actuel-tps_prec2>SPEED2){
            for(int i=0;i<4;i++){
                b2[i]=a2[i];
                a2[i].y++;
            }
            tps_prec2=tps_actuel;
            if(!check2()){
                for(int i=0;i<4;i++){
                    field2[b2[i].y][b2[i].x]=blocks[n2].couleur;
                }
                used2=false;
                n2=next2;
                next2=rand()%7;
                for(int i=0;i<4;i++){
                    if(a2[i].y<2){
                        keep_window_open=false;
                    }
                    a2[i].x = blocks[n2].cases[i]%2 + N/2 -1;
                    a2[i].y = blocks[n2].cases[i]/2-1;
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
                if(field2[i][j]!=0){
                    SDL_Rect PLACE2 = {(j+25)*TILESIZE,(i+1)*TILESIZE,TILESIZE,TILESIZE};
                    SDL_BlitSurface(tiles,&tab[field2[i][j]-1],window_surface,&PLACE2);
                }
            }
        }
        //Check Lines
        //Player 1
        int k=M-1;
        int mult=0;
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
            else{
                mult++;
                lcount++;
            }
        }

        if(lcount>10){
            level++;
            lcount=lcount-10;
        }

        switch(mult){
            case 0:
                break;
            case 1:
                score+= 40*level;
                break;
            case 2:
                score+= 100*level;
                break;
            case 3:
                score+= 300*level;
                break;
            case 4:
                score+= 1200*level;
                break;
        }
        give_player2(mult);
        //Player 2
        int k2=M-1;
        int mult2=0;
        for(int i=M-1;i>0;i--){
            int count=0;
            for(int j=0;j<N;j++){
                if(field2[i][j]){
                    count++;
                }
                field2[k2][j]=field2[i][j];
            }
            if(count<N){
                k2--;
            }
            else{
                mult2++;
                lcount2++;
            }
        }

        if(lcount2>10){
            level2++;
            lcount2=lcount2-10;
        }

        switch(mult2){
            case 0:
                break;
            case 1:
                score2+= 40*level;
                break;
            case 2:
                score2+= 100*level;
                break;
            case 3:
                score2+= 300*level;
                break;
            case 4:
                score2+= 1200*level;
                break;
        }
        give_player1(mult2);


        //Fin
        reset=false;
        SPEED=speed_ini;
        SPEED2=speed_ini;
        dx=0;
        dx2=0;
        rotate=0;
        rotate2=0;
        space=0;
        space2=0;
        hold=0;
        hold2=0;
        for(int i=0;i<4;i++){
            SDL_Rect POS = {(a[i].x+1)*TILESIZE,(a[i].y)*TILESIZE,TILESIZE,TILESIZE};
            SDL_BlitSurface(tiles,&blocks[n].sprite,window_surface,&POS);
            SDL_Rect POS2 = {(a2[i].x+25)*TILESIZE,(a2[i].y)*TILESIZE,TILESIZE,TILESIZE};
            SDL_BlitSurface(tiles,&blocks[n2].sprite,window_surface,&POS2);
        }
        for(int i=0;i<4;i++){
            SDL_Rect NEXTPOS = {(14+blocks[next].cases[i]%2)*TILESIZE,(4+blocks[next].cases[i]/2)*TILESIZE,TILESIZE,TILESIZE};
            SDL_BlitSurface(tiles,&blocks[next].sprite,window_surface,&NEXTPOS);
            SDL_Rect NEXTPOS2 = {(20+blocks[next2].cases[i]%2)*TILESIZE,(4+blocks[next2].cases[i]/2)*TILESIZE,TILESIZE,TILESIZE};
            SDL_BlitSurface(tiles,&blocks[next2].sprite,window_surface,&NEXTPOS2);
        }
        if(held!=-1){
            for(int i=0;i<4;i++){
                SDL_Rect HELDPOS = {(14+blocks[held].cases[i]%2)*TILESIZE,(13+blocks[held].cases[i]/2)*TILESIZE,TILESIZE,TILESIZE};
                SDL_BlitSurface(tiles,&blocks[held].sprite,window_surface,&HELDPOS);
            }
        }
        if(held2!=-1){
            for(int i=0;i<4;i++){
                SDL_Rect HELDPOS2 = {(20+blocks[held2].cases[i]%2)*TILESIZE,(13+blocks[held2].cases[i]/2)*TILESIZE,TILESIZE,TILESIZE};
                SDL_BlitSurface(tiles,&blocks[held2].sprite,window_surface,&HELDPOS2);
            }
        }
        SDL_UpdateWindowSurface(window);
    }
    std::cout << "score=" << score << std::endl;
    std::cout << "level=" << level << std::endl;
    std::cout << "score2=" << score2 << std::endl;
    std::cout << "level2=" << level2 << std::endl;
}