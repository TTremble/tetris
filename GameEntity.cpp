#include "GameEntity.h"

GameEntity::GameEntity(float x, float y) {

    mPos.x = x;
    mPos.y = y;

    mRotation = 0.0f;

    mParent = NULL;
}

GameEntity::~GameEntity() {

    mParent = NULL;

}

void GameEntity::Pos(point pos) {

    mPos = pos;
}

point GameEntity::Pos(SPACE space) {

    if (space == local || mParent == NULL) {
        return mPos;
    }
    return mParent->Pos(world); 

}

point GameEntity::Rotation(float a, float b[]) {
    point p= a[1]; //Centre de rotation
    for (int i=0;i<4;i++){
        b[i]=a[i];
        float x=a[i].y-p.y;
        float y=a[i].x-p.x;
        a[i].x = p.x-x;
        a[i].y = p.y+y;
    }
    return a;
}

void GameEntity::Active(bool active) {
    mActive = active;
}

bool GameEntity::Active() {
    return mActive;
}

void GameEntity::Parent(GameEntity* parent) {
    mPos = Pos(world) - parent->Pos(world);
    mParent = parent;
}

GameEntity* GameEntity::Parent() {
    return mParent;
}

void GameEntity::Update() {

}

void GameEntity::Render() {

}








