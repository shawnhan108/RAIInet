#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H
#include <iostream>
#include <memory>
#include <vector>
#include "window.h"
class Board;

typedef std::shared_ptr<Board> BoardPtr;
typedef std::shared_ptr<Xwindow> WindowPtr;

class GraphicsDisplay {
  WindowPtr window; //Owns a Xwindow.
  const int gridSize;
  bool player1First;
 public:
  GraphicsDisplay(); //ctor
  ~GraphicsDisplay(); //dtor
  void notify(Board* b); //notify method
  void switchPlayer(); //switch view info
};
#endif
