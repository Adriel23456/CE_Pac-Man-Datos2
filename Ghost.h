#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include "Nodo.h"

class Ghost : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

private:
    int direction; //Puede ser 0(Nula), 1(Izquierda), 2(Arriba), 3(Derecha) o 4(Abajo)
    Nodo* currentPosition; //Nodo donde se encuentra
    const int reloadTime; // Tiempo de reaparición en milisegundos

public:
    Ghost(Nodo* currentPosition);
    ~Ghost();

    void setDirection(int direction); 
    int getDirection();

    void setCurrentPosition(Nodo* newCurrentPosition);
    Nodo* getCurrentPosition();

    int getReloadTime() const;
};