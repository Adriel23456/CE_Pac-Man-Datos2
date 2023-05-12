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

    int getDirectionPacMan(int** matriz, Nodo* final, Nodo* inicio, int rows, int columns);

    void setCurrentPosition(Nodo* newCurrentPosition);
    Nodo* getCurrentPosition();

    int getReloadTime() const;
};