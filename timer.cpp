#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<SDL2/SDL.h>

#include "timer.h"


/*************************************** TIMER ***************************************/

Timer* Timer::Temps = NULL;

Timer* Timer::sTemps {
    
    if ( Temps == NULL ) {
        Temps = new Timer();
    }

    return Temps;
}

void Timer::Release() {

    delete Temps;
    sTemps = NULL;

}

Timer::Timer() {

}

Timer::~Timer() {

    Reset();
    EchelleDeTemps = 1.0f;
}

void Timer::Reset() {

    HeureDebut = SDL_GetTicks(); // retourne le temps en millisecondes
    TempsEcoule = 0.0;
    TempsDelta = 0.0f;
}

float Timer::sTempsDelta() {

    return TempsDelta;

}

float Timer::sTempsEcoule(float t) {

    TempsEcoule = t ;

}

void Timer::sTempsEcoule() {

    return TempsEcoule;

}

void Timer::Update() {

    TempsEcoule = SDL_GetTicks() - HeureDebut;
    TempsDelta = TempsEcoule * 0.001f;

}