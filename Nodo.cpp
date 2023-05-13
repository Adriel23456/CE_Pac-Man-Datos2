#include "Nodo.h"

Nodo::Nodo() : type(0), hasFood(false), hasPower(false), row(0), col(0) {
}

Nodo::Nodo(int type, int row, int col){
    this->type = type;
    this->hasFood = true;
    this->hasPower = false;
    this->row = row;
    this->col = col;
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

int Nodo::getRow(){
    return this->row;
}

int Nodo::getCol(){
    return this->col;
}