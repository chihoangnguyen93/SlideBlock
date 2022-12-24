//
//  GameBoard.h
//  SlideBlock
//
//  Created by Harry Nguyen on 11/29/16.
//
//

#ifndef GameBoard_h
#define GameBoard_h

#include "cocos2d.h"
#include "GameObject.h"
#include "LevelGame.h"
USING_NS_CC;

class GameBoard: public Layer {
private:
  int modeGame;
  Size contentSize;
  std::vector<GameObject*> arrayObject;
  int getMaxXCanMove(int fromXIndex, int toXIndex, int y);
  int getMaxYCanMove(int fromYIndex, int toYIndex, int x);
  void initGameBoardMatrix();
  void readLevelFromXMLFile(const char* levelFileName);
public:
  Vec2 winMatrixForGameBoard;
  std::vector<int> arrayGameBoardMatrix;
  GameBoard(int modeGame, int level);
  virtual~GameBoard();
  GameObject* getGameObjectAtPosition(const Vec2& pos);
  
  Vec2 convertPositionToMatrix(const Vec2& position);
  Vec2 convertMatrixToPosition(const Vec2& matrix);
  void updateGameBoardMatrixValue(GameObject* gameObject, int value);
  bool checkGameObjectCanMove(GameObject* gameObject, const Vec2& position);
  bool checkReachToWinPoint(GameObject* touchObject);
  Size getContentSize();
};
#endif /* GameBoard_h */
