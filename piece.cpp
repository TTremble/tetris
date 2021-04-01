#include "piece.h"

Piece* Piece::Instance = NULL;

Piece* Piece::Instance() {
    
    if ( sInstance == NULL ){
        sInstance = new Piece();
    }

    return sInstance;

}

void Piece::Release() {

    delete sInstance;
    sInstance = NULL;
}

Piece::Piece() {

    mGraphics = Graphics::Instance();
    if(!Graphics::Initialized())
        mQuit = true;
}