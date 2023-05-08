#pragma once
#include <QWidget>
#include "Nivel.h"

class Game : public QWidget {
    Q_OBJECT

public:
    explicit Game(QWidget* parent = nullptr);
    ~Game();

private:
    Nivel niveles;
    Nodo* currentLevel;
    int puntos;

    void cambiaNivel();
};