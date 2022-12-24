
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
#include <iostream>

GameBoard::GameBoard(int modeGame) {
  if(!arrayObject.empty()) {
    arrayObject.clear();
  }
  if(!arrayGameBoardMatrix.empty()) {
    arrayGameBoardMatrix.clear();
  }
  this->modeGame = modeGame;
}

GameBoard::~GameBoard() {}

void GameBoard::initGameBoardMatrix() {
  for (int i = 0; i < MAX_COLUMN_ROW_GAMEBOARD; i++) {
    for (int j = 0; j < MAX_COLUMN_ROW_GAMEBOARD; j++) {
      arrayGameBoardMatrix.push_back(INVISIBLE);
    }
  }
}

void GameBoard::readLevelFromXMLFile(const char* fileName) {
  LevelGame* levelData = new LevelGame();
  levelData->createDataWithFile(fileName);
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
        }
        if(data.at(i)->y1 == data.at(i)->y2) {
          fileName = "target_horizontal_" + std::to_string(modeGame) + "_" +  std::to_string(object->getLengthObject()) + ".png";
        }
      } else {
        if(data.at(i)->x1 == data.at(i)->x2) {
          fileName = "vertical_" + std::to_string(modeGame) + "_" +  std::to_string(object->getHeightObject()) + ".png";
        }
        if(data.at(i)->y1 == data.at(i)->y2) {
          fileName = "horizontal_" + std::to_string(modeGame) + "_" +  std::to_string(object->getLengthObject()) + ".png";
        }
      }
      object->createWithFileName(fileName);
      object->setPositionObject(this->convertMatrixToPosition(Vec2(data.at(i)->x1, data.at(i)->y1)));
      object->setTypeObject(data.at(i)->typeObject);
      this->addChild(object,10);
      arrayObject.push_back(object);
      this->updateGameBoardMatrixValue(object, VISIBLE);
    } else {
      this->setGameBoardWinMatrixValue(Vec2(data.at(i)->x1, data.at(i)->y1));
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
      arrayGameBoardMatrix.at(headMatrix.x + i*MAX_COLUMN_ROW_GAMEBOARD) = value;
    }
  } else {
    int minX = fmin(headMatrix.x, lastMatrix.x);
    int maxX = fmax(headMatrix.x, lastMatrix.x);
    for (int i = minX ; i < maxX; i++) {
      arrayGameBoardMatrix.at(i + headMatrix.y*MAX_COLUMN_ROW_GAMEBOARD) = value;
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
  
  if(distanceX >= 0.5) {
    intX += 1;
  }
  
  if(distanceY >= 0.5) {
    intY += 1;
  }
  
  return Vec2(intX, intY);
}

Vec2 GameBoard::convertMatrixToPosition(const Vec2& matrix) {
  if(matrix.x < 0 ||
     matrix.y < 0 ||
     matrix.x > MAX_COLUMN_ROW_GAMEBOARD-1 ||
     matrix.y > MAX_COLUMN_ROW_GAMEBOARD-1) {
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
      int toXIndex = fmin(gameObject->getLastMatrix().x + nextMoveX - 1, MAX_COLUMN_ROW_GAMEBOARD - 1);
      Vec2 result = this->getMaxMatrixCanMoveX(gameObject->getLastMatrix().x - 1,
                                               toXIndex,
                                               gameObject->getHeadMatrix().y);
      if (result.x == INVALID_MATRIX_VALUE) { return false;}
      //      CCLOG("RIGHT toXIndex : %d",toXIndex);
      //      CCLOG("RIGHT result.x: %f",result.x);
      return result.x >= toXIndex;
    } else {
      int toXIndex = fmax(gameObject->getHeadMatrix().x - nextMoveX, 0);
      Vec2 result = this->getMaxMatrixCanMoveX(gameObject->getHeadMatrix().x,
                                               toXIndex,
                                               gameObject->getHeadMatrix().y);
      if (result.x == INVALID_MATRIX_VALUE) { return false;}
      //      CCLOG("LEFT toXIndex : %d",toXIndex);
      //      CCLOG("LEFT result.x: %f",result.x);
      return result.x <= toXIndex;
    }
  } else {
    int nextMoveY = static_cast<int>(abs(floatDistanceY)) + 1;
    
    if(floatDistanceY > 0.0) {
      int toYIndex = fmin(gameObject->getLastMatrix().y + nextMoveY - 1, MAX_COLUMN_ROW_GAMEBOARD - 1);
      Vec2 result = this->getMaxMatrixCanMoveY(gameObject->getLastMatrix().y - 1,
                                               toYIndex,
                                               gameObject->getHeadMatrix().x);
      if (result.y == INVALID_MATRIX_VALUE) { return false;}
      //      CCLOG("UP toYIndex : %d",toYIndex);
      //      CCLOG("UP result.y: %f",result.y);
      return result.y >= toYIndex;
    } else {
      int toYIndex = fmax(gameObject->getHeadMatrix().y - nextMoveY, 0);
      Vec2 result = this->getMaxMatrixCanMoveY(gameObject->getHeadMatrix().y,
                                               toYIndex,
                                               gameObject->getHeadMatrix().x);
      if (result.y == INVALID_MATRIX_VALUE) { return false;}
      //      CCLOG("DOWN toYIndex : %d",toYIndex);
      //      CCLOG("DOWN result.y: %f",result.y);
      return result.y <= toYIndex;
    }
  }
}

/// Get minX if move to left
/// Get maxY if move to right
/// if nextX = INVALID_MATRIX_VALUE ---> can't move
Vec2 GameBoard::getMaxMatrixCanMoveX(int fromXIndex, int toXIndex, int y) {
  //  CCLOG("fromXIndex : %d",fromXIndex);
  //  CCLOG("toXIndex : %d",toXIndex);
  int nextX = INVALID_MATRIX_VALUE;
  if(fromXIndex < toXIndex) {
    for(int i = fromXIndex + 1; i <= toXIndex; i++) {
      int index = i + y * MAX_COLUMN_ROW_GAMEBOARD;
      if(this->arrayGameBoardMatrix.at(index) == VISIBLE) {
        break;
      } else {
        nextX = i;
      }
    }
  } else {
    for(int i = fromXIndex - 1; i >= toXIndex; i--) {
      int index = i + y * MAX_COLUMN_ROW_GAMEBOARD;
      if(this->arrayGameBoardMatrix.at(index) == VISIBLE) {
        break;
      } else {
        nextX = i;
      }
    }
  }
  return Vec2(nextX, y);
}

/// Get minY if move to down
/// Get maxY if move to up
/// if nextY = INVALID_MATRIX_VALUE ---> can't move
Vec2 GameBoard::getMaxMatrixCanMoveY(int fromYIndex, int toYIndex, int x) {
  //  CCLOG("fromYIndex : %d",fromYIndex);
  //  CCLOG("toYIndex : %d",toYIndex);
  int nextY = INVALID_MATRIX_VALUE;
  if(fromYIndex < toYIndex) {
    for(int i = fromYIndex + 1; i <= toYIndex; i++) {
      int index = x + i * MAX_COLUMN_ROW_GAMEBOARD;
      if(this->arrayGameBoardMatrix.at(index) == VISIBLE) {
        break;
      } else {
        nextY = i;
      }
    }
  } else {
    for(int i = fromYIndex - 1; i >= toYIndex; i--) {
      int index = x + i * MAX_COLUMN_ROW_GAMEBOARD;
      if(this->arrayGameBoardMatrix.at(index) == VISIBLE) {
        break;
      } else {
        nextY = i;
      }
    }
  }
  return Vec2(x, nextY);
}

void GameBoard::setGameBoardWinMatrixValue(Vec2 point) {
  this->gameBoardWinMatrixValue = point;
}

Vec2 GameBoard::getGameBoardWinMatrixValue() {
  return gameBoardWinMatrixValue;
}

bool GameBoard::checkWinGame() {
  for (int i = 0 ; i < arrayObject.size(); i++) {
    GameObject* touchObject = arrayObject.at(i);
    if(touchObject != NULL && touchObject->getTypeObject() == WIN_OBJECT) {
      Vec2 winGameBoardMatrix = getGameBoardWinMatrixValue();
      Vec2 lastMatrix = touchObject->getLastMatrix();
      
      if(touchObject->getTypeDirection() == HORIZONTAL) {
        if(lastMatrix.x == winGameBoardMatrix.x) {
          touchObject->runActionGameWin(Vec2(touchObject->getPosition().x + WIDTH_HEIGHT_OBJECT_PIXEL*4, touchObject->getPosition().y));
          if(delegate) {
            delegate->sendNotificationForGameScene();
          }
          return true;
        }
      } else {
        if(lastMatrix.y == winGameBoardMatrix.y) {
          touchObject->runActionGameWin(Vec2(touchObject->getPosition().x, touchObject->getPosition().y + WIDTH_HEIGHT_OBJECT_PIXEL*4));
          if(delegate) {
            delegate->sendNotificationForGameScene();
          }
          return true;
        }
      }
    }
  }
  return false;
}

void GameBoard::setDelegateForGame(DelegateNotification* delegate) {
  this->delegate = delegate;
}

void GameBoard::printGameBoard() {
  std::cout<<"\n"<<"\n";
  for (int i = MAX_COLUMN_ROW_GAMEBOARD-1 ; i >= 0; i--) {
    for (int j = 0; j < MAX_COLUMN_ROW_GAMEBOARD; j++) {
      std::cout<< arrayGameBoardMatrix.at(j + i* MAX_COLUMN_ROW_GAMEBOARD) << " ";
    }
    std::cout<<"\n"<<"\n";
  }
  std::cout<<"\n"<<"\n";
}
