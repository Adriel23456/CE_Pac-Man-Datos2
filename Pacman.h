#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include "Nivel.h"

class Nivel; // Añade esta línea para la declaración adelantada

class Pacman : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Pacman();
    int getLives() const;
    void loseLife();
    void setPosition(int x, int y);
    void setNivel(Nivel* nivel);

    static const int REAPPEAR_TIME = 3000; // Tiempo de reaparición en milisegundos

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    int lives;
    int directionX;
    int directionY;
    Nivel* nivel;

    void move();
    bool canMove(int newX, int newY);
};