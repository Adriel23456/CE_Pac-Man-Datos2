#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "Nivel.h"

class Game : public QGraphicsView {
    Q_OBJECT

public:
    explicit Game(QWidget* parent = nullptr);
    ~Game();

private:
    void cambiaNivel();
    void renderizaNivel();

    QGraphicsScene* scene;
    QTimer* timer;
    Nivel niveles;
    int currentLevel; // Cambiar esta línea
    int puntos;
};