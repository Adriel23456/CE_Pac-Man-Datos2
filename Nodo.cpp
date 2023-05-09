#include "Nodo.h"

Nodo::Nodo() : type(0), enemyPassed(false), hasFood(false), hasPower(false) {
}

Nodo::Nodo(int type, bool enemyPassed, bool hasFood, bool hasPower)
    : type(type), enemyPassed(enemyPassed), hasFood(hasFood), hasPower(hasPower) {
}

Nodo::~Nodo() {
    // No hay nada que hacer aquí porque no hay asignacion de memoria dinamica...
}

int Nodo::getType() const {
    return type;
}

void Nodo::setType(int type) {
    this->type = type;
}

bool Nodo::getEnemyPassed() const {
    return enemyPassed;
}

void Nodo::setEnemyPassed(bool enemyPassed) {
    this->enemyPassed = enemyPassed;
}

bool Nodo::getHasFood() const {
    return hasFood;
}

void Nodo::setHasFood(bool hasFood) {
    this->hasFood = hasFood;
}

bool Nodo::getHasPower() const {
    return hasPower;
}

void Nodo::setHasPower(bool hasPower) {
    this->hasPower = hasPower;
}

int Nodo::getCellSize() const {
    return CELL_SIZE;
}