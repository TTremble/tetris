#ifndef _GAMEENTITY_H
#define _GAMEENTITY_H

#include <SDL2/SDL.h>

#include "tricks.h"

class GameEntity {

    public:

        enum SPACE {local = 0, world = 1}; 

        GameEntity(float x=0.0f, float y=0.0f);
        ~GameEntity();
        
        void Pos(point pos);
        point Pos(SPACE space = world);

        point Rotation(float a[], float b[]);

        void Active(bool active);
        bool Active();

        void Parent(GameEntity* parent);
        GameEntity* Parent();

        virtual void Update();
        virtual void Render();

    private:
        
        point mPos;
        float mRotation;

        bool mActive;
        GameEntity* mParent;

};

#endif