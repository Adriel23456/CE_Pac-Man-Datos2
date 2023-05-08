#pragma once
#include <QObject>
#include <QGraphicsPixmapItem>
#include "Nivel.h"

class Pacman : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Pacman(Nivel* nivel);

    int getLives() const;
    void loseLife();
    void setPosition(int x, int y);

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