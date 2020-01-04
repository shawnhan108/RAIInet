#include "piece.h"
#include <string>
#include <iostream>

Piece::Piece(std::string id, std::string type, int x, int y, int strength, bool isAlive, bool isRevealed, int boost, std::string owner):
    id{id}, type{type}, x{x}, y{y}, strength{strength}, isAlive{isAlive}, isRevealed{isRevealed}, boost{boost}, owner{owner}{
        diagonal = 0;
    }

Piece::Piece(){}

void Piece::setDead() { isAlive = 0; }
bool Piece::isPieceAlive() { return isAlive; }

void Piece::setRevealed() { isRevealed = 1; }
bool Piece::isPieceRevealed() { return isRevealed; }

void Piece::setBoost(int x) { boost = x; }
int Piece::getBoost() { return boost; }

std::string Piece::getInfo() {
    if (type == "data") {
        return "D" + std::to_string(strength);
    } else {
        return "V" + std::to_string(strength);
    }
}

int Piece::getStrength() { return strength; }
std::string Piece::getType() { return type; }

std::string Piece::getName() { return id; }

int Piece::getPosx() { return x; }
int Piece::getPosy() { return y; }

void Piece::switchType() {
    if (type == "virus") {
        type = "data";
    } else {
        type = "virus";
    }
}

void Piece::setPos(int x, int y) {
    this->x = x;
    this->y = y;
}

std::string Piece::getOwner(){ return owner;}

void Piece::setDiagonal() {diagonal = true; }

bool Piece::getDiagonal() { return diagonal; }


