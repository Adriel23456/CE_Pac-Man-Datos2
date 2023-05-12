#include "Ghost.h"

Ghost::Ghost(Nodo* currentPosition): direction(0), currentPosition(currentPosition), reloadTime(8000){}   

Ghost::~Ghost() {
    // No hay nada que hacer aquí porque no hay asignacion de memoria dinamica...
}

void Ghost::setDirection(int newDirection){
    this->direction = newDirection;
}

int Ghost::getDirection(){
    return direction;
}

void Ghost::setCurrentPosition(Nodo* newCurrentPosition){
    this->currentPosition = newCurrentPosition;
}

Nodo* Ghost::getCurrentPosition(){
    return currentPosition;
}

int Ghost::getReloadTime() const{
    return reloadTime;
}