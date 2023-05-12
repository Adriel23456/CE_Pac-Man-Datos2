#include "Game.h"
#include "Nivel.h"
#include "Pacman.h"
#include "Ghost.h"
#include <QDebug>
#include <QFontDatabase>
#include <QMessageBox>
#include <QCoreApplication>

Game::Game(QWidget* parent): QGraphicsView(parent) {
    //Datos de inicio basicos
    this->nivel = new Nivel();
    this->puntos = 0;
    this->setFixedSize(800, 500);
    this->setWindowTitle("CEPac-Man");
    this->firstGeneration = true;
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
    timer->start(1000); // Actualiza cada 1s
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
    // Comprueba si se debe cambiar de nivel
    this->cambiaNivel();
    if(this->getFirstGeneration() == true){
        //Se establecen las alturas y anchuras relativas del nivel actual
        int anchoCelda = this->width() / this->getCurrentNivel()->getColumns();
        int altoCelda = this->height() / this->getCurrentNivel()->getRows();
        // Cambia el tamaño de las imagenes para que quepan en un celda del nivel actual
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
                this->puntos += 10;
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
            }else{
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
        //Codigo para actualizar a los fantasmas...
        int currentlevelGame = this->getCurrentNivel()->getCurrentLevel();
        std::vector<Ghost*> ghosts = this->getCurrentNivel()->getGhosts();
        if (currentlevelGame == 1){
            Ghost* ghost1 = ghosts[0];
            int newRowG1 = ghost1->getCurrentPosition()->getRow();
            int newColG1 = ghost1->getCurrentPosition()->getCol();
            qDebug() << "Se va a intentar aplicar el codigo de obtencion de una nueva direccion";
            int directionG1 = ghost1->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            //Codigo para actualizar la posicion de los fantasmas:
            if(directionG1 == 0){
            }else if(directionG1 == 1){
                newColG1--;
            }else if(directionG1 == 2){
                newRowG1--;
            }else if(directionG1 == 3){
                newColG1++;
            }else{
                newRowG1++;
            }
            Nodo* nuevoNodoGhost1 = this->getCurrentNivel()->getNode(newRowG1, newColG1);
            ghost1->setCurrentPosition(nuevoNodoGhost1);
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            //Se actualizará la posición de los fanstasmas en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
        }else if(currentlevelGame == 2){
            Ghost* ghost1 = ghosts[0];
            Ghost* ghost2 = ghosts[1];
            int newRowG1 = ghost1->getCurrentPosition()->getRow();
            int newColG1 = ghost1->getCurrentPosition()->getCol();
            int newRowG2 = ghost2->getCurrentPosition()->getRow();
            int newColG2 = ghost2->getCurrentPosition()->getCol();
            qDebug() << "Se va a intentar aplicar el codigo de obtencion de una nueva direccion";
            int directionG1 = ghost1->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            int directionG2 = ghost2->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            //Codigo para actualizar la posicion de los fantasmas:
            if(directionG1 == 0){
            }else if(directionG1 == 1){
                newColG1--;
            }else if(directionG1 == 2){
                newRowG1--;
            }else if(directionG1 == 3){
                newColG1++;
            }else{
                newRowG1++;
            }
            if(directionG2 == 0){
            }else if(directionG2 == 1){
                newColG2--;
            }else if(directionG2 == 2){
                newRowG2--;
            }else if(directionG2 == 3){
                newColG2++;
            }else{
                newRowG2++;
            }
            Nodo* nuevoNodoGhost1 = this->getCurrentNivel()->getNode(newRowG1, newColG1);
            ghost1->setCurrentPosition(nuevoNodoGhost1);
            Nodo* nuevoNodoGhost2 = this->getCurrentNivel()->getNode(newRowG2, newColG2);
            ghost2->setCurrentPosition(nuevoNodoGhost2);
            int currentGhost1Row = (ghost1->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost1Col = (ghost1->getCurrentPosition()->getCol())* anchoCelda;
            int currentGhost2Row = (ghost2->getCurrentPosition()->getRow()) * altoCelda;
            int currentGhost2Col = (ghost2->getCurrentPosition()->getCol())* anchoCelda;
            //Se actualizará la posición de los fanstasmas en la escena:
            ghost1->setPos(currentGhost1Col, currentGhost1Row);
            ghost2->setPos(currentGhost2Col, currentGhost2Row);
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
            qDebug() << "Se va a intentar aplicar el codigo de obtencion de una nueva direccion";
            int directionG1 = ghost1->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            int directionG2 = ghost2->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            int directionG3 = ghost3->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost3->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            //Codigo para actualizar la posicion de los fantasmas:
            if(directionG1 == 0){
            }else if(directionG1 == 1){
                newColG1--;
            }else if(directionG1 == 2){
                newRowG1--;
            }else if(directionG1 == 3){
                newColG1++;
            }else{
                newRowG1++;
            }
            if(directionG2 == 0){
            }else if(directionG2 == 1){
                newColG2--;
            }else if(directionG2 == 2){
                newRowG2--;
            }else if(directionG2 == 3){
                newColG2++;
            }else{
                newRowG2++;
            }
            if(directionG3 == 0){
            }else if(directionG3 == 1){
                newColG3--;
            }else if(directionG3 == 2){
                newRowG3--;
            }else if(directionG3 == 3){
                newColG3++;
            }else{
                newRowG3++;
            }
            Nodo* nuevoNodoGhost1 = this->getCurrentNivel()->getNode(newRowG1, newColG1);
            ghost1->setCurrentPosition(nuevoNodoGhost1);
            Nodo* nuevoNodoGhost2 = this->getCurrentNivel()->getNode(newRowG2, newColG2);
            ghost2->setCurrentPosition(nuevoNodoGhost2);
            Nodo* nuevoNodoGhost3 = this->getCurrentNivel()->getNode(newRowG3, newColG3);
            ghost3->setCurrentPosition(nuevoNodoGhost3);
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
            qDebug() << "Se va a intentar aplicar el codigo de obtencion de una nueva direccion";
            int directionG1 = ghost1->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost1->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            int directionG2 = ghost2->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost2->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            int directionG3 = ghost3->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost3->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            int directionG4 = ghost4->getDirectionPacMan(this->getCurrentNivel()->getCurrentMatriz(),this->getCurrentNivel()->getPacman()->getCurrentPosition(),ghost4->getCurrentPosition(), this->getCurrentNivel()->getRows(), this->getCurrentNivel()->getColumns());
            //Codigo para actualizar la posicion de los fantasmas:
            if(directionG1 == 0){
            }else if(directionG1 == 1){
                newColG1--;
            }else if(directionG1 == 2){
                newRowG1--;
            }else if(directionG1 == 3){
                newColG1++;
            }else{
                newRowG1++;
            }
            if(directionG2 == 0){
            }else if(directionG2 == 1){
                newColG2--;
            }else if(directionG2 == 2){
                newRowG2--;
            }else if(directionG2 == 3){
                newColG2++;
            }else{
                newRowG2++;
            }
            if(directionG3 == 0){
            }else if(directionG3 == 1){
                newColG3--;
            }else if(directionG3 == 2){
                newRowG3--;
            }else if(directionG3 == 3){
                newColG3++;
            }else{
                newRowG3++;
            }
            if(directionG4 == 0){
            }else if(directionG4 == 1){
                newColG4--;
            }else if(directionG4 == 2){
                newRowG4--;
            }else if(directionG4 == 3){
                newColG4++;
            }else{
                newRowG4++;
            }
            Nodo* nuevoNodoGhost1 = this->getCurrentNivel()->getNode(newRowG1, newColG1);
            ghost1->setCurrentPosition(nuevoNodoGhost1);
            Nodo* nuevoNodoGhost2 = this->getCurrentNivel()->getNode(newRowG2, newColG2);
            ghost2->setCurrentPosition(nuevoNodoGhost2);
            Nodo* nuevoNodoGhost3 = this->getCurrentNivel()->getNode(newRowG3, newColG3);
            ghost3->setCurrentPosition(nuevoNodoGhost3);
            Nodo* nuevoNodoGhost4 = this->getCurrentNivel()->getNode(newRowG4, newColG4);
            ghost4->setCurrentPosition(nuevoNodoGhost4);
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

void Game::playBackgroundMusic() {
    backgroundMusic.play();
}

void Game::playPacmanEatSound() {
    pacmanEatSound.play();
}

void Game::playVictorySound() {
    victorySound.play();
}

void Game::playDeathSound() {
    deathSound.play();
}