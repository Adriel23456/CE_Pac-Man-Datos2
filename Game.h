#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTimer>
#include "Nivel.h"
#include "Pacman.h"

//class Nivel; // Añado esta línea para la declaración adelantada
//class Pacman; // Añado esta línea para la declaración adelantada
//class Nodo; // Añado esta línea para la declaración adelantada

class Game : public QGraphicsView {
    Q_OBJECT

private:
    Nivel* nivel;
    QGraphicsScene* scene;
    QTimer* timer;
    int puntos;

public:
    explicit Game(QWidget* parent = nullptr);
    ~Game();
    Nivel* getCurrentNivel();
    void update();
    void cambiaNivel();
    QGraphicsScene* getScene();
protected:
    void keyPressEvent(QKeyEvent *event);
};