#include <iostream>
#include <vector>
#include <string>
#include "textdisplay.h"
#include "board.h"
#include <memory>

typedef std::shared_ptr<Cell> CellPtr;

TextDisplay::TextDisplay() {
    player1First = 1;
    for (int i = 0; i < 8; ++i) {
        auto Line = std::vector<char>{};
        for (int j = 0; j < 8; ++j) {
            Line.emplace_back('.');
        }
        theBoard.emplace_back(std::move(Line));
    }
    theBoard[0][3] = 'S';
    theBoard[0][4] = 'S';
    theBoard[7][3] = 'S';
    theBoard[7][4] = 'S';
}

void TextDisplay::notify(Board* b){

    player1DownloadData = b->getP1()->getDownDataNum();
    player2DownloadData = b->getP2()->getDownDataNum();

    player1DownloadVirus = b->getP1()->getDownVirusNum();
    player2DownloadVirus = b->getP2()->getDownVirusNum();

    player1AbilityRemain = b->getP1()->abilityRemain();
    player2AbilityRemain = b->getP2()->abilityRemain();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            theBoard[i][j] = '.';
        }
    }
    theBoard[0][3] = 'S';
    theBoard[0][4] = 'S';
    theBoard[7][3] = 'S';
    theBoard[7][4] = 'S';

    for (auto i: ((b->getP1())->getPieces())) {
        if (i->isPieceAlive()) {
            theBoard[i->getPosx()][i->getPosy()] = i->getName()[0];
        }
    }
    
    std::vector<std::vector<CellPtr>>* a = b->getcells();
    
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((*a)[i][j]->hasFirewallOne() && (*a)[i][j]->hasFirewallTwo()) {
                if ((*a)[i][j]->ifOccP1() || (*a)[i][j]->ifOccP2()){
                    theBoard[i][j] = (*a)[i][j]->getName()[0];
                }else{
                    theBoard[i][j] = 'Z';
                }
            }else if ((*a)[i][j]->hasFirewallOne()) {
                if ((*a)[i][j]->ifOccP1() || (*a)[i][j]->ifOccP2()){
                    theBoard[i][j] = (*a)[i][j]->getName()[0];
                }else{
                    theBoard[i][j] = 'M';
                }
            } else if ((*a)[i][j]->hasFirewallTwo()) {
                if ((*a)[i][j]->ifOccP1() || (*a)[i][j]->ifOccP2()){
                    theBoard[i][j] = (*a)[i][j]->getName()[0];
                }else{
                    theBoard[i][j] = 'W';
                }
            }
        }
    }


    for (auto i: ((b->getP2())->getPieces())) {
        if (i->isPieceAlive()) {
            theBoard[i->getPosx()][i->getPosy()] = i->getName()[0];
        }
    }

    player1PieceInfo = b->getP1()->getPieces();
    player2PieceInfo = b->getP2()->getPieces();


}


void TextDisplay::setPlayer() { 
    player1First = !player1First; }

std::ostream& operator<<(std::ostream &out, const TextDisplay &td) {

    // print out player1's name
    if (td.player1First) {
        out << "Player 1:" << '\n';

        out << "Downloaded: " << td.player1DownloadData << "D, " 
        << td.player1DownloadVirus << "V" << '\n';

        out << "Abilities: " << td.player1AbilityRemain << '\n';

        for (int i = 0; i < 4; ++i) {
            out << td.player1PieceInfo[i]->getName() << ": " << td.player1PieceInfo[i]->getInfo() << "   ";
        }
        out << '\n';

        for (int i = 4; i < 8; ++i) {
            out << td.player1PieceInfo[i]->getName() << ": " << td.player1PieceInfo[i]->getInfo() << "   ";
        }
        out << '\n';

        out << "========" << '\n';
    } else {
        out << "Player 1:" << '\n';

        out << "Downloaded: " << td.player1DownloadData << "D, " 
        << td.player1DownloadVirus << "V" << '\n';

        out << "Abilities: " << td.player1AbilityRemain << '\n';

        for (int i = 0; i < 4; ++i) {
            out << td.player1PieceInfo[i]->getName() << ": ";
            if (td.player1PieceInfo[i]->isPieceRevealed()) {
                out << td.player1PieceInfo[i]->getInfo();
            } else {
                out << "?";
            }
            out << "   ";
        }
        out << '\n';

        for (int i = 4; i < 8; ++i) {
            out << td.player1PieceInfo[i]->getName() << ": ";
            if (td.player1PieceInfo[i]->isPieceRevealed()) {
                out << td.player1PieceInfo[i]->getInfo();
            } else {
                out << "?";
            }
            out << "   ";
        }
        out << '\n';
    }


    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            out << td.theBoard[i][j];
        }
        out << '\n';
    }

    out << "========" << '\n';

    if (td.player1First) {
        out << "Player 2:" << '\n';

        out << "Downloaded: " << td.player2DownloadData << "D, " 
        << td.player2DownloadVirus << "V" << '\n';

        out << "Abilities: " << td.player2AbilityRemain << '\n';

        for (int i = 0; i < 4; ++i) {
            out << td.player2PieceInfo[i]->getName() << ": ";
            if (td.player2PieceInfo[i]->isPieceRevealed()) {
                out << td.player2PieceInfo[i]->getInfo();
            } else {
                out << "?";
            }
            out << "   ";
        }
        out << '\n';

        for (int i = 4; i < 8; ++i) {
            out << td.player2PieceInfo[i]->getName() << ": ";
            if (td.player2PieceInfo[i]->isPieceRevealed()) {
                out << td.player2PieceInfo[i]->getInfo();
            } else {
                out << "?";
            }
            out << "   ";
        }
        out << '\n';
    } else {
        out << "Player 2:" << '\n';

        out << "Downloaded: " << td.player2DownloadData << "D, " 
        << td.player2DownloadVirus << "V" << '\n';

        out << "Abilities: " << td.player2AbilityRemain << '\n';

        for (int i = 0; i < 4; ++i) {
            out << td.player2PieceInfo[i]->getName() << ": " << td.player2PieceInfo[i]->getInfo() << "   ";
        }
        out << '\n';

        for (int i = 4; i < 8; ++i) {
            out << td.player2PieceInfo[i]->getName() << ": " << td.player2PieceInfo[i]->getInfo() << "   ";
        }
        out << '\n';

        out << "========" << '\n';
    }
    out << '\n';
    return out;
}
