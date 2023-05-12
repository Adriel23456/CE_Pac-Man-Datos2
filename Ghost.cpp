#include "Ghost.h"
#include <vector>
#include <queue>

Ghost::Ghost(Nodo* currentPosition): direction(0), currentPosition(currentPosition), reloadTime(8000){}   

Ghost::~Ghost() {
    // No hay nada que hacer aquí porque no hay asignacion de memoria dinamica...
}

void Ghost::setDirection(int newDirection){
    this->direction = newDirection;
}

// Definimos la estructura de un nodo (celda de la matriz)
struct Node {
    int x;
    int y;
    int cost;
    Node *parent;
    Node(int x, int y, Node *parent = nullptr)
        : x(x), y(y), cost(INT_MAX), parent(parent) {}
};

// Comparador para la cola de prioridad
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->cost > b->cost;
    }
};

// Función para obtener la dirección del primer movimiento
int getFirstDirection(Node* start, Node* end) {
    Node* currentNode = end;
    while(currentNode->parent != start) {
        currentNode = currentNode->parent;
    }
    if(currentNode->x < start->x) {
        return 1;
    } else if(currentNode->y > start->y) {
        return 2;
    } else if(currentNode->x > start->x) {
        return 3;
    } else { // currentNode->y < start->y
        return 4;
    }
}

int Ghost::getDirectionPacMan(int** matriz, Nodo* final, Nodo* inicio, int rows, int columns){
    int xStart = inicio->getCol();
    int yStart = inicio->getRow();
    int xFinish = final->getCol();
    int yFinish = final->getRow();
    int max_xMatriz = columns;
    int max_yMatriz = rows;
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    Node* start = new Node(xStart, yStart);
    start->cost = 0;

    pq.push(start);

    while(!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        // Si hemos llegado al destino, devolvemos la dirección del primer movimiento
        if(current->x == xFinish && current->y == yFinish) {
            int direction = getFirstDirection(start, current);
            delete start;
            return direction;
        }

        // Visitamos los vecinos
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                if(i == 0 && j == 0) continue;

                int newX = current->x + i;
                int newY = current->y + j;

                // Verificamos si la nueva posición está dentro de los límites y si es una celda transitable
                if(newX >= 0 && newX < max_xMatriz && newY >= 0 && newY < max_yMatriz && matriz[newX][newY] == 0) {
                    Node* neighbor = new Node(newX, newY, current);
                    neighbor->cost = current->cost + 1; // Asumimos un costo de 1 para cada movimiento

                    pq.push(neighbor);
                }
            }
        }
    }
    delete start;
    return -1; // Si no se encontró un camino, devolvemos -1
}

void Ghost::setCurrentPosition(Nodo* newCurrentPosition){
    this->currentPosition = newCurrentPosition;
}

Nodo* Ghost::getCurrentPosition(){
    return currentPosition;
}

int Ghost::getReloadTime() const{
    return reloadTime;
}