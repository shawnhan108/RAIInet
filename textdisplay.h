#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
#include <iostream>
#include <vector>
#include "cell.h"
#include "piece.h"
#include <memory>
class Board;

typedef std::shared_ptr<Piece> PiecePtr;
typedef std::shared_ptr<Board> BoardPtr;

class TextDisplay {
    std::vector<std::vector<char>> theBoard;
    
    // will print out player1's view if player1First = 1
    // player2's view if player1First = 0;
    bool player1First;

    int player1DownloadData;
    int player2DownloadData;

    int player1DownloadVirus;
    int player2DownloadVirus;

    int player1AbilityRemain;
    int player2AbilityRemain;

    // a vector of Piece, textdisplay will call Piece.getInfo and getName
    std::vector<PiecePtr> player1PieceInfo;
    std::vector<PiecePtr> player2PieceInfo;

public:
    TextDisplay();
    void notify(Board* b); // pass in a player, and update the data ready to print out
    void setPlayer(); // switch between player
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

std::ostream &operator<<(std::ostream &out, const TextDisplay &td);

#endif
