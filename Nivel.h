#pragma once

#include "Nodo.h"
#include "Pacman.h"
#include "Ghost.h"
#include <vector>

//Esta clase va a tener los metodos para recargar al jugador, los nodos del nivel (En base a su estado) y los enemigos
//Esta clase va a tener los metodos para la generacion de cada nivel

class Nivel {
private:
    int currentLevel; //Nivel actual
    std::vector<Nodo*> matrizNodos; //Vector de nodos del nivel actual
    Nodo* nodoPrincipio; //Nodo donde inicia el jugador
    std::vector<Ghost*> enemigos; //Nodos donde iniciarian los enemigos
    std::vector<Nodo*> powerUpNodes; //Nodos con las casillas de los superpoderes...
    Pacman* pacman; //El jugador que se genera para este nivel
    int** currentMatriz; //Puntero total de la matriz actual...
    int comidaRestante; //Cuantas celdas con comida quedan...
    int rows; //Filas de este nivel
    int columns; //Columnas de este nivel

public:
    Nivel(); //Constructor del nivel 1
    Nivel(int newLevel); //Constructor de los demas niveles
    ~Nivel(); //Destructor predeterminado

    int** getCurrentMatriz(); //Metodo para obtener la matriz con la que esta el nivel
    Nodo* getNode(int row, int col); //Metodo para obtener el puntero del nodo dada una posicion (x,y) en la matriz
    int getRows(); //Obtener las filas del nivel
    int getColumns(); //Obtener las columnas del nivel
    int getCurrentLevel(); //Obtener el nivel actual
    int getComidaRestante(); //Obtener la comida restante del nivel
    void setComidaRestante(int newComida);
    Nodo* getNodoPrincipio(); //Obtener el nodo de la posicion inicial del pacman
    std::vector<Ghost*> getGhosts(); //Obtener la lista de nodos del nivel actual...
    std::vector<Nodo*> getPowerUpNodes(); //Obtener la lista de nodos del nivel actual...
    void addPowerUpNode(Nodo* newNodo); //Metodo para agregarle los nodos con los superpodetes al vector de nodos de superpoderes...
    void deleteFirstPowerUpNode(); //Metodo para agregarle los nodos con los superpodetes al vector de nodos de superpoderes...
    Nodo* getFirstPowerUpNode(); //Metodo para obtener el primer nodo de la lista de nodos
    Pacman* getPacman(); //Obtener al objeto QT pacman
};