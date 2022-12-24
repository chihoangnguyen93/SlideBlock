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

class GameScene : public Layer, DelegateNotification {
private:
  Size winSize;
  
  int level;
  int status;
  int styleGame;
  int countMove;
  int timeMinutes;
  int hightLevel;
  float timeSecondRunGame;
  
  Label* lbLevelGame;
  Label* lbMoveGame;
  Label* lbTimeGame;
  ui::Button* btnBackGame;
  ui::Button* btnBackSlot;
  ui::Button* btnNextSlot;
  
  GameBoard* gameBoard;
  GameObject* touchObject;
  Vec2 previousHeadMatrix;
  Vec2 touchBeginPosition;
  
public:
  GameScene();
  virtual ~GameScene();
  void update(float dt);
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
  void showFullScreenAdvertisement(const char* key, int frequency);
  void onEnter();
  void onExit();
  virtual void sendNotificationForGameScene();
};

#endif /* GameScene_h */
