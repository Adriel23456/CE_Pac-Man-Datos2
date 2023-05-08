#include "Nivel.h"

Nivel::Nivel() {
    initializeLevels();
}

Nodo* Nivel::getLevel(int levelNumber) {
    switch (levelNumber) {
        case 1:
            return &level1[0][0];
        case 2:
            return &level2[0][0];
        case 3:
            return &level3[0][0];
        case 4:
            return &level4[0][0];
        default:
            return nullptr;
    }
}

void Nivel::initializeLevels() {
    // Inicializa los niveles con los valores deseados
    // Puedes cambiar los valores para personalizar la estructura del laberinto en cada nivel

    // Valores de ejemplo para level1
    int level1Data[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    // Copia los valores de level1Data a la matriz level1 de objetos Nodo
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            level1[i][j] = Nodo(level1Data[i][j], false, true, false);
        }
    }

    // Valores de ejemplo para level2
    int level2Data[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    // Copia los valores de level1Data a la matriz level1 de objetos Nodo
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            level2[i][j] = Nodo(level1Data[i][j], false, true, false);
        }
    }

    // Valores de ejemplo para level3
    int level3Data[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    // Copia los valores de level1Data a la matriz level1 de objetos Nodo
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            level3[i][j] = Nodo(level1Data[i][j], false, true, false);
        }
    }
}