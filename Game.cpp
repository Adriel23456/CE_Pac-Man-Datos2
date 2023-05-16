#include "Game.h"
#include "Nivel.h"
#include "Pacman.h"
#include "Ghost.h"
#include <QDebug>
#include <QFontDatabase>
#include <QMessageBox>
#include <QCoreApplication>
#include <cmath>
#include <climits>

// Función para calcular la distancia euclidiana entre dos puntos
double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

Game::Game(QWidget* parent): QGraphicsView(parent) {
    //Datos de inicio basicos
    this->nivel = new Nivel();
    this->puntos = 0;
    this->pointsForActive = 200;
    this->powerUpActive = false;
    this->powerUps = 0;
    this->pacmanDeath = false;
    this->firstGeneration = true;
    this->setFixedSize(800, 500);
    this->setWindowTitle("CEPac-Man");
    //Detalles de la ventana y de la escena
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene = new QGraphicsScene(this);
    setScene(this->scene);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
    //Se añade el font en la aplicacion
    int id = QFontDatabase::addApplicationFont("fonts/Joystix.TTF");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont retroFont(family);
    //Se cargan las imagenes
    foodPixmap.load("Images/food.png");
    specialFoodPixmap.load("Images/specialFood.png");
    nofoodPixmap.load("Images/nofood.png");
    wallPixmap.load("Images/wall.png");
    pacmanPixmap.load("Images/pacman.png");
    ghost1Pixmap.load("Images/ghost1.png");
    ghost2Pixmap.load("Images/ghost2.png");
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
    // Cargar la música de fondo
    if (!backgroundMusic.openFromFile("sounds/background.ogg"))
        return; // error
    backgroundMusic.setLoop(true); // Repetir indefinidamente
    backgroundMusic.setVolume(15);
    // Cargar la música de poder
    if (!powerUpMusic.openFromFile("sounds/powerUp.ogg"))
        return; // error
    powerUpMusic.setLoop(true); // Repetir indefinidamente
    powerUpMusic.setVolume(15);
    // Cargar el sonido de comer
    if (!pacmanEatBuffer.loadFromFile("sounds/eaten.ogg"))
        return; // error
    pacmanEatSound.setBuffer(pacmanEatBuffer);
    pacmanEatSound.setVolume(10);
    // Cargar el sonido de victoria
    if (!victorySoundBuffer.loadFromFile("sounds/victory.ogg"))
        return; // error
    victorySound.setBuffer(victorySoundBuffer);
    victorySound.setVolume(20);
    // Cargar el sonido de muerte
    if (!deathSoundBuffer.loadFromFile("sounds/death.ogg"))
        return; // error
    deathSound.setBuffer(deathSoundBuffer);
    deathSound.setVolume(20);
    // Configura un temporizador para controlar la velocidad de actualización del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Game::update));
    timer->start(450); // Actualiza cada 450ms
    playBackgroundMusic();
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
    //Comprobar si esta activado en el tablero algun PowerUp
    if(this->getPowerUps() == 0){
        this->setPowerUpActive(false);
    }
    // Comprueba si se debe cambiar de nivel
    this->cambiaNivel();
    if(this->getFirstGeneration() == true){
        //Se establecen las alturas y anchuras relativas del nivel actual
        int anchoCelda = this->width() / this->getCurrentNivel()->getColumns();
        int altoCelda = this->height() / this->getCurrentNivel()->getRows();
        // Cambia el tamaño de las imagenes para que quepan en un celda del nivel actual
        specialFoodPixmap = specialFoodPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        foodPixmap = foodPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        nofoodPixmap = nofoodPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        wallPixmap = wallPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        pacmanPixmap = pacmanPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ghost1Pixmap = ghost1Pixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ghost2Pixmap = ghost2Pixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        //Codigo para dibujar los nodos (El nivel):
        for (int i = 0; i < this->getCurrentNivel()->getRows(); ++i) {
            for (int j = 0; j < this->getCurrentNivel()->getColumns(); ++j) {
                //Se van a establecer los nodos con su valor respectivo...
                Nodo* nodo = (this->getCurrentNivel()->getNode(i, j));
                if (nodo->getType() == 1) {
                    //Pared
                    nodo->setPixmap(wallPixmap);
                } else {
                    if(nodo->getHasPower() == true){
                        //Poder
                        nodo->setPixmap(specialFoodPixmap);
                    }
                    else if(nodo->getHasFood()==true){
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
                this->puntos += 10;
                playPacmanEatSound();
            }
            //O poner mamadisimo al pacman si este nodo tenia power...
            if (nuevoNodoPacman->getHasPower() == true){
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                this->setPowerUps(this->getPowerUps()-1);
                pacman->setPowerOn(true);
                stopBackgroundMusic();
                playPowerMusic();
                if(this->getPowerUps() == 0){
                    qDebug() << "El PacMan se lo comio y ya no quedan poderes...";
                    this->setPowerUpActive(false);
                }
                timerPower = new QTimer(this);
                timerPower->setSingleShot(true); // Este timer solo se activará una vez
                connect(timerPower, &QTimer::timeout, this, &Game::deactivatePower);
                timerPower->start(8000); // Comienza el timer para 3 segundos (3000 milisegundos)
            }
        }

        //Codigo para dibujar al PacMan:
        int currentPacmanRow = (this->getCurrentNivel()->getPacman()->getCurrentPosition()->getRow()) * altoCelda;
        int currentPacmanCol = (this->getCurrentNivel()->getPacman()->getCurrentPosition()->getCol())* anchoCelda;
        pacman->setPixmap(pacmanPixmap);
        //Se actualizará la posición del pacman en la escena:
        pacman->setPos(currentPacmanCol, currentPacmanRow);
        pacman->setZValue(2);
        this->getScene()->addItem(pacman);
        //Codigo para establecer la posicion y dibujar a los fantasmas:
        int currentlevelGame = this->getCurrentNivel()->getCurrentLevel();
        std::vector<Ghost*> ghosts = this->getCurrentNivel()->getGhosts();
        if (currentlevelGame == 1){
            Ghost* ghost1 = ghosts[0];
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            ghost1->setPixmap(ghost1Pixmap);
            //Se actualizará la posición del fantasma en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            ghost1->setZValue(2);
            this->getScene()->addItem(ghost1);
        }else if(currentlevelGame == 2){
            Ghost* ghost1 = ghosts[0];
            Ghost* ghost2 = ghosts[1];
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost2Row = (ghost2->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost2Col = (ghost2->getCurrentPosition()->getCol())* anchoCelda;
            ghost1->setPixmap(ghost1Pixmap);
            ghost2->setPixmap(ghost2Pixmap);
            //Se actualizará la posición del nodo en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            ghost1->setZValue(2);
            this->getScene()->addItem(ghost1);
            ghost2->setPos(currentGhost2Col, currentGhost2Row);
            ghost2->setZValue(2);
            this->getScene()->addItem(ghost2);
        }else if(currentlevelGame == 3){
            Ghost* ghost1 = ghosts[0];
            Ghost* ghost2 = ghosts[1];
            Ghost* ghost3 = ghosts[2];
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost2Row = (ghost2->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost2Col = (ghost2->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost3Row = (ghost3->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost3Col = (ghost3->getCurrentPosition()->getCol())* anchoCelda;
            ghost1->setPixmap(ghost1Pixmap);
            ghost2->setPixmap(ghost2Pixmap);
            ghost3->setPixmap(ghost1Pixmap);
            //Se actualizará la posición del nodo en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            ghost1->setZValue(2);
            this->getScene()->addItem(ghost1);
            ghost2->setPos(currentGhost2Col, currentGhost2Row);
            ghost2->setZValue(2);
            this->getScene()->addItem(ghost2);
            ghost3->setPos(currentGhost3Col, currentGhost3Row);
            ghost3->setZValue(2);
            this->getScene()->addItem(ghost3);
        }else{
            Ghost* ghost1 = ghosts[0];
            Ghost* ghost2 = ghosts[1];
            Ghost* ghost3 = ghosts[2];
            Ghost* ghost4 = ghosts[3];
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost2Row = (ghost2->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost2Col = (ghost2->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost3Row = (ghost3->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost3Col = (ghost3->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost4Row = (ghost4->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost4Col = (ghost4->getCurrentPosition()->getCol())* anchoCelda;
            ghost1->setPixmap(ghost1Pixmap);
            ghost2->setPixmap(ghost2Pixmap);
            ghost3->setPixmap(ghost1Pixmap);
            ghost4->setPixmap(ghost2Pixmap);
            //Se actualizará la posición del nodo en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            ghost1->setZValue(2);
            this->getScene()->addItem(ghost1);
            ghost2->setPos(currentGhost2Col, currentGhost2Row);
            ghost2->setZValue(2);
            this->getScene()->addItem(ghost2);
            ghost3->setPos(currentGhost3Col, currentGhost3Row);
            ghost3->setZValue(2);
            this->getScene()->addItem(ghost3);
            ghost4->setPos(currentGhost4Col, currentGhost4Row);
            ghost4->setZValue(2);
            this->getScene()->addItem(ghost4);
        }
        //Ya no se estaran generando los objetos:
        this->setFirstGeneration(false);
    }else{
        //Se establecen las alturas y anchuras relativas del nivel actual
        int anchoCelda = this->width() / this->getCurrentNivel()->getColumns();
        int altoCelda = this->height() / this->getCurrentNivel()->getRows();
        // Cambia el tamaño de las imagenes para que quepan en un celda del nivel actual
        specialFoodPixmap = specialFoodPixmap.scaled(anchoCelda, altoCelda, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
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
                this->puntos += 10;
                playPacmanEatSound();
            }
            //O poner mamadisimo al pacman si este nodo tenia power...
            if (nuevoNodoPacman->getHasPower() == true){
                nuevoNodoPacman->setHasFood(false);
                nuevoNodoPacman->setHasPower(false);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                this->setPowerUps(this->getPowerUps()-1);
                pacman->setPowerOn(true);
                stopBackgroundMusic();
                playPowerMusic();
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
                timerPower = new QTimer(this);
                timerPower->setSingleShot(true); // Este timer solo se activará una vez
                connect(timerPower, &QTimer::timeout, this, &Game::deactivatePower);
                timerPower->start(8000); // Comienza el timer para 3 segundos (3000 milisegundos)
            }
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
                    if(nodo->getHasPower() == true){
                        //Poder
                        nodo->setPixmap(specialFoodPixmap);
                    }
                    else if(nodo->getHasFood()==true){
                        //Comida
                        nodo->setPixmap(foodPixmap);
                    } else{
                        //No Comida
                        nodo->setPixmap(nofoodPixmap);
                    }
                }
            }
        }
        //Codigo para actualizar a los fantasmas...
        int currentlevelGame = this->getCurrentNivel()->getCurrentLevel();
        std::vector<Ghost*> ghosts = this->getCurrentNivel()->getGhosts();
        if (currentlevelGame == 1){
            Ghost* ghost1 = ghosts[0];
            int newRowG1 = ghost1->getCurrentPosition()->getRow();
            int newColG1 = ghost1->getCurrentPosition()->getCol();
            int directionG1;
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost1->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost1->getCurrentPosition())){
                directionG1 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo A* para el fanstasma 1:";
                    directionG1 = ghost1->getDirectionPowerA(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG1 = ghost1->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            //Codigo para actualizar la posicion de los fantasmas:
            if((directionG1 == 1) && (ghost1->getDeath()==false)){
                newColG1--;
            }else if((directionG1 == 2) && (ghost1->getDeath()==false)){
                newRowG1--;
            }else if((directionG1 == 3) && (ghost1->getDeath()==false)){
                newColG1++;
            }else if((directionG1 == 4) && (ghost1->getDeath()==false)){
                newRowG1++;
            }else{
            }
            Nodo* nuevoNodoGhost1 = this->getCurrentNivel()->getNode(newRowG1, newColG1);
            ghost1->setCurrentPosition(nuevoNodoGhost1);
            if(nuevoNodoGhost1->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost1->setHasPower(false);
                nuevoNodoGhost1->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            //Se actualizará la posición de los fanstasmas en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            //Todo el codigo para matar al pacman/fantasma en caso de que este acabara de morir
            Nodo* pacmanActualNodo = pacman->getCurrentPosition();
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost1 == pacmanActualNodo) && (ghost1->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost1->setDeath(true);
                ghost1->hide();
                timerGhost1 = new QTimer(this);
                timerGhost1->setSingleShot(true);
                connect(timerGhost1, &QTimer::timeout, this, &Game::respawnGhost1);
                timerGhost1->start(5000); // Comienza el timer para 5 segundos
            }
            if ((nuevoNodoGhost1 == pacmanActualNodo) && (pacmanDeath == false) && (pacman->getPowerOn() == false)){
                playDeathSound();
                pacmanDeath = true;
                this->getCurrentNivel()->getPacman()->setDirection(0);
                pacman->loseLife();
                if(pacman->getLives() == 0){
                    gameOver();
                }
                pacman->hide();
                timerPacman = new QTimer(this);
                timerPacman->setSingleShot(true); // Este timer solo se activará una vez
                connect(timerPacman, &QTimer::timeout, this, &Game::respawnPacMan);
                timerPacman->start(pacman->getReloadTime()); // Comienza el timer para 3 segundos (3000 milisegundos)
            }
        }else if(currentlevelGame == 2){
            Ghost* ghost1 = ghosts[0];
            Ghost* ghost2 = ghosts[1];
            int newRowG1 = ghost1->getCurrentPosition()->getRow();
            int newColG1 = ghost1->getCurrentPosition()->getCol();
            int newRowG2 = ghost2->getCurrentPosition()->getRow();
            int newColG2 = ghost2->getCurrentPosition()->getCol();
            int directionG1;
            int directionG2;
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost1->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost1->getCurrentPosition())){
                directionG1 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo A* para el fanstasma 1:";
                    directionG1 = ghost1->getDirectionPowerA(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG1 = ghost1->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost2->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost2->getCurrentPosition())){
                directionG2 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo Backtracking para el fanstasma 2:";
                    directionG2 = ghost2->getDirectionPowerB(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG2 = ghost2->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            //Codigo para actualizar la posicion de los fantasmas:
            if((directionG1 == 1) && (ghost1->getDeath()==false)){
                newColG1--;
            }else if((directionG1 == 2) && (ghost1->getDeath()==false)){
                newRowG1--;
            }else if((directionG1 == 3) && (ghost1->getDeath()==false)){
                newColG1++;
            }else if((directionG1 == 4) && (ghost1->getDeath()==false)){
                newRowG1++;
            }else{
            }
            if((directionG2 == 1) && (ghost2->getDeath()==false)){
                newColG2--;
            }else if((directionG2 == 2) && (ghost2->getDeath()==false)){
                newRowG2--;
            }else if((directionG2 == 3) && (ghost2->getDeath()==false)){
                newColG2++;
            }else if((directionG2 == 4) && (ghost2->getDeath()==false)){
                newRowG2++;
            }else{
            }
            Nodo* nuevoNodoGhost1 = this->getCurrentNivel()->getNode(newRowG1, newColG1);
            ghost1->setCurrentPosition(nuevoNodoGhost1);
            Nodo* nuevoNodoGhost2 = this->getCurrentNivel()->getNode(newRowG2, newColG2);
            ghost2->setCurrentPosition(nuevoNodoGhost2);
            if(nuevoNodoGhost1->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost1->setHasPower(false);
                nuevoNodoGhost1->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            if(nuevoNodoGhost2->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost2->setHasPower(false);
                nuevoNodoGhost2->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost2Row = (ghost2->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost2Col = (ghost2->getCurrentPosition()->getCol())* anchoCelda;
            //Se actualizará la posición de los fanstasmas en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            ghost2->setPos(currentGhost2Col, currentGhost2Row);
            //Todo el codigo para matar al pacman/fantasma en caso de que este acabara de morir
            Nodo* pacmanActualNodo = pacman->getCurrentPosition();
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost1 == pacmanActualNodo) && (ghost1->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost1->setDeath(true);
                ghost1->hide();
                timerGhost1 = new QTimer(this);
                timerGhost1->setSingleShot(true);
                connect(timerGhost1, &QTimer::timeout, this, &Game::respawnGhost1);
                timerGhost1->start(5000); // Comienza el timer para 5 segundos
            }
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost2 == pacmanActualNodo) && (ghost2->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost2->setDeath(true);
                ghost2->hide();
                timerGhost2 = new QTimer(this);
                timerGhost2->setSingleShot(true);
                connect(timerGhost2, &QTimer::timeout, this, &Game::respawnGhost2);
                timerGhost2->start(5000); // Comienza el timer para 5 segundos
            }
            if ((((nuevoNodoGhost1 == pacmanActualNodo) && (pacmanDeath == false))||((nuevoNodoGhost2 == pacmanActualNodo) && (pacmanDeath == false))) && (pacman->getPowerOn() == false)){
                playDeathSound();
                pacmanDeath = true;
                this->getCurrentNivel()->getPacman()->setDirection(0);
                pacman->loseLife();
                if(pacman->getLives() == 0){
                    gameOver();
                }
                pacman->hide();
                timerPacman = new QTimer(this);
                timerPacman->setSingleShot(true); // Este timer solo se activará una vez
                connect(timerPacman, &QTimer::timeout, this, &Game::respawnPacMan);
                timerPacman->start(pacman->getReloadTime()); // Comienza el timer para 3 segundos (3000 milisegundos)
            }
        }else if(currentlevelGame == 3){
            Ghost* ghost1 = ghosts[0];
            Ghost* ghost2 = ghosts[1];
            Ghost* ghost3 = ghosts[2];
            int newRowG1 = ghost1->getCurrentPosition()->getRow();
            int newColG1 = ghost1->getCurrentPosition()->getCol();
            int newRowG2 = ghost2->getCurrentPosition()->getRow();
            int newColG2 = ghost2->getCurrentPosition()->getCol();
            int newRowG3 = ghost3->getCurrentPosition()->getRow();
            int newColG3 = ghost3->getCurrentPosition()->getCol();
            int directionG1;
            int directionG2;
            int directionG3;
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost1->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost1->getCurrentPosition())){
                directionG1 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo A* para el fanstasma 1:";
                    directionG1 = ghost1->getDirectionPowerA(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG1 = ghost1->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost2->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost2->getCurrentPosition())){
                directionG2 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo Backtracking para el fanstasma 2:";
                    directionG2 = ghost2->getDirectionPowerB(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG2 = ghost2->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost3->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost3->getCurrentPosition())){
                directionG3 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo A* para el fanstasma 3:";
                    directionG3 = ghost3->getDirectionPowerA(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost3->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG3 = ghost3->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost3->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            //Codigo para actualizar la posicion de los fantasmas:
            if((directionG1 == 1) && (ghost1->getDeath()==false)){
                newColG1--;
            }else if((directionG1 == 2) && (ghost1->getDeath()==false)){
                newRowG1--;
            }else if((directionG1 == 3) && (ghost1->getDeath()==false)){
                newColG1++;
            }else if((directionG1 == 4) && (ghost1->getDeath()==false)){
                newRowG1++;
            }else{
            }
            if((directionG2 == 1) && (ghost2->getDeath()==false)){
                newColG2--;
            }else if((directionG2 == 2) && (ghost2->getDeath()==false)){
                newRowG2--;
            }else if((directionG2 == 3) && (ghost2->getDeath()==false)){
                newColG2++;
            }else if((directionG2 == 4) && (ghost2->getDeath()==false)){
                newRowG2++;
            }else{
            }
            if((directionG3 == 1) && (ghost3->getDeath()==false)){
                newColG3--;
            }else if((directionG3 == 2) && (ghost3->getDeath()==false)){
                newRowG3--;
            }else if((directionG3 == 3) && (ghost3->getDeath()==false)){
                newColG3++;
            }else if((directionG3 == 4) && (ghost3->getDeath()==false)){
                newRowG3++;
            }else{
            }
            Nodo* nuevoNodoGhost1 = this->getCurrentNivel()->getNode(newRowG1, newColG1);
            ghost1->setCurrentPosition(nuevoNodoGhost1);
            Nodo* nuevoNodoGhost2 = this->getCurrentNivel()->getNode(newRowG2, newColG2);
            ghost2->setCurrentPosition(nuevoNodoGhost2);
            Nodo* nuevoNodoGhost3 = this->getCurrentNivel()->getNode(newRowG3, newColG3);
            ghost3->setCurrentPosition(nuevoNodoGhost3);
            if(nuevoNodoGhost1->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost1->setHasPower(false);
                nuevoNodoGhost1->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            if(nuevoNodoGhost2->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost2->setHasPower(false);
                nuevoNodoGhost2->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            if(nuevoNodoGhost3->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost3->setHasPower(false);
                nuevoNodoGhost3->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost2Row = (ghost2->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost2Col = (ghost2->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost3Row = (ghost3->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost3Col = (ghost3->getCurrentPosition()->getCol())* anchoCelda;
            //Se actualizará la posición de los fanstasmas en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            ghost2->setPos(currentGhost2Col, currentGhost2Row);
            ghost3->setPos(currentGhost3Col, currentGhost3Row);
            //Todo el codigo para matar al pacman/fantasma en caso de que este acabara de morir
            Nodo* pacmanActualNodo = pacman->getCurrentPosition();
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost1 == pacmanActualNodo) && (ghost1->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost1->setDeath(true);
                ghost1->hide();
                timerGhost1 = new QTimer(this);
                timerGhost1->setSingleShot(true);
                connect(timerGhost1, &QTimer::timeout, this, &Game::respawnGhost1);
                timerGhost1->start(5000); // Comienza el timer para 5 segundos
            }
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost2 == pacmanActualNodo) && (ghost2->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost2->setDeath(true);
                ghost2->hide();
                timerGhost2 = new QTimer(this);
                timerGhost2->setSingleShot(true);
                connect(timerGhost2, &QTimer::timeout, this, &Game::respawnGhost2);
                timerGhost2->start(5000); // Comienza el timer para 5 segundos
            }
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost3 == pacmanActualNodo) && (ghost3->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost3->setDeath(true);
                ghost3->hide();
                timerGhost3 = new QTimer(this);
                timerGhost3->setSingleShot(true);
                connect(timerGhost3, &QTimer::timeout, this, &Game::respawnGhost3);
                timerGhost3->start(5000); // Comienza el timer para 5 segundos
            }
            if ((((nuevoNodoGhost1 == pacmanActualNodo) && (pacmanDeath == false))||((nuevoNodoGhost2 == pacmanActualNodo) && (pacmanDeath == false))||((nuevoNodoGhost3 == pacmanActualNodo) && (pacmanDeath == false)))&& (pacman->getPowerOn() == false)){
                playDeathSound();
                pacmanDeath = true;
                this->getCurrentNivel()->getPacman()->setDirection(0);
                pacman->loseLife();
                if(pacman->getLives() == 0){
                    gameOver();
                }
                pacman->hide();
                timerPacman = new QTimer(this);
                timerPacman->setSingleShot(true); // Este timer solo se activará una vez
                connect(timerPacman, &QTimer::timeout, this, &Game::respawnPacMan);
                timerPacman->start(pacman->getReloadTime()); // Comienza el timer para 3 segundos (3000 milisegundos)
            }
        }else{
            Ghost* ghost1 = ghosts[0];
            Ghost* ghost2 = ghosts[1];
            Ghost* ghost3 = ghosts[2];
            Ghost* ghost4 = ghosts[3];
            int newRowG1 = ghost1->getCurrentPosition()->getRow();
            int newColG1 = ghost1->getCurrentPosition()->getCol();
            int newRowG2 = ghost2->getCurrentPosition()->getRow();
            int newColG2 = ghost2->getCurrentPosition()->getCol();
            int newRowG3 = ghost3->getCurrentPosition()->getRow();
            int newColG3 = ghost3->getCurrentPosition()->getCol();
            int newRowG4 = ghost4->getCurrentPosition()->getRow();
            int newColG4 = ghost4->getCurrentPosition()->getCol();
            int directionG1;
            int directionG2;
            int directionG3;
            int directionG4;
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost1->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost1->getCurrentPosition())){
                directionG1 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo A* para el fanstasma 1:";
                    directionG1 = ghost1->getDirectionPowerA(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG1 = ghost1->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost2->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost2->getCurrentPosition())){
                directionG2 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo Backtracking para el fanstasma 2:";
                    directionG2 = ghost2->getDirectionPowerB(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG2 = ghost2->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost3->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost3->getCurrentPosition())){
                directionG3 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo A* para el fanstasma 3:";
                    directionG3 = ghost3->getDirectionPowerA(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost3->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG3 = ghost3->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost3->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            if((this->getCurrentNivel()->getPacman()->getCurrentPosition() == ghost4->getCurrentPosition()) || (pacmanDeath == true) || (this->getCurrentNivel()->getFirstPowerUpNode() == ghost4->getCurrentPosition())){
                directionG4 = 0;
            }else{
                if(this->getPowerUpActive() == true){
                    qDebug() << "Algoritmo Backtracking para el fanstasma 4:";
                    directionG4 = ghost4->getDirectionPowerB(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getFirstPowerUpNode(),ghost4->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }else{
                    directionG4 = ghost4->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost4->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
                }
            }
            //Codigo para actualizar la posicion de los fantasmas:
            if((directionG1 == 1) && (ghost1->getDeath()==false)){
                newColG1--;
            }else if((directionG1 == 2) && (ghost1->getDeath()==false)){
                newRowG1--;
            }else if((directionG1 == 3) && (ghost1->getDeath()==false)){
                newColG1++;
            }else if((directionG1 == 4) && (ghost1->getDeath()==false)){
                newRowG1++;
            }else{
            }
            if((directionG2 == 1) && (ghost2->getDeath()==false)){
                newColG2--;
            }else if((directionG2 == 2) && (ghost2->getDeath()==false)){
                newRowG2--;
            }else if((directionG2 == 3) && (ghost2->getDeath()==false)){
                newColG2++;
            }else if((directionG2 == 4) && (ghost2->getDeath()==false)){
                newRowG2++;
            }else{
            }
            if((directionG3 == 1)&& (ghost3->getDeath()==false)){
                newColG3--;
            }else if((directionG3 == 2) && (ghost3->getDeath()==false)){
                newRowG3--;
            }else if((directionG3 == 3) && (ghost3->getDeath()==false)){
                newColG3++;
            }else if((directionG3 == 4) && (ghost3->getDeath()==false)){
                newRowG3++;
            }else{
            }
            if((directionG4 == 1) && (ghost4->getDeath()==false)){
                newColG4--;
            }else if((directionG4 == 2) && (ghost4->getDeath()==false)){
                newRowG4--;
            }else if((directionG4 == 3) && (ghost4->getDeath()==false)){
                newColG4++;
            }else if((directionG4 == 4) && (ghost4->getDeath()==false)){
                newRowG4++;
            }else{
            }
            Nodo* nuevoNodoGhost1 = this->getCurrentNivel()->getNode(newRowG1, newColG1);
            ghost1->setCurrentPosition(nuevoNodoGhost1);
            Nodo* nuevoNodoGhost2 = this->getCurrentNivel()->getNode(newRowG2, newColG2);
            ghost2->setCurrentPosition(nuevoNodoGhost2);
            Nodo* nuevoNodoGhost3 = this->getCurrentNivel()->getNode(newRowG3, newColG3);
            ghost3->setCurrentPosition(nuevoNodoGhost3);
            Nodo* nuevoNodoGhost4 = this->getCurrentNivel()->getNode(newRowG4, newColG4);
            ghost4->setCurrentPosition(nuevoNodoGhost4);
            if(nuevoNodoGhost1->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost1->setHasPower(false);
                nuevoNodoGhost1->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            if(nuevoNodoGhost2->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost2->setHasPower(false);
                nuevoNodoGhost2->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            if(nuevoNodoGhost3->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost3->setHasPower(false);
                nuevoNodoGhost3->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            if(nuevoNodoGhost4->getHasPower() == true){
                this->setPowerUps(this->getPowerUps()-1);
                this->getCurrentNivel()->deleteFirstPowerUpNode();
                nuevoNodoGhost4->setHasPower(false);
                nuevoNodoGhost4->setHasFood(false);
                this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()-1);
                if(this->getPowerUps() == 0){
                    this->setPowerUpActive(false);
                }
            }
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost2Row = (ghost2->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost2Col = (ghost2->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost3Row = (ghost3->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost3Col = (ghost3->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost4Row = (ghost4->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost4Col = (ghost4->getCurrentPosition()->getCol())* anchoCelda;
            //Se actualizará la posición de los fanstasmas en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            ghost2->setPos(currentGhost2Col, currentGhost2Row);
            ghost3->setPos(currentGhost3Col, currentGhost3Row);
            ghost4->setPos(currentGhost4Col, currentGhost4Row);
            //Todo el codigo para matar al pacman en caso de que este acabara de morir
            Nodo* pacmanActualNodo = pacman->getCurrentPosition();
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost1 == pacmanActualNodo) && (ghost1->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost1->setDeath(true);
                ghost1->hide();
                timerGhost1 = new QTimer(this);
                timerGhost1->setSingleShot(true);
                connect(timerGhost1, &QTimer::timeout, this, &Game::respawnGhost1);
                timerGhost1->start(5000); // Comienza el timer para 5 segundos
            }
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost2 == pacmanActualNodo) && (ghost2->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost2->setDeath(true);
                ghost2->hide();
                timerGhost2 = new QTimer(this);
                timerGhost2->setSingleShot(true);
                connect(timerGhost2, &QTimer::timeout, this, &Game::respawnGhost2);
                timerGhost2->start(5000); // Comienza el timer para 5 segundos
            }
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost3 == pacmanActualNodo) && (ghost3->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost3->setDeath(true);
                ghost3->hide();
                timerGhost3 = new QTimer(this);
                timerGhost3->setSingleShot(true);
                connect(timerGhost3, &QTimer::timeout, this, &Game::respawnGhost3);
                timerGhost3->start(5000); // Comienza el timer para 5 segundos
            }
            if ((pacman->getPowerOn() == true) && (nuevoNodoGhost4 == pacmanActualNodo) && (ghost4->getDeath() == false)){
                playPacmanEatSound();
                puntos = puntos + 50; //Se suma un extra de 50pts
                ghost4->setDeath(true);
                ghost4->hide();
                timerGhost4 = new QTimer(this);
                timerGhost4->setSingleShot(true);
                connect(timerGhost4, &QTimer::timeout, this, &Game::respawnGhost4);
                timerGhost4->start(5000); // Comienza el timer para 5 segundos
            }
            if ((((nuevoNodoGhost1 == pacmanActualNodo) && (pacmanDeath == false))||((nuevoNodoGhost2 == pacmanActualNodo) && (pacmanDeath == false))||((nuevoNodoGhost3 == pacmanActualNodo) && (pacmanDeath == false))||((nuevoNodoGhost4 == pacmanActualNodo) && (pacmanDeath == false))) && (pacman->getPowerOn() == false)){
                playDeathSound();
                pacmanDeath = true;
                this->getCurrentNivel()->getPacman()->setDirection(0);
                pacman->loseLife();
                if(pacman->getLives() == 0){
                    gameOver();
                }
                pacman->hide();
                timerPacman = new QTimer(this);
                timerPacman->setSingleShot(true); // Este timer solo se activará una vez
                connect(timerPacman, &QTimer::timeout, this, &Game::respawnPacMan);
                timerPacman->start(pacman->getReloadTime()); // Comienza el timer para 3 segundos (3000 milisegundos)
            }
        }
        // Actualizar los objetos
        this->getScene()->update();
    }
    if(this->puntos >= pointsForActive){
        this->setPowerUpActive(true);
        this->setPowerUps(this->getPowerUps()+1);
        pointsForActive = pointsForActive + 200;
        //Codigo para agarrar un nodo alejado y  establecerlo como el nuevo nodo con power up...
        Nodo* nodoAway = farAwayNode();
        this->getCurrentNivel()->addPowerUpNode(nodoAway);
        nodoAway->setHasPower(true);
        if (nodoAway->getHasFood() == true){
        }else{
            nodoAway->setHasFood(true);
            this->getCurrentNivel()->setComidaRestante(this->getCurrentNivel()->getComidaRestante()+1);
        }
    }
    // Actualizar el texto del puntaje
    scoreText->setPlainText(QString("Score: %1").arg(this->puntos));
    // Actualizar el texto de las vidas
    livesText->setPlainText(QString("Lives: %1").arg(this->getCurrentNivel()->getPacman()->getLives()));
    // Actualizar el texto del nivel
    levelText->setPlainText(QString("Level: %1").arg(this->getCurrentNivel()->getCurrentLevel()));
}

void Game::cambiaNivel() {
    if (this->getCurrentNivel()->getComidaRestante() == 0) {
        int currentLevel = this->getCurrentNivel()->getCurrentLevel();
        // Incrementa el nivel actual
        currentLevel++;
        if(currentLevel == 5){
            backgroundMusic.stop();
            playVictorySound();
            int id = QFontDatabase::addApplicationFont("fonts/Joystix.TTF");
            QString family = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont retroFont(family);
            QMessageBox victoryMessage;
            victoryMessage.setFont(retroFont);
            victoryMessage.setWindowTitle("!VICTORY!");
            victoryMessage.setText(QString("WINNER\n TOTAL POINTS = %1").arg(puntos));
            victoryMessage.exec();
            // Cerrar el programa
            QCoreApplication::quit();
        }else{
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
}

void Game::deactivatePower() {
    Pacman* pacman = this->getCurrentNivel()->getPacman();
    pacman->setPowerOn(false);
    stopPowerMusic();
    playBackgroundMusic();
    delete timerPower; // Eliminar el timer
    timerPower = nullptr; // Asegurarse de que el puntero del timer esté a nullptr
}


void Game::keyPressEvent(QKeyEvent* event) {
    int valor = 0;
    if(pacmanDeath == true){
        this->getCurrentNivel()->getPacman()->setDirection(0);
    }else{
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
}


void Game::respawnPacMan() {
    Pacman* pacman = this->getCurrentNivel()->getPacman();
    pacman->show(); // Hacemos reaparecer al PacMan
    pacmanDeath = false; // Resetear el estado de muerte del PacMan
    pacman->setCurrentPosition(farAwayNode());
    delete timerPacman; // Eliminar el timer
    timerPacman = nullptr; // Asegurarse de que el puntero del timer esté a nullptr
}

void Game::respawnGhost1() {
    ///Codigo para agarrar al fantasma 1 y asignarle una nueva posicion y luego mostrarlo
    Ghost* ghost1 = (this->getCurrentNivel()->getGhosts())[0];
    ghost1->setDeath(false);
    ghost1->show();
    ghost1->setCurrentPosition(farAwayNode());
    delete timerGhost1; // Eliminar el timer
    timerGhost1 = nullptr; // Asegurarse de que el puntero del timer esté a nullptr
}

void Game::respawnGhost2() {
    ///Codigo para agarrar al fantasma 1 y asignarle una nueva posicion y luego mostrarlo
    Ghost* ghost2 = (this->getCurrentNivel()->getGhosts())[1];
    ghost2->setDeath(false);
    ghost2->show();
    ghost2->setCurrentPosition(farAwayNode());
    delete timerGhost2; // Eliminar el timer
    timerGhost2 = nullptr; // Asegurarse de que el puntero del timer esté a nullptr
}

void Game::respawnGhost3() {
    ///Codigo para agarrar al fantasma 1 y asignarle una nueva posicion y luego mostrarlo
    Ghost* ghost3 = (this->getCurrentNivel()->getGhosts())[2];
    ghost3->setDeath(false);
    ghost3->show();
    ghost3->setCurrentPosition(farAwayNode());
    delete timerGhost3; // Eliminar el timer
    timerGhost3 = nullptr; // Asegurarse de que el puntero del timer esté a nullptr
}

void Game::respawnGhost4() {
    ///Codigo para agarrar al fantasma 1 y asignarle una nueva posicion y luego mostrarlo
    Ghost* ghost4 = (this->getCurrentNivel()->getGhosts())[3];
    ghost4->setDeath(false);
    ghost4->show();
    ghost4->setCurrentPosition(farAwayNode());
    delete timerGhost4; // Eliminar el timer
    timerGhost4 = nullptr; // Asegurarse de que el puntero del timer esté a nullptr
}

int* Game::nodeAway(int* puntos, int** matrizBase, int x_Max, int y_Max, int arraySize) {
    // inicializar la distancia máxima a -1 y las coordenadas del punto más alejado a -1
    double maxDistance = -1;
    static int farthestPoint[2] = {-1, -1};
    // recorrer la matriz
        for (int j = 0; j < y_Max; j++) {
        for (int i = 0; i < x_Max; i++) {
            // si el valor de la matriz es 0
            if (matrizBase[j][i] == 0) {
                // calcular la distancia desde el punto actual a los puntos proporcionados
                double dist = distance(puntos[0], puntos[1], i, j);
                if (arraySize == 4) {
                    double dist2 = distance(puntos[2], puntos[3], i, j);
                    dist = (dist < dist2) ? dist : dist2;
                }
                // si la distancia es mayor que la distancia máxima registrada hasta ahora
                if (dist > maxDistance) {
                    // actualizar la distancia máxima y las coordenadas del punto más alejado
                    maxDistance = dist;
                    farthestPoint[0] = i;
                    farthestPoint[1] = j;
                }
            }
        }
    }
    // devolver las coordenadas del punto más alejado
    return farthestPoint;
}


Nodo* Game::farAwayNode() {
    std::vector<Ghost*> ghosts = this->getCurrentNivel()->getGhosts();
    int currentLevel = this->getCurrentNivel()->getCurrentLevel();
    if(currentLevel == 1){
        Ghost* ghost1 = ghosts[0];
        int xG1 = ghost1->getCurrentPosition()->getCol();
        int yG1 = ghost1->getCurrentPosition()->getRow();
        int arrayG1[2] = {xG1,yG1};
        int* ptrArray = arrayG1;
        int** matrizBase = this->getCurrentNivel()->getCurrentMatriz();
        int x_Max = this->getCurrentNivel()->getColumns();
        int y_Max = this->getCurrentNivel()->getRows();
        int arraySize = 2;
        int* arrayResult = nodeAway(ptrArray, matrizBase, x_Max, y_Max, arraySize);
        int xFinal = arrayResult[0];
        int yFinal = arrayResult[1];
        Nodo* nodo = this->getCurrentNivel()->getNode(yFinal,xFinal);
        return nodo;
    }else if(currentLevel == 2){
        Ghost* ghost1 = ghosts[0];
        Ghost* ghost2 = ghosts[1];
        int xG1 = ghost1->getCurrentPosition()->getCol();
        int yG1 = ghost1->getCurrentPosition()->getRow();
        int xG2 = ghost2->getCurrentPosition()->getCol();
        int yG2 = ghost2->getCurrentPosition()->getRow();
        int arrayG2[4] = {xG1,yG1,xG2,yG2};
        int* ptrArray = arrayG2;
        int** matrizBase = this->getCurrentNivel()->getCurrentMatriz();
        int x_Max = this->getCurrentNivel()->getColumns();
        int y_Max = this->getCurrentNivel()->getRows();
        int arraySize = 4;
        int* arrayResult = nodeAway(ptrArray, matrizBase, x_Max, y_Max, arraySize);
        // Acceder a los valores del arreglo
        int xFinal = arrayResult[0];
        int yFinal = arrayResult[1];
        Nodo* nodo = this->getCurrentNivel()->getNode(yFinal,xFinal);
        return nodo;
    }else if(currentLevel == 3){
        Ghost* ghost1 = ghosts[0];
        Ghost* ghost2 = ghosts[1];
        Ghost* ghost3 = ghosts[2];
        int xG1 = ghost1->getCurrentPosition()->getCol();
        int yG1 = ghost1->getCurrentPosition()->getRow();
        int xG2 = ghost2->getCurrentPosition()->getCol();
        int yG2 = ghost2->getCurrentPosition()->getRow();
        int xG3 = ghost3->getCurrentPosition()->getCol();
        int yG3 = ghost3->getCurrentPosition()->getRow();
        int arrayG3[6] = {xG1,yG1,xG2,yG2,xG3,yG3};
        int* ptrArray = arrayG3;
        int** matrizBase = this->getCurrentNivel()->getCurrentMatriz();
        int x_Max = this->getCurrentNivel()->getColumns();
        int y_Max = this->getCurrentNivel()->getRows();
        int arraySize = 6;
        int* arrayResult = nodeAway(ptrArray, matrizBase, x_Max, y_Max, arraySize);
        // Acceder a los valores del arreglo
        int xFinal = arrayResult[0];
        int yFinal = arrayResult[1];
        Nodo* nodo = this->getCurrentNivel()->getNode(yFinal,xFinal);
        return nodo;
    }else{
        Ghost* ghost1 = ghosts[0];
        Ghost* ghost2 = ghosts[1];
        Ghost* ghost3 = ghosts[2];
        Ghost* ghost4 = ghosts[3];
        int xG1 = ghost1->getCurrentPosition()->getCol();
        int yG1 = ghost1->getCurrentPosition()->getRow();
        int xG2 = ghost2->getCurrentPosition()->getCol();
        int yG2 = ghost2->getCurrentPosition()->getRow();
        int xG3 = ghost3->getCurrentPosition()->getCol();
        int yG3 = ghost3->getCurrentPosition()->getRow();
        int xG4 = ghost4->getCurrentPosition()->getCol();
        int yG4 = ghost4->getCurrentPosition()->getRow();
        int arrayG4[8] = {xG1,yG1,xG2,yG2,xG3,yG3,xG4,yG4};
        int* ptrArray = arrayG4;
        int** matrizBase = this->getCurrentNivel()->getCurrentMatriz();
        int x_Max = this->getCurrentNivel()->getColumns();
        int y_Max = this->getCurrentNivel()->getRows();
        int arraySize = 8;
        int* arrayResult = nodeAway(ptrArray, matrizBase, x_Max, y_Max, arraySize);
        // Acceder a los valores del arreglo
        int xFinal = arrayResult[0];
        int yFinal = arrayResult[1];
        Nodo* nodo = this->getCurrentNivel()->getNode(yFinal,xFinal);
        return nodo;
    }
}

// En otro lugar en tu clase
void Game::gameOver() {
    backgroundMusic.stop();
    int id = QFontDatabase::addApplicationFont("fonts/Joystix.TTF");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont retroFont(family);
    QMessageBox gameOveryMessage;
    gameOveryMessage.setFont(retroFont);
    gameOveryMessage.setWindowTitle("GAME OVER");
    gameOveryMessage.setText(QString("GAME OVER\n TOTAL POINTS = %1").arg(puntos));
    gameOveryMessage.exec();
    // Cerrar el programa
    QCoreApplication::quit();
}

void Game::setFirstGeneration(bool newValue) {
    this->firstGeneration = newValue;
}

bool Game::getFirstGeneration() {
    return this->firstGeneration;
}

void Game::setPacmanDeath(bool newValue) {
    this->pacmanDeath = newValue;
}

bool Game::getPacmanDeath() {
    return this->pacmanDeath;
}

void Game::setPowerUpActive(bool newValue) {
    this->powerUpActive = newValue;
}

bool Game::getPowerUpActive() {
    return this->powerUpActive;
}

void Game::setPointsForActive(int newValue) {
    this->pointsForActive = newValue;
}

bool Game::getPointsForActive() {
    return this->pointsForActive;
}

void Game::setPowerUps(int newValue) {
    this->powerUps = newValue;
}

int Game::getPowerUps() {
    return this->powerUps;
}

void Game::playBackgroundMusic() {
    backgroundMusic.play();
}

void Game::stopBackgroundMusic() {
    backgroundMusic.stop();
}

void Game::playPacmanEatSound() {
    pacmanEatSound.play();
}

void Game::playVictorySound() {
    victorySound.play();
}

void Game::playPowerMusic() {
    powerUpMusic.play();
}

void Game::stopPowerMusic() {
    powerUpMusic.stop();
}

void Game::playDeathSound() {
    deathSound.play();
}