#ifndef _BOARD_H_
#define _BOARD_H_
#include <vector>
#include <string>
#include <memory>
#include <cstddef>
#include "cell.h"
#include "piece.h"
#include "player.h"

class TextDisplay;
class GraphicsDisplay;

typedef std::shared_ptr<Player> PlayerPtr;
typedef std::shared_ptr<TextDisplay> TextDisplayPtr;
typedef std::shared_ptr<GraphicsDisplay> GraphicsDisplayPtr;
typedef std::shared_ptr<Cell> CellPtr;
typedef std::shared_ptr<Piece> PiecePtr;

class Board { //remeber to remove the player
    protected:
    std::vector<std::vector<CellPtr>> cells;
    TextDisplayPtr td;
    GraphicsDisplayPtr gd;
    PlayerPtr player1;
    PlayerPtr player2;
    std::vector<PiecePtr> player1Piece;// init all pieces at the begining
    std::vector<PiecePtr> player2Piece;
    bool initGraph;
    public:
    void init(std::string ids1, std::string ids2, std::string abs1, std::string abs2, bool initGraph);
    void placePiece(int x, int y, std::string ID);
    void battle(PiecePtr self, PiecePtr opp);
    void movePiece(PlayerPtr who, std::string ID, std::string dir); // dir can be only "N" "S" "E" "W"
    bool hasWon(); // handle this in main function
    GraphicsDisplayPtr getGD();
    PlayerPtr getP1();
    PlayerPtr getP2();
    std::vector<std::vector<CellPtr>>* getcells();
    //use one of the abilities
    // playername must be one of "player1" or "player2"
    // abilityIndex must in the range of 1-5, indicating which ability to be used.
    // param1 is the ID of the piece the ability is added on. In case the ability is a firewall, then param1 is the x coord of the cell.
    // param2 only used if ability is firewall, then param2 is the y coord of the cell.
    void useAbility(std::string playerName, int abilityIndex, std::string param1, std::string param2 = "");
    friend std::ostream& operator<<(std::ostream& out, const Board& b); //why??(private)   
    void Playerflip();
    ~Board();
};

#endif
