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


class Tetris {

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
        bool gameover=false;
        bool new_piece=true;

        /*************************************************************************
         Check est une fonction qui sert pour v??rifier si deux pi??ces se touchent 
        ou s'il y a une collision avec les murs ou le sol.
        
        @return true ou false en fonction de s'il y a collision ou non.                        
        *************************************************************************/
        bool check();

        /*************************************************************************
         Donne une ligne blanche ?? l'adversaire quand le joueur en complete une
         ligne

         @arg mult, donne le nombre de ligne ?? donner                     
        *************************************************************************/
        void give_line(int mult);

        /*************************************************************************
         D??placement horizontal de la pi??ce.                  
        *************************************************************************/
        void Horizontal();

        /*************************************************************************
         Permet de revenir ?? la position pr??c??dente lors de la v??rification d'un 
         d??placement.                    
        *************************************************************************/
        void Cancel();

        /*************************************************************************
         Fait tourner la pi??ce                  
        *************************************************************************/
        void Tourner();

        /*************************************************************************
         Permets de cr??er une nouvelle pi??ce dans le jeu                   
        *************************************************************************/
        void Creer();

        /*************************************************************************
         Pose la pi??ce instantan??ment quand on utilise la touche espace                   
        *************************************************************************/
        void Instant();

        /*************************************************************************
         Permets de mettre la pi??ce de c??t?? dans le jeu                  
        *************************************************************************/
        void Hold();

        /*************************************************************************
         Fait tourner la pi??ce                  
        *************************************************************************/
        void Avancer(int tps_actuel);

        /*************************************************************************
         Affiche le terrain, le plateau de jeu      

         @arg tiles,window_surface,offset pour l'affichage (la fenetre, l'image et
         un offsett pour la position initiale de la pi??ce)            
        *************************************************************************/
        void Draw_field(SDL_Surface* tiles, SDL_Surface* window_surface, int offset);

        /*************************************************************************
         Supprime une ligne complete et augmente le score

         @return le score de la ligne                  
        *************************************************************************/
        int Check_lines();

        /*************************************************************************
         Reset tous les commandes ?? z??o ?? la fin de la frame et accel??re le jeu

         @arg speed init qui va augmenter ensuite                  
        *************************************************************************/
        void New_Frame(int speed_ini);

        /*************************************************************************
         Affiche la pi??ce
        
         @arg comme pour Draw_Field               
        *************************************************************************/
        void Draw_Piece(SDL_Surface* tiles, SDL_Surface* window_surface, int offset);

        /*************************************************************************
         Affiche la prochaine pi??ce qui arrivera sur le jeu
         
         @arg comme pour Draw_Field            
        *************************************************************************/
        void Draw_Next_Hold(SDL_Surface* tiles, SDL_Surface* window_surface, int offset);


};


class AI {

    public:
        
        /*************************************************************************
         Compte le nombre de trous  sur le field
         
         @arg field poss??dant des trous         
        *************************************************************************/
        int compte_trous(int field[M][N]);

        /*************************************************************************
         Utile pour noter chaque configuration. Attribue un score au field.
         
         @arg field ?? ??valuer            
        *************************************************************************/
        int evaluer_jeu(int field[M][N]);

        /*************************************************************************
         Compare les field entre eux et garde le meilleur
         
         @arg n num??ro de la pi??ce
         @arg field, plateau ?? ??valuer            
        *************************************************************************/
        void evaluer_positions(int n, int field[M][N]);

        /*************************************************************************
         Fais l'action ??valu??e comme optimale
         
         @arg T le jeu          
        *************************************************************************/
        void Jouer(Tetris* T);

        /*************************************************************************
         Copie un plateau par dessus un autre
         
         @arg field1, field2 ?? copier            
        *************************************************************************/
        void copy(int field1[M][N],int field2[M][N]);

        /*************************************************************************
        Affiche l'etat du field sur la console. Cette fonction servait surtout
        pour d??bugguer l'IA
                 
         @arg field affich??            
        *************************************************************************/
        void show(int field[M][N]);

        int rotations=0;
        int mouvements=0;
        int etapes_rota=0;
        int etapes_move=0;
        bool new_piece=true;
        int trous_prec=0;
        bool hold=false;

};