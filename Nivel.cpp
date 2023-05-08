#include "Nivel.h"

Nivel::Nivel() : rows(7), columns(10) {
    initializeLevels();
}

Nodo Nivel::getNode(int row, int col) const {
    int index = row * columns + col;
    if (index >= 0 && index < level1.size()) {
        return level1[index];
    }
    return Nodo(); // Devuelve un nodo vacío en caso de que los índices estén fuera de rango
}

int Nivel::getRows() const {
    return rows;
}

int Nivel::getColumns() const {
    return columns;
}

void Nivel::initializeLevels() {
    // Inicializa los niveles con los valores deseados
    // Puedes cambiar los valores para personalizar la estructura del laberinto en cada nivel

    // Valores de ejemplo para level1
    int level1Data[7][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    // Copia los valores de level1Data a la matriz level1 de objetos Nodo
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            level1.push_back(Nodo(level1Data[i][j], false, true, false));
        }
    }

    // Copia los valores de level1Data a las matrices level2, level3 y level4.
    // Estos valores se pueden cambiar para personalizar cada nivel individualmente.
    level2 = level1;
    level3 = level1;
    level4 = level1;
}