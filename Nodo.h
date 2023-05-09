#pragma once

class Nodo {

private:
    int type;           // 0 para espacio vacío, 1 para pared
    bool enemyPassed;   // Verdadero si un enemigo ha pasado por esta celda en los últimos 5 frames
    bool hasFood;       // Verdadero si la celda tiene comida
    bool hasPower;      // Verdadero si la celda puede tener un poder
    static const int CELL_SIZE = 32;

public:
    Nodo();
    Nodo(int type, bool enemyPassed, bool hasFood, bool hasPower);
    ~Nodo();

    int getType() const;
    void setType(int type);

    bool getEnemyPassed() const;
    void setEnemyPassed(bool enemyPassed);

    bool getHasFood() const;
    void setHasFood(bool hasFood);

    bool getHasPower() const;
    void setHasPower(bool hasPower);

    int getCellSize() const;
};