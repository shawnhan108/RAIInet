#include "board.h"
#include "graphicsdisplay.h"
#include "textdisplay.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

int myabs(int x) {
    if (x < 0)
        return -x;
    return x;
}

// count how many steps will y step on my face
int step(Piece x, Piece y) {

    int count = 0;
    int x0 = x.getPosx();
    int x1 = x.getPosy();
    int y0 = y.getPosx();
    int y1 = y.getPosy();
    int xDis = myabs(x0 - y0);
    int yDis = myabs(x1 - y1);
    int boostY = y.getBoost();

    // if we cant move diagonal
    if (!y.getDiagonal()) {
        // only if they can reach me
        if ((xDis % boostY == 0) && (yDis % boostY == 0)) {
            count = xDis / boostY + yDis / boostY;
        } else {
            count = 999;
        }
    } else {
        if (boostY == 1) {
            if (xDis < yDis) {
                count = yDis;
            } else {
                count = xDis;
            }
        } else {
            // do some sht here
            // link boost + diagonal move
            if (boostY == 2) {
                if ((xDis + yDis) % 2 == 0) {
                    count = (xDis + yDis) / 2;
                } else {
                    count = 999;
                }
            } /*else {
                if ((xDis % boostY == 0) && (yDis % boostY == 0)) {
                    count = xDis / boostY + yDis / boostY;
                } else {
                    count = 999;
            }*/else {
                if ((xDis + yDis) % 2 == 0) {
                    if ((xDis == 3) && (x1 - 4 >= 0)) {
                        xDis = 1;
                        ++count;
                    } if ((yDis == 3) && (y1 - 4 >= 0)) {
                        yDis = 1;
                        ++count;
                    } if (xDis >= 4) {
                        ++count;
                        xDis -= 4;
                    } if (yDis >= 4) {
                        ++count;
                        yDis -= 4;
                    }
                    count += (xDis + yDis) / 2;
                    if (xDis != yDis) {
                        ++count;
                    }
                } else {
                    count = -999;
                }
            }
        }
    }
    return count;
}

GraphicsDisplayPtr Board::getGD(){
    return this->gd;
}

bool Board::hasWon() {
    if ((player1->getDownDataNum() == 4) ||
        (player2->getDownVirusNum() == 4)) {
        cout << "Congradulation!! Player1 you winnnnnn!!!!" << endl;
        return true;
    }
    if ((player2->getDownDataNum() == 4) ||
        (player1->getDownVirusNum() == 4)) {
        cout << "Congradulation!! Player2 you winnnnnn!!!!" << endl;
        return true;
    }
    return false;
}

void Board::init(
    string ids1, string ids2, string abs1, string abs2,
    bool initGraph) { // ids like v1v3v5.. abs is ablities
    // x is row and y is col

    player1 = PlayerPtr(new Player{"player1", ids1, abs1});
    player2 = PlayerPtr(new Player{"player2", ids2, abs2});
    for (int i = 0; i < 8; ++i) {
        player1Piece.emplace_back((player1->getPieces())[i]);
        player2Piece.emplace_back((player2->getPieces())[i]);
    }
    td = TextDisplayPtr(new TextDisplay());
    this->initGraph = initGraph;
    if (initGraph) {
        gd = GraphicsDisplayPtr(new GraphicsDisplay());
    }

    // init cells
    for (int i = 0; i < 8; ++i) { // init normal cells
        vector<CellPtr> v;
        for (int j = 0; j < 8; ++j) {
            v.emplace_back(CellPtr(new Cell{i, j}));
        }
        cells.emplace_back(v);
    }
    for (int i = 0; i < 8; ++i) { // init the cell port
        for (int j = 0; j < 8; ++j) {
            if (i == 0 && (j == 3 || j == 4)) { // port for P1
                cells[i][j]->setPort1(true);
            }
            if (i == 7 && (j == 3 || j == 4)) { // port for P2
                cells[i][j]->setPort2(true);
            }
        }
    }

    // place all the pieces to corresponding cell
    // for p1
    placePiece(0, 0, "a");
    placePiece(0, 1, "b");
    placePiece(0, 2, "c");
    placePiece(0, 5, "f");
    placePiece(0, 6, "g");
    placePiece(0, 7, "h");
    placePiece(1, 3, "d");
    placePiece(1, 4, "e");
    // for p2
    placePiece(7, 0, "A");
    placePiece(7, 1, "B");
    placePiece(7, 2, "C");
    placePiece(7, 5, "F");
    placePiece(7, 6, "G");
    placePiece(7, 7, "H");
    placePiece(6, 3, "D");
    placePiece(6, 4, "E");
    // init port
    cells[0][3]->setPort1(true);
    cells[0][4]->setPort1(true);
    cells[7][3]->setPort2(true);
    cells[7][4]->setPort2(true);
}

void Board::placePiece(int x, int y,
                       string ID) { // x is row and y is col
    // mutate each piece
    if (ID[0] >= 'a') { // the ID is for player1
        for (int i = 0; i < 8; ++i) {
            if (ID[0] == 'a' + i) {
                // mutate corresponding p1 cell
                for (int a = 0; a < 8; ++a) {
                    for (int j = 0; j < 8; ++j) {
                        if (a == x && j == y) {
                            cells[a][j]->setOccP1(true);
                            cells[a][j]->setPiece(player1Piece[i]);
                            cells[a][j]->setName(ID);
                        }
                    }
                }
            }
        }
    } else { // the ID is for player2
        for (int i = 0; i < 8; ++i) {
            if (ID[0] == 'A' + i) {
                // mutate corresponding p2 cell
                for (int a = 0; a < 8; ++a) {
                    for (int j = 0; j < 8; ++j) {
                        if (a == x && j == y) {
                            cells[a][j]->setOccP2(true);
                            cells[a][j]->setPiece(player2Piece[i]);
                            cells[a][j]->setName(ID);
                        }
                    }
                }
            }
        }
    }
    td->notify(this);
    if (this->initGraph) {
        gd->notify(this);
    }
}

void Board::battle(PiecePtr selfpiece, PiecePtr opp) {
    // sideeffect will change the downloadnumber of the player who won
    // the battle
    // determine which player has self piece
    player1->addRevealedNum();
    player2->addRevealedNum();
    PlayerPtr self;
    PlayerPtr oppP;
    if (selfpiece->getOwner() == "player1") { // check if it is P1
        self = player1;
        oppP = player2;
    } else { // check if it is P2
        self = player2;
        oppP = player1;
    }
    int oppstr = opp->getStrength();
    int cellnum = cells.size();
    int strength = selfpiece->getStrength();
    if (strength >= oppstr) { // self has bigger strength going to
                              // download the data or the virus
        oppP->subAliveNum(); // the opponent player is the loser which
                             // will lose one piece
        if (opp->getType() == "virus") {
            self->addDownVirusNum(); // change selfplayer's download
                                     // number
            self->subAliveNum();
            opp->setDead();
            selfpiece->setRevealed();
            opp->setRevealed();
            // change self cell since it won and change the position
            // to the loser's position
            for (int i = 0; i < cellnum; ++i) {
                for (int j = 0; j < cellnum; ++j) {
                    if (cells[i][j]->getName() ==
                        selfpiece
                            ->getName()) { // moving the pieces and
                                           // change the self's
                                           // original cell
                        if (self->getName() ==
                            "player1") { // self is p1
                            cells[i][j]->setOccP1(false);
                        } else { // self is p2
                            cells[i][j]->setOccP2(false);
                        }
                        cells[i][j]->setName(
                            ""); // since no pieces in the original
                                 // cell
                        cells[i][j]->setPiece(nullptr);
                    }
                    if (cells[i][j]->getName() ==
                        opp->getName()) { // change the name of the
                                          // target cell since we won
                                          // the game
                        if (self->getName() ==
                            "player1") { // change the occupation
                                         // situation rob!!! the loser
                            cells[i][j]->setOccP1(true);
                            cells[i][j]->setOccP2(false);
                        } else {
                            cells[i][j]->setOccP1(false);
                            cells[i][j]->setOccP2(true);
                        }
                        cells[i][j]->setName(
                            selfpiece
                                ->getName()); // change the name of
                                              // the loser's cell
                        cells[i][j]->setPiece(selfpiece);
                        selfpiece->setPos(cells[i][j]->getx(),
                                          cells[i][j]->gety());
                    }
                }
            }
        } else {
            self->addDownDataNum(); // change selfplayer's download
                                    // number
            opp->setDead();
            selfpiece->setRevealed();
            opp->setRevealed();
            // remove opp's Data
            for (int i = 0; i < cellnum; ++i) {
                for (int j = 0; j < cellnum; ++j) {
                    if (cells[i][j]->getName() ==
                        selfpiece
                            ->getName()) { // moving the pieces and
                                           // change the self's
                                           // original cell
                        if (self->getName() == "player1") {
                            cells[i][j]->setOccP1(false);
                        } else { // self is p2
                            cells[i][j]->setOccP2(false);
                        }
                        cells[i][j]->setName(
                            ""); // since no pieces in the original
                                 // cell
                        cells[i][j]->setPiece(nullptr);
                    }
                    if (cells[i][j]->getName() ==
                        opp->getName()) { // change the name of the
                                          // target cell since we won
                                          // the game
                        if (self->getName() ==
                            "player1") { // change the occupation
                                         // situation rob!!! the loser
                            cells[i][j]->setOccP1(true);
                            cells[i][j]->setOccP2(false);
                        } else {
                            cells[i][j]->setOccP1(false);
                            cells[i][j]->setOccP2(true);
                        }
                        cells[i][j]->setName(
                            selfpiece
                                ->getName()); // change the name of
                                              // the loser's cell
                        cells[i][j]->setPiece(selfpiece);
                        selfpiece->setPos(
                            cells[i][j]->getx(),
                            cells[i][j]->gety()); // change the pos of
                                                  // the moving piece
                    }
                }
            }
        }
    } else {
        self->subAliveNum(); // the self player is the loser which
                             // will lose one piece
        if (selfpiece->getType() == "virus") {
            oppP->addDownVirusNum(); // change otherplayer's download
                                     // number
            selfpiece->setDead();
            selfpiece->setRevealed();
            opp->setRevealed();
            // remove self Virus
            for (int i = 0; i < cellnum; ++i) {
                for (int j = 0; j < cellnum; ++j) {
                    if (cells[i][j]->getName() ==
                        selfpiece->getName()) {
                        if (self->getName() == "player1") {
                            cells[i][j]->setOccP1(false);
                        } else {
                            cells[i][j]->setOccP2(false);
                        }
                        cells[i][j]->setName(
                            ""); // since no pieces in the original
                                 // cell
                        cells[i][j]->setPiece(nullptr);
                    }
                }
            }
        } else {
            oppP->addDownDataNum(); // change otherplayer's download
                                    // number
            selfpiece->setDead();
            selfpiece->setRevealed();
            opp->setRevealed();
            // remove self Data
            for (int i = 0; i < cellnum; ++i) {
                for (int j = 0; j < cellnum; ++j) {
                    if (cells[i][j]->getName() ==
                        selfpiece->getName()) {
                        if (self->getName() == "player1") {
                            cells[i][j]->setOccP1(false);
                        } else {
                            cells[i][j]->setOccP2(false);
                        }
                        cells[i][j]->setName(
                            ""); // since no pieces in the original
                                 // cell
                        cells[i][j]->setPiece(nullptr);
                    }
                }
            }
        }
    }
}

// isSelfPiece(p, who) is going find out if the piece p is owned by
// the player who
bool isSelfPiece(PiecePtr p, PlayerPtr who) {
    if (p) {
        string ID = p->getName();
        for (int i = 0; i < 8; ++i) {
            if (ID == (who->getPieces()[i]->getName())) {
                return true;
            }
        }
    }
    return false;
}

// isSelfPort(c, p) is going find out if the cell c is the player p's
// port
bool isSelfPort(CellPtr c, PlayerPtr p) {
    if ((c->ifp1port() == true) && (p->getName() == "player1")) {
        return true;
    }
    if ((c->ifp2port() == true) && (p->getName() == "player2")) {
        return true;
    }
    return false;
}

// isOppPort(c, p) is going find out if the cell c is the opponent
// player p's port
bool isOppPort(CellPtr c, PlayerPtr p) {
    if ((c->ifp1port() == true) && (p->getName() == "player2")) {
        return true;
    }
    if ((c->ifp2port() == true) && (p->getName() == "player1")) {
        return true;
    }
    return false;
}

// player p is going to download the link in cell
void downloadLink(PiecePtr piece, CellPtr c, PlayerPtr player) {
    if (piece->getType() == "virus") {
        player->addDownVirusNum();
        piece->setDead();
        piece->setRevealed();
    } else {
        player->addDownDataNum();
        piece->setDead();
        piece->setRevealed();
    }
    c->setOccP1(false);
    c->setOccP2(false);
    c->setName("");
    c->setPiece(nullptr);
}

// isemptyCell(c) is going to find out if the cell c is occupied by
// any other piece
bool isemptyCell(CellPtr c) {
    if (c->getName() == "") {
        return true;
    } else {
        return false;
    }
}

// occuppyCell(p, c1, c2) is going implement which the piece p from
// cell c1 is going to captured the cell c2
void occuppyCell(PiecePtr p, CellPtr c1, CellPtr c2,
                 PlayerPtr player1, PlayerPtr player2) {
    if ((p->getName())[0] >= 'a') { // the piece belongs to player1
                                    // detecting the firewall
        if (c2->hasFirewallTwo() == true) {
            p->setRevealed();
            player1->addRevealedNum();
            if (p->getType() == "virus") {
                player1->subAliveNum();
                downloadLink(p, c1, player1);
                return;
            }
        }
        c2->setOccP1(true);
    } else { // the piece belongs to player2
             // detecting the firewall
        if (c2->hasFirewallOne() == true) {
            p->setRevealed();
            player2->addRevealedNum();
            if (p->getType() == "virus") {
                player2->subAliveNum();
                downloadLink(p, c1, player2);
                return;
            }
        }
        c2->setOccP2(true);
    }
    c1->setOccP1(false);
    c1->setOccP2(false);
    c1->setName("");
    c1->setPiece(nullptr);
    c2->setName(p->getName());
    c2->setPiece(p);
    p->setPos(c2->getx(), c2->gety());
}

// firewallDetect is going to dectect if there is a fire wall in the
// des cell against the piece in tar cell
bool firewallDetect(CellPtr tar, CellPtr des, PlayerPtr player1,
                    PlayerPtr player2) {
    PiecePtr target = tar->whoOcc();
    if ((target->getName())[0] >= 'a') {
        if (des->hasFirewallTwo() == true) {
            player1->addRevealedNum();
            if (target->getType() == "virus") {
                player1->subAliveNum();
                downloadLink(target, tar, player1);
            }
            return true;
        }
    } else {
        if (des->hasFirewallOne() == true) {
            player2->addRevealedNum();
            if (target->getType() == "virus") {
                player2->subAliveNum();
                downloadLink(target, tar, player2);
            }
            return true;
        }
    }
    return false;
}

void Board::movePiece(PlayerPtr who, std::string ID,
                      std::string dir) {
    PiecePtr target; // the pointer to the piece we want to move
    // PlayerPtr who; // who is moving
    bool isFound = false;
    for (size_t k = 0; k < who->getPieces().size(); ++k) {
        if (who->getPieces()[k]->getName() == ID) {
            isFound = true;
            if ((!(who->getPieces()[k]->isPieceAlive()))) {
                throw std::logic_error{
                    "The link you intended to move (" + ID +
                    ") has been downloaded."};
            }
        }
    }
    if (!isFound) {
        throw std::logic_error{"The link you intended to move (" +
                               ID + ") is not your link"};
    }

    PlayerPtr opponent = (who->getName() == "player1") ? this->player2 : this->player1;
    CellPtr curcell; // the current cell
    int row_x;
    int col_y;
    int boost;
    for (int i = 0; i < 8; ++i) { // find the target piece
        for (int j = 0; j < 8; ++j) {

            if ((cells[i][j]) && (cells[i][j]->getName() == ID)) {
                curcell = cells[i][j];
                target = curcell->whoOcc();
                if (!(target)) {
                    return;
                } // if the cell has no piece
                if (!(target->isPieceAlive())) {
                    return;
                } // if piece is dead
                if (who->getName() == "player1" &&
                    curcell->ifOccP2()) {
                    throw std::logic_error{
                        "You cannot move your opponent's link. "};
                }
                if (who->getName() == "player2" &&
                    curcell->ifOccP1()) {
                    throw std::logic_error{
                        "You cannot move your opponent's link. "};
                }
            }
        }
    }
    if (!(target)) {
        return;
    }
    row_x = curcell->getx();
    col_y = curcell->gety();
    boost = target->getBoost();

    // for diagonal move
    if ((!target->getDiagonal()) &&
        ((dir == "NE") || (dir == "NW") || (dir == "SW") ||
         (dir == "SE"))) {
        throw std::logic_error{
            "The piece (" + ID +
            ") you intended to move diagonally (" + dir +
            ") does not have diagonal ability applied."};
    }
    if (target->getDiagonal()) {
        if (dir == "NE") {
            if (who->ifoverBaseLimit(target->getPosx() - 1)) {
                throw std::out_of_range{
                    "You cannot move into your own base. "};
            } else if (who->ifoverOppLimit(target->getPosx() - 1)) {
                downloadLink(target, curcell, who);
            } else if ((target->getPosy() + 1) > 7) {
                throw std::out_of_range{
                    "You cannot move out of the right boundary of "
                    "the board."};
            } else if (isSelfPiece(
                           cells[row_x - 1][col_y + 1]->whoOcc(),
                           who)) {
                throw std::logic_error{
                    "You cannot download your own link."};
            } else if (isSelfPort(cells[row_x - 1][col_y + 1], who)) {
                throw std::logic_error{
                    "You cannot move into your own port."};
            } else if (isOppPort(cells[row_x - 1][col_y + 1], who)) {
                downloadLink(target, curcell, opponent);
            } else if (isemptyCell(cells[row_x - 1][col_y + 1])) {
                occuppyCell(target, cells[row_x][col_y],
                            cells[row_x - 1][col_y + 1], player1,
                            player2);
            } else { // there is a piece in the cell that we are going
                     // to
                PiecePtr oppPiece =
                    (cells[row_x - 1][col_y + 1])->whoOcc();
                // detect fireWall first
                if (firewallDetect(cells[row_x][col_y],
                                   cells[row_x - 1][col_y + 1],
                                   player1, player2)) {
                    td->notify(this);
                    if (initGraph) {
                        
                        gd->notify(this);
                    }
                    return;
                }
                this->battle(target, oppPiece);
            }
        } else if (dir == "NW") {
            if (who->ifoverBaseLimit(target->getPosx() - 1)) {
                throw std::out_of_range{
                    "You cannot move into your own base. "};
            } else if (who->ifoverOppLimit(target->getPosx() - 1)) {
                downloadLink(target, curcell, who);
            } else if ((target->getPosy() - 1) < 0) {
                throw std::out_of_range{
                    "You cannot move out of the left boundary of the "
                    "board."};
            } else if (isSelfPiece(
                           cells[row_x - 1][col_y - 1]->whoOcc(),
                           who)) {
                throw std::logic_error{
                    "You cannot download your own link."};
            } else if (isSelfPort(cells[row_x - 1][col_y - 1], who)) {
                throw std::logic_error{
                    "You cannot move into your own port."};
            } else if (isOppPort(cells[row_x - 1][col_y - 1], who)) {
                // next cell is a oppnoent port
                downloadLink(target, curcell, opponent);
            } else if (isemptyCell(cells[row_x - 1][col_y - 1])) {
                occuppyCell(target, cells[row_x][col_y],
                            cells[row_x - 1][col_y - 1], player1,
                            player2);
            } else { // there is a piece in the cell that we are going
                     // to
                PiecePtr oppPiece =
                    (cells[row_x - 1][col_y - 1])->whoOcc();
                // detect fireWall first
                if (firewallDetect(cells[row_x][col_y],
                                   cells[row_x - 1][col_y - 1],
                                   player1, player2)) {
                    td->notify(this);
                    if (initGraph) {
                        gd->notify(this);
                    }
                    return;
                }
                this->battle(target, oppPiece);
            }
        } else if (dir == "SE") {
            if (who->ifoverBaseLimit(target->getPosx() + 1)) {
                throw std::out_of_range{
                    "You cannot move into your own base. "};
            } else if (who->ifoverOppLimit(target->getPosx() + 1)) {
                downloadLink(target, curcell, who);
            } else if ((target->getPosy() + 1) > 7) {
                throw std::out_of_range{
                    "You cannot move out of the right boundary of "
                    "the board."};
            } else if (isSelfPiece(
                           cells[row_x + 1][col_y + 1]->whoOcc(),
                           who)) {
                throw std::logic_error{
                    "You cannot download your own link."};
            } else if (isSelfPort(cells[row_x + 1][col_y + 1], who)) {
                throw std::logic_error{
                    "You cannot move into your own port."};
            } else if (isOppPort(cells[row_x + 1][col_y + 1], who)) {
                // next cell is a oppnoent port
                downloadLink(target, curcell, opponent);
            } else if (isemptyCell(cells[row_x + 1][col_y + 1])) {
                occuppyCell(target, cells[row_x][col_y],
                            cells[row_x + 1][col_y + 1], player1,
                            player2);
            } else { // there is a piece in the cell that we are going
                     // to
                PiecePtr oppPiece =
                    (cells[row_x + 1][col_y + 1])->whoOcc();
                // detect fireWall first
                if (firewallDetect(cells[row_x][col_y],
                                   cells[row_x + 1][col_y + 1],
                                   player1, player2)) {
                    td->notify(this);
                    if (initGraph) {
                        
                        gd->notify(this);
                    }
                    return;
                }
                this->battle(target, oppPiece);
            }
        } else if (dir == "SW") {
            if (who->ifoverBaseLimit(target->getPosx() + 1)) {
                throw std::out_of_range{
                    "You cannot move into your own base. "};
            } else if (who->ifoverOppLimit(target->getPosx() + 1)) {
                downloadLink(target, curcell, who);
            } else if ((target->getPosy() - 1) < 0) {
                throw std::out_of_range{
                    "You cannot move out of the left boundary of the "
                    "board."};
            } else if (isSelfPiece(
                           cells[row_x + 1][col_y - 1]->whoOcc(),
                           who)) {
                throw std::logic_error{
                    "You cannot download your own link."};
            } else if (isSelfPort(cells[row_x + 1][col_y - 1], who)) {
                throw std::logic_error{
                    "You cannot move into your own port."};
            } else if (isOppPort(cells[row_x + 1][col_y - 1], who)) {
                // next cell is a oppnoent port
                downloadLink(target, curcell, opponent);
            } else if (isemptyCell(cells[row_x + 1][col_y - 1])) {
                occuppyCell(target, cells[row_x][col_y],
                            cells[row_x + 1][col_y - 1], player1,
                            player2);
            } else { // there is a piece in the cell that we are going
                     // to
                PiecePtr oppPiece =
                    (cells[row_x + 1][col_y - 1])->whoOcc();
                // detect fireWall first
                if (firewallDetect(cells[row_x][col_y],
                                   cells[row_x + 1][col_y - 1],
                                   player1, player2)) {
                    td->notify(this);
                    if (initGraph) {
                        
                        gd->notify(this);
                    }
                    return;
                }
                this->battle(target, oppPiece);
            }
        }
    }

    // normal move without ablity
    if (dir == "N") {
        if (who->ifoverBaseLimit(target->getPosx() - boost)) {
            throw std::out_of_range{
                "You cannot move into your own base. "};
        } else if (who->ifoverOppLimit(target->getPosx() - boost)) {
            downloadLink(target, curcell, who);
        } else if (isSelfPiece(cells[row_x - boost][col_y]->whoOcc(),
                               who)) {
            throw std::logic_error{
                "You cannot download your own link."};
        } else if (isSelfPort(cells[row_x - boost][col_y], who)) {
            throw std::logic_error{
                "You cannot move into your own port."};
        } else if (isOppPort(cells[row_x - boost][col_y], who)) {
            downloadLink(target, curcell, opponent);
        } else if (isemptyCell(cells[row_x - boost][col_y])) {
            occuppyCell(target, cells[row_x][col_y],
                        cells[row_x - boost][col_y], player1,
                        player2);
        } else { // there is a piece in the cell that we are going to
            PiecePtr oppPiece =
                (cells[row_x - boost][col_y])->whoOcc();
            // detect fireWall first
            if (firewallDetect(cells[row_x][col_y],
                               cells[row_x - boost][col_y], player1,
                               player2)) {
                td->notify(this);
                if (initGraph) {
                    
                    gd->notify(this);
                }
                return;
            }
            this->battle(target, oppPiece);
        }
    } else if (dir == "S") {
        if (who->ifoverBaseLimit(target->getPosx() + boost)) {
            throw std::out_of_range{
                "You cannot move into your own base. "};
        } else if (who->ifoverOppLimit(target->getPosx() + boost)) {
            downloadLink(target, curcell, who);
        } else if (isSelfPiece(cells[row_x + boost][col_y]->whoOcc(),
                               who)) {
            throw std::logic_error{
                "You cannot download your own link."};
        } else if (isSelfPort(cells[row_x + boost][col_y], who)) {
            throw std::logic_error{
                "You cannot move into your own port."};
        } else if (isOppPort(cells[row_x + boost][col_y], who)) {
            downloadLink(target, curcell, opponent);
        } else if (isemptyCell(cells[row_x + boost][col_y])) {
            occuppyCell(target, cells[row_x][col_y],
                        cells[row_x + boost][col_y], player1,
                        player2);
        } else { // there is a piece in the cell that we are going to
            PiecePtr oppPiece =
                (cells[row_x + boost][col_y])->whoOcc();
            if (firewallDetect(cells[row_x][col_y],
                               cells[row_x + boost][col_y], player1,
                               player2)) {
                td->notify(this);
                if (initGraph) {
                    
                    gd->notify(this);
                }
                return;
            }
            this->battle(target, oppPiece);
        }
    } else if (dir == "E") {
        if ((target->getPosy() + boost) > 7) {
            throw std::out_of_range{"You cannot move out of the left "
                                    "boundary of the board."};
        } else if (isSelfPiece(cells[row_x][col_y + boost]->whoOcc(),
                               who)) {
            throw std::logic_error{
                "You cannot download your own link."};
        } else if (isSelfPort(cells[row_x][col_y + boost], who)) {
            throw std::logic_error{
                "You cannot move into your own port."};
        } else if (isOppPort(cells[row_x][col_y + boost], who)) {
            downloadLink(target, curcell, opponent);
        } else if (isemptyCell(cells[row_x][col_y + boost])) {
            occuppyCell(target, cells[row_x][col_y],
                        cells[row_x][col_y + boost], player1,
                        player2);
        } else { // there is a piece in the cell that we are going to
            PiecePtr oppPiece =
                (cells[row_x][col_y + boost])->whoOcc();
            if (firewallDetect(cells[row_x][col_y],
                               cells[row_x][col_y + boost], player1,
                               player2)) {
                td->notify(this);
                if (initGraph) {
                    
                    gd->notify(this);
                }
                return;
            }
            this->battle(target, oppPiece);
        }
    } else if (dir == "W") {
        if ((target->getPosy() - boost) < 0) {
            throw std::out_of_range{"You cannot move out of the "
                                    "right boundary of the board."};
        } else if (isSelfPiece(cells[row_x][col_y - boost]->whoOcc(),
                               who)) {
            throw std::logic_error{
                "You cannot download your own link."};
        } else if (isSelfPort(cells[row_x][col_y - boost], who)) {
            throw std::logic_error{
                "You cannot move into your own port."};
        } else if (isOppPort(cells[row_x][col_y - boost], who)) {
            downloadLink(target, curcell, opponent);
        } else if (isemptyCell(cells[row_x][col_y - boost])) {
            occuppyCell(target, cells[row_x][col_y],
                        cells[row_x][col_y - boost], player1,
                        player2);
        } else { // there is a piece in the cell that we are going to
            PiecePtr oppPiece =
                (cells[row_x][col_y - boost])->whoOcc();
            if (firewallDetect(cells[row_x][col_y],
                               cells[row_x][col_y - boost], player1,
                               player2)) {
                td->notify(this);
                if (initGraph) {
                    
                    gd->notify(this);
                }
                return;
            }
            this->battle(target, oppPiece);
        }
    }

    td->notify(
        this); // notify the textdisplay to update the information
    if (initGraph) {
        
        gd->notify(this);
    }
}

std::ostream &operator<<(std::ostream &out, const Board &b) {
    out << *(b.td);
    return out;
}

void Board::Playerflip() { td->setPlayer(); }

Board::~Board() {}

std::vector<Piece> moveMove(Piece p) {
    // normal = 1, normal move; normal = 0, move diagonal
    std::vector<Piece> nmsl;
    Piece tmp = p;

    // nmsl[0], move right in x axis
    int newX0 = p.getPosx() + p.getBoost();
    if (newX0 <= 7) {
        tmp.setPos(newX0, p.getPosy());
        nmsl.emplace_back(tmp);
        tmp = p;
    }

    // nmsl[1], move left in x axis
    int newX1 = p.getPosx() - p.getBoost();
    if (newX1 >= 0) {
        tmp.setPos(newX1, p.getPosy());
        nmsl.emplace_back(tmp);
        tmp = p;
    }

    // nmsl[2], move up in y axis
    int newY0 = p.getPosy() + p.getBoost();
    if (newY0 <= 7) {
        tmp.setPos(p.getPosx(), newY0);
        nmsl.emplace_back(tmp);
        tmp = p;
    }

    // nmsl[3], move down in y axis
    int newY1 = p.getPosy() - p.getBoost();
    if (newY0 >= 0) {
        tmp.setPos(p.getPosx(), newY1);
        nmsl.emplace_back(tmp);
        tmp = p;
    }

    if (p.getDiagonal() && p.getBoost() != 4) {

        // nmsl[4], move right-up
        int newX2 = p.getPosx() + 1;
        int newY2 = p.getPosy() + 1;
        if ((newX2 <= 7) && (newY2 <= 7)) {
            tmp.setPos(newX2, newY2);
            nmsl.emplace_back(tmp);
            tmp = p;
        }

        // nmsl[5], move left-down
        int newX3 = p.getPosx() - 1;
        int newY3 = p.getPosy() - 1;
        if ((newX3 >= 0) && (newY3 >= 0)) {
            tmp.setPos(newX3, newY3);
            nmsl.emplace_back(tmp);
            tmp = p;
        }

        // nmsl[6], move right-down
        int newX4 = p.getPosx() + 1;
        int newY4 = p.getPosy() - 1;
        if ((newX4 <= 7) && (newY4 >= 0)) {
            tmp.setPos(newX4, newY4);
            nmsl.emplace_back(tmp);
            tmp = p;
        }

        // nmsl[7], move left-up
        int newX5 = p.getPosx() - 1;
        int newY5 = p.getPosy() + 1;
        if ((newY5 <= 7) && (newX5 >= 0)) {
            tmp.setPos(newX5, newY5);
            nmsl.emplace_back(tmp);
            tmp = p;
        }
    }
    return nmsl;
}

// p1 move to p2
void moveTo(Piece p1, Piece p2, PlayerPtr who, Board *b) {
    int y = p1.getPosx() - p2.getPosx();
    int x = p1.getPosy() - p2.getPosy();
    std::string direction = "";
    if (y < 0) {
        direction = direction + "S";
        if (x < 0) {
            direction = direction + "E";
        } else if (x > 0) {
            direction = direction + "W";
        }
    } else if (y > 0) {
        direction = direction + "N";
        if (x < 0) {
            direction = direction + "E";
        } else if (x > 0) {
            direction = direction + "W";
        }
    } else {
        if (x < 0) {
            direction = direction + "E";
        } else if (x > 0) {
            direction = direction + "W";
        }
    }
    //cout << direction << endl;
    //cout << "MoveTo function says: pos x: " << x << "pos y: " << y << endl;
    b->movePiece(who, p1.getName(), direction);
}

bool isSelfPiecePosCheck(Piece p, PlayerPtr pp) {
    int x = p.getPosx();
    int y = p.getPosy();
    for (int i = 0; i < 8; ++i) {
        if (pp->getPieces()[i]->isPieceAlive()) {
            if ((pp->getPieces()[i]->getPosx() == x) && (pp->getPieces()[i]->getPosy() == y)) {
                return true;
            } 
        }
    }
    return false;
}

int weightedSafe(std::vector<int> enemyStep) {
    if (enemyStep.size() == 0) {
        return -999;
    }
    int maxi = enemyStep[0];
    for (size_t i = 0; i < enemyStep.size(); ++i) {
        if (enemyStep[i] > maxi) {
            maxi = enemyStep[i];
        }
    }
    return maxi;
}

void Board::useAbility(string playerName, int abilityIndex, string param1, string param2) {

    PlayerPtr thePlayer =
        (playerName == "player1") ? this->player1 : this->player2;
    PlayerPtr opponent =
        (playerName == "player2") ? this->player1 : this->player2;

    if (abilityIndex > 5) {
        throw std::out_of_range{"You only have 5 abilities. Please "
                                "enter a valid choice."};
    }
    string theAbility = thePlayer->getAbilities()[abilityIndex - 1];

    char theRealAbility = theAbility[0];
    vector<PiecePtr> thePieces = thePlayer->getPieces();
    vector<PiecePtr> oppoPieces = opponent->getPieces();

    if (theRealAbility == 'L') { // LinkBoost. Param1 holds the ID of the piece
        bool found = false;
        for (unsigned int i = 0; i < thePieces.size(); ++i) {
            if (thePieces[i]->getName() == param1) {
                found = true;
                thePieces[i]->setBoost(thePieces[i]->getBoost() * 2);
                break;
            }
        }
        if (!found) {
            string abilityLeft = "";
            for (size_t j = 0; j < thePlayer->getAbilities().size();
                 ++j) {
                abilityLeft += thePlayer->getAbilities()[j];
            }
            throw std::out_of_range{
                "The ability (" + theAbility +
                ") cannot be applied to the piece (" + param1 +
                ") you indicated. Abilities left: " + abilityLeft};
        }
    } else if (theRealAbility == 'F') { // Firewall. Param1 holds x, param2 holds y.
        if ((!(isdigit(param1[0]))) || (!(isdigit(param2[0]))) ||
            (param1.length() > 1) || (param2.length() > 1)) {
            throw std::out_of_range{
                "Please re-enter a valid cell coordinate (0 ≤ x, y "
                "≤ 7). You entered (" +
                param1 + " ," + param2 + ")."};
        }
        istringstream temp1{param1};
        int x;
        temp1 >> x;
        istringstream temp2{param2};
        int y;
        temp2 >> y;

        if ((x > 7) || (y > 7) || (x < 0) || (y < 0)) {
            throw std::out_of_range{
                "Please re-enter a valid cell coordinate (0 ≤ x, y "
                "≤ 7). You entered (" +
                param1 + " ," + param2 + ")."};
        }

        CellPtr theCell = (this->cells)[x][y];
        if (theCell->ifOccP1() || theCell->ifOccP2()) {
            throw std::out_of_range{
                "The cell you indicated (" + param1 + " ," + param2 +
                ") has been occupied. Please re-enter."};
        }
        if (theCell->ifp1port() || theCell->ifp2port()) {
            throw std::out_of_range{"The cell you indicated (" +
                                    param1 + " ," + param2 +
                                    ") is a port. Please re-enter."};
        }

        if (playerName == "player1") {
            theCell->setFirewallOne(true);
        } else {
            theCell->setFirewallTwo(true);
        }
    } else if (theRealAbility == 'D') { // Download. Param1 holds the ID of the piece
        bool found = false;
        for (unsigned int i = 0; i < oppoPieces.size(); ++i) {
            if (oppoPieces[i]->getName() == param1) {
                found = true;

                // mutate piece info
                oppoPieces[i]->setDead();
                // oppoPieces[i]->setRevealed();
                bool isVirus =
                    oppoPieces[i]->getInfo()[0] == 'V' ? true : false;

                // mutate cell info
                int x = oppoPieces[i]->getPosx();
                int y = oppoPieces[i]->getPosy();
                CellPtr theCell = (this->cells)[x][y];
                theCell->setPiece(nullptr);

                bool isPlayer1sPiece =
                    (oppoPieces[i]->getOwner() == "player1") ? true
                                                             : false;
                if (isPlayer1sPiece) {
                    theCell->setOccP1(false);
                } else {
                    theCell->setOccP2(false);
                }
                theCell->setName("");

                // mutate player info
                if (isVirus) {
                    thePlayer->addDownVirusNum();
                } else {
                    thePlayer->addDownDataNum();
                }
                // opponent->addRevealedNum();
                break;
            }
        }

        if (!found) {
            string abilityLeft = "";
            for (size_t j = 0; j < thePlayer->getAbilities().size();
                 ++j) {
                abilityLeft += thePlayer->getAbilities()[j];
            }
            throw std::out_of_range{
                "The ability (" + theAbility +
                ") cannot be applied to the piece (" + param1 +
                ") you indicated. Abilities left: " + abilityLeft};
        }

    } else if (theRealAbility == 'S') { // Scan, Param1 holds the ID of the piece
        bool found = false;
        for (unsigned int i = 0; i < oppoPieces.size(); ++i) {
            if (oppoPieces[i]->getName() == param1) {
                found = true;
                oppoPieces[i]->setRevealed();
                break;
            }
        }
        if (!found) {
            string abilityLeft = "";
            for (size_t j = 0; j < thePlayer->getAbilities().size();
                 ++j) {
                abilityLeft += thePlayer->getAbilities()[j];
            }
            throw std::out_of_range{
                "The ability (" + theAbility +
                ") cannot be applied to the piece (" + param1 +
                ") you indicated. Abilities left: " + abilityLeft};
        }
        thePlayer->addRevealedNum();

    } else if (theRealAbility == 'P') { // Polarize, Param1 holds the ID of the piece
        bool found = false;
        for (unsigned int i = 0; i < oppoPieces.size(); ++i) {
            if (oppoPieces[i]->getName() == param1) {
                found = true;
                oppoPieces[i]->switchType();

                // check if the piece is on firewall
                int x = oppoPieces[i]->getPosx();
                int y = oppoPieces[i]->getPosy();
                CellPtr theCell = (this->cells)[x][y];
                if ((theCell->hasFirewallOne()) &&
                    (oppoPieces[i]->getOwner() == "player2") &&
                    (oppoPieces[i]->getType() == "virus")) {
                    downloadLink(oppoPieces[i], theCell,
                                 this->player2);
                } else if ((theCell->hasFirewallTwo()) &&
                           (oppoPieces[i]->getOwner() == "player1") &&
                           (oppoPieces[i]->getType() == "virus")) {
                    downloadLink(oppoPieces[i], theCell,
                                 this->player1);
                }
                break;
            }
        }

        for (unsigned int j = 0; j < thePieces.size(); ++j) {
            if (thePieces[j]->getName() == param1) {
                found = true;
                thePieces[j]->switchType();
                break;
            }
        }

        if (!found) {
            string abilityLeft = "";
            for (size_t j = 0; j < thePlayer->getAbilities().size();
                 ++j) {
                abilityLeft += thePlayer->getAbilities()[j];
            }
            throw std::out_of_range{
                "The ability (" + theAbility +
                ") cannot be applied to the piece (" + param1 +
                ") you indicated. Abilities left: " + abilityLeft};
        }

    } else if (theRealAbility == 'Z') {
        bool found = false;

        for (size_t i = 0; i < thePieces.size(); ++i) {
            if (thePieces[i]->getName() == param1) {
                found = true;
                thePieces[i]->setDiagonal();
                break;
            }
        }

        if (!found) {
            string abilityLeft = "";
            for (size_t j = 0; j < thePlayer->getAbilities().size();
                 ++j) {
                abilityLeft += thePlayer->getAbilities()[j];
            }
            throw std::out_of_range{
                "The ability (" + theAbility +
                ") cannot be applied to the piece (" + param1 +
                ") you indicated. Abilities left: " + abilityLeft};
        }
    } else if (theRealAbility == 'A') { 
        // attack, the computer will move the piece to
        // attack enemy
        // input will be something like useAbility("player1", 'A')

        int a = 999;
        // int b = 999;
        Piece selfAttack;
        Piece oppoAttack;
        bool first = 1;
        int selfStep = 999;
        int oppoStep = 999;

        for (size_t i = 0; i < thePieces.size(); ++i) {
            for (size_t j = 0; j < oppoPieces.size(); ++j) {
                // check isAlive
                if ((oppoPieces[j]->getType() == "data" ) && thePieces[i]->isPieceAlive() && oppoPieces[j]->isPieceAlive() && (thePieces[i]->getStrength() >= oppoPieces[j]->getStrength())) {

                    oppoStep = step(*thePieces[i], *oppoPieces[j]);
                    selfStep = step(*oppoPieces[j], *thePieces[i]);

                    //cout << "We reached here: line 1263: oppoStep = " << oppoStep << " and selfStep = " << selfStep << endl;
                    //cout << "selfAttack is " << (*thePieces[i]).getName() << " : x = " << (*thePieces[i]).getPosx() << " y = " << (*thePieces[i]).getPosy() << endl;
                    //cout << "oppoAttack is " << (*oppoPieces[j]).getName() << " : x = " << (*oppoPieces[j]).getPosx() << " y = " << (*oppoPieces[j]).getPosy() << endl;

                    if (selfStep < oppoStep) {

                        // another question: what if diagonal?

                        // case1: A . a && selfStep == oppoStep &&  A.boost = 1 -> we dont go
                        //    A.           .A              and 2 other cases
                        //     a           a
                        // o.w. we go

                        // case2: A .. a && selfStep == oppoStep && A.boost = 2 -> we dont go similar

                        // case3: A .... a && selfStep == oppoStep && A.boost = 4 -> we dont go similar
                        // also update down there

                        if (first) {
                            a = selfStep;
                            // b = oppoStep;
                            selfAttack = *thePieces[i];
                            oppoAttack = *oppoPieces[j];
                            //cout << "Are we here line 1286? Yes we are" << " oppoStep = " << oppoStep << " and selfStep = " << selfStep << endl;
                            //cout << "selfAttack is " << selfAttack.getName() << " and oppoAttack is " << oppoAttack.getName() << endl;
                            first = 0;
                        }
                        if (selfStep < a) {
                            a = selfStep;
                            // b = oppoStep;
                            selfAttack = *thePieces[i];
                            oppoAttack = *oppoPieces[j];
                            //cout << "Are we here line 1294? Yes we are" << " oppoStep = " << oppoStep << " and selfStep = " << selfStep << endl;
                            //cout << "selfAttack is " << selfAttack.getName() << " and oppoAttack is " << oppoAttack.getName() << endl;
                        }
                    } 
                    
                    else if (selfStep == oppoStep) {
                        if ((selfStep != 2) || ((selfStep == 2) && (thePieces[i]->getStrength() > oppoPieces[j]->getStrength()))) {
                            if (first) {
                                a = selfStep;
                                // b = oppoStep;
                                selfAttack = *thePieces[i];
                                oppoAttack = *oppoPieces[j];
                                //cout << "Are we here line 1307? Yes we are" << " oppoStep = " << oppoStep << " and selfStep = " << selfStep << endl;
                                //cout << "selfAttack is " << selfAttack.getName() << " and oppoAttack is " << oppoAttack.getName() << endl;
                                first = 0;
                            }
                            if (selfStep < a) {
                                a = selfStep;
                                // b = oppoStep;
                                selfAttack = *thePieces[i];
                                oppoAttack = *oppoPieces[j];
                                //cout << "Are we here line 1310? Yes we are" << " oppoStep = " << oppoStep << " and selfStep = " << selfStep << endl;
                                //cout << "selfAttack is " << selfAttack.getName() << " and oppoAttack is " << oppoAttack.getName() << endl;
                            }
                        }
                    } else {
                        if (thePieces[i]->getStrength() > oppoPieces[j]->getStrength()) {
                            if (first) {
                                a = selfStep;
                                // b = oppoStep;
                                selfAttack = *thePieces[i];
                                oppoAttack = *oppoPieces[j];
                                //cout << "Are we here line 1307? Yes we are" << " oppoStep = " << oppoStep << " and selfStep = " << selfStep << endl;
                                //cout << "selfAttack is " << selfAttack.getName() << " and oppoAttack is " << oppoAttack.getName() << endl;
                                first = 0;
                            }
                            if (selfStep < a) {
                                a = selfStep;
                                // b = oppoStep;
                                selfAttack = *thePieces[i];
                                oppoAttack = *oppoPieces[j];
                                cout << "Are we here line 1310? Yes we are" << " oppoStep = " << oppoStep << " and selfStep = " << selfStep << endl;
                                cout << "selfAttack is " << selfAttack.getName() << " and oppoAttack is " << oppoAttack.getName() << endl;
                            }
                        }
                    }
                }
            }
        }

        // now we have the attack piece info, and the oppo piece that
        // will be attacked
        if (a != 999) {
            std::vector<Piece> checkThis = moveMove(selfAttack);

            for (size_t i = 0; i < checkThis.size(); ++i) {

                oppoStep = step(checkThis[i], oppoAttack); // may have 999 here
                selfStep = step(oppoAttack, checkThis[i]); // may have 999 here

                //cout << "Are we here line 1329? Yes we are" << " oppoStep = " << oppoStep << " and selfStep = " << selfStep << endl;
                //cout << "selfAttack is " << checkThis[i].getName() << " and oppoAttack is " << oppoAttack.getName() << endl;
                //cout << a << endl;

                // a is the previous selfStep
                if (selfStep < oppoStep) {
                    if (selfStep < a) {
                        if (playerName == "player1") {
                            //cout << "Pos x: " << selfAttack.getPosx() << " Pos y: " << selfAttack.getPosy() << endl;
                            //cout << "Pos x: " << checkThis[i].getPosx() << " Pos y: " << checkThis[i].getPosy() << endl;

                            // check move condition here
                            // isSelfPort, isOppoPort, isSelfPiece, isFirewall

                            if (!(isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1)) 
                                && (!isOppPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1))
                                && (!isSelfPiecePosCheck(checkThis[i], player1))) { 

                                    PiecePtr ppp = cells[checkThis[i].getPosx()][checkThis[i].getPosy()]->whoOcc();

                                    if (ppp != nullptr) { // have piece here
                                        if (ppp->getType() != "virus") { // we dont want to eat virus
                                            moveTo(selfAttack, checkThis[i], player1, this);
                                            return;
                                        }
                                    } else { 
                                        moveTo(selfAttack, checkThis[i], player1, this);
                                        return;
                                    }
                                }
                        } else {

                            if (!(isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2)) 
                                && (!isOppPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2))
                                && (!isSelfPiecePosCheck(checkThis[i], player2))) {


                                    PiecePtr ppp = cells[checkThis[i].getPosx()][checkThis[i].getPosy()]->whoOcc();

                                    if (ppp != nullptr) { // have piece here
                                        if (ppp->getType() != "virus") { // we dont want to eat virus
                                            moveTo(selfAttack, checkThis[i], player2, this);
                                            return;
                                        }
                                    } else { 
                                        moveTo(selfAttack, checkThis[i], player2, this);
                                        return;
                                    }
                                }
                        }
                        
                    }
                } else if (selfStep == oppoStep) {
                    //cout << "In here, and selfStep is " << selfStep << endl;
                    if (selfStep < a) {
                    //if ((selfStep < a) && (selfStep != 2)) {
                        if (playerName == "player1") {
                            //cout << "Pos x: " << selfAttack.getPosx() << " Pos y: " << selfAttack.getPosy() << endl;
                            //cout << "Pos x: " << checkThis[i].getPosx() << " Pos y: " << checkThis[i].getPosy() << endl;
                            if (!(isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1)) 
                                && (!isOppPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1))
                                && (!isSelfPiecePosCheck(checkThis[i], player1))) {

                                    PiecePtr ppp = cells[checkThis[i].getPosx()][checkThis[i].getPosy()]->whoOcc();

                                    if (ppp != nullptr) { // have piece here
                                        if (ppp->getType() != "virus") { // we dont want to eat virus
                                            moveTo(selfAttack, checkThis[i], player1, this);
                                            return;
                                        }
                                    } else { 
                                        moveTo(selfAttack, checkThis[i], player1, this);
                                        return;
                                    }
                            }
                        } else {
                            if (!(isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2)) 
                                && (!isOppPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2))
                                && (!isSelfPiecePosCheck(checkThis[i], player2))) {

                                    PiecePtr ppp = cells[checkThis[i].getPosx()][checkThis[i].getPosy()]->whoOcc();

                                    if (ppp != nullptr) { // have piece here
                                        if (ppp->getType() != "virus") { // we dont want to eat virus
                                            moveTo(selfAttack, checkThis[i], player2, this);
                                            return;
                                        }
                                    } else { 
                                        moveTo(selfAttack, checkThis[i], player2, this);
                                        return;
                                    }
                            }
                        }
                    }
                } else {
                    if (checkThis[i].getStrength() > oppoAttack.getStrength()) {
                        if (selfStep < a) {
                    //if ((selfStep < a) && (selfStep != 2)) {
                        if (playerName == "player1") {
                            //cout << "Pos x: " << selfAttack.getPosx() << " Pos y: " << selfAttack.getPosy() << endl;
                            //cout << "Pos x: " << checkThis[i].getPosx() << " Pos y: " << checkThis[i].getPosy() << endl;
                            if (!(isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1)) 
                                && (!isOppPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1))
                                && (!isSelfPiecePosCheck(checkThis[i], player1))) {

                                    PiecePtr ppp = cells[checkThis[i].getPosx()][checkThis[i].getPosy()]->whoOcc();

                                    if (ppp != nullptr) { // have piece here
                                        if (ppp->getType() != "virus") { // we dont want to eat virus
                                            moveTo(selfAttack, checkThis[i], player1, this);
                                            return;
                                        }
                                    } else { 
                                        moveTo(selfAttack, checkThis[i], player1, this);
                                        return;
                                    }
                            }
                        } else {
                            if (!(isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2)) 
                                && (!isOppPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2))
                                && (!isSelfPiecePosCheck(checkThis[i], player2))) {

                                    PiecePtr ppp = cells[checkThis[i].getPosx()][checkThis[i].getPosy()]->whoOcc();

                                    if (ppp != nullptr) { // have piece here
                                        if (ppp->getType() != "virus") { // we dont want to eat virus
                                            moveTo(selfAttack, checkThis[i], player2, this);
                                            return;
                                        }
                                    } else { 
                                        moveTo(selfAttack, checkThis[i], player2, this);
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        std::cout << "Attack hint is not available at the moment, try later" << '\n';


    } else if (theRealAbility == 'G') {

        bool found = false;
        Piece gouPiece;

        for (unsigned int i = 0; i < thePieces.size(); ++i) {
            if (thePieces[i]->getName() == param1) {
                found = true;
                gouPiece = *thePieces[i];
                break;
            }
        }
        if (!found) {
            string abilityLeft = "";
            for (size_t j = 0; j < thePlayer->getAbilities().size(); ++j) {
                abilityLeft += thePlayer->getAbilities()[j];
            }
            throw std::out_of_range{
                "The ability (" + theAbility +
                ") cannot be applied to the piece (" + param1 +
                ") you indicated. Abilities left: " + abilityLeft};
        }

        // selfStep counts how many step do i take to reach enemy
        // oppoStep counts how many step do enemy take to reach me

        if (!gouPiece.isPieceAlive()) {
            string abilityLeft = "";
            for (size_t j = 0; j < thePlayer->getAbilities().size();
                 ++j) {
                abilityLeft += thePlayer->getAbilities()[j];
            }
            throw std::out_of_range {"Why are you running? You are dead! Abilities left: " + abilityLeft };
        }

        if (gouPiece.getType() == "virus") {
            string abilityLeft = "";
            for (size_t j = 0; j < thePlayer->getAbilities().size();
                 ++j) {
                abilityLeft += thePlayer->getAbilities()[j];
            }
            throw std::out_of_range {"Why are you running? You are a VIRUS! Abilities left: " + abilityLeft };
        }
        
        

        int selfStep = 999;
        int oppoStep = 999;
        std::vector<Piece> runFrom;
        std::vector<int> enemyStep;

        //cout << "I am gouPiece: " << gouPiece.getName() << endl;

        for (size_t j = 0; j < oppoPieces.size(); ++j) {
            if (oppoPieces[j]->isPieceAlive() && (gouPiece.getStrength() <= oppoPieces[j]->getStrength())) {
                selfStep = step(*oppoPieces[j], gouPiece);
                oppoStep = step(gouPiece, *oppoPieces[j]);

                if (selfStep <= oppoStep) { // we run from them
                    runFrom.emplace_back(*oppoPieces[j]);
                    enemyStep.emplace_back(oppoStep);
                    
                    //cout << "Who do we have here? OppoPiece is " << oppoPieces[j]->getName() << endl;
                    //cout <<"My step to gouPiece is " << oppoStep << " and his step to me is " << selfStep << endl;
                }
            }
        }

        if (runFrom.size() != 0) {
            std::vector<Piece> checkThis = moveMove(gouPiece);

            int weight = weightedSafe(enemyStep);
            int safe = 0;
            int maxSafe = -999;
            Piece Target;
            bool first = 1;


            for (size_t i = 0; i < checkThis.size(); ++i) {
                if (playerName == "player1") {
                    if ((isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1)) || isSelfPiecePosCheck(checkThis[i], player1)) {
                        checkThis.erase(checkThis.begin()+i);
                        continue;
                    } if ((checkThis[i].getType() == "data") && (isOppPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1))) {
                        checkThis.erase(checkThis.begin()+i);
                        continue;
                    }
                } else {
                    if ((isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2)) || isSelfPiecePosCheck(checkThis[i], player2)) {
                        checkThis.erase(checkThis.begin()+i); 
                        continue;
                    } if ((checkThis[i].getType() == "data") && (isOppPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2))) {
                        checkThis.erase(checkThis.begin()+i);
                        continue;
                    }
                }
            }

            for (size_t i = 0; i < checkThis.size(); ++i) {


                bool all = 0;

                for (size_t j = 0; j < runFrom.size(); ++j) {

                    selfStep = step(runFrom[j], checkThis[i]);
                    oppoStep = step(checkThis[i], runFrom[j]);

                    //cout << "Line 1464: gouPiece " << gouPiece.getName() << " is running away from " << runFrom[j].getName() << endl;

                    if (oppoStep != 1) {
                        if (oppoStep < enemyStep[j]) {
                            // we move closer to the same enemy piece, not safe
                            safe -= (weight - oppoStep);
                        } else if (oppoStep > enemyStep[j]) { // we are safer
                            safe += (weight - oppoStep);
                        }
                        all = 1;
                    } else {
                        all = 0;
                        break;
                    }

                    //cout << "Line 1479: safe value is " << safe << endl;
                }

                //cout << "try to go to x: " << checkThis[i].getPosx() << " y: " << checkThis[i].getPosy() << " with all value = " << all << endl;

                if (all) {
                    if (playerName == "player1") {
                        if (!(isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player1)) 
                        && (!isSelfPiecePosCheck(checkThis[i], player1))) {
                            // we dont want any 1-step
                            
                            if (first) {
                                first = 0;
                                Target = checkThis[i];
                                maxSafe = safe;
                            }
                            if (maxSafe < safe) {
                                Target = checkThis[i];
                                maxSafe = safe;
                            }
                        }
                    } else {
                        if (!(isSelfPort(cells[checkThis[i].getPosx()][checkThis[i].getPosy()], player2)) 
                        && (!isSelfPiecePosCheck(checkThis[i], player2))) {
                            // we dont want any 1-step
                            if (first) {
                                first = 0;
                                Target = checkThis[i];
                                maxSafe = safe;
                            }
                            if (maxSafe < safe) {
                                Target = checkThis[i];
                                maxSafe = safe;
                                //cout << "maxSafe = " << maxSafe << "position is: x = " << Target.getPosx() << " y = " << Target.getPosy() << endl;
                            }
                        }
                    }
                }
                safe = 0;
            }

            //cout << "maxSafe value is " << maxSafe << " and we want to move to: x = " << Target.getPosx() << " , y = " << Target.getPosy() << endl;

            if (maxSafe != -999) {
                if (playerName == "player1") {
                    moveTo(gouPiece, Target, player1, this);
                    return;
                } else {
                    moveTo(gouPiece, Target, player2, this);
                    return;
                }
            }
        }
        std::cout << "Defense hint is not available at the moment, try later" << '\n';



    } else if (theRealAbility == '^') {
        string abilityLeft = "";
        for (size_t j = 0; j < thePlayer->getAbilities().size();
             ++j) {
            abilityLeft += thePlayer->getAbilities()[j];
        }
        throw std::out_of_range{
            "You have used this ability. Abilities left: " +
            abilityLeft};
    } else {
        string abilityLeft = "";
        for (size_t j = 0; j < thePlayer->getAbilities().size();
             ++j) {
            abilityLeft += thePlayer->getAbilities()[j];
        }
        throw std::out_of_range{
            "You do not have this ability. Abilities left: " +
            abilityLeft};
    }
    
    thePlayer->setAbility(abilityIndex - 1, "^"); // set the ability from single-char string to be "USED".
    td->notify(this);
    if (initGraph) {
        gd->notify(this);
    }
}

PlayerPtr Board::getP1() { return player1; }

PlayerPtr Board::getP2() { return player2; }

std::vector<std::vector<CellPtr>> *Board::getcells() {
    return &cells;
}
