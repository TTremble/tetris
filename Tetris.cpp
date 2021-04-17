/*************************************************************************/
/*                                                                       */
/*                           TETRIS IN A NUTCHELL                        */
/*                                                                       */
/*************************************************************************/


#include "Tetris.h"

// Methode du Tetris

bool Tetris::check(){
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

void Tetris::give_line(int mult){
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

void Tetris::Horizontal(){
    for(int i=0;i<4;i++){
        b[i]=a[i];
        a[i].x+=dx;
    }
    if(!check()){
        Cancel();
    }
}

void Tetris::Cancel(){
    for(int i=0;i<4;i++){
        a[i]=b[i];
    }
}

void Tetris::Tourner(){
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

void Tetris::Creer(){
    for(int i=0;i<4;i++){
        if(n==1){
            a[i].x = blocks[n].cases[i]%2 + N/2 -1;
            a[i].y = blocks[n].cases[i]/2 + 1;
        }
        else{
            a[i].x = blocks[n].cases[i]%2 + N/2 -1;
            a[i].y = blocks[n].cases[i]/2;
        }
        if(field[a[i].y][a[i].x]!=0){
            gameover=true;
        }
    }
    new_piece=true;
}

void Tetris::Instant(){
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
            field[a[i].y+diff-1][a[i].x]=blocks[n].couleur;
        }
        n=next;
        next=rand()%7;
        used=false;
        Creer();
    }
}

void Tetris::Hold(){
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

void Tetris::Avancer(int tps_actuel){
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
                Creer();
            }
        }
    }
}

void Tetris::Draw_field(SDL_Surface* tiles, SDL_Surface* window_surface, int offset){
    for(int i =0;i<M;i++){
        for(int j=0;j<N;j++){
            if(field[i][j]!=0){
                SDL_Rect PLACE = {(j+1+offset)*TILESIZE,(i+1)*TILESIZE,TILESIZE,TILESIZE};
                SDL_BlitSurface(tiles,&tab[field[i][j]-1],window_surface,&PLACE);
            }
        }
    }
}


int Tetris::Check_lines(){
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

void Tetris::New_Frame(int speed_ini){
    SPEED=speed_ini;
    dx=0;
    rotate=0;
    space=0;
    hold=0;
}

void Tetris::Draw_Piece(SDL_Surface* tiles, SDL_Surface* window_surface, int offset){
    for(int i=0;i<4;i++){
        SDL_Rect POS = {(a[i].x+1+offset)*TILESIZE,(a[i].y)*TILESIZE,TILESIZE,TILESIZE};
        SDL_BlitSurface(tiles,&blocks[n].sprite,window_surface,&POS);
    }
}

void Tetris::Draw_Next_Hold(SDL_Surface* tiles, SDL_Surface* window_surface, int offset){
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
}

// Methode de l'IA

int AI::compte_trous(int field[M][N]){
    int ntrous=0;
    for(int j=0;j<N;j++){
        int trous=0;
        bool sous=false;
        for(int i=0;i<M;i++){
            if(field[i][j]!=0){
                sous=true;
            }
            if(field[i][j]==0&&sous==true){
                sous=false;
                trous++;
            }
        }
        ntrous+=trous;
    }
    return ntrous;
}

int AI::evaluer_jeu(int field[M][N]){
    int hauteur=M;
    int hauteurcolumns[N];
    std::fill_n(hauteurcolumns,N,M);
    int score=0;
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            if(field[i][j]!=0&&hauteur==M){
                hauteur=M-i;
            }
            if(field[i][j]!=0&&hauteurcolumns[j]==M){
                hauteurcolumns[j]=hauteur-(M-i);
            }
            if(i==M-1&&hauteurcolumns[j]==M){
                hauteurcolumns[j]=hauteur;
            }
        }
    }
    score = pow(8*hauteur,2) + 1000*compte_trous(field);
    for(int j=0;j<N;j++){
        score= score + pow(hauteurcolumns[j],2);
    }
    return score;
    
}

void AI::evaluer_positions(int n, int field[M][N]){
    trous_prec=compte_trous(field);
    int best=100000;
    int trous_ac=trous_prec;
    for(int r=0;r<5;r++){
        for(int p=5;p>-1;p+=-1){
            Tetris Brain;
            copy(Brain.field,field);
            Brain.n=n;
            Brain.Creer();
            Brain.space=true;
            Brain.dx=-1;
            Brain.rotate=true;
            for (int t=0;t<r;t++){
                Brain.Tourner();
            }
            for (int d=0;d<p;d++){
                Brain.Horizontal();
            }
            Brain.Instant();
            Brain.Check_lines();
            int e=evaluer_jeu(Brain.field);
            if(e<best){
                best=e;
                trous_ac=compte_trous(Brain.field);
                if(trous_ac>trous_prec){
                    hold=true;
                }
                else{
                    hold=false;
                }
                rotations=r;
                mouvements=-1*p;
            }
            
        }
        for(int p=5;p>-1;p+=-1){
            Tetris Brain;
            copy(Brain.field,field);
            Brain.n=n;
            Brain.Creer();
            Brain.space=true;
            Brain.dx=+1;
            Brain.rotate=true;
            for (int t=0;t<r;t++){
                Brain.Tourner();
            }
            for (int d=0;d<p;d++){
                Brain.Horizontal();
            }
            Brain.Instant();
            Brain.Check_lines();
            int e=evaluer_jeu(Brain.field);
            if(e<best){
                best=e;
                trous_ac=compte_trous(Brain.field);
                if(trous_ac>trous_prec){
                    hold=true;
                }
                else{
                    hold=false;
                }
                rotations=r;
                mouvements=p;
            }
        }
    }
}

void AI::Jouer(Tetris* T){
    if(T->new_piece){
        evaluer_positions(T->n,T->field);
        T->new_piece=false;
    }
    else{
        if(hold==true&&T->used==false){
            T->hold=true;
            new_piece=true;
            hold=false;
            
        }
        else{
            if(etapes_rota<rotations){
                T->rotate=true;
                etapes_rota++;
            }
            if(etapes_move<abs(mouvements)){
                if(mouvements>0){
                    T->dx=+1;
                }
                if(mouvements<0){
                    T->dx=-1;
                }
                etapes_move++;
            }
            if(etapes_rota==rotations&&etapes_move==abs(mouvements)){
                T->space=true;
                rotations=0;
                mouvements=0;
                etapes_rota=0;
                etapes_move=0;
            }
        }
    }
}

void AI::copy(int field1[M][N],int field2[M][N]){
    for(int i=0;i<M;i++){
        for(int j=0;j<M;j++){
            field1[i][j]=field2[i][j];
        }
    }
}

void AI::show(int field[M][N]){
    for (int i = 0; i < M; i++) 
    {
        for (int j = 0; j < N; j++)
        {
            std::cout << field[i][j] << " ";
        }            
        // Newline for new row
        std::cout << std::endl;
        std::cout << "===========================" << std::endl;
    }
}

//fonction de lancement du jeu

/*************************************************************************
    Affiche la prochaine pièce qui arrivera sur le jeu
    
    @arg comme pour Draw_Field            
*************************************************************************/

void run(SDL_Window *window, SDL_Surface *window_surface, SDL_Surface *image, SDL_Surface *tiles, SDL_Surface *cadre ) 
{
    
    if ( TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    bool menu_open = true;

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

        bool actif = false, actif2 = false, actif3 = false;

        while(SDL_PollEvent(&event) > 0)
        {
            //closing window
            switch(event.type){
                case SDL_QUIT:
                    SDL_FreeSurface(text_menu);SDL_FreeSurface(text1J);SDL_FreeSurface(text2J);SDL_FreeSurface(text_vsAI);SDL_FreeSurface(text_quit);
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
                    SDL_FreeSurface(text_menu);SDL_FreeSurface(text1J);SDL_FreeSurface(text2J);SDL_FreeSurface(text_vsAI);SDL_FreeSurface(text_quit);
                    menu_open = false;
                    exit(1);
                }
                else if(event.key.keysym.sym==SDLK_1){
                    actif2 = true;
                }
                else if(event.key.keysym.sym==SDLK_3){
                    actif3 = true;
                }
                break;
            }
        }
        //1 JOUEUR
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
                            SDL_FreeSurface(text_menu);SDL_FreeSurface(text1J);SDL_FreeSurface(text2J);SDL_FreeSurface(text_vsAI);SDL_FreeSurface(text_quit);
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
                        
                        SDL_Surface *text_gameover = NULL, *text_tryAgain = NULL, *text_scoreGO = NULL, *text_levelGO = NULL;
                        SDL_Surface *scGO = NULL, *lvlGO = NULL;

                        std::string S = std::to_string(T.score);
                        sc = S.c_str();
                        std::string L = std::to_string(T.level);
                        l = L.c_str();

                        lvlGO = TTF_RenderText_Solid(police_nh, sc, couleurPolice);
                        scGO = TTF_RenderText_Solid(police_nh, l, couleurPolice);

                        text_scoreGO = TTF_RenderText_Solid(police_nh, "SCORE ", couleurPolice);
                        text_levelGO = TTF_RenderText_Solid(police_nh, "LEVEL ", couleurPolice);
                        text_gameover = TTF_RenderText_Solid(police_nh, "GAME OVER", couleurPolice);
                        text_tryAgain = TTF_RenderText_Solid(police_nh, "TRY AGAIN                T", couleurPolice);

                        SDL_FillRect(window_surface, NULL, 0x000000);

                        SDL_Rect positionGO = {13*TILESIZE,4*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_gameover, NULL, window_surface, &positionGO);

                        SDL_Rect positionTryAgain = {10*TILESIZE,12*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_tryAgain, NULL, window_surface, &positionTryAgain);

                        SDL_Rect positionScoreGO = {10*TILESIZE,8*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_scoreGO, NULL, window_surface, &positionScoreGO);

                        SDL_Rect positionLevelGO = {10*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_levelGO, NULL, window_surface, &positionLevelGO);
                        
                        SDL_Rect positionScGO = {24*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(scGO, NULL, window_surface, &positionScGO);
                        
                        SDL_Rect positionLvlGO = {24*TILESIZE,8*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(lvlGO, NULL, window_surface, &positionLvlGO);

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
                                    SDL_FreeSurface(text_menu);SDL_FreeSurface(text1J);SDL_FreeSurface(text2J);SDL_FreeSurface(text_vsAI);SDL_FreeSurface(text_quit);
                                    menu_open = false;
                                    actif2 = false;
                                    keep_window_open=false;
                                    break;
                                }
                                break;
                            }
                        }
                        if(again){ 
                            SDL_FreeSurface(scGO);SDL_FreeSurface(lvlGO);SDL_FreeSurface(text_gameover);SDL_FreeSurface(text_tryAgain);SDL_FreeSurface(text_scoreGO);SDL_FreeSurface(text_levelGO);
                            run(window, window_surface, image, tiles, cadre);
                            exit(1);
                        }
                        if(game_close){
                            SDL_FreeSurface(scGO);SDL_FreeSurface(lvlGO);SDL_FreeSurface(text_gameover);SDL_FreeSurface(text_tryAgain);SDL_FreeSurface(text_scoreGO);SDL_FreeSurface(text_levelGO);
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
                            SDL_FreeSurface(text_menu);SDL_FreeSurface(text1J);SDL_FreeSurface(text2J);SDL_FreeSurface(text_vsAI);SDL_FreeSurface(text_quit);
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
                        else if(e.key.keysym.sym==SDLK_c){
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
                SDL_Rect positionScore1 = {12*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_score1, NULL, window_surface, &positionScore1);
                SDL_Rect positionScore2 = {18*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_score2, NULL, window_surface, &positionScore2);
                SDL_Rect positionLevel1 = {12*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_lvl1, NULL, window_surface, &positionLevel1);
                SDL_Rect positionLevel2 = {18*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_lvl2, NULL, window_surface, &positionLevel2);
                //les scores et lvl effectifs
                SDL_Rect positionSc1 = {15*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_sc1, NULL, window_surface, &positionSc1);
                SDL_Rect positionSc2 = {21*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_sc2, NULL, window_surface, &positionSc2);
                SDL_Rect positionL1 = {15*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_l1, NULL, window_surface, &positionL1);
                SDL_Rect positionL2 = {21*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
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
                if(T1.gameover && T2.gameover){
                    SDL_FreeSurface(text_next);SDL_FreeSurface(text_hold);SDL_FreeSurface(text_score1);SDL_FreeSurface(text_score2);SDL_FreeSurface(text_lvl1);SDL_FreeSurface(text_lvl2);SDL_FreeSurface(text_l1);SDL_FreeSurface(text_l2);SDL_FreeSurface(text_sc1);SDL_FreeSurface(text_sc2);
                    bool go = true; 
                    while(go){

                        SDL_Surface *scGO1 = NULL, *scGO2 = NULL, *lvlGO1 = NULL,  *lvlGO2 = NULL;

                        std::string S1 = std::to_string(T1.score);
                        sc1 = S1.c_str();
                        std::string S2 = std::to_string(T2.score);
                        sc2 = S2.c_str();
                        std::string L1 = std::to_string(T1.level);
                        l1 = L1.c_str();
                        std::string L2 = std::to_string(T2.level);
                        l2 = L2.c_str();

                        scGO1 = TTF_RenderText_Solid(police_nh, sc1, couleurPolice);
                        scGO2 = TTF_RenderText_Solid(police_nh, sc2, couleurPolice);
                        lvlGO1 = TTF_RenderText_Solid(police_nh, l1, couleurPolice);
                        lvlGO2 = TTF_RenderText_Solid(police_nh, l2, couleurPolice);

                        SDL_Surface * text_gameover = NULL, *text_tryAgain = NULL, *text_scoreGO = NULL, *text_levelGO = NULL, *text_joueurs = NULL;
                    
                        text_joueurs = TTF_RenderText_Solid(police_score, "PLAYER 1               PLAYER 2", couleurPolice);
                        text_gameover = TTF_RenderText_Solid(police_nh, "GAME END", couleurPolice);
                        text_tryAgain = TTF_RenderText_Solid(police_nh, "TRY AGAIN                T", couleurPolice);
                        text_scoreGO = TTF_RenderText_Solid(police_nh, "SCORE ", couleurPolice);
                        text_levelGO = TTF_RenderText_Solid(police_nh, "LEVEL ", couleurPolice);

                        SDL_FillRect(window_surface, NULL, 0x000000);

                        SDL_Rect positionGO = {14*TILESIZE,4*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_gameover, NULL, window_surface, &positionGO);

                        SDL_Rect positionTryAgain = {10*TILESIZE,12*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_tryAgain, NULL, window_surface, &positionTryAgain);

                        SDL_Rect positionJoueurs = {22*TILESIZE,6*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_joueurs, NULL, window_surface, &positionJoueurs);

                        SDL_Rect positionScoreGO = {10*TILESIZE,8*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_scoreGO, NULL, window_surface, &positionScoreGO);

                        SDL_Rect positionLevelGO = {10*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_levelGO, NULL, window_surface, &positionLevelGO);
                        
                        SDL_Rect positionScGO1 = {20*TILESIZE,8*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(scGO1, NULL, window_surface, &positionScGO1);
                        
                        SDL_Rect positionScGO2 = {26*TILESIZE,8*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(scGO2, NULL, window_surface, &positionScGO2);

                        SDL_Rect positionLvlGO1 = {20*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(lvlGO1, NULL, window_surface, &positionLvlGO1);

                        SDL_Rect positionLvlGO2 = {26*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(lvlGO2, NULL, window_surface, &positionLvlGO2);

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
                                    SDL_FreeSurface(text_menu);SDL_FreeSurface(text1J);SDL_FreeSurface(text2J);SDL_FreeSurface(text_vsAI);SDL_FreeSurface(text_quit);
                                    menu_open = false;
                                    actif2 = false;
                                    keep_window_open=false;
                                    break;
                                }
                                break;
                            }
                        }
                        if(again){ 
                            run(window, window_surface, image, tiles, cadre);
                            SDL_FreeSurface(scGO1);SDL_FreeSurface(scGO2);SDL_FreeSurface(lvlGO1);SDL_FreeSurface(lvlGO2);SDL_FreeSurface(text_gameover);SDL_FreeSurface(text_tryAgain);SDL_FreeSurface(text_scoreGO);SDL_FreeSurface(text_levelGO);SDL_FreeSurface(text_joueurs);
                            exit(1);
                        }
                        if(game_close){
                            go = false;
                            SDL_FreeSurface(scGO1);SDL_FreeSurface(scGO2);SDL_FreeSurface(lvlGO1);SDL_FreeSurface(lvlGO2);SDL_FreeSurface(text_gameover);SDL_FreeSurface(text_tryAgain);SDL_FreeSurface(text_scoreGO);SDL_FreeSurface(text_levelGO);SDL_FreeSurface(text_joueurs);
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

        // vs IA
        while(actif3){
            
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
            //variables
            int tps_actuel=0;
            int speed_ini=500;
            bool start=true;
            AI ai;
            Tetris T1;
            Tetris T2;
            int tps2=0;

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
                            SDL_FreeSurface(text_menu);SDL_FreeSurface(text1J);SDL_FreeSurface(text2J);SDL_FreeSurface(text_vsAI);SDL_FreeSurface(text_quit);
                            menu_open = false;
                            actif3 = false;
                            keep_window_open=false;
                            break;
                    }
                    //Key presses
                    if(e.type==SDL_KEYDOWN){
                        if(e.key.keysym.sym==SDLK_LEFT){
                            T1.dx=-1;
                        }
                        else if(e.key.keysym.sym==SDLK_RIGHT){
                            T1.dx=+1;
                        }
                        else if(e.key.keysym.sym==SDLK_UP){
                            T1.rotate=true;
                        }
                        else if(e.key.keysym.sym==SDLK_DOWN){
                            T1.SPEED=T1.SPEED/10;
                        }
                        else if(e.key.keysym.sym==SDLK_SPACE){
                            T1.space=true;
                        }
                        else if(e.key.keysym.sym==SDLK_c){
                            T1.hold=true;
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
                SDL_Rect positionScore1 = {12*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_score1, NULL, window_surface, &positionScore1);
                SDL_Rect positionScore2 = {18*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_score2, NULL, window_surface, &positionScore2);
                SDL_Rect positionLevel1 = {12*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_lvl1, NULL, window_surface, &positionLevel1);
                SDL_Rect positionLevel2 = {18*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_lvl2, NULL, window_surface, &positionLevel2);
                //les scores et lvl effectifs
                SDL_Rect positionSc1 = {15*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_sc1, NULL, window_surface, &positionSc1);
                SDL_Rect positionSc2 = {21*TILESIZE,19*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_sc2, NULL, window_surface, &positionSc2);
                SDL_Rect positionL1 = {15*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_l1, NULL, window_surface, &positionL1);
                SDL_Rect positionL2 = {21*TILESIZE,20*TILESIZE,6*TILESIZE,6*TILESIZE};
                SDL_BlitSurface(text_l2, NULL, window_surface, &positionL2);

                //Ini
                if(start){
                    T1.Creer();
                    T2.Creer();
                }
                //Decision
                int tps=SDL_GetTicks();
                if(tps-tps2>100){
                    ai.Jouer(&T2);
                    tps2=tps;
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
                if(T1.gameover){
                    SDL_FreeSurface(text_next);SDL_FreeSurface(text_hold);SDL_FreeSurface(text_score1);SDL_FreeSurface(text_score2);SDL_FreeSurface(text_lvl1);SDL_FreeSurface(text_lvl2);SDL_FreeSurface(text_l1);SDL_FreeSurface(text_l2);SDL_FreeSurface(text_sc1);SDL_FreeSurface(text_sc2);
                    bool go = true; 
                    while(go){

                        SDL_Surface *scGO1 = NULL, *scGO2 = NULL, *lvlGO1 = NULL,  *lvlGO2 = NULL;

                        std::string S1 = std::to_string(T1.score);
                        sc1 = S1.c_str();
                        std::string S2 = std::to_string(T2.score);
                        sc2 = S2.c_str();
                        std::string L1 = std::to_string(T1.level);
                        l1 = L1.c_str();
                        std::string L2 = std::to_string(T2.level);
                        l2 = L2.c_str();

                        scGO1 = TTF_RenderText_Solid(police_nh, sc1, couleurPolice);
                        scGO2 = TTF_RenderText_Solid(police_nh, sc2, couleurPolice);
                        lvlGO1 = TTF_RenderText_Solid(police_nh, l1, couleurPolice);
                        lvlGO2 = TTF_RenderText_Solid(police_nh, l2, couleurPolice);

                        SDL_Surface * text_gameover = NULL, *text_tryAgain = NULL, *text_scoreGO = NULL, *text_levelGO = NULL, *text_joueurs = NULL;
                    
                        text_joueurs = TTF_RenderText_Solid(police_score, "PLAYER 1               PLAYER 2", couleurPolice);
                        text_gameover = TTF_RenderText_Solid(police_nh, "GAME END", couleurPolice);
                        text_tryAgain = TTF_RenderText_Solid(police_nh, "TRY AGAIN                T", couleurPolice);
                        text_scoreGO = TTF_RenderText_Solid(police_nh, "SCORE ", couleurPolice);
                        text_levelGO = TTF_RenderText_Solid(police_nh, "LEVEL ", couleurPolice);

                        SDL_FillRect(window_surface, NULL, 0x000000);

                        SDL_Rect positionGO = {14*TILESIZE,4*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_gameover, NULL, window_surface, &positionGO);

                        SDL_Rect positionTryAgain = {10*TILESIZE,12*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_tryAgain, NULL, window_surface, &positionTryAgain);

                        SDL_Rect positionJoueurs = {22*TILESIZE,6*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_joueurs, NULL, window_surface, &positionJoueurs);

                        SDL_Rect positionScoreGO = {10*TILESIZE,8*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_scoreGO, NULL, window_surface, &positionScoreGO);

                        SDL_Rect positionLevelGO = {10*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(text_levelGO, NULL, window_surface, &positionLevelGO);
                        
                        SDL_Rect positionScGO1 = {20*TILESIZE,8*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(scGO1, NULL, window_surface, &positionScGO1);
                        
                        SDL_Rect positionScGO2 = {26*TILESIZE,8*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(scGO2, NULL, window_surface, &positionScGO2);

                        SDL_Rect positionLvlGO1 = {20*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(lvlGO1, NULL, window_surface, &positionLvlGO1);

                        SDL_Rect positionLvlGO2 = {26*TILESIZE,10*TILESIZE,6*TILESIZE,6*TILESIZE};
                        SDL_BlitSurface(lvlGO2, NULL, window_surface, &positionLvlGO2);

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
                                    SDL_FreeSurface(text_menu);SDL_FreeSurface(text1J);SDL_FreeSurface(text2J);SDL_FreeSurface(text_vsAI);SDL_FreeSurface(text_quit);
                                    menu_open = false;
                                    actif2 = false;
                                    keep_window_open=false;
                                    break;
                                }
                                break;
                            }
                        }
                        if(again){ 
                            run(window, window_surface, image, tiles, cadre);
                            SDL_FreeSurface(scGO1);SDL_FreeSurface(scGO2);SDL_FreeSurface(lvlGO1);SDL_FreeSurface(lvlGO2);SDL_FreeSurface(text_gameover);SDL_FreeSurface(text_tryAgain);SDL_FreeSurface(text_scoreGO);SDL_FreeSurface(text_levelGO);SDL_FreeSurface(text_joueurs);
                            exit(1);
                        }
                        if(game_close){
                            go = false;
                            SDL_FreeSurface(scGO1);SDL_FreeSurface(scGO2);SDL_FreeSurface(lvlGO1);SDL_FreeSurface(lvlGO2);SDL_FreeSurface(text_gameover);SDL_FreeSurface(text_tryAgain);SDL_FreeSurface(text_scoreGO);SDL_FreeSurface(text_levelGO);SDL_FreeSurface(text_joueurs);
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

    SDL_Window *window = SDL_CreateWindow("Tetris",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1152,736,0);
    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    SDL_Surface *tiles = SDL_LoadBMP("Tiles.bmp");
    SDL_Surface *cadre = SDL_LoadBMP("cadre.bmp");
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    run(window, window_surface, image, tiles, cadre);
    SDL_FreeSurface(image);
    SDL_FreeSurface(tiles);
    SDL_FreeSurface(cadre);
    SDL_FreeSurface(window_surface);
    return 0;
}
