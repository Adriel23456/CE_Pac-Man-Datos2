#include "Game.h"
#include "Nivel.h"
#include "Pacman.h"
#include <QGraphicsRectItem>

Game::Game(QWidget* parent)
    : QGraphicsView(parent), puntos(0) {
    setFixedSize(850, 625);
    setWindowTitle("CE-Pac-Man");

    // Inicializa la escena y la vista
    scene = new QGraphicsScene(this);
    setScene(scene);

    // Inicializa el objeto niveles
    niveles = new Nivel();
    pacman = new Pacman();

    // Inicia el juego en el primer nivel
    currentLevel = 1;
    renderizaNivel();

    // Configura un temporizador para controlar la velocidad de actualización del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, scene, &QGraphicsScene::advance);
    timer->start(200); // Actualiza cada 200 ms
}

Game::~Game() {
    delete pacman; // Eliminar el objeto Pacman
    delete timer;
    delete scene;
    delete niveles;
}

void Game::cambiaNivel() {
    if (puntos >= 5000) {
        // Incrementa el nivel actual si se alcanzan 5000 puntos
        currentLevel++;
        // Restablece los puntos al inicio del siguiente nivel
        puntos = 0;
        renderizaNivel();
    }
}

void Game::renderizaNivel() {
    // Limpia la escena antes de renderizar un nuevo nivel
    scene->clear();

    int anchoCelda = width() / niveles->getColumns();
    int altoCelda = height() / niveles->getRows();

    for (int i = 0; i < niveles->getRows(); ++i) {
        for (int j = 0; j < niveles->getColumns(); ++j) {
            QGraphicsRectItem* rect = new QGraphicsRectItem(j * anchoCelda, i * altoCelda, anchoCelda, altoCelda);
            Nodo nodo = niveles->getNode(i, j);

            if (nodo.getType() == 1) {
                // Pared
                rect->setBrush(Qt::black);
            } else {
                // Nodo de movimiento libre
                rect->setBrush(Qt::white);
            }

            scene->addItem(rect);
        }
    }

    pacman->setNivel(niveles); // Establece el nivel en el objeto Pacman

    // Establece la posición de Pac-Man en función del tamaño de las celdas
    int centerX = (niveles->getColumns() / 2) * Nivel::CELL_SIZE;
    int centerY = (niveles->getRows() / 2) * Nivel::CELL_SIZE;
    pacman->setPosition(centerX, centerY);

    // Cambia el tamaño de Pac-Man para que quepa en una celda
    QPixmap pacmanPixmap = pacman->pixmap();
    pacmanPixmap = pacmanPixmap.scaled(anchoCelda, altoCelda, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pacman->setPixmap(pacmanPixmap);

    // Añade Pac-Man a la escena
    scene->addItem(pacman);
}

int Game::getCurrentLevel() const {
    return currentLevel;
}