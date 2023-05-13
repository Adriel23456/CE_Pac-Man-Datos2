#include "Nivel.h"
#include "Pacman.h"
#include "Game.h"

//Metodo para contar los ceros en las matrices
int countZeros(int* matriz, int rows, int cols) {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matriz[i * cols + j] == 0) {
                count++;
            }
        }
    }
    return count;
}

int** get2DArrayPointer1(int arr[7][10], int rows, int cols) {
    int** array2D = new int*[rows];
    for(int i = 0; i < rows; i++) {
        array2D[i] = new int[cols];
        for(int j = 0; j < cols; j++) {
            array2D[i][j] = arr[i][j];
        }
    }
    return array2D;
}

int** get2DArrayPointer2(int arr[14][20], int rows, int cols) {
    int** array2D = new int*[rows];
    for(int i = 0; i < rows; i++) {
        array2D[i] = new int[cols];
        for(int j = 0; j < cols; j++) {
            array2D[i][j] = arr[i][j];
        }
    }
    return array2D;
}

int** get2DArrayPointer3(int arr[12][33], int rows, int cols) {
    int** array2D = new int*[rows];
    for(int i = 0; i < rows; i++) {
        array2D[i] = new int[cols];
        for(int j = 0; j < cols; j++) {
            array2D[i][j] = arr[i][j];
        }
    }
    return array2D;
}

int** get2DArrayPointer4(int arr[19][29], int rows, int cols) {
    int** array2D = new int*[rows];
    for(int i = 0; i < rows; i++) {
        array2D[i] = new int[cols];
        for(int j = 0; j < cols; j++) {
            array2D[i][j] = arr[i][j];
        }
    }
    return array2D;
}

// Valores para level1
int level1Data[7][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

// Valores para level2
int level2Data[14][20] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

// Valores para level3
int level3Data[12][33] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

// Valores para level4
int level4Data[19][29] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

std::vector<Nodo*> initializeLevel1(){
    std::vector<Nodo*> level1;
    // Copia los valores de level1Data a la matriz level1 de objetos Nodo
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 10; ++j) {
            Nodo* nodo = new Nodo(level1Data[i][j], i,j);
            level1.push_back(nodo);
        }
    }
    return level1;
}

std::vector<Ghost*> initializeGhosts1(std::vector<Nodo*> matrizNodo, int columns){
    std::vector<Ghost*> ghosts1;
    //Se dejan claros los indices de aparición de los fantasmas
    int index1 = 1 * columns + 1;
    // Obtiene el nodo de los fantanstas, los genera en base a ese nodo y los añade a la lista...
    Nodo* nodo1 = matrizNodo[index1];
    Ghost* ghost1 = new Ghost(nodo1);
    ghosts1.push_back(ghost1);
    return ghosts1;
}

Nodo* getNodoPrincipio1(std::vector<Nodo*> matrizNodo, int columns){
    int index = 3 * columns + 4;
    Nodo* nodoPrincipio = matrizNodo[index];
    return nodoPrincipio;
}

std::vector<Nodo*> initializeLevel2(){
    std::vector<Nodo*> level2;
    // Copia los valores de level1Data a la matriz level1 de objetos Nodo
    for (int i = 0; i < 14; ++i) {
        for (int j = 0; j < 20; ++j) {
            Nodo* nodo = new Nodo(level2Data[i][j],i,j);
            level2.push_back(nodo);
        }
    }
    return level2;
}

std::vector<Ghost*> initializeGhosts2(std::vector<Nodo*> matrizNodo, int columns){
    std::vector<Ghost*> ghosts2;
    //Se dejan claros los indices de aparición de los fantasmas
    int index1 = 2 * columns + 1;
    int index2 = 12 * columns + 16;
    // Obtiene el nodo de los fantanstas, los genera en base a ese nodo y los añade a la lista...
    Nodo* nodo1 = matrizNodo[index1];
    Nodo* nodo2 = matrizNodo[index2];
    Ghost* ghost1 = new Ghost(nodo1);
    Ghost* ghost2 = new Ghost(nodo2);
    ghosts2.push_back(ghost1);
    ghosts2.push_back(ghost2);
    return ghosts2;
}

Nodo* getNodoPrincipio2(std::vector<Nodo*> matrizNodo, int columns){
    int index = 6 * columns + 10;
    Nodo* nodoPrincipio = matrizNodo[index];
    return nodoPrincipio;
}

std::vector<Nodo*> initializeLevel3(){
    std::vector<Nodo*> level3;
    // Copia los valores de level1Data a la matriz level1 de objetos Nodo
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 33; ++j) {
            Nodo* nodo = new Nodo(level3Data[i][j],i,j);
            level3.push_back(nodo);
        }
    }
    return level3;
}

std::vector<Ghost*> initializeGhosts3(std::vector<Nodo*> matrizNodo, int columns){
    std::vector<Ghost*> ghosts3;
    //Se dejan claros los indices de aparición de los fantasmas
    int index1 = 2 * columns + 1;
    int index2 = 10 * columns + 1;
    int index3 = 10 * columns + 31;
    // Obtiene el nodo de los fantanstas, los genera en base a ese nodo y los añade a la lista...
    Nodo* nodo1 = matrizNodo[index1];
    Nodo* nodo2 = matrizNodo[index2];
    Nodo* nodo3 = matrizNodo[index3];
    Ghost* ghost1 = new Ghost(nodo1);
    Ghost* ghost2 = new Ghost(nodo2);
    Ghost* ghost3 = new Ghost(nodo3);
    ghosts3.push_back(ghost1);
    ghosts3.push_back(ghost2);
    ghosts3.push_back(ghost3);
    return ghosts3;
}

Nodo* getNodoPrincipio3(std::vector<Nodo*> matrizNodo, int columns){
    int index = 6 * columns + 15;
    Nodo* nodoPrincipio = matrizNodo[index];
    return nodoPrincipio;
}

std::vector<Nodo*> initializeLevel4(){
    std::vector<Nodo*> level4;
    // Copia los valores de level1Data a la matriz level1 de objetos Nodo
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 29; ++j) {
            Nodo* nodo = new Nodo(level4Data[i][j], i,j);
            level4.push_back(nodo);
        }
    }
    return level4;
}

std::vector<Ghost*> initializeGhosts4(std::vector<Nodo*> matrizNodo, int columns){
    std::vector<Ghost*> ghosts4;
    //Se dejan claros los indices de aparición de los fantasmas
    int index1 = 1 * columns + 1;
    int index2 = 17 * columns + 1;
    int index3 = 1 * columns + 26;
    int index4 = 17 * columns + 26;
    // Obtiene el nodo de los fantanstas, los genera en base a ese nodo y los añade a la lista...
    Nodo* nodo1 = matrizNodo[index1];
    Nodo* nodo2 = matrizNodo[index2];
    Nodo* nodo3 = matrizNodo[index3];
    Nodo* nodo4 = matrizNodo[index4];
    Ghost* ghost1 = new Ghost(nodo1);
    Ghost* ghost2 = new Ghost(nodo2);
    Ghost* ghost3 = new Ghost(nodo3);
    Ghost* ghost4 = new Ghost(nodo4);
    ghosts4.push_back(ghost1);
    ghosts4.push_back(ghost2);
    ghosts4.push_back(ghost3);
    ghosts4.push_back(ghost4);
    return ghosts4;
}

Nodo* getNodoPrincipio4(std::vector<Nodo*> matrizNodo, int columns){
    int index = 9 * columns + 14;
    Nodo* nodoPrincipio = matrizNodo[index];
    return nodoPrincipio;
}

Nivel::Nivel(){
    this->currentLevel = 1;
    this->rows = 7; //rows
    this->columns = 10; //cols
    this->comidaRestante = 5;//countZeros(&level1Data[0][0], 7, 10);
    this->matrizNodos = initializeLevel1();
    this->enemigos = initializeGhosts1(matrizNodos, columns);
    this->nodoPrincipio = getNodoPrincipio1(matrizNodos, columns);
    this->pacman = new Pacman(nodoPrincipio);
    this->currentMatriz = get2DArrayPointer1(level1Data, rows, columns);
}

Nivel::Nivel(int newLevel){
    this->currentLevel = newLevel;
    if (currentLevel == 2){
        this->rows = 14;
        this->columns = 20;
        this->comidaRestante = 5;//countZeros(&level2Data[0][0], 14, 20);
        this->matrizNodos = initializeLevel2();
        this->enemigos = initializeGhosts2(matrizNodos, columns);
        this->nodoPrincipio = getNodoPrincipio2(matrizNodos, columns);
        this->pacman = new Pacman(nodoPrincipio);
        this->currentMatriz = get2DArrayPointer2(level2Data, rows, columns);
    }else if(currentLevel == 3){
        this->rows = 12;
        this->columns = 33;
        this->comidaRestante = 5;//countZeros(&level3Data[0][0], 12, 33);
        this->matrizNodos = initializeLevel3();
        this->enemigos = initializeGhosts3(matrizNodos, columns);
        this->nodoPrincipio = getNodoPrincipio3(matrizNodos, columns);
        this->pacman = new Pacman(nodoPrincipio);
        this->currentMatriz = get2DArrayPointer3(level3Data, rows, columns);
    }else{
        this->rows = 19;
        this->columns = 29;
        this->comidaRestante = 5;//countZeros(&level4Data[0][0], 19, 29);
        this->matrizNodos = initializeLevel4();
        this->enemigos = initializeGhosts4(matrizNodos, columns);
        this->nodoPrincipio = getNodoPrincipio4(matrizNodos, columns);
        this->pacman = new Pacman(nodoPrincipio);
        this->currentMatriz = get2DArrayPointer4(level4Data, rows, columns);
    }
}

Nivel::~Nivel(){
    delete pacman;
    for (Nodo* nodo : matrizNodos) {
        delete nodo;
    }
    for (Ghost* ghost : enemigos) {
        delete ghost;
    }
    if (currentLevel == 1){
        for(int i = 0; i < 7; i++) {
            delete[] currentMatriz[i];
        }
        delete[] currentMatriz;
    }else if (currentLevel == 2){
        for(int i = 0; i < 14; i++) {
            delete[] currentMatriz[i];
        }
        delete[] currentMatriz;
    }else if (currentLevel == 3){
        for(int i = 0; i < 12; i++) {
            delete[] currentMatriz[i];
        }
        delete[] currentMatriz;
    }else{
        for(int i = 0; i < 19; i++) {
            delete[] currentMatriz[i];
        }
        delete[] currentMatriz;
    }
}

Nodo* Nivel::getNode(int row, int col){
    int index = row * columns + col;
    return matrizNodos[index];
}

int Nivel::getRows(){
    return rows;
}

int Nivel::getColumns(){
    return columns;
}

int** Nivel::getCurrentMatriz(){
    return currentMatriz;
}

int Nivel::getCurrentLevel(){
    return currentLevel;
}

int Nivel::getComidaRestante(){
    return comidaRestante;
}

Nodo* Nivel::getNodoPrincipio(){
    return nodoPrincipio;
}

Pacman* Nivel::getPacman(){
    return pacman;
}

void Nivel::setComidaRestante(int newComida){
    comidaRestante = newComida;
}

std::vector<Ghost*> Nivel::getGhosts(){
    return enemigos;
}