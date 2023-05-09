#pragma once

#include "Nodo.h"
#include "Game.h"
#include <vector>

class Game;

class Nivel {
public:
    Nivel();
    Nodo getNode(int row, int col) const;
    int getRows();
    int getColumns();
    void setRows(int rows);
    void setColumns(int columns);
    void setGameInstance(Game* gameInstance);

    static const int CELL_SIZE = 32;

private:
    std::vector<Nodo> level1;
    std::vector<Nodo> level2;
    std::vector<Nodo> level3;
    std::vector<Nodo> level4;
    Game* gameInstance;
    int rows;
    int columns;

    void initializeLevels();
};