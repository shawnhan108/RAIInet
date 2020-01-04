#ifndef PIECE_H
#define PIECE_H
#include <string>

class Piece {
    // id is either one of "abcdefg" or "ABCDEFG"
    std::string id;

    // type is one of "virus" or "data"
    std::string type;

    // postion of the piece
    int x, y;

    int strength;

    // if the piece is downlowded by opponents, set isAlive to 0
    bool isAlive;

    // if the piece is revealed by opponents, set isRevealed to 1
    bool isRevealed;

    // boost is one of 1, 2, 4
    int boost;

    // owner is either "player1" or "player2"
    std::string owner;

    bool diagonal;

public:

    Piece(std::string id, std::string type, int x, int y, int strength, bool isAlive, bool isRevealed, int boost, std::string owner);
    Piece();

    // set the isAlive to 0
    void setDead();
    bool isPieceAlive();

    // set the isRevealed to 1
    void setRevealed();
    bool isPieceRevealed();

    // set boost to x
    void setBoost(int x);
    int getBoost();

    // return a string [VD][1-4]
    std::string getInfo();
    int getStrength();
    std::string getType();

    // give the name of the piece
    std::string getName();

    int getPosx();
    int getPosy();

    void setPos(int x, int y);

    void switchType();

    std::string getOwner();

    void setDiagonal();
    bool getDiagonal();

};

#endif
