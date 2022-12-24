//
//  LevelGame.h
//  SlideBlock
//
//  Created by Harry Nguyen on 12/8/16.
//
//

#ifndef LevelGame_h
#define LevelGame_h

#include <stdio.h>
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
#include "DataGame.h"
USING_NS_CC;
using namespace tinyxml2;

class LevelGame {
private:
  std::vector<DataGame*> arrayData;
public:
  LevelGame();
  virtual ~LevelGame();
  void createDataWithFile(const char* fileName);
  std::vector<DataGame*> getDataForLevel();
};

#endif /* LevelGame_h */
