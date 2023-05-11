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
    bool firstGeneration;
    QPixmap foodPixmap;
    QPixmap nofoodPixmap;
    QPixmap wallPixmap;
    QPixmap pacmanPixmap;

public:
    explicit Game(QWidget* parent = nullptr);
    ~Game();
    Nivel* getCurrentNivel();
    void update();
    void cambiaNivel();
    QGraphicsScene* getScene();
    void setFirstGeneration(bool newValue);
    bool getFirstGeneration();
protected:
    void keyPressEvent(QKeyEvent *event);
};