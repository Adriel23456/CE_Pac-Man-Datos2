#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "Nivel.h"

class Nivel; // Añade esta línea para la declaración adelantada
class Pacman; // Añade esta línea para la declaración adelantada

class Game : public QGraphicsView {
    Q_OBJECT

public:
    explicit Game(QWidget* parent = nullptr);
    int getCurrentLevel() const;
    ~Game();

private:
    void cambiaNivel();
    void renderizaNivel();
    Pacman* pacman;
    QGraphicsScene* scene;
    QTimer* timer;
    Nivel* niveles;
    int currentLevel;
    int puntos;
};