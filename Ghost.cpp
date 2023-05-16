#include "Ghost.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <climits>
#include <cmath>
#include <QDebug>
#include <algorithm>
#include <random>

Ghost::Ghost(Nodo* currentPosition): direction(0), currentPosition(currentPosition), reloadTime(8000), death(false){}   

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

struct Pos {
    int x;
    int y;
    Pos(int x = 0, int y = 0) : x(x), y(y) {}
};

bool isSafe(int x, int y, int** matriz, std::vector<std::vector<bool>>& visited, int max_xMatriz, int max_yMatriz) {
    return x >= 0 && y >= 0 && x < max_xMatriz && y < max_yMatriz && matriz[y][x] == 0 && !visited[y][x];
}

// Función para calcular la distancia de Manhattan entre dos puntos
int manhattanDistance2(Pos a, Pos b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Función para ordenar las direcciones posibles basándose en la distancia de Manhattan hasta el destino
std::vector<Pos> getOrderedDirections(Pos current, Pos destination) {
    std::vector<Pos> directions = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    std::sort(directions.begin(), directions.end(),
              [&current, &destination](Pos dir1, Pos dir2) {
                  Pos newPos1(current.x + dir1.x, current.y + dir1.y);
                  Pos newPos2(current.x + dir2.x, current.y + dir2.y);
                  return manhattanDistance2(newPos1, destination) < manhattanDistance2(newPos2, destination);
              });
    return directions;
}


// Función recursiva para resolver el laberinto utilizando backtracking
bool solveMaze(int x, int y, int xFinish, int yFinish, std::vector<Pos>& path, int** matriz, std::vector<std::vector<bool>>& visited, int max_xMatriz, int max_yMatriz) {
    // Si hemos llegado al destino, añadimos la celda actual al camino y devolvemos true
    if (x == xFinish && y == yFinish) {
        return true;
    }
    // Marcamos la celda actual como visitada
    visited[y][x] = true;
    // Definimos las posibles direcciones en las que podemos movernos
    //std::vector<Pos> directions = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    // Recorremos todas las direcciones posibles
    std::vector<Pos> directions = getOrderedDirections(Pos(x, y), Pos(xFinish, yFinish));
    // Recorremos todas las direcciones posibles
    for (const auto& dir : directions) {
        int newX = x + dir.x;
        int newY = y + dir.y;
        // Si la celda es válida y no ha sido visitada, y es transitable
        if (isSafe(newX, newY, matriz, visited, max_xMatriz, max_yMatriz)) {
            path.push_back(Pos(newX, newY));
            // Imprimir el camino actual.
            qDebug() << "Current path:";
            for(const auto& pos : path) {
                qDebug() << "(" << pos.x << ", " << pos.y << ")";
            }
            if (solveMaze(newX, newY, xFinish, yFinish, path, matriz, visited, max_xMatriz, max_yMatriz)) {
                // Si podemos resolverlo, añadimos la celda al camino y devolvemos true
                return true;
            }
            path.pop_back();
        }
    }
    // Si no podemos resolver el laberinto a partir de la celda actual, devolvemos false
    return false;
}

// Función para obtener la dirección del primer movimiento en backtracking
int getFirstDirectionB(const std::vector<Pos>& path) {
    if(path[1].x < path[0].x) {
        return 1;
    } else if(path[1].y < path[0].y) {
        return 2;
    } else if(path[1].x > path[0].x) {
        return 3;
    } else if(path[1].y > path[0].y) {
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

int Ghost::getDirectionPowerA(int** matriz, Nodo* final, Nodo* inicio, int rows, int columns){
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::unordered_set<std::pair<int, int>, pair_hash> openList;   // Almacena las celdas en la open list
    std::unordered_set<std::pair<int, int>, pair_hash> closedList; // Almacena las celdas en la closed list
    
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
    openList.insert(std::make_pair(start->x, start->y)); // Agregamos el nodo inicial a la open list

    pq.push(start);

    while(!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        if (visited.find(std::make_pair(current->x, current->y)) != visited.end()) {
            continue;
        }
        visited.insert(std::make_pair(current->x, current->y));
        closedList.insert(std::make_pair(current->x, current->y)); // Agregamos el nodo actual a la closed list
        openList.erase(std::make_pair(current->x, current->y)); // Eliminamos el nodo actual de la open list

        // Imprimimos las listas
        qDebug() << "Open List:";
        for (const auto& pair : openList) {
            qDebug() << "(" << pair.first << ", " << pair.second << ")";
        }
        qDebug() << "Closed List:";
        for (const auto& pair : closedList) {
            qDebug() << "(" << pair.first << ", " << pair.second << ")";
        }

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
                openList.insert(std::make_pair(newX, newY)); // Agregamos el nodo vecino a la open list
            }
        }
    }
    // Si no se encontró un camino, eliminamos todos los nodos y devolvemos -1
    for (Node* node : allNodes) {
        delete node;
    }
    return -1; 
}

//Metodo de aplicacion del algoritmo de Backtracking a los fatasmas correctos
int Ghost::getDirectionPowerB(int** matriz, Nodo* final, Nodo* inicio, int rows, int columns){
    int xStart = inicio->getCol();
    int yStart = inicio->getRow();
    int xFinish = final->getCol();
    int yFinish = final->getRow();
    int max_xMatriz = columns;
    int max_yMatriz = rows;

    std::vector<std::vector<bool>> visited(max_yMatriz, std::vector<bool>(max_xMatriz, false));
    // Vector para almacenar el camino desde el inicio hasta el destino
    std::vector<Pos> path;
    path.clear();  // Limpiar la lista del camino
    path.push_back(Pos(xStart, yStart));
    // Intentamos resolver el laberinto utilizando backtracking
    if (!solveMaze(xStart, yStart, xFinish, yFinish, path, matriz, visited, max_xMatriz, max_yMatriz)) {
        return -1; // Si no encontramos una solución, regresamos -1
    }
    // Si encontramos una solución, obtenemos la dirección del primer movimiento
    int direction = getFirstDirectionB(path);
    return direction;
}

void Ghost::setCurrentPosition(Nodo* newCurrentPosition){
    this->currentPosition = newCurrentPosition;
}

Nodo* Ghost::getCurrentPosition(){
    return currentPosition;
}

void Ghost::setDeath(bool newValue){
    this->death = newValue;
}

bool Ghost::getDeath(){
    return death;
}

int Ghost::getReloadTime() const{
    return reloadTime;
}