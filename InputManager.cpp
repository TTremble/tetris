#include "InputManager.h"

InputManager* InputManager::sInstance = NULL;

InputManager* InputManager::Instance() {

    if ( sInstance == NULL) {
        sIntance = new InputManager();
    }

    return sInstance;
}

void InputManager::Release() {

    delete sInstance;
    sInstance = NULL;
}

InputManager::InputManager() {

}

InputManager::~InputManager() {

}

bool InputManager::KeyDown(SDL_Scancode scancode) {
    return mKeyboardStates[scancode];
}

void InputManager::Update(){

    mKeyboardStates =SDL_GetKeyboardState(NULL);
}