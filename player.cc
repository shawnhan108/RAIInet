#include "player.h"
#include "piece.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

//ctor
Player::Player(string name, string piecesID, string ab):
    name{name}, downVirusNum{0}, downDataNum{0}, aliveNum{8}, revealedNum{0}{
    if (name == "player1"){
        this->baseLimit = 0;
        this->oppLimit = 7;
    }else{
        this->baseLimit = 7;
        this->oppLimit = 0;
    }
    vector<string> abilityVec;
    abilityVec.emplace_back(ab.substr(0, 1));
    abilityVec.emplace_back(ab.substr(1, 1));
    abilityVec.emplace_back(ab.substr(2, 1));
    abilityVec.emplace_back(ab.substr(3, 1));
    abilityVec.emplace_back(ab.substr(4, 1));
    this->ability = abilityVec; 
    vector<string> ids;
    int k = 0;

    for (int i = 0; i < 8; ++i) {
        k = 2 * i;
        ids.emplace_back(piecesID.substr(k, 2));
    }

    vector<PiecePtr> pieceVec;
    for (size_t i = 0; i < ids.size(); ++i){
        string id = ids[i];
        string type = (id[0] == 'V') ? "virus" : "data";
        string temptemp(1, id[1]);
        stringstream temp(temptemp); 
        int strength; 
        temp >> strength; 
        int rowNum;
        string colIDs;
        if (name == "player1"){
            colIDs = "abcdefgh";
            if ((i == 3) || (i == 4)) rowNum = 1;
            else rowNum = 0;
        }else{
            colIDs = "ABCDEFGH";
            if ((i == 3) || (i == 4)) rowNum = 6;
            else rowNum = 7;
        }
        string newID(1, colIDs[i]);
        PiecePtr newPiece = PiecePtr(new Piece(newID, type, rowNum, i, strength, true, false, 1, name));
        pieceVec.emplace_back(newPiece);
    }
    this->pieces = pieceVec;
}

//getters
string Player::getName(){return this->name;}
int Player::getDownDataNum(){return this->downDataNum;}
int Player::getDownVirusNum(){return this->downVirusNum;}
int Player::getAliveNum(){return this->aliveNum;}
int Player::getRevealedNum(){return this->revealedNum;}
vector<PiecePtr> Player::getPieces(){return this->pieces;}
vector<string> Player::getAbilities(){return this->ability;}
int Player::abilityRemain(){
    int remain = 0;
    for (size_t i = 0; i < this->ability.size(); ++i) {
        if (!(this->ability[i] == "^")){ remain++; }
    }
    return remain;
}

//returns int: 1 = Player has won, 2 = Player has lost, 3 = not determined
int Player::haveWon(){
    if (this->downDataNum == 4) return 1;
    if (this->downVirusNum == 4) return 2;
    return 3;
}

//setters
void Player::addDownVirusNum(){this->downVirusNum += 1;}
void Player::addDownDataNum(){this->downDataNum += 1;}
void Player::addRevealedNum(){this->revealedNum += 1;}
void Player::subAliveNum(){this->aliveNum -= 1;}
void Player::setAbility(int index, string content){this->ability[index] = content;}

//dtor
Player::~Player(){
    // for (size_t i = 0; i < this->pieces.size(); ++i){
    //     remove this->pieces[i];
    // }
}

bool Player::ifoverBaseLimit(int x){
    if (this->name == "player1"){
        if (x < 0) {return true;}
        else {return false;}
    }else{
        if (x > 7) return true;
        else return false;
    }
}

bool Player::ifoverOppLimit(int x){
    if (this->name == "player1"){
        if (x > 7) return true;
        else return false;
    }else{
        if (x < 0) return true;
        else return false;
    }
}
