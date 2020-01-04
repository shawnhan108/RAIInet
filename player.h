#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <string>
#include <memory>
#include "piece.h"

typedef std::shared_ptr<Piece> PiecePtr;

class Player{
    std::string name;
    std::vector<PiecePtr> pieces; // a vector of heap-allocated piece pointers
    int downVirusNum;
    int downDataNum;
    int aliveNum;
    int revealedNum;
    int baseLimit;
    int oppLimit;
    std::vector<std::string> ability;
    public:
    //ctor: must pass in: 
    //the 5-char acronym for abilities(i.e. "LFFDD"), default is "LFDSP"
    //player's name (either "player1" or "player2")
    //a 16-digit string indicating the ids of 8 pieces (i.e. "V1D4V3V2D3V4D2D1")
    Player(std::string name, std::string piecesID, std::string ab = "LFDSP");
    std::string getName();
    int getDownDataNum();
    int getDownVirusNum();
    int getAliveNum();
    int getRevealedNum();
    std::vector<PiecePtr> getPieces();
    std::vector<std::string> getAbilities();
    int haveWon(); //returns int: 1 = Player has won, 2 = Player has lost, 3 = not determined
    void addDownVirusNum();
    void addDownDataNum();
    void addRevealedNum();
    void setAbility(int index, std::string content); //index range from 0-4
    int abilityRemain();
    bool ifoverBaseLimit(int x); // return true if yes, otherwise no
    bool ifoverOppLimit(int x); // return true if yes, otherwise no.
    void subAliveNum();
    ~Player(); //dtor
};

#endif


