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
/*
    SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;
*/
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


/*    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);


    if(mRenderer == NULL) {

        printf("Renderer Creation Error: %s\n", SDL_GetError());
        return false;
    }
*/

    return true;
}

void Graphics::ShowGame(SDL_Surface* image, SDL_Surface* window_surface){

    SDL_BlitSurface(image, NULL, window_surface, NULL);

}

/*
SDL_Texture* Graphics::LoadTexture(std::string path) {

    SDL_Texture* tex = NULL;

    SDL_Surface* surface = IMG_Load(path.c_str());
    if(surface == NULL) {

        printf("Image Load Error: Path(%s) - Error(%s)\n", path.c_str(), IMG_GetError());
        return tex;
    }

    tex = SDL_CreateTextureFromSurface(mRenderer, surface);
    if(tex == NULL) {

        printf("Create Texture Error: %s\n", SDL_GetError());
        return tex;
    }

    SDL_FreeSurface(surface);

    return tex;
}

SDL_Texture* Graphics::CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color) {

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if(surface == NULL) {

        printf("Text Render Error: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surface);
    if(tex == NULL) {

        printf("Text Texture Creation Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_FreeSurface(surface);

    return tex;
}


void Graphics::ClearBackBuffer() {

    SDL_RenderClear(mRenderer);
}

void Graphics::DrawTexture(SDL_Texture* tex, SDL_Rect* clip, SDL_Rect* rend, float angle, SDL_RendererFlip flip) {

    SDL_RenderCopyEx(mRenderer, tex, clip, rend, angle, NULL, flip);
}

void Graphics::Render() {

    SDL_RenderPresent(mRenderer);
}

*/

















