#pragma once
#include "Nodo.h"

class Nivel {
public:
    Nivel();
    Nodo* getLevel(int levelNumber);

private:
    static const int ROWS = 6;
    static const int COLS = 10;
    Nodo level1[ROWS][COLS];
    Nodo level2[ROWS][COLS];
    Nodo level3[ROWS][COLS];
    Nodo level4[ROWS][COLS];

    void initializeLevels();
};