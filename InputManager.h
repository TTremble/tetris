#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include <SDL2/SDL.h>

class InputManager {

    public:

        static InputManager Instance();
        static void Release();

        bool KeyDown(SDL_Scancode scancode);

        void Update();

    private :

        static InputManager* sInstance;
        const Uint8* mKeyboardStates;

        InputManager();
        ~InputManager();

};

#endif