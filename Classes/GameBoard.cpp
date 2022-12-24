
//
//  GameBoard.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 11/29/16.
//
//

#include "GameBoard.h"
#include "GameDefine.h"
#include "GameObject.h"

GameBoard::GameBoard(int modeGame, int level) {
  if(!arrayObject.empty()) { arrayObject.clear(); }
  if(!arrayGameBoardMatrix.empty()) { arrayGameBoardMatrix.clear(); }
  this->modeGame = modeGame;
  
  initGameBoardMatrix();
  char levelFileName[512] = {0};
  snprintf(levelFileName, sizeof(levelFileName), "level-%d-%d.xml",modeGame, level);
  readLevelFromXMLFile(levelFileName);
  
  Size bufferBorder(28, 38);
  std::string backgroundFileName = winMatrixForGameBoard.y == 2 ? "gameboard_2.png" : "gameboard_3.png";
  Sprite* background = Sprite::create(backgroundFileName);
  background->setAnchorPoint(Vec2::ZERO);
  background->setPosition(Vec2(-bufferBorder.width, -bufferBorder.height));
  this->addChild(background, -1);
  
  contentSize = Vec2(background->getContentSize().width - bufferBorder.width,
                     background->getContentSize().height - bufferBorder.height);
}

Size GameBoard::getContentSize() { return contentSize; }

GameBoard::~GameBoard() {}

void GameBoard::initGameBoardMatrix() {
  for (int i = 0; i < MAX_COLUMN_ROW_GAME_BOARD; i++) {
    for (int j = 0; j < MAX_COLUMN_ROW_GAME_BOARD; j++) {
      arrayGameBoardMatrix.push_back(INVISIBLE);
    }
  }
}

void GameBoard::readLevelFromXMLFile(const char* levelFileName) {
  LevelGame* levelData = new LevelGame();
  levelData->createDataWithFile(levelFileName);
  std::vector<DataGame*> data = levelData->getDataForLevel();
  
  for (int i = 0; i < data.size(); i++) {
    if(i != data.size() - 1) {
      GameObject* object = new GameObject();
      std::string fileName;
      object->setHeadAndLastMatrix(Vec2(data.at(i)->x1, data.at(i)->y1),
                                   Vec2(data.at(i)->x2, data.at(i)->y2));
      if(data.at(i)->typeObject == WIN_OBJECT) {
        if(data.at(i)->x1 == data.at(i)->x2) {
          fileName = "target_vertical_" + std::to_string(modeGame) + "_" +  std::to_string(object->getHeightObject()) + ".png";
        } else if(data.at(i)->y1 == data.at(i)->y2) {
          fileName = "target_horizontal_" + std::to_string(modeGame) + "_" +  std::to_string(object->getLengthObject()) + ".png";
        }
      } else {
        if(data.at(i)->x1 == data.at(i)->x2) {
          fileName = "vertical_" + std::to_string(modeGame) + "_" +  std::to_string(object->getHeightObject()) + ".png";
        } else if(data.at(i)->y1 == data.at(i)->y2) {
          fileName = "horizontal_" + std::to_string(modeGame) + "_" +  std::to_string(object->getLengthObject()) + ".png";
        }
      }
      object->createWithFileName(fileName);
      object->setPositionObject(this->convertMatrixToPosition(Vec2(data.at(i)->x1, data.at(i)->y1)));
      object->setTypeObject(data.at(i)->typeObject);
      this->addChild(object);
      arrayObject.push_back(object);
      this->updateGameBoardMatrixValue(object, VISIBLE);
    } else {
      winMatrixForGameBoard = Vec2(data.at(i)->x1, data.at(i)->y1);
    }
  }
}

void GameBoard::updateGameBoardMatrixValue(GameObject* gameObject, int value) {
  Vec2 headMatrix = gameObject->getHeadMatrix();
  Vec2 lastMatrix = gameObject->getLastMatrix();
  
  if(headMatrix.x == lastMatrix.x) {
    int minY = fmin(headMatrix.y, lastMatrix.y);
    int maxY = fmax(headMatrix.y, lastMatrix.y);
    for (int i = minY ; i < maxY; i++) {
      arrayGameBoardMatrix.at(headMatrix.x + i*MAX_COLUMN_ROW_GAME_BOARD) = value;
    }
  } else {
    int minX = fmin(headMatrix.x, lastMatrix.x);
    int maxX = fmax(headMatrix.x, lastMatrix.x);
    for (int i = minX ; i < maxX; i++) {
      arrayGameBoardMatrix.at(i + headMatrix.y*MAX_COLUMN_ROW_GAME_BOARD) = value;
    }
  }
}

GameObject* GameBoard::getGameObjectAtPosition(const Vec2& pos) {
  float floatDelX = pos.x/ WIDTH_HEIGHT_OBJECT_PIXEL;
  float floatDelY = pos.y/ WIDTH_HEIGHT_OBJECT_PIXEL;
  Vec2 floatVector(floatDelX,floatDelY);
  
  int intDelX = pos.x/ WIDTH_HEIGHT_OBJECT_PIXEL;
  int intDelY = pos.y/ WIDTH_HEIGHT_OBJECT_PIXEL;
  Vec2 intVector(intDelX,intDelY);
  
  for (int i = 0; i < arrayObject.size(); i++) {
    GameObject* object = arrayObject.at(i);
    if(object == NULL) { continue; }
    Vec2 headerMatrix = object->getHeadMatrix();
    Vec2 lastMatrix = object->getLastMatrix();
    
    if(object->getTypeDirection() == HORIZONTAL) {
      if(floatVector.x > headerMatrix.x &&
         floatVector.x < lastMatrix.x &&
         intVector.y == headerMatrix.y) {
        return object;
      }
    }
    if(object->getTypeDirection() == VERTICAL) {
      if(floatVector.y > headerMatrix.y &&
         floatVector.y < lastMatrix.y &&
         intVector.x == headerMatrix.x) {
        return object;
      }
    }
  }
  return NULL;
}

Vec2 GameBoard::convertPositionToMatrix(const Vec2& position) {
  int intX = position.x/WIDTH_HEIGHT_OBJECT_PIXEL;
  int intY = position.y/WIDTH_HEIGHT_OBJECT_PIXEL;
  
  float floatX = position.x/ WIDTH_HEIGHT_OBJECT_PIXEL;
  float floatY = position.y/ WIDTH_HEIGHT_OBJECT_PIXEL;
  
  float distanceX = floatX - intX;
  float distanceY = floatY - intY;
  if(distanceX >= 0.5) { intX += 1; }
  if(distanceY >= 0.5) { intY += 1; }
  
  return Vec2(intX, intY);
}

Vec2 GameBoard::convertMatrixToPosition(const Vec2& matrix) {
  if(matrix.x < 0 ||
     matrix.y < 0 ||
     matrix.x > MAX_COLUMN_ROW_GAME_BOARD-1 ||
     matrix.y > MAX_COLUMN_ROW_GAME_BOARD-1) {
    return Vec2(INVALID_MATRIX_VALUE,INVALID_MATRIX_VALUE);
  } else {
    float pointX = matrix.x * WIDTH_HEIGHT_OBJECT_PIXEL;
    float pointY = matrix.y * WIDTH_HEIGHT_OBJECT_PIXEL;
    return Vec2(pointX,pointY);
  }
}

bool GameBoard::checkGameObjectCanMove(GameObject* gameObject, const Vec2& position) {
  float floatDistanceX = position.x/ WIDTH_HEIGHT_OBJECT_PIXEL - gameObject->getHeadMatrix().x;
  float floatDistanceY = position.y/ WIDTH_HEIGHT_OBJECT_PIXEL - gameObject->getHeadMatrix().y;
  
  if(gameObject->getTypeDirection() == HORIZONTAL) {
    /// nextMove is int
    /// if floatDistanceX = 0.001 -> nextMove = 1
    /// if floatDistanceX = 1.001 -> nextMove = 2
    /// if floatDistanceX = 1.101 -> nextMove = 2
    /// if floatDistanceX = 2.00 -> nextMove = 3
    int nextMoveX = static_cast<int>(abs(floatDistanceX)) + 1;
    
    if(floatDistanceX > 0.0) {
      int toXIndex = fmin(gameObject->getLastMatrix().x + nextMoveX - 1, MAX_COLUMN_ROW_GAME_BOARD - 1);
      int maxXCanMove = this->getMaxXCanMove(gameObject->getLastMatrix().x - 1,
                                             toXIndex,
                                             gameObject->getHeadMatrix().y);
      if (maxXCanMove == INVALID_MATRIX_VALUE) { return false;}
      //      CCLOG("RIGHT toXIndex : %d",toXIndex);
      return maxXCanMove >= toXIndex;
    } else {
      int toXIndex = fmax(gameObject->getHeadMatrix().x - nextMoveX, 0);
      int maxXCanMove = this->getMaxXCanMove(gameObject->getHeadMatrix().x,
                                             toXIndex,
                                             gameObject->getHeadMatrix().y);
      if (maxXCanMove == INVALID_MATRIX_VALUE) { return false;}
      //      CCLOG("LEFT toXIndex : %d",toXIndex);
      return maxXCanMove <= toXIndex;
    }
  } else {
    int nextMoveY = static_cast<int>(abs(floatDistanceY)) + 1;
    
    if(floatDistanceY > 0.0) {
      int toYIndex = fmin(gameObject->getLastMatrix().y + nextMoveY - 1, MAX_COLUMN_ROW_GAME_BOARD - 1);
      int maxYCanMove = this->getMaxYCanMove(gameObject->getLastMatrix().y - 1,
                                             toYIndex,
                                             gameObject->getHeadMatrix().x);
      if (maxYCanMove == INVALID_MATRIX_VALUE) { return false;}
      //      CCLOG("UP toYIndex : %d",toYIndex);
      return maxYCanMove >= toYIndex;
    } else {
      int toYIndex = fmax(gameObject->getHeadMatrix().y - nextMoveY, 0);
      int maxYCanMove = this->getMaxYCanMove(gameObject->getHeadMatrix().y,
                                             toYIndex,
                                             gameObject->getHeadMatrix().x);
      if (maxYCanMove == INVALID_MATRIX_VALUE) { return false;}
      //      CCLOG("DOWN toYIndex : %d",toYIndex);
      return maxYCanMove <= toYIndex;
    }
  }
}

/// Get minX if move to left
/// Get maxY if move to right
/// if nextX = INVALID_MATRIX_VALUE ---> can't move
int GameBoard::getMaxXCanMove(int fromXIndex, int toXIndex, int y) {
  int nextX = INVALID_MATRIX_VALUE;
  if(fromXIndex < toXIndex) {
    for(int i = fromXIndex + 1; i <= toXIndex; i++) {
      int index = i + y * MAX_COLUMN_ROW_GAME_BOARD;
      if(this->arrayGameBoardMatrix.at(index) == VISIBLE) {
        break;
      } else {
        nextX = i;
      }
    }
  } else {
    for(int i = fromXIndex - 1; i >= toXIndex; i--) {
      int index = i + y * MAX_COLUMN_ROW_GAME_BOARD;
      if(this->arrayGameBoardMatrix.at(index) == VISIBLE) {
        break;
      } else {
        nextX = i;
      }
    }
  }
  return nextX;
}

/// Get minY if move to down
/// Get maxY if move to up
/// if nextY = INVALID_MATRIX_VALUE ---> can't move
int GameBoard::getMaxYCanMove(int fromYIndex, int toYIndex, int x) {
  int nextY = INVALID_MATRIX_VALUE;
  if(fromYIndex < toYIndex) {
    for(int i = fromYIndex + 1; i <= toYIndex; i++) {
      int index = x + i * MAX_COLUMN_ROW_GAME_BOARD;
      if(this->arrayGameBoardMatrix.at(index) == VISIBLE) {
        break;
      } else {
        nextY = i;
      }
    }
  } else {
    for(int i = fromYIndex - 1; i >= toYIndex; i--) {
      int index = x + i * MAX_COLUMN_ROW_GAME_BOARD;
      if(this->arrayGameBoardMatrix.at(index) == VISIBLE) {
        break;
      } else {
        nextY = i;
      }
    }
  }
  return nextY;
}

bool GameBoard::checkReachToWinPoint(GameObject* touchObject) {
  if(touchObject->getTypeObject() != WIN_OBJECT || touchObject->getTypeDirection() != HORIZONTAL) { return false; }
  return touchObject->getLastMatrix().x == winMatrixForGameBoard.x;
}
