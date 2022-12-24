//
//  GameBoard.h
//  SlideBlock
//
//  Created by Harry Nguyen on 11/29/16.
//
//

#ifndef GameBoard_h
#define GameBoard_h

#include <stdio.h>
#include "cocos2d.h"
#include "GameObject.h"
#include "LevelGame.h"
USING_NS_CC;


class DelegateNotification {
public:
  virtual void sendNotificationForGameScene() = 0;
};

class GameBoard: public Layer {
public:
  std::vector<int> arrayGameBoardMatrix;
private:
  Vec2 originGameBoardPosition;
  std::vector<GameObject*> arrayObject;
  Vec2 gameBoardWinMatrixValue;
  int modeGame;
  
  DelegateNotification* delegate;
public:
  
  GameBoard(int modeGame);
  virtual~GameBoard();
  void initGameBoardMatrix();
  void readLevelFromXMLFile(const char* fileName);
  GameObject* getGameObjectAtPosition(const Vec2& pos);
  
  Vec2 convertPositionToMatrix(const Vec2& position);
  Vec2 convertMatrixToPosition(const Vec2& matrix);
  
  void updateGameBoardMatrixValue(GameObject* gameObject, int value);
  
  void setGameBoardWinMatrixValue(Vec2 point);
  Vec2 getGameBoardWinMatrixValue();
  
  void setDelegateForGame(DelegateNotification* delegate);
  
  void printGameBoard();
  
  bool checkGameObjectCanMove(GameObject* gameObject, const Vec2& position);
  bool checkWinGame();
private:
  Vec2 getMaxMatrixCanMoveX(int fromXIndex, int toXIndex, int y);
  Vec2 getMaxMatrixCanMoveY(int fromYIndex, int toYIndex, int x);
};
#endif /* GameBoard_h */
