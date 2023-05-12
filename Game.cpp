#include "Game.h"
#include "Nivel.h"
#include "Pacman.h"
#include <QDebug>
#include <QFontDatabase>

Game::Game(QWidget* parent): QGraphicsView(parent) {
    //Datos de inicio basicos
    this->nivel = new Nivel();
    this->puntos = 0;
    this->setFixedSize(800, 500);
    this->setWindowTitle("CEPac-Man");
    this->firstGeneration = true;

    //Se añade el font en la aplicacion
    int id = QFontDatabase::addApplicationFont("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/fonts/Joystix.TTF");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont retroFont(family);


    //Detalles de la ventana y de la escena
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene = new QGraphicsScene(this);
    setScene(this->scene);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    //Se cargan las imagenes
    foodPixmap.load("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/food.png");
    nofoodPixmap.load("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/nofood.png");
    wallPixmap.load("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/wall.png");
    pacmanPixmap.load("/home/adriel/Desktop/Proyecto#2/CE_Pac-Man-Datos2/Images/pacman.png");

    // Inicializar el texto del puntaje
    scoreText = new QGraphicsTextItem();
    scoreText->setPlainText(QString("Score: %1").arg(puntos));
    scoreText->setZValue(3);
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(retroFont);
    scoreText->setPos(5, 5); // Colocar en la esquina superior izquierda
    scene->addItem(scoreText);

    // Inicializar el texto de las vidas
    livesText = new QGraphicsTextItem();
    livesText->setZValue(3);
    livesText->setPlainText(QString("Lives: %1").arg(this->getCurrentNivel()->getPacman()->getLives())); // Asumiendo que Pacman tiene un método getLives()
    livesText->setDefaultTextColor(Qt::white);
    livesText->setFont(retroFont);
    livesText->setPos(5, 25); // Colocar un poco debajo del texto del puntaje
    scene->addItem(livesText);

    // Inicializar el texto del nivel
    levelText = new QGraphicsTextItem();
    levelText->setZValue(3);
    levelText->setPlainText(QString("Level: %1").arg(this->getCurrentNivel()->getCurrentLevel())); // Asumiendo que Nivel tiene un método getCurrentLevel()
    levelText->setDefaultTextColor(Qt::white);
    levelText->setFont(retroFont);
    levelText->setScale(2);
    levelText->setPos(this->width()/2, this->height() - 60); // Colocar en el centro de la parte inferior de la pantalla
    scene->addItem(levelText);

    // Configura un temporizador para controlar la velocidad de actualización del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Game::update));
    timer->start(850); // Actualiza cada 850 ms
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
    if(this->getFirstGeneration() == true){
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
                nodo->setZValue(1);
                nodo->setPos(x, y);
                this->getScene()->addItem(nodo);
            }
        }
        //Codigo para actualizar la posicion que sera la siguiente del Pacman:
        Pacman* pacman = this->getCurrentNivel()->getPacman();
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
        //Codigo para dibujar al PacMan:
        int currentPacmanRow = (this->getCurrentNivel()->getPacman()->getCurrentPosition()->getRow()) * altoCelda;
        int currentPacmanCol = (this->getCurrentNivel()->getPacman()->getCurrentPosition()->getCol())* anchoCelda;
        pacman->setPixmap(pacmanPixmap);

        //Se actualizará la posición del nodo en la escena:
        pacman->setPos(currentPacmanCol, currentPacmanRow);
        pacman->setZValue(2);
        this->getScene()->addItem(pacman);
        //Ya no se estaran generando los objetos:
        this->setFirstGeneration(false);
    }else{
        //Se establecen las alturas y anchuras relativas del nivel actual
        int anchoCelda = this->width() / this->getCurrentNivel()->getColumns();
        int altoCelda = this->height() / this->getCurrentNivel()->getRows();
        // Cambia el tamaño de las imagenes para que quepan en un celda del nivel actual
        foodPixmap = foodPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        nofoodPixmap = nofoodPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        wallPixmap = wallPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        pacmanPixmap = pacmanPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        //Codigo para actualizar la posicion que sera la siguiente del Pacman:
        Pacman* pacman = this->getCurrentNivel()->getPacman();
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
        //Codigo para dibujar al PacMan:
        int currentPacmanRow = (this->getCurrentNivel()->getPacman()->getCurrentPosition()->getRow()) * altoCelda;
        int currentPacmanCol = (this->getCurrentNivel()->getPacman()->getCurrentPosition()->getCol())* anchoCelda;
        //Se actualizará la posición del nodo en la escena:
        pacman->setPos(currentPacmanCol, currentPacmanRow);

        //Codigo para dibujar los nodos (El nivel):
        for (int i = 0; i < this->getCurrentNivel()->getRows(); ++i) {
            for (int j = 0; j < this->getCurrentNivel()->getColumns(); ++j) {
                //Se van a establecer los nodos con su valor respectivo...
                Nodo* nodo = (this->getCurrentNivel()->getNode(i, j));
                if (nodo->getType() == 1) {
                    //Pared
                } else {
                    if(nodo->getHasFood()==true){
                        //Comida
                        nodo->setPixmap(foodPixmap);
                    } else{
                        //No Comida
                        nodo->setPixmap(nofoodPixmap);
                    }
                }
            }
        }
        // Actualizar los objetos
        this->getScene()->update();
    }
    // Actualizar el texto del puntaje
    scoreText->setPlainText(QString("Score: %1").arg(this->puntos));
    // Actualizar el texto de las vidas
    livesText->setPlainText(QString("Lives: %1").arg(this->getCurrentNivel()->getPacman()->getLives()));
    // Actualizar el texto del nivel
    levelText->setPlainText(QString("Level: %1").arg(this->getCurrentNivel()->getCurrentLevel()));
    // Comprueba si se debe cambiar de nivel
    this->cambiaNivel();
}

void Game::cambiaNivel() {
    if (this->getCurrentNivel()->getComidaRestante() == -1) {
        int currentLevel = this->getCurrentNivel()->getCurrentLevel();
        // Incrementa el nivel actual
        currentLevel++;
        // Elimina el nivel actual
        delete this->nivel;
        // Crea un nuevo nivel
        this->nivel = new Nivel(currentLevel);
        // Resetea el flag de primera generación
        this->setFirstGeneration(true);
        // Actualiza la pantalla
        this->update();
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

void Game::setFirstGeneration(bool newValue) {
    this->firstGeneration = newValue;
}

bool Game::getFirstGeneration() {
    return this->firstGeneration;
}