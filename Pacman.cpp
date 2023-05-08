#include "Pacman.h"
#include <QTimer>
#include <QKeyEvent>

Pacman::Pacman(Nivel* nivel) : lives(3), directionX(0), directionY(0), nivel(nivel) {
    // Establece la imagen del personaje de Pacman
    setPixmap(QPixmap(":/images/pacman.png"));

    // Configura el temporizador para el movimiento de Pacman
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Pacman::move);
    timer->start(100); // Actualiza la posición cada 100 ms
}

int Pacman::getLives() const {
    return lives;
}

void Pacman::loseLife() {
    lives--;
}

void Pacman::setPosition(int x, int y) {
    setPos(x, y);
}

void Pacman::keyPressEvent(QKeyEvent* event) {
    int newX = x();
    int newY = y();

    switch (event->key()) {
        case Qt::Key_W:
            newY -= Nivel::CELL_SIZE;
            break;
        case Qt::Key_A:
            newX -= Nivel::CELL_SIZE;
            break;
        case Qt::Key_S:
            newY += Nivel::CELL_SIZE;
            break;
        case Qt::Key_D:
            newX += Nivel::CELL_SIZE;
            break;
    }

    if (canMove(newX, newY)) {
        directionX = newX - x();
        directionY = newY - y();
    }
}

void Pacman::move() {
    int newX = x() + directionX;
    int newY = y() + directionY;

    if (canMove(newX, newY)) {
        setPos(newX, newY);
    }
}

bool Pacman::canMove(int newX, int newY) {
    int row = newY / Nivel::CELL_SIZE;
    int col = newX / Nivel::CELL_SIZE;

    if (row >= 0 && row < nivel->getRows() && col >= 0 && col < nivel->getColumns()) {
        Nodo node = nivel->getNode(row, col);
        return node.getType() != 0;
    }
    return false;
}