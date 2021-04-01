#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<SDL2/SDL.h>

#include "tetris.h"

GameManager* GameManager::sInstance = NULL;

GameManager* GameManager::Instance() {
    
    if ( sInstance == NULL ){
        sInstance = new GameManager();
    }

    return sInstance;

}

void GameManager::Release() {

    delete sInstance;
    sInstance = NULL;
}

GameManager::GameManager() {

    mQuit = false;
    mGraphics = Graphics::Instance();
    if(!Graphics::Initialized())
        mQuit = true;

    //mInputMngr = InputManager::Instance();

    mTimer = Timer::Instance();

    mParent = new GameEntity(100.0f, 400.0f); 
    mChild = new GameEntity(100.0f, 500.0f); 

    printf("Child local pos: (%F, %F)\n", mChild->Pos(GameEntity::local).x, mChild->Pos(GameEntity::local).y);

    mChild->Parent(mParent);

    printf("Child local pos: (%F, %F)\n", mChild->Pos(GameEntity::local).x, mChild->Pos(GameEntity::local).y);

}

GameManager::~GameManager() {

    Graphics::Release();
    mGraphics = NULL;

    Timer::Release();
    mTimer = NULL;

    //InputManager::Release();
    //mInputMngr = NULL;
}

void GameManager::Run() {

    while(!mQuit) {

        mTimer->Update();

        while(SDL_PollEvent(&mEvents) > 0){
            if(mEvents.type == SDL_QUIT){
                mQuit = true;
            }
        }

        if ( mTimer->DeltaTime() >= (1.0f / FRAME_RATE) ) {

            //mInputMngr->Update();

            printf("DeltaTime: %F\n", mTimer->DeltaTime());

            mTimer->Reset();
        
        }
    }
}


