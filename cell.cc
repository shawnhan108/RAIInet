#include "cell.h"
using namespace std;

Cell::Cell(int x, int y): 
    x{x}, y{y}, isOccP1{false}, isOccP2{false}, firewallOne{false},
    firewallTwo{false},OccupiedID{""}, player1Port{false}, player2Port{false}, thePiece{nullptr}{}

int Cell::getx() {return x;}
int Cell::gety() {return y;}
bool Cell::ifOccP1() {return isOccP1;}
void Cell::setOccP1(bool isO) {isOccP1 = isO;}
bool Cell::ifOccP2() {return isOccP2;}
void Cell::setOccP2(bool isO) {isOccP2 = isO;}
bool Cell::hasFirewallOne() {return firewallOne;}
void Cell::setFirewallOne(bool isW) {firewallOne = isW;}
bool Cell::hasFirewallTwo() {return firewallTwo;}
void Cell::setFirewallTwo(bool isW) {firewallTwo = isW;}
string Cell::getName() {return OccupiedID;}
void Cell::setName(string ID) {OccupiedID = ID;}
bool Cell::ifp1port() {return player1Port;}
void Cell::setPort1(bool isP) {player1Port = isP;}
bool Cell::ifp2port() {return player2Port;}
void Cell::setPort2(bool isP) {player2Port = isP;}
PiecePtr Cell::whoOcc() {return thePiece;}
void Cell::setPiece(PiecePtr newP) {thePiece = newP;}
