#include <vector>
#include <string>
#include "window.h"
#include "graphicsdisplay.h"
#include "board.h"
#include "cell.h"
#include <memory>

typedef std::shared_ptr<Cell> CellPtr;

//ctor, setup the window with squares to represent grid
GraphicsDisplay::GraphicsDisplay():
  window{WindowPtr(new Xwindow())}, gridSize{8}, player1First{true}{
    int n = 8;
    int blockLen = 500 / 8;
    for (int i = 0; i < n; ++i){
      for (int j = 0; j < n; ++j){
        this->window->fillRectangle(i * (500 / 8), j * (500 / 8), 500 / 8, 500 / 8, 4);
      }
    }
    this->window->fillRectangle(3 * blockLen, 0, blockLen *2 , blockLen, 1);
    this->window->fillRectangle(3 * blockLen, 7 * blockLen, blockLen *2 , blockLen, 1);
  }

//Notify function, read the changed colour, and update the drawn square in window accordingly.
// player1 = black, player2 = white, data = green, virus = red
void GraphicsDisplay::notify(Board* b){
    int blockLen = 500 / 8;
    for (size_t i = 0; i < (*(b->getcells())).size(); ++i) {
      for (size_t j = 0; j < (*(b->getcells()))[i].size(); ++j) {
        CellPtr theCell = (*(b->getcells()))[i][j];

        //draw piece colour
        if (theCell->ifOccP1() || theCell->ifOccP2()){
            if (theCell->whoOcc()->isPieceRevealed() || (this->player1First && theCell->ifOccP1()) || ((!this->player1First) && theCell->ifOccP2())){
                if (theCell->whoOcc()->getInfo()[0] == 'V'){
                    this->window->fillRectangle((theCell->gety() * blockLen), (theCell->getx() * blockLen), blockLen, blockLen, 2); 
                }else if (theCell->whoOcc()->getInfo()[0] == 'D'){
                    this->window->fillRectangle((theCell->gety() * blockLen), (theCell->getx() * blockLen), blockLen, blockLen, 3);
                }
                this->window->drawString((theCell->gety() * blockLen) + (blockLen / 2), (theCell->getx() * blockLen) + (blockLen / 2), theCell->whoOcc()->getName() + ": " + theCell->whoOcc()->getInfo());
            }else{
                this->window->fillRectangle((theCell->gety() * blockLen), (theCell->getx() * blockLen), blockLen, blockLen, 0);
                this->window->drawString((theCell->gety() * blockLen) + (blockLen / 2), (theCell->getx() * blockLen) + (blockLen / 2), theCell->whoOcc()->getName());
            }
        }else if (theCell->hasFirewallOne() || theCell->hasFirewallTwo()){
          this->window->fillRectangle((theCell->gety() * blockLen ), (theCell->getx() * blockLen), blockLen, blockLen, 5);
        }
        else{
          this->window->fillRectangle((theCell->gety() * blockLen ), (theCell->getx() * blockLen), blockLen, blockLen, 4);
        }
      }

      for (size_t i = 0; i < (*(b->getcells())).size(); ++i) {
      for (size_t j = 0; j < (*(b->getcells()))[i].size(); ++j) {
        CellPtr theCell = (*(b->getcells()))[i][j];

        //draw piece string
        if (theCell->ifOccP1() || theCell->ifOccP2()){
            if (theCell->whoOcc()->isPieceRevealed()){
                this->window->drawString((theCell->gety() * blockLen) + (blockLen / 2), (theCell->getx() * blockLen) + (blockLen / 2), theCell->whoOcc()->getName() + ": " + theCell->whoOcc()->getInfo());
            }else if (this->player1First && theCell->ifOccP1()){
                this->window->drawString((theCell->gety() * blockLen) + (blockLen / 2), (theCell->getx() * blockLen) + (blockLen / 2), theCell->whoOcc()->getName() + ": " + theCell->whoOcc()->getInfo());
            }else if ((!this->player1First) && theCell->ifOccP2()){
                this->window->drawString((theCell->gety() * blockLen) + (blockLen / 2), (theCell->getx() * blockLen) + (blockLen / 2), theCell->whoOcc()->getName() + ": " + theCell->whoOcc()->getInfo());
            }else{
                this->window->drawString((theCell->gety() * blockLen) + (blockLen / 2), (theCell->getx() * blockLen) + (blockLen / 2), theCell->whoOcc()->getName());
            }
        }

        //draw firewall
        if ((theCell->hasFirewallOne()) || (theCell->hasFirewallTwo())){
          std::string firewallSym;
          if (theCell->hasFirewallOne() && theCell->hasFirewallTwo()){
            if (theCell->ifOccP1() || theCell->ifOccP2()){
              if (theCell->whoOcc()->isPieceRevealed()){
                firewallSym = theCell->whoOcc()->getName() + ": " + theCell->whoOcc()->getInfo();
              }else{
                firewallSym = theCell->whoOcc()->getName();
              }
            }else{
              firewallSym = "Z";
            }
            
          }else if (theCell->hasFirewallOne()){
            if (theCell->ifOccP1() || theCell->ifOccP2()){
              if (theCell->whoOcc()->isPieceRevealed()){
                firewallSym = theCell->whoOcc()->getName() + ": " + theCell->whoOcc()->getInfo();
              }else{
                firewallSym = theCell->whoOcc()->getName();
              }
            }else{
              firewallSym = "M";
            }
          }else if (theCell->hasFirewallTwo()){
            if (theCell->ifOccP1() || theCell->ifOccP2()){
              if (theCell->whoOcc()->isPieceRevealed()){
                firewallSym = theCell->whoOcc()->getName() + ": " + theCell->whoOcc()->getInfo();
              }else{
                firewallSym = theCell->whoOcc()->getName();
              }
            }else{
              firewallSym = "W";
            }
          }
          int firewallX = (theCell->gety()) * blockLen + (blockLen/2);
          int firewallY = (theCell->getx()) * blockLen + (blockLen/2);
          this->window->drawString(firewallX, firewallY,firewallSym);
        }
      }
    }
    this->window->fillRectangle(3 * blockLen, 0, blockLen *2 , blockLen, 1);
    this->window->fillRectangle(3 * blockLen, 7 * blockLen, blockLen *2 , blockLen, 1);
  }
}

//dtor
GraphicsDisplay::~GraphicsDisplay(){}

void GraphicsDisplay::switchPlayer(){
  this->player1First = !player1First; 
}
