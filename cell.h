#ifndef _CELL_H_
#define _CELL_H_
#include <string>
#include <memory>
#include "piece.h"

typedef std::shared_ptr<Piece> PiecePtr;

class Cell {
    int x; //x is row and y is col
    int y;
    bool isOccP1 = false;
    bool isOccP2 = false;
    bool firewallOne = false;
    bool firewallTwo = false;
    std::string OccupiedID = "";
    bool player1Port = false;
    bool player2Port = false;
    PiecePtr thePiece = nullptr;
    public:
    Cell(int x, int y);
    int getx();
    int gety();
    bool ifOccP1();
    void setOccP1(bool isO);
    bool ifOccP2();
    void setOccP2(bool isO);
    bool hasFirewallOne();
    void setFirewallOne(bool isW);
    bool hasFirewallTwo();
    void setFirewallTwo(bool isW);
    std::string getName();
    void setName(std::string ID);
    bool ifp1port();
    void setPort1(bool isP);
    bool ifp2port();
    void setPort2(bool isP);
    PiecePtr whoOcc();
    void setPiece(PiecePtr newP);
};

#endif
