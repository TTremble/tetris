#include "graphic.h"

Graphics* Graphics::sInstance = NULL;
bool Graphics::sInitialized = false;

Graphics* Graphics::Instance() {

    if(sInstance == NULL) {
        sInstance = new Graphics();
    }
    return sInstance;
}

void Graphics::Release() {
    
    delete sInstance;
    sInstance = NULL;

    sInitialized = false;
}

bool Graphics::Initialized() {

    return sInitialized;
}

Graphics::Graphics() {

    sInitialized = Init();

}

Graphics::~Graphics() {

    SDL_DestroyWindow(mWindow);
    mWindow = NULL;
	SDL_Quit();

}

bool Graphics::Init() {

    if ( SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Window *mWindow=SDL_CreateWindow(WINDOW_TITLE,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,0);
    SDL_Surface *tiles = SDL_LoadBMP("Tiles.bmp");
    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    if(!image || !tiles) {
        printf("Image Load Error: %s\n", SDL_GetError());
    }
    SDL_Surface *window_surface = SDL_GetWindowSurface(mWindow);

    if(mWindow == NULL){
        printf("Window Creation Error: %s\n", SDL_GetError());
        return false;
    }

    Graphics::ShowGame(image, window_surface);

    SDL_UpdateWindowSurface(mWindow);

    return true;
}

void Graphics::ShowGame(SDL_Surface* image, SDL_Surface* window_surface){

    SDL_BlitSurface(image, NULL, window_surface, NULL);

}

