#include "Pacman.h"

#include <QKeyEvent>

Pacman::Pacman() : lives(3), direction(0), currentPosition(new Nodo()), reloadTime(3000) {
}

Pacman::Pacman(Nodo* currentPosition): lives(3), direction(0), currentPosition(currentPosition), reloadTime(3000){
}
    

Pacman::~Pacman() {
    // No hay nada que hacer aquí porque no hay asignacion de memoria dinamica...
}

int Pacman::getLives(){
    return lives;
}

void Pacman::loseLife() {
    lives--;
}

bool Pacman::canMove(Nodo* newNode) {
    if (newNode->getType()==1) {
        return false;
    } else{
        return true;
    }
}

void Pacman::keyPressEvent(QKeyEvent* event) {
    int valor = 0;
    switch (event->key()) {
        case Qt::Key_W:
            valor = 2;
            break;
        case Qt::Key_A:
            valor = 1;
            break;
        case Qt::Key_S:
            valor = 3;
            break;
        case Qt::Key_D:
            valor = 4;
            break;
    }
    setDirection(valor);
}

void Pacman::setDirection(int newDirection){
    this->direction = newDirection;
}

int Pacman::getDirection(){
    return direction;
}

void Pacman::setCurrentPosition(Nodo* newCurrentPosition){
    this->currentPosition = newCurrentPosition;
}

Nodo* Pacman::getCurrentPosition(){
    return currentPosition;
}