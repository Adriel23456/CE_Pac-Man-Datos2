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
        return h1 ^ h2;  //XOR binario
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

struct Node2{
    Pos pos;
    std::vector<Node2*> children;
    Node2* parent;
    Node2(Pos pos, Node2* parent = nullptr) : pos(pos), parent(parent) {}
};

// Función para calcular la distancia de Manhattan entre dos puntos
int manhattanDistance2(Pos a, Pos b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Función para ordenar las direcciones posibles basándose en la distancia de Manhattan hasta el destino
std::vector<Pos> getOrderedDirections(Pos current, Pos destination) {
    std::vector<Pos> directions = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    // Crear un vector de pares, donde el primer elemento del par es la dirección y el segundo elemento es la distancia
    std::vector<std::pair<Pos, int>> distances;
    for (const auto& dir : directions) {
        Pos newPos(current.x + dir.x, current.y + dir.y);
        distances.push_back({dir, manhattanDistance2(newPos, destination)});
    }
    // Ordenar el vector de pares basándose en la distancia
    std::sort(distances.begin(), distances.end(), [](const std::pair<Pos, int>& a, const std::pair<Pos, int>& b) {
        return a.second < b.second;
    });
    // Crear un nuevo vector de direcciones ordenadas
    std::vector<Pos> orderedDirections;
    for (const auto& pair : distances) {
        orderedDirections.push_back(pair.first);
    }
    return orderedDirections;
}

void printTree(Node2* node, int level = 0) {
    qDebug() << QString(level, ' ') << "(" << node->pos.x << ", " << node->pos.y << ")";
    for (Node2* child : node->children) {
        printTree(child, level + 1);
    }
}

bool isSafe(int x, int y, int** matriz, std::vector<std::vector<bool>>& visited, int max_xMatriz, int max_yMatriz) {
    return x >= 0 && y >= 0 && x < max_xMatriz && y < max_yMatriz && matriz[y][x] == 0 && !visited[y][x];
}

Node2* exploreMaze(int x, int y, int xFinish, int yFinish, Node2* parentNode, int** matriz, std::vector<std::vector<bool>>& visited, int max_xMatriz, int max_yMatriz) {
    visited[y][x] = true;
    Node2* currentNode = new Node2(Pos(x, y), parentNode);
    if (x == xFinish && y == yFinish) {
        return currentNode;
    }
    std::vector<Pos> directions = getOrderedDirections(Pos(x, y), Pos(xFinish, yFinish));
    for (const auto& dir : directions) {
        int newX = x + dir.x;
        int newY = y + dir.y;
        if (isSafe(newX, newY, matriz, visited, max_xMatriz, max_yMatriz)) {
            Node2* result = exploreMaze(newX, newY, xFinish, yFinish, currentNode, matriz, visited, max_xMatriz, max_yMatriz);
            if (result != nullptr) {
                currentNode->children.push_back(result);
                return currentNode;
            }
        }
    }
    delete currentNode;
    return nullptr;
}

int getFirstDirectionB(Node2* node) {
    if (node == nullptr || node->parent == nullptr) {
        return 0;
    }
    Pos parentPos = node->parent->pos;
    Pos currentPos = node->pos;
    if (currentPos.x < parentPos.x) {
        return 1;
    } else if (currentPos.y < parentPos.y) {
        return 2;
    } else if (currentPos.x > parentPos.x) {
        return 3;
    } else if (currentPos.y > parentPos.y) {
        return 4;
    }
    return 0;
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

int Ghost::getDirectionPowerB(int** matriz, Nodo* final, Nodo* inicio, int rows, int columns) {
    int xStart = inicio->getCol();
    int yStart = inicio->getRow();
    int xFinish = final->getCol();
    int yFinish = final->getRow();
    int max_xMatriz = columns;
    int max_yMatriz = rows;
    std::vector<std::vector<bool>> visited(max_yMatriz, std::vector<bool>(max_xMatriz, false));
    Node2* root = exploreMaze(xStart, yStart, xFinish, yFinish, nullptr, matriz, visited, max_xMatriz, max_yMatriz);
    printTree(root);
    if (root == nullptr) {    return -1; // Si no encontramos una solución, regresamos -1
    }

    // Buscamos el nodo objetivo en el árbol
    std::queue<Node2*> queue;
    Node2* targetNode = nullptr;
    queue.push(root);
    while (!queue.empty()) {
        Node2* node = queue.front();
        queue.pop();
        if (node->pos.x == xFinish && node->pos.y == yFinish) {
            targetNode = node;
            break;
        }
        for (Node2* child : node->children) {
            queue.push(child);
        }
    }

    // Si no encontramos el nodo objetivo, regresamos -1
    if (targetNode == nullptr) {
        return -1;
    }

    // Retrocedemos desde el nodo objetivo hasta el nodo raíz para encontrar el primer movimiento
    Node2* node = targetNode;
    while (node->parent != nullptr && node->parent->parent != nullptr) {
        node = node->parent;
    }

    // Obtenemos la dirección del primer movimiento
    int direction = getFirstDirectionB(node);
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