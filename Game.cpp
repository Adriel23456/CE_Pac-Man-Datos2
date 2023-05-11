#include "Game.h"
#include "Nivel.h"
#include "Pacman.h"
#include <QGraphicsRectItem>
#include <QDebug>

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
    // Configura un temporizador para controlar la velocidad de actualización del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Game::update));
    timer->start(1250); // Actualiza cada 1250 ms
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

QPixmap foodPixmap("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/food.png");
QPixmap nofoodPixmap("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/nofood.png");
QPixmap wallPixmap("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/wall.png");
QPixmap pacmanPixmap("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/pacman.png");

void Game::update(){
    qDebug() << "Actualizado de pantalla...";
    
    //Se limpia la escena
    this->getScene()->clear();

    //Se establecen las alturas y anchuras relativas del nivel actual
    int anchoCelda = this->width() / this->getCurrentNivel()->getColumns();
    int altoCelda = this->height() / this->getCurrentNivel()->getRows();

    // Cambia el tamaño de las imagenes para que quepan en un celda del nivel actual
    foodPixmap = foodPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    nofoodPixmap = nofoodPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    wallPixmap = wallPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    pacmanPixmap = pacmanPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    //Codigo para dibujar los nodos (El nivel):
    for (int i = 0; i < this->getCurrentNivel()->getRows(); ++i) {
        for (int j = 0; j < this->getCurrentNivel()->getColumns(); ++j) {
            //Se van a establecer los nodos con su valor respectivo...
            Nodo* nodo = (this->getCurrentNivel()->getNode(i, j));
            if (nodo->getType() == 1) {
                //Pared
                nodo->setPixmap(wallPixmap);
            } else {
                if(nodo->getHasFood()==true){
                    //Comida
                    nodo->setPixmap(foodPixmap);
                } else{
                    //No Comida
                    nodo->setPixmap(nofoodPixmap);
                }
            }
            //Se actualizara la posicion del nodo y se añadera en la escena:
            int x = nodo->getCol() * anchoCelda;
            int y = nodo->getRow() * altoCelda;
            nodo->setPos(x, y);
            this->getScene()->addItem(nodo);
        }
    }

    //Codigo para dibujar al PacMan:
    int currentPacmanRow = (this->getCurrentNivel()->getPacman()->getCurrentPosition()->getRow()) * anchoCelda;
    int currentPacmanCol = (this->getCurrentNivel()->getPacman()->getCurrentPosition()->getCol())* altoCelda;
    qDebug() << QString("La fila del Pacman es: %1").arg(this->getCurrentNivel()->getPacman()->getCurrentPosition()->getRow());
    qDebug() << QString("La columna del Pacman es: %1").arg(this->getCurrentNivel()->getPacman()->getCurrentPosition()->getCol());
    Pacman* pacman = this->getCurrentNivel()->getPacman();
    pacman->setPixmap(pacmanPixmap);
    //Se actualizará la posición del nodo en la escena:
    pacman->setPos(currentPacmanRow, currentPacmanRow);
    this->getScene()->addItem(pacman);

    //Codigo para actualizar la posicion que sera la siguiente del Pacman:
    int newRow = this->getCurrentNivel()->getPacman()->getCurrentPosition()->getRow();
    int newCol = this->getCurrentNivel()->getPacman()->getCurrentPosition()->getCol();
    int direction = this->getCurrentNivel()->getPacman()->getDirection();
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

    Nodo* nuevoNodoPacman = this->getCurrentNivel()->getNode(newRow, newCol);
    qDebug() << QString("EL nodo del posible movimiento es, fila: %1").arg(newRow);
    qDebug() << QString("EL nodo del posible movimiento es, columna: %1").arg(newCol);

    if(pacman->canMove(nuevoNodoPacman)){
        pacman->setCurrentPosition(nuevoNodoPacman);
        //Se debe de cambiar la informacion del nodo para que ya no tenga comida...
        if (nuevoNodoPacman->getHasFood() == true){
            nuevoNodoPacman->setHasFood(false);
            this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
            this->puntos += 50;
        }else{
        }
    }else{
    }
    // Actualiza los objetos
    this->getScene()->update();
    // Comprueba si se debe cambiar de nivel
    cambiaNivel();
}

void Game::cambiaNivel() {
    if (nivel->getComidaRestante() == 0) {
        int currentLevel = nivel->getCurrentLevel();
        this->nivel = new Nivel(currentLevel++);
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
}