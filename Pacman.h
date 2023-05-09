#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include "Nodo.h"

class Pacman : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

private:
    int lives; //Sera 3 automaticamente
    int direction; //Puede ser 0(Nula), 1(Izquierda), 2(Arriba), 3(Derecha) o 4(Abajo)
    Nodo* currentPosition; //Nodo donde se encuentra
    const int reloadTime; // Tiempo de reaparición en milisegundos

public:
    Pacman();
    Pacman(Nodo* currentPosition);
    ~Pacman();

    int getLives();
    void loseLife();
    
    bool canMove(Nodo* newNode);

    void setDirection(int direction);
    int getDirection();

    void setCurrentPosition(Nodo* newCurrentPosition);
    Nodo* getCurrentPosition();

protected:
    void keyPressEvent(QKeyEvent* event);
};