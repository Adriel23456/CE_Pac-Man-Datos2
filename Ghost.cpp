#include "Ghost.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <climits>
#include <cmath>

Ghost::Ghost(Nodo* currentPosition): direction(0), currentPosition(currentPosition), reloadTime(8000){}   

Ghost::~Ghost() {
    // No hay nada que hacer aquí porque no hay asignacion de memoria dinamica...
}

// Definimos la estructura de un nodo (celda de la matriz)
struct Node {
    int x;
    int y;
    int g; // Costo hasta ahora
    int h; // Heurística (estimación del costo para llegar al objetivo)
    Node *parent;
    Node(int x, int y, Node *parent = nullptr)
        : x(x), y(y), g(INT_MAX), h(INT_MAX), parent(parent) {}
};

// Comparador para la cola de prioridad
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->g + a->h > b->g + b->h;
    }
};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second); 
        return h1 ^ h2;  
    }
};

// Función para obtener la distancia de Manhattan
int manhattanDistance(int x1, int y1, int x2, int y2) {
    return std::abs(x2 - x1) + std::abs(y2 - y1);
}

// Función para obtener la dirección del primer movimiento
int getFirstDirection(Node* start, Node* end) {
    Node* currentNode = end;
    while(currentNode->parent != start) {
        currentNode = currentNode->parent;
    }
    if(currentNode->x < start->x) {
        return 1;
    } else if(currentNode->y < start->y) {
        return 2;
    } else if(currentNode->x > start->x) {
        return 3;
    } else if(currentNode->y > start->y) {
        return 4;
    }
    return 0;
}

int Ghost::getDirectionPacMan(int** matriz, Nodo* final, Nodo* inicio, int rows, int columns){
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    int xStart = inicio->getCol();
    int yStart = inicio->getRow();
    int xFinish = final->getCol();
    int yFinish = final->getRow();
    int max_xMatriz = columns;
    int max_yMatriz = rows;
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    // Vector para almacenar todos los nodos creados
    std::vector<Node*> allNodes;

    Node* start = new Node(xStart, yStart);
    start->g = 0;
    start->h = manhattanDistance(xStart, yStart, xFinish, yFinish);
    
    // Agregamos el nodo inicial al vector
    allNodes.push_back(start);

    pq.push(start);

    while(!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        if (visited.find(std::make_pair(current->x, current->y)) != visited.end()) {
            continue;
        }
        visited.insert(std::make_pair(current->x, current->y));

        // Si hemos llegado al destino, eliminamos todos los nodos y devolvemos la dirección del primer movimiento
        if(current->x == xFinish && current->y == yFinish) {
            int direction = getFirstDirection(start, current);
            for (Node* node : allNodes) {
                delete node;
            }
            return direction;
        }

        // Visitamos los vecinos
        std::vector<std::pair<int, int>> directions = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
        for(auto& dir : directions) {
            int newX = current->x + dir.first;
            int newY = current->y + dir.second;
            // Verificamos si la nueva posición está dentro de los límites y si es una celda transitable
            if(newX >= 0 && newX < max_xMatriz && newY >= 0 && newY < max_yMatriz && matriz[newY][newX] == 0 && visited.find(std::make_pair(newX, newY)) == visited.end()) {
                Node* neighbor = new Node(newX, newY, current);
                neighbor->g = current->g + 1;
                neighbor->h = manhattanDistance(newX, newY, xFinish, yFinish);
                // Agregamos el nodo vecino al vector
                allNodes.push_back(neighbor);
                pq.push(neighbor);
            }
        }
    }

    // Si no se encontró un camino, eliminamos todos los nodos y devolvemos -1
    for (Node* node : allNodes) {
        delete node;
    }
    return -1; 
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