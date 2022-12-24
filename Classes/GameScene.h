//
//  GameScene.h
//  SlideBlock
//
//  Created by Harry Nguyen on 11/28/16.
//
//

#ifndef GameScene_h
#define GameScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "GameBoard.h"
#include "GameObject.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class GameScene : public Layer {
private:
  int level, status, modeGame, countMove;
  int timeRunGameInSeconds;
  Vec2 previousHeadMatrix, touchBeginPosition;
  Size winSize;
  Label* levelGameLabel, *countMoveLabel, *timeRunningLabel;
  ui::Button* backButton, *previousLevelButton, *nextLevelButton;
  GameBoard* gameBoard;
  GameObject* touchObject;
  void updateCounterTime(int timeInSeconds);
public:
  GameScene();
  virtual ~GameScene();
  void updateTimeRemaining(float dt);
  static Scene* createGameScene(int level, int style);
  void  initUpdateUI();
  CREATE_FUNC(GameScene);
  bool handleTouchBegan(Touch* mTouch, Event* pEvent);
  void handleTouchMove(Touch* mTouch, Event* pEvent);
  void handleTouchEnd(Touch* mTouch, Event* pEvent);
  void updateGameBoardWhenTouchEnd(GameObject* gameObject, Vec2 newPosition);
  void createHeaderUI();
  void createFooterUI();
  void buttonClickGameScene(Ref* pSender);
  void createPopUpPause();
  void displayGameWinScene();
};

#endif /* GameScene_h */
