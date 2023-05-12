#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <SFML/Audio.hpp>
#include <QKeyEvent>
#include <QTimer>
#include "Nivel.h"
#include "Pacman.h"

class Game : public QGraphicsView {
    Q_OBJECT

private:
    int puntos;
    bool firstGeneration;

    Nivel* nivel;
    QGraphicsScene* scene;
    QTimer* timer;
    
    QPixmap foodPixmap;
    QPixmap nofoodPixmap;
    QPixmap wallPixmap;
    QPixmap pacmanPixmap;
    QPixmap ghost1Pixmap;
    QPixmap ghost2Pixmap;

    QGraphicsTextItem* scoreText;
    QGraphicsTextItem* livesText;
    QGraphicsTextItem* levelText;

    sf::Music backgroundMusic;
    sf::Sound pacmanEatSound;
    sf::SoundBuffer pacmanEatBuffer;
    sf::Sound victorySound;
    sf::SoundBuffer victorySoundBuffer;
    sf::Sound deathSound;
    sf::SoundBuffer deathSoundBuffer;

public:
    explicit Game(QWidget* parent = nullptr);
    ~Game();
    Nivel* getCurrentNivel();
    void update();
    void cambiaNivel();
    QGraphicsScene* getScene();
    void setFirstGeneration(bool newValue);
    bool getFirstGeneration();
    void playBackgroundMusic();
    void playPacmanEatSound();
    void playVictorySound();
    void playDeathSound();
protected:
    void keyPressEvent(QKeyEvent *event);
};