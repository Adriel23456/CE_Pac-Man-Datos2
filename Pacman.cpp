#include "Pacman.h"

#include <QTimer>
#include <QKeyEvent>

Pacman::Pacman() : lives(3), direction(0), currentPosition(new Nodo()), reloadTime(3000) {
}

Pacman::Pacman(Nodo* currentPosition): lives(3), direction(0), currentPosition(currentPosition), reloadTime(3000) {
    // Establece la imagen del personaje de Pacman
    QPixmap pacmanPixmap("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/pacman.png");
    int scaledSize = this->currentPosition->getCellSize(); // Ajusta el tamaño de Pac-Man al tamaño de las celdas
    setPixmap(pacmanPixmap.scaled(scaledSize, scaledSize, Qt::KeepAspectRatio));
    
    // Configura el temporizador para el movimiento de Pacman
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Pacman::setCurrentPosition);
    timer->start(150); // Actualiza la posición cada 150 ms
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
    int valor;
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

int Pacman::getDirection(){
    return direction;
}

void Pacman::setCurrentPosition(Nodo* newCurrentPosition){
    this->currentPosition = newCurrentPosition;
}

Nodo* Pacman::getCurrentPosition(){
    return currentPosition;
}