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
    bool pacmanDeath;
    bool powerUpActive;
    int pointsForActive;
    int powerUps;

    Nivel* nivel;
    QGraphicsScene* scene;
    QTimer* timer;
    QTimer* timerPacman;
    QTimer* timerPower;
    QTimer* timerGhost1;
    QTimer* timerGhost2;
    QTimer* timerGhost3;
    QTimer* timerGhost4;
    
    QPixmap foodPixmap;
    QPixmap specialFoodPixmap;
    QPixmap nofoodPixmap;
    QPixmap wallPixmap;
    QPixmap pacmanPixmap;
    QPixmap ghost1Pixmap;
    QPixmap ghost2Pixmap;

    QGraphicsTextItem* scoreText;
    QGraphicsTextItem* livesText;
    QGraphicsTextItem* levelText;

    sf::Music backgroundMusic;
    sf::Music powerUpMusic;
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
    void respawnPacMan();
    void respawnGhost1();
    void respawnGhost2();
    void respawnGhost3();
    void respawnGhost4();
    void cambiaNivel();
    void gameOver();
    void deactivatePower();
    Nodo* farAwayNode();
    QGraphicsScene* getScene();
    void setFirstGeneration(bool newValue);
    bool getFirstGeneration();
    void setPacmanDeath(bool newValue);
    int getPowerUps();
    void setPowerUps(int newValue);
    bool getPacmanDeath();
    void setPowerUpActive(bool newValue);
    bool getPowerUpActive();
    void setPointsForActive(int newValue);
    bool getPointsForActive();
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void playPacmanEatSound();
    void playVictorySound();
    void playPowerMusic();
    void stopPowerMusic();
    void playDeathSound();
    int* nodeAway(int* puntos, int** matrizBase, int x_Max, int y_Max, int arraySize);
protected:
    void keyPressEvent(QKeyEvent *event);
};