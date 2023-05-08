#pragma once
#include "Nodo.h"
#include <vector>

class Nivel {
public:
    Nivel();
    Nodo getNode(int row, int col) const;
    int getRows() const;
    int getColumns() const;

    static const int CELL_SIZE = 32;

private:
    std::vector<Nodo> level1;
    std::vector<Nodo> level2;
    std::vector<Nodo> level3;
    std::vector<Nodo> level4;

    int rows;
    int columns;

    void initializeLevels();
};