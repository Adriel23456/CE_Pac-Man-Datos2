#include "Game.h"

Game::Game(QWidget* parent)
    : QWidget(parent), puntos(0) {
    setFixedSize(500, 300);
    setWindowTitle("CE-Pac-Man");

    // Inicia el juego en el primer nivel
    currentLevel = niveles.getLevel(1);
}

Game::~Game() {
}

void Game::cambiaNivel() {
    if (puntos >= 5000) {
        // Incrementa el nivel actual si se alcanzan 5000 puntos
        int nivelActual = (puntos / 5000);
        currentLevel = niveles.getLevel(nivelActual + 1);

        // Restablece los puntos al inicio del siguiente nivel
        puntos = 0;
    }
}