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
#include<SDL2/SDL_ttf.h>


const int TILESIZE=32;

const int M=20;
const int N=10;

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
};

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

class Tetris{
    public:
        point a[4];
        point b[4];
        int field[M][N]={0};
        int score=0;
        int lcount=0;
        int level=1;
        int SPEED=300;
        int tps_prec=0;
        int next=rand()%7;
        int n=rand()%7;
        int dx=0;
        bool space=false;
        bool rotate=false;
        bool hold=false;
        int held=-1;
        bool used=false;
        // a changer apres test
        bool gameover=false;

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

        void give_line(int mult){
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

        void Horizontal(){
            for(int i=0;i<4;i++){
                b[i]=a[i];
                a[i].x+=dx;
            }
            if(!check()){
                Cancel();
            }
        }

        void Cancel(){
            for(int i=0;i<4;i++){
                a[i]=b[i];
            }
        }

        void Tourner(){
            if(rotate){
                point p=a[1]; //Centre de rotation
                for (int i=0;i<4;i++){
                    b[i]=a[i];
                    int x=a[i].y-p.y;
                    int y=a[i].x-p.x;
                    a[i].x = p.x-x;
                    a[i].y = p.y+y;
                }
                if(!check()){
                    Cancel();
                }
            }
        }

        void Creer(){
            for(int i=0;i<4;i++){
                a[i].x = blocks[n].cases[i]%2 + N/2 -1;
                a[i].y = blocks[n].cases[i]/2 -1;
            }
        }

        void Instant(){
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
        }

        void Hold(){
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
                Creer();
                used=true;
            }
        }

        void Avancer(int tps_actuel){
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
                        if(b[i].y<1){
                            gameover=true;
                        }
                        Creer();
                    }
                }
            }
        }

        void Draw_field(SDL_Surface* tiles, SDL_Surface* window_surface, int offset){
            for(int i =0;i<M;i++){
                for(int j=0;j<N;j++){
                    if(field[i][j]!=0){
                        SDL_Rect PLACE = {(j+1+offset)*TILESIZE,(i+1)*TILESIZE,TILESIZE,TILESIZE};
                        SDL_BlitSurface(tiles,&tab[field[i][j]-1],window_surface,&PLACE);
                    }
                }
            }
        }


        int Check_lines(){
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

            return mult;
        }

        void New_Frame(int speed_ini){
            SPEED=speed_ini;
            dx=0;
            rotate=0;
            space=0;
            hold=0;
        }

        void Draw_Piece(SDL_Surface* tiles, SDL_Surface* window_surface, int offset){
            for(int i=0;i<4;i++){
                SDL_Rect POS = {(a[i].x+1+offset)*TILESIZE,(a[i].y)*TILESIZE,TILESIZE,TILESIZE};
                SDL_BlitSurface(tiles,&blocks[n].sprite,window_surface,&POS);
            }
        }

        

        void Draw_Next_Hold(SDL_Surface* tiles, SDL_Surface* window_surface, int offset){
            for(int i=0;i<4;i++){
                SDL_Rect NEXTPOS = {(offset+14+blocks[next].cases[i]%2)*TILESIZE,(4+blocks[next].cases[i]/2)*TILESIZE,TILESIZE,TILESIZE};
                SDL_BlitSurface(tiles,&blocks[next].sprite,window_surface,&NEXTPOS);
            }
            if(held!=-1){
                for(int i=0;i<4;i++){
                    SDL_Rect HELDPOS = {(offset+14+blocks[held].cases[i]%2)*TILESIZE,(13+blocks[held].cases[i]/2)*TILESIZE,TILESIZE,TILESIZE};
                    SDL_BlitSurface(tiles,&blocks[held].sprite,window_surface,&HELDPOS);
                }
            }
        };
};


void run() 
{
    
    if ( TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Window *window = SDL_CreateWindow("Tetris",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1152,736,0);
    bool menu_open = true;
    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    SDL_Surface *tiles = SDL_LoadBMP("Tiles.bmp");
    SDL_Surface *cadre = SDL_LoadBMP("cadre.bmp");
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    TTF_Font *police_nh = NULL, * police_score = NULL;
    SDL_Color couleurPolice = {168,168,168};
    SDL_Surface *text_menu = NULL, *text1J = NULL, *text2J = NULL, *text_vsAI = NULL, *text_quit = NULL;

    police_nh = TTF_OpenFont("SilverMedal.ttf", 42);
    text_menu = TTF_RenderText_Solid(police_nh, "MENU", couleurPolice);
    text1J = TTF_RenderText_Solid(police_nh,    "MODE 1 JOUEUR      1", couleurPolice);
    text2J = TTF_RenderText_Solid(police_nh,    "MODE 2 JOUEURS  2", couleurPolice);
    text_vsAI = TTF_RenderText_Solid(police_nh, "MODE VS IA                3", couleurPolice);
    text_quit = TTF_RenderText_Solid(police_nh, "QUIT                                  Q", couleurPolice);

    SDL_Rect positionMenu = {16*TILESIZE,1*TILESIZE,6*TILESIZE,6*TILESIZE};
    SDL_Rect position1J = {10*TILESIZE,7*TILESIZE,6*TILESIZE,6*TILESIZE};
    SDL_Rect position2J = {10*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
    SDL_Rect positionVSAI = {10*TILESIZE,13*TILESIZE,6*TILESIZE,6*TILESIZE};
    SDL_Rect positionQuit = {10*TILESIZE,16*TILESIZE,6*TILESIZE,6*TILESIZE};


    
    while(menu_open)
    {
        SDL_FillRect(window_surface, NULL, 0x000000);
        SDL_BlitSurface(text_menu, NULL, window_surface, &positionMenu);
        SDL_BlitSurface(text1J, NULL, window_surface, &position1J);
        SDL_BlitSurface(text2J, NULL, window_surface, &position2J);
        SDL_BlitSurface(text_vsAI, NULL, window_surface, &positionVSAI);
        SDL_BlitSurface(text_quit, NULL, window_surface, &positionQuit);
        
        SDL_Event event;

        bool actif = false, actif2 = false;

        while(SDL_PollEvent(&event) > 0)
        {
            //closing window
            switch(event.type){
                case SDL_QUIT:
                    menu_open = false;
                    actif = false;
                    actif2 = false;
                    break;
            }
            //Key presses
            if(event.type==SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_2){
                    actif = true;
                }
                else if(event.key.keysym.sym==SDLK_q){
                    menu_open = false;
                }
                else if(event.key.keysym.sym==SDLK_1){
                    actif2 = true;
                }
                break;
            }
        }
        while(actif2) {

            TTF_Font *police_solo = NULL;

            police_solo = TTF_OpenFont("SilverMedal.ttf", 25);

            SDL_Surface *text_nxt = NULL, *text_hld = NULL, *text_score = NULL, *text_lvl = NULL;
            SDL_Surface *text_l = NULL, *text_sc = NULL;

            text_nxt = TTF_RenderText_Solid(police_solo, "NEXT BLOCK", couleurPolice);
            text_hld = TTF_RenderText_Solid(police_solo, "HOLD", couleurPolice);

            text_score = TTF_RenderText_Solid(police_solo, "SCORE", couleurPolice);
            text_lvl = TTF_RenderText_Solid(police_solo, "LEVEL", couleurPolice);

            const char * sc;
            const char * l;

            //Quelques variables
            int tps_actuel=0;
            int speed_ini=300;
            bool start=true;
            Tetris T;
            
            bool keep_window_open = true;

            while(keep_window_open)
            {
                //Events
                SDL_Event e;
                while(SDL_PollEvent(&e) > 0)
                {
                    //closing window
                    switch(e.type){
                        case SDL_QUIT:
                            menu_open = false;
                            actif2 = false;
                            keep_window_open=false;
                            break;
                    }
                    //Key presses
                    if(e.type==SDL_KEYDOWN){
                        if(e.key.keysym.sym==SDLK_LEFT){
                            T.dx=-1;
                        }
                        else if(e.key.keysym.sym==SDLK_RIGHT){
                            T.dx=+1;
                        }
                        else if(e.key.keysym.sym==SDLK_UP){
                            T.rotate=true;
                        }
                        else if(e.key.keysym.sym==SDLK_DOWN){
                            T.SPEED=T.SPEED/10;
                        }
                        else if(e.key.keysym.sym==SDLK_SPACE){
                            T.space=true;
                        }
                        else if(e.key.keysym.sym==SDLK_c){
                            T.hold=true;
                        }
                    }
                }
                
                //Ini
                if(start){
                    T.Creer();
                }
                //Movement
                if(!T.gameover){
                    T.Horizontal();
                }
                //Rotation
                if(!T.gameover){
                    T.Tourner();
                }
                //Aterrissage
                if(!T.gameover){
                    T.Instant();
                }
                //Hold
                if(!T.gameover){
                    T.Hold();
                }
                //Avancée
                tps_actuel=SDL_GetTicks();
                if(!T.gameover){
                    T.Avancer(tps_actuel);
                }
                //Accélération
                if(SDL_GetTicks()%5000==0){
                    speed_ini+=-1;
                }
                //Check Lines
                if(!T.gameover){
                T.Check_lines();
                }
                //game over
                if(T.gameover){
                    bool go = true; 
                    while(go){
                        SDL_Surface * text_gameover = NULL, *text_tryAgain = NULL;
                        text_gameover = TTF_RenderText_Solid(police_nh, "GAME OVER", couleurPolice);
                        text_tryAgain = TTF_RenderText_Solid(police_nh, "TRY AGAIN                T", couleurPolice);

                        SDL_FillRect(window_surface, NULL, 0x000000);

                        SDL_Rect positionGO = {13*TILESIZE,4*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_gameover, NULL, window_surface, &positionGO);

                        SDL_Rect positionTryAgain = {10*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_tryAgain, NULL, window_surface, &positionTryAgain);

                        SDL_BlitSurface(text_quit, NULL, window_surface, &positionQuit);
                        
                        bool again = false;
                        bool game_close = false;
                        
                        while(SDL_PollEvent(&event) > 0)
                        {
                            
                            //closing window
                            switch(event.type){
                                case SDL_QUIT:
                                    go = false;
                                    exit(1);
                                    break;
                            }
                            //Key presses
                            if(event.type==SDL_KEYDOWN){
                                if(event.key.keysym.sym==SDLK_t){
                                    again = true;
                                }
                                else if(event.key.keysym.sym==SDLK_q){
                                    game_close = true;
                                    menu_open = false;
                                    actif2 = false;
                                    keep_window_open=false;
                                    break;
                                }
                                break;
                            }
                        }
                        if(again){ 
                            run();
                        }
                        if(game_close){
                            go = false;
                            exit(1);
                        }

                        SDL_UpdateWindowSurface(window);
                    }
                }
                //Fin
                start=false;
                if(!T.gameover){
                    T.New_Frame(speed_ini);
                }
                // Blit
                std::string S = std::to_string(T.score);
                sc = S.c_str();
                std::string L = std::to_string(T.level);
                l = L.c_str();

                text_sc = TTF_RenderText_Solid(police_solo, sc, couleurPolice);
                text_l = TTF_RenderText_Solid(police_solo, l, couleurPolice);

                SDL_FillRect(window_surface, NULL, 0x000000);

                SDL_Rect Joueur = {12*TILESIZE,0,12*TILESIZE,22*TILESIZE};
                SDL_BlitSurface(image, NULL, window_surface, &Joueur);
                SDL_Rect CADRE1 = {24*TILESIZE,3*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_Rect CADRE2 = {24*TILESIZE,12*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(cadre,NULL,window_surface,&CADRE1);
                SDL_BlitSurface(cadre,NULL,window_surface,&CADRE2);

                // pour écrire les informations sur le panneau d'affichage       
                // le next
                SDL_Rect position = {24*TILESIZE,1*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_nxt, NULL, window_surface, &position);
                // le hold
                SDL_Rect position2 = {26*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_hld, NULL, window_surface, &position2);
                //les affichages des mots score et lvl
                SDL_Rect positionScore = {24*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_score, NULL, window_surface, &positionScore);
                SDL_Rect positionLevel = {24*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_lvl, NULL, window_surface, &positionLevel);
                //les scores et lvl effectifs
                SDL_Rect positionSc = {28*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_sc, NULL, window_surface, &positionSc);
                SDL_Rect positionL = {28*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_l, NULL, window_surface, &positionL);
                // Draw
                T.Draw_field(tiles,window_surface,12);
                T.Draw_Piece(tiles,window_surface,12);
                T.Draw_Next_Hold(tiles,window_surface,12);
                SDL_UpdateWindowSurface(window);
            }

        }
        while(actif){

            SDL_Surface *text_next = NULL, *text_hold = NULL, *text_score1 = NULL, *text_score2 = NULL, *text_lvl1 = NULL, * text_lvl2 = NULL;
            SDL_Surface *text_l1 = NULL, *text_l2 = NULL, *text_sc1 = NULL, *text_sc2 = NULL;
            
            text_next = TTF_RenderText_Solid(police_nh, "NEXT BLOCK", couleurPolice);
            text_hold = TTF_RenderText_Solid(police_nh, "HOLD", couleurPolice);

            police_score = TTF_OpenFont("SilverMedal.ttf", 20);
            text_score1 = TTF_RenderText_Solid(police_score, "SCORE", couleurPolice);
            text_score2 = TTF_RenderText_Solid(police_score, "SCORE", couleurPolice);
            text_lvl1 = TTF_RenderText_Solid(police_score, "LEVEL", couleurPolice);
            text_lvl2 = TTF_RenderText_Solid(police_score, "LEVEL", couleurPolice);

            const char * sc1;
            const char * sc2;
            const char * l1;
            const char * l2;

            //Quelques variables
            int tps_actuel=0;
            int speed_ini=300;
            bool start=true;
            Tetris T1;
            Tetris T2;
            
            bool keep_window_open = true;

            while(keep_window_open)
            {
                std::string S1 = std::to_string(T1.score);
                sc1 = S1.c_str();
                std::string S2 = std::to_string(T2.score);
                sc2 = S2.c_str();
                std::string L1 = std::to_string(T1.level);
                l1 = L1.c_str();
                std::string L2 = std::to_string(T2.level);
                l2 = L2.c_str();

                text_sc1 = TTF_RenderText_Solid(police_score, sc1, couleurPolice);
                text_sc2 = TTF_RenderText_Solid(police_score, sc2, couleurPolice);
                text_l1 = TTF_RenderText_Solid(police_score, l1, couleurPolice);
                text_l2 = TTF_RenderText_Solid(police_score, l2, couleurPolice);

                SDL_FillRect(window_surface, NULL, 0x000000);
                //Events
                SDL_Event e;
                while(SDL_PollEvent(&e) > 0)
                {
                    //closing window
                    switch(e.type){
                        case SDL_QUIT:
                            menu_open = false;
                            actif = false;
                            keep_window_open=false;
                            break;
                    }
                    //Key presses
                    if(e.type==SDL_KEYDOWN){
                        if(e.key.keysym.sym==SDLK_q){ 
                            T1.dx=-1;
                        }
                        else if(e.key.keysym.sym==SDLK_d){ 
                            T1.dx=+1;
                        }
                        else if(e.key.keysym.sym==SDLK_z){ 
                            T1.rotate=true;
                        }
                        else if(e.key.keysym.sym==SDLK_s){ 
                            T1.SPEED=T1.SPEED/10;
                        }
                        else if(e.key.keysym.sym==SDLK_a){ 
                            T1.space=true;
                        }
                        else if(e.key.keysym.sym==SDLK_e){ 
                            T1.hold=true;
                        }
                        if(e.key.keysym.sym==SDLK_LEFT){ 
                            T2.dx=-1;
                        }
                        else if(e.key.keysym.sym==SDLK_RIGHT){
                            T2.dx=+1;
                        }
                        else if(e.key.keysym.sym==SDLK_UP){
                            T2.rotate=true;
                        }
                        else if(e.key.keysym.sym==SDLK_DOWN){
                            T2.SPEED=T2.SPEED/10;
                        }
                        else if(e.key.keysym.sym==SDLK_SPACE){
                            T2.space=true;
                        }
                        else if(e.key.keysym.sym==SDLK_q){
                            T2.hold=true;
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

                // pour écrire les informations sur le panneau d'affichage       
                //le hold
                SDL_Rect position = {13*TILESIZE,1*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_next, NULL, window_surface, &position);
                //le next
                SDL_Rect position2 = {16*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_hold, NULL, window_surface, &position2);
                //les affichages des mots score et lvl
                SDL_Rect positionScore1 = {13*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_score1, NULL, window_surface, &positionScore1);
                SDL_Rect positionScore2 = {19*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_score2, NULL, window_surface, &positionScore2);
                SDL_Rect positionLevel1 = {13*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_lvl1, NULL, window_surface, &positionLevel1);
                SDL_Rect positionLevel2 = {19*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_lvl2, NULL, window_surface, &positionLevel2);
                //les scores et lvl effectifs
                SDL_Rect positionSc1 = {16*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_sc1, NULL, window_surface, &positionSc1);
                SDL_Rect positionSc2 = {22*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_sc2, NULL, window_surface, &positionSc2);
                SDL_Rect positionL1 = {16*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_l1, NULL, window_surface, &positionL1);
                SDL_Rect positionL2 = {22*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_l2, NULL, window_surface, &positionL2);
                //Ini
                if(start){
                    T1.Creer();
                    T2.Creer();
                }
                //Movement
                if(!T1.gameover){
                    T1.Horizontal();
                }
                if(!T2.gameover){
                    T2.Horizontal();
                }
                //Rotation
                if(!T1.gameover){
                T1.Tourner();
                }
                if(!T2.gameover){
                T2.Tourner();
                }
                //Aterrissage
                if(!T1.gameover){
                T1.Instant();
                }
                if(!T2.gameover){
                T2.Instant();
                }
                //Hold
                if(!T1.gameover){
                T1.Hold();
                }
                if(!T2.gameover){
                T2.Hold();
                }
                //Avancée
                tps_actuel=SDL_GetTicks();
                if(!T1.gameover){
                T1.Avancer(tps_actuel);
                }
                tps_actuel=SDL_GetTicks();
                if(!T2.gameover){
                T2.Avancer(tps_actuel);
                }
                //Accélération
                if(SDL_GetTicks()%5000==0){
                    speed_ini+=-1;
                }
                //Check Lines
                if(!T1.gameover){
                T2.give_line(T1.Check_lines());
                }
                if(!T2.gameover){
                T1.give_line(T2.Check_lines());
                }
                //game over
                if(T1.gameover || T2.gameover){
                    bool go = true; 
                    while(go){
                        SDL_Surface * text_gameover = NULL, *text_tryAgain = NULL;
                        text_gameover = TTF_RenderText_Solid(police_nh, "GAME OVER", couleurPolice);
                        text_tryAgain = TTF_RenderText_Solid(police_nh, "TRY AGAIN                T", couleurPolice);

                        SDL_FillRect(window_surface, NULL, 0x000000);

                        SDL_Rect positionGO = {13*TILESIZE,4*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_gameover, NULL, window_surface, &positionGO);

                        SDL_Rect positionTryAgain = {10*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_tryAgain, NULL, window_surface, &positionTryAgain);

                        SDL_BlitSurface(text_quit, NULL, window_surface, &positionQuit);
                        
                        bool again = false;
                        bool game_close = false;
                        
                        while(SDL_PollEvent(&event) > 0)
                        {
                            
                            //closing window
                            switch(event.type){
                                case SDL_QUIT:
                                    go = false;
                                    exit(1);
                                    break;
                            }
                            //Key presses
                            if(event.type==SDL_KEYDOWN){
                                if(event.key.keysym.sym==SDLK_t){
                                    again = true;
                                }
                                else if(event.key.keysym.sym==SDLK_q){
                                    game_close = true;
                                    menu_open = false;
                                    actif2 = false;
                                    keep_window_open=false;
                                    break;
                                }
                                break;
                            }
                        }
                        if(again){ 
                            run();
                        }
                        if(game_close){
                            go = false;
                            exit(1);
                        }

                        SDL_UpdateWindowSurface(window);
                    }
                }
                //Fin
                start=false;
                if(!T1.gameover){
                T1.New_Frame(speed_ini);
                }
                if(!T2.gameover){
                T2.New_Frame(speed_ini);
                }
                //Draw
                T1.Draw_field(tiles,window_surface,0);
                T2.Draw_field(tiles,window_surface,24);
                T1.Draw_Piece(tiles,window_surface,0);
                T2.Draw_Piece(tiles,window_surface,24);
                T1.Draw_Next_Hold(tiles,window_surface,0);
                T2.Draw_Next_Hold(tiles,window_surface,6);
                SDL_UpdateWindowSurface(window);
            }
        }
        SDL_UpdateWindowSurface(window);
    }
}

int main(){
    run();
    return 0;
}