//
//  DataGame.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 12/8/16.
//
//

#include "DataGame.h"

DataGame::DataGame(int x1, int y1,int x2, int y2, int typeObject) {
  this->x1 = x1;
  this->y1 = y1;
  this->x2 = x2;
  this->y2 = y2;
  this->typeObject = typeObject;
}

DataGame::~DataGame() {}
