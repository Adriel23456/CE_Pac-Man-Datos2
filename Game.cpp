#include "Game.h"
#include "Nivel.h"
#include "Pacman.h"
#include <QGraphicsRectItem>
#include <QDebug>

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
                if(nodo->getHasFood()==true){
                    rect->setBrush(Qt::darkYellow);
                } else{
                    rect->setBrush(Qt::white);
                }
            }
            game->getScene()->addItem(rect);
        }
    }
    
    // Cambia el tamaño de Pac-Man para que quepa en una celda
    QPixmap pacmanPixmap("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/pacman.png");
    pacmanPixmap = pacmanPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    game->getCurrentNivel()->getPacman()->setPixmap(pacmanPixmap);

    //Se va a agarrar al nodo donde esta el Pacman:
    Nodo* nodoPrincipio = game->getCurrentNivel()->getPacman()->getCurrentPosition();

    //Se actualizara la posicion de Pac-Man en la escena:
    int x = nodoPrincipio->getCol() * anchoCelda;
    int y = nodoPrincipio->getRow() * altoCelda;
    game->getCurrentNivel()->getPacman()->setPos(x, y);

    // Añade Pac-Man a la escena
    game->getScene()->addItem(game->getCurrentNivel()->getPacman());
}

Game::Game(QWidget* parent): QGraphicsView(parent) {
    this->nivel = new Nivel();
    this->puntos = 0;
    this->setFixedSize(800, 500);
    this->setWindowTitle("CEPac-Man");
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Inicializa la escena y la vista
    scene = new QGraphicsScene(this);

    setScene(scene);
    renderizaNivel(this);
    // Configura un temporizador para controlar la velocidad de actualización del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Game::update));
    timer->start(1250); // Actualiza cada 1000 ms
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
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
    qDebug() << "Actualizado de pantalla...";

    int anchoCelda = this->width() / this->getCurrentNivel()->getColumns();
    int altoCelda = this->height() / this->getCurrentNivel()->getRows();
    int direction = this->getCurrentNivel()->getPacman()->getDirection();

    // Obtén las coordenadas actuales de Pac-Man en la matriz
    Nodo* nodoActual = this->getCurrentNivel()->getPacman()->getCurrentPosition();

    int currentRow = nodoActual->getRow();
    int currentCol = nodoActual->getCol();
    qDebug() << QString("La fila del Pacman es: %1").arg(currentRow);
    qDebug() << QString("La columna del Pacman es: %1").arg(currentCol);

    // Calcula las nuevas coordenadas en función de la dirección
    int newRow = currentRow;
    int newCol = currentCol;
    if(direction == 0){
    }else if(direction == 1){
        newCol--;
    }else if(direction == 2){
        newRow--;
    }else if(direction == 3){
        newCol++;
    }else{
        newRow++;
    }

    Nodo* nuevoNodo = this->getCurrentNivel()->getNode(newRow, newCol);

    qDebug() << QString("EL nodo del posible movimiento es, fila: %1").arg(newRow);
    qDebug() << QString("EL nodo del posible movimiento es, columna: %1").arg(newCol);

    // Comprueba si Pac-Man puede moverse al nuevo nodo
    if (this->getCurrentNivel()->getPacman()->canMove(nuevoNodo)) {
        this->getCurrentNivel()->getPacman()->setCurrentPosition(nuevoNodo);
        // Actualiza la posición de Pac-Man en la escena
        //Se actualizara la posicion de Pac-Man en la escena:
        int x = nuevoNodo->getCol() * anchoCelda;
        int y = nuevoNodo->getRow() * altoCelda;
        this->getCurrentNivel()->getPacman()->setPos(x, y);
        

        // Si el nodo tiene comida, actualiza la comida restante y suma puntos
        if (nuevoNodo->getHasFood()) {
            nuevoNodo->setHasFood(false);
            this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
            this->puntos += 50;
        }

    } else {
        // Si Pac-Man no puede moverse al nuevo nodo, regresa a la posición anterior
        this->getCurrentNivel()->getPacman()->setCurrentPosition(nodoActual);
    }
    // Comprueba si se debe cambiar de nivel
    cambiaNivel();
}

void Game::cambiaNivel() {
    if (nivel->getComidaRestante() == 0) {
        int currentLevel = nivel->getCurrentLevel();
        this->nivel = new Nivel(currentLevel++);
        renderizaNivel(this);
    }
}

void Game::keyPressEvent(QKeyEvent* event) {
    int valor = 0;
    switch (event->key()) {
        case Qt::Key_W:
            valor = 2;
            this->getCurrentNivel()->getPacman()->setDirection(valor);
            break;
        case Qt::Key_A:
            valor = 1;
            this->getCurrentNivel()->getPacman()->setDirection(valor);
            break;
        case Qt::Key_S:
            valor = 4;
            this->getCurrentNivel()->getPacman()->setDirection(valor);
            break;
        case Qt::Key_D:
            valor = 3;
            this->getCurrentNivel()->getPacman()->setDirection(valor);
            break;
    }
    qDebug() << "Tecla presionada y valor cambiado";
}