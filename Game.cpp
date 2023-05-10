#include "Game.h"
#include "Nivel.h"
#include "Pacman.h"
#include <QGraphicsRectItem>

void renderizaNivel(Game* game) {
    // Limpia la escena antes de renderizar un nuevo nivel
    game->getScene()->clear();

    int anchoCelda = game->width() / game->getCurrentNivel()->getColumns();
    int altoCelda = game->height() / game->getCurrentNivel()->getRows();

    for (int i = 0; i < game->getCurrentNivel()->getRows(); ++i) {
        for (int j = 0; j < game->getCurrentNivel()->getColumns(); ++j) {
            QGraphicsRectItem* rect = new QGraphicsRectItem(j * anchoCelda, i * altoCelda, anchoCelda, altoCelda);
            Nodo* nodo = (game->getCurrentNivel()->getNode(i, j));
            if (nodo->getType() == 1) {
                // Pared
                rect->setBrush(Qt::black);
            } else {
                // Nodo de movimiento libre
                rect->setBrush(Qt::white);
            }
            game->getScene()->addItem(rect);
        }
    }
    // Cambia el tamaño de Pac-Man para que quepa en una celda
    QPixmap pacmanPixmap = game->getCurrentNivel()->getPacman()->pixmap();
    pacmanPixmap = pacmanPixmap.scaled(anchoCelda, altoCelda, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    game->getCurrentNivel()->getPacman()->setPixmap(pacmanPixmap);

    //Se va a establecer el nodo donde va a posicionarse el Pacman (Incompleto)
    Nodo* nodoPrincipio = game->getCurrentNivel()->getNodoPrincipio();
    game->getCurrentNivel()->getPacman()->setCurrentPosition(nodoPrincipio);

    // Añade Pac-Man a la escena
    game->getScene()->addItem(game->getCurrentNivel()->getPacman());
}

Game::Game(QWidget* parent): QGraphicsView(parent) {
    this->nivel = new Nivel();
    this->puntos = 0;
    setFixedSize(850, 625);
    setWindowTitle("CE-Pac-Man");
    // Inicializa la escena y la vista
    scene = new QGraphicsScene(this);
    setScene(scene);
    renderizaNivel(this);
    // Configura un temporizador para controlar la velocidad de actualización del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, scene, &QGraphicsScene::advance);
    timer->start(200); // Actualiza cada 200 ms
}

Game::~Game() {
    delete nivel;
    delete timer;
    delete scene;
}

Nivel* Game::getCurrentNivel(){
    return this->nivel;
}

QGraphicsScene* Game::getScene(){
    return this->scene;
}

void Game::update(){
    //Logica de preguntar si el jugador presiono una tecla
    //Logica de preguntar si el jugador puede moverse a la posicion presionada
    //Logica de preguntarse si ya se va a realizar el movimiento de los objetos
        //Si si se puede mover, redibuja al jugador en la nueva posicion y actualiza al nodo nuevo como un nodo con "falso" en comida y suma 50 a la variable de puntos (El jugador se dibuja encima del nodo)
        //Se pregunta, dependiendo del nivel, si ya se comio todas las celdas de movimiento
            //Si esto es correcto, se elimina el nivel y se genera un nuevo nivel, el que le sigue
                //Renderiza el nuevo nivel y se sigue aplicando este update
            //Si esto es correcto y es el ultimo nivel, se muestra una ventana de juego terminado
    //Si aun no son los 200ms, no se movera el personaje y se repite este codigo...
}

void Game::cambiaNivel() {
    if (nivel->getComidaRestante() == 0) {
        int currentLevel = nivel->getCurrentLevel();
        this->nivel = new Nivel(currentLevel++);
        renderizaNivel(this);
    }
}