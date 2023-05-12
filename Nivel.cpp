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
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
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

Nodo* getNodoPrincipio4(std::vector<Nodo*> matrizNodo, int columns){
    int index = 9 * columns + 14;
    Nodo* nodoPrincipio = matrizNodo[index];
    return nodoPrincipio;
}

Nivel::Nivel(){
    this->currentLevel = 1;
    this->rows = 7; //rows
    this->columns = 10; //cols
    this->comidaRestante = countZeros(&level1Data[0][0], 7, 10);
    this->matrizNodos = initializeLevel1();
    this->nodoPrincipio = getNodoPrincipio1(matrizNodos, columns);
    this->pacman = new Pacman(nodoPrincipio);
}

Nivel::Nivel(int newLevel){
    this->currentLevel = newLevel;
    if (currentLevel == 2){
        this->rows = 14;
        this->columns = 20;
        this->comidaRestante = countZeros(&level2Data[0][0], 14, 20);
        this->matrizNodos = initializeLevel2();
        this->nodoPrincipio = getNodoPrincipio2(matrizNodos, columns);
        this->pacman = new Pacman(nodoPrincipio);
    }else if(currentLevel == 3){
        this->rows = 12;
        this->columns = 33;
        this->comidaRestante = countZeros(&level3Data[0][0], 12, 33);
        this->matrizNodos = initializeLevel3();
        this->nodoPrincipio = getNodoPrincipio3(matrizNodos, columns);
        this->pacman = new Pacman(nodoPrincipio);
    }else{
        this->rows = 19;
        this->columns = 29;
        this->comidaRestante = countZeros(&level4Data[0][0], 19, 29);
        this->matrizNodos = initializeLevel4();
        this->nodoPrincipio = getNodoPrincipio4(matrizNodos, columns);
        this->pacman = new Pacman(nodoPrincipio);
    }
}

Nivel::~Nivel(){
    delete pacman;
    for (Nodo* nodo : matrizNodos) {
        delete nodo;
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

int* Nivel::getMatriz(){
    if(currentLevel == 1){
        return *level1Data;
    } else if(currentLevel == 2){
        return *level2Data;
    } else if(currentLevel == 3){
        return *level3Data;
    } else{
        return *level4Data;
    }
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