//
//  GameScene.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 11/28/16.
//
//

#include "GameScene.h"
#include "GameDefine.h"
#include "LevelGame.h"
#include "HomeScene.h"
#include "AdmobManager.h"

static char bufferTimeGame[1024] = {0};

GameScene::GameScene() {}

GameScene::~GameScene() {}

Scene* GameScene::createGameScene(int level, int style) {
  auto scene = Scene::create();
  auto layer = new GameScene();
  layer->level = level;
  layer->styleGame = style;
  layer->initUpdateUI();
  scene->addChild(layer);
  return scene;
}

void GameScene::update(float dt) {
  if(status == RUNNING) {
    timeSecondRunGame += dt;
    if(timeSecondRunGame > 59) {
      timeSecondRunGame = 0.0f;
      timeMinutes ++;
    }
    std::string stringValue;
    if(timeMinutes < 10) {
      if(timeSecondRunGame > 9.5f) {
        sprintf(bufferTimeGame, "0%d:%.0f",timeMinutes,timeSecondRunGame);
      } else {
        sprintf(bufferTimeGame, "0%d:0%.0f",timeMinutes,timeSecondRunGame);
      }
    } else {
      if(timeSecondRunGame > 9.5f) {
        sprintf(bufferTimeGame, "%d:%.0f",timeMinutes, timeSecondRunGame);
      } else {
        sprintf(bufferTimeGame, "%d:0%.0f",timeMinutes,timeSecondRunGame);
      }
    }
    lbTimeGame->setString(bufferTimeGame);
  }
}

void GameScene::initUpdateUI() {
  winSize = Director::getInstance()->getWinSize();
  Sprite* spBg = Sprite::create("bgHomeScene.png");
  spBg->setPosition(Vec2(winSize.width/2, winSize.height/2));
  this->addChild(spBg,1);
  hightLevel = 0;
  if(styleGame == EASY_LEVEL) {
    hightLevel = UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_EASY,1);
  } else if (styleGame == MEDIUM_LEVEL) {
    hightLevel = UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_MEDIUM,1);
  } else if (styleGame == HARD_LEVEL) {
    hightLevel = UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_HARD, 1);
  } else {
    hightLevel = UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_EXPERT, 1);
  }
  
  status = PAUSE;
  countMove = 0;
  timeMinutes = 0;
  timeSecondRunGame = 0.0f;
  
  gameBoard = new GameBoard(styleGame);
  gameBoard->setDelegateForGame(this);
  gameBoard->initGameBoardMatrix();
  gameBoard->setPosition(Vec2(80, 200));
  char fileName[512] = {0};
  sprintf(fileName, "level-%d-%d.xml",styleGame, level);
  gameBoard->readLevelFromXMLFile(fileName);
  this->addChild(gameBoard,100);
  //  gameBoard->printGameBoard();
  
  createHeaderUI();
  createFooterUI();
  
  /// Create game board background
  if(gameBoard->getGameBoardWinMatrixValue().y == 2 ) {
    Sprite* bgThumbGameBoard = Sprite::create("bg_ingame2.png");
    bgThumbGameBoard->setPosition(Vec2(winSize.width/2 + 28, winSize.height/2 - 44));
    this->addChild(bgThumbGameBoard,5);
  }
  if(gameBoard->getGameBoardWinMatrixValue().y == 3) {
    Sprite* bgThumbGameBoard = Sprite::create("bg_ingame3.png");
    bgThumbGameBoard->setPosition(Vec2(winSize.width/2 + 28, winSize.height/2 - 44));
    this->addChild(bgThumbGameBoard,5);
  }
  
  /*Add Event*/
  auto lister = EventListenerTouchOneByOne::create();
  lister->setSwallowTouches(true);
  lister->onTouchBegan = CC_CALLBACK_2(GameScene::handleTouchBegan, this);
  lister->onTouchMoved = CC_CALLBACK_2(GameScene::handleTouchMove, this);
  lister->onTouchEnded = CC_CALLBACK_2(GameScene::handleTouchEnd, this);
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lister, this);
  this->scheduleUpdate();
  
  // Show admod full screen
  showFullScreenAdvertisement(OPEN_GAME_SCREEN_KEY, OPEN_GAME_SCREEN_FREQUENCY);
  
  AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
}

void GameScene::createHeaderUI() {
  Sprite* spBgHeader = Sprite::create("box_time.png");
  spBgHeader->setPosition(Vec2(winSize.width/2, winSize.height - spBgHeader->getContentSize().height/2));
  this->addChild(spBgHeader,2);
  
  /*btnPause*/
  ui::Button* btnPause = ui::Button::create("pause_normal.png","pause_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnPause->setPosition(Vec2(spBgHeader->getPositionX() + spBgHeader->getContentSize().width/2 - btnPause->getContentSize().width*0.8, spBgHeader->getPositionY() + btnPause->getContentSize().height*0.4));
  btnPause->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  btnPause->setTag(TAG_BTN_PAUSE_GAMESCENE);
  this->addChild(btnPause,3);
  
  ui::Button* btnReplay = ui::Button::create("replay_normal.png","replay_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnReplay->setPosition(Vec2(btnPause->getPositionX() - btnReplay->getContentSize().width*1.1, btnPause->getPositionY()));
  btnReplay->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  btnReplay->setTag(TAG_BTN_REPLAY_GAMESCENE);
  this->addChild(btnReplay,3);
  
  /*Move Best*/
  auto labelMoveBest = Label::createWithTTF("MoveBest : --", "HKABEL.TTF", 30);
  labelMoveBest->setPosition(Vec2(btnReplay->getPositionX() - btnReplay->getContentSize().width/2, btnReplay->getPositionY() - btnReplay->getContentSize().height*0.66));
  labelMoveBest->setAnchorPoint(Vec2(0, 0.5f));
  this->addChild(labelMoveBest,3);
  
  char bufferMoveBestDisplay[512] = {0};
  sprintf(bufferMoveBestDisplay, "MoveBest-%d-%d", styleGame, level);
  
  int countMoveBestPrevious = UserDefault::getInstance()->getIntegerForKey(bufferMoveBestDisplay,0);
  
  /* labelMoveBest */
  if(countMoveBestPrevious > 0) {
    std::string bufferCountMoveBest = "MoveBest : " + std::to_string(countMoveBestPrevious);
    labelMoveBest->setString(bufferCountMoveBest);
    labelMoveBest->setVisible(true);
  } else {
    labelMoveBest->setVisible(false);
  }
  
  /* Sprite level*/
  Sprite* spLevelTheme = Sprite::create("box_level.png");
  spLevelTheme->setPosition(Vec2(btnReplay->getPositionX() - btnReplay->getContentSize().width*1.9,
                                 btnReplay->getPositionY() + spLevelTheme->getContentSize().height/4));
  this->addChild(spLevelTheme,3);
  
  /*Sprite Move*/
  Sprite* spMoveTheme = Sprite::create("box_moves.png");
  spMoveTheme->setPosition(Vec2(spLevelTheme->getPositionX(), spLevelTheme->getPositionY() - spLevelTheme->getContentSize().height*1.1));
  this->addChild(spMoveTheme,3);
  
  
  /* Label lbLevelGame */
  lbLevelGame = Label::createWithTTF(std::to_string(level), "HKABEL.TTF", 40);
  lbLevelGame->setPosition(Vec2(spLevelTheme->getPositionX() + spLevelTheme->getContentSize().width*0.2,
                                spLevelTheme->getPositionY() + + lbLevelGame->getContentSize().height/10));
  this->addChild(lbLevelGame,4);
  
  
  /* Label lbCountMoveGame; */
  lbMoveGame = Label::createWithTTF(std::to_string(countMove), "HKABEL.TTF", 40);
  lbMoveGame->setPosition(Vec2(spMoveTheme->getPositionX() + spMoveTheme->getContentSize().width*0.2,
                               spMoveTheme->getPositionY() + lbMoveGame->getContentSize().height/10));
  this->addChild(lbMoveGame,4);
  
  lbTimeGame = Label::createWithTTF("00:00", "HKABEL.TTF", 36);
  lbTimeGame->setPosition(Vec2(spBgHeader->getPositionX(),
                               spBgHeader->getPositionY() - spBgHeader->getContentSize().height*0.36));
  this->addChild(lbTimeGame,4);
}

void GameScene::createFooterUI() {
  /*Button Back game*/
  btnBackGame = ui::Button::create("backscene_normal.png",
                                   "backscene_pressed.png","",
                                   ui::Widget::TextureResType::LOCAL);
  btnBackGame->setPosition(Vec2(btnBackGame->getContentSize().height/2,
                                btnBackGame->getContentSize().height/2));
  btnBackGame->setTag(TAG_BTN_BACK_GAMESCENE);
  btnBackGame->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  this->addChild(btnBackGame,100);
  
  btnBackSlot = ui::Button::create();
  btnNextSlot = ui::Button::create();
  if(level == 1) {
    btnBackSlot->loadTextures("back_end.png","","",ui::Widget::TextureResType::LOCAL);
    btnBackSlot->setEnabled(false);
    if(level == hightLevel) {
      btnNextSlot->loadTextures("next_end.png","","",ui::Widget::TextureResType::LOCAL);
      btnNextSlot->setEnabled(false);
    } else {
      btnNextSlot->loadTextures("next_normal.png","next_pressed.png","",ui::Widget::TextureResType::LOCAL);
      btnNextSlot->setEnabled(true);
    }
  } else {
    btnBackSlot->loadTextures("back_normal.png","back_pressed.png","",ui::Widget::TextureResType::LOCAL);
    btnNextSlot->setEnabled(true);
    
    if(level == hightLevel) {
      btnNextSlot->loadTextures("next_end.png","","",ui::Widget::TextureResType::LOCAL);
      btnNextSlot->setEnabled(false);
    } else {
      btnNextSlot->loadTextures("next_normal.png","next_pressed.png","",ui::Widget::TextureResType::LOCAL);
      btnNextSlot->setEnabled(true);
    }
  }
  
  /*Button Back Slot*/
  btnBackSlot->setPosition(Vec2(winSize.width/2 - btnBackSlot->getContentSize().width*0.6, btnBackGame->getPositionY()));
  btnBackSlot->setTag(TAG_BTN_BACKSLOT_GAMESCENE);
  btnBackSlot->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  this->addChild(btnBackSlot,100);
  
  /*Button Next Slot*/
  btnNextSlot->setPosition(Vec2(winSize.width/2 + btnNextSlot->getContentSize().width*0.6,btnBackGame->getPositionY()));
  btnNextSlot->setTag(TAG_BTN_NEXTSLOT_GAMESCENE);
  btnNextSlot->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  this->addChild(btnNextSlot,100);
}

void GameScene::buttonClickGameScene(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  if(tag == TAG_BTN_BACK_GAMESCENE) {
    Director::getInstance()->replaceScene(HomeScene::createHomeScene());
  }
  if(tag == TAG_BTN_REPLAY_GAMESCENE) {
    Director::getInstance()->replaceScene(GameScene::createGameScene(level, styleGame));
  }
  if(tag == TAG_BTN_PAUSE_GAMESCENE) {
    this->createPopUpPause();
  }
  if(tag == TAG_POPUP_MENU) {
    Director::getInstance()->replaceScene(HomeScene::createHomeScene());
  }
  if(tag == TAG_POPUP_REPLAY) {
    Director::getInstance()->replaceScene(GameScene::createGameScene(level, styleGame));
  }
  if(tag == TAG_POPUP_RESUME) {
    status = RUNNING;
    if(this->getChildByTag(1256)) {
      this->removeChildByTag(1256);
    }
  }
  
  if(tag == TAG_BTN_BACKSLOT_GAMESCENE) {
    if(level != 1) {
      Director::getInstance()->replaceScene(GameScene::createGameScene(level-1, styleGame));
    }
  }
  
  if(tag == TAG_BTN_NEXTSLOT_GAMESCENE) {
    if(level <= hightLevel) {
      Director::getInstance()->replaceScene(GameScene::createGameScene(level+1, styleGame));
    }
  }
}

void GameScene::createPopUpPause() {
  status = PAUSE;
  LayerColor* pauseLayer  = LayerColor::create(COLOR4V, winSize.width, winSize.height);
  pauseLayer->setPosition(Vec2(0,0));
  pauseLayer->setTag(1256);
  this->addChild(pauseLayer,1000);
  
  auto lister = EventListenerTouchOneByOne::create();
  lister->setSwallowTouches(true);
  lister->onTouchBegan = [=](Touch* pTouch, Event* mEvent) {
    return true;
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lister, pauseLayer);
  
  Sprite* bgPause = Sprite::create("bgHomeScene.png");
  bgPause->setPosition(Vec2(winSize.width/2, winSize.height/2));
  pauseLayer->addChild(bgPause,-1);
  
  Sprite* thumbPause = Sprite::create("bg_pause.png");
  thumbPause->setPosition(Vec2(winSize.width/2, winSize.height/2));
  pauseLayer->addChild(thumbPause,1);
  
  /*btnPlay*/
  ui::Button* btnResume = ui::Button::create("resume_normal.png","resume_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnResume->setPosition(Vec2(thumbPause->getPositionX(), thumbPause->getPositionY() - btnResume->getContentSize().height*0.56));
  btnResume->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  btnResume->setTag(TAG_POPUP_RESUME);
  pauseLayer->addChild(btnResume,3);
  
  /*btnReplayPause*/
  ui::Button* btnReplayPauseGame = ui::Button::create("replay_normal.png","replay_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnReplayPauseGame->setPosition(Vec2(btnResume->getPositionX() + btnReplayPauseGame->getContentSize().width*1.1, btnResume->getPositionY()));
  btnReplayPauseGame->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  btnReplayPauseGame->setTag(TAG_POPUP_REPLAY);
  pauseLayer->addChild(btnReplayPauseGame,3);
  
  /*btnHome*/
  ui::Button* btnMenu = ui::Button::create("menu_normal.png","menu_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnMenu->setPosition(Vec2(btnResume->getPositionX() - btnMenu->getContentSize().width*1.1, btnResume->getPositionY()));
  btnMenu->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  btnMenu->setTag(TAG_POPUP_MENU);
  pauseLayer->addChild(btnMenu,3);
}

void GameScene::sendNotificationForGameScene() {
  level ++;
  if(level >= hightLevel) {
    if(styleGame == EASY_LEVEL) {
      UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_EASY, level);
    } else if (styleGame == MEDIUM_LEVEL) {
      UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_MEDIUM, level);
    } else if (styleGame == HARD_LEVEL) {
      UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_HARD, level);
    } else {
      UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_EXPERT, level);
    }
  }
  if(level <= 100) {
    char bufferMoveBest[512] = {0};
    sprintf(bufferMoveBest, "MoveBest-%d-%d", styleGame, level-1);
    int moveBestPrevious = UserDefault::getInstance()->getIntegerForKey(bufferMoveBest,1000);
    if(moveBestPrevious > countMove) {
      UserDefault::getInstance()->setIntegerForKey(bufferMoveBest, countMove);
    }
    this->unschedule("UpdateLevel");
    this->scheduleOnce([=]( float dt){
      Director::getInstance()->replaceScene(GameScene::createGameScene(level, styleGame));
    }, 0.4f, "UpdateLevel");
  }
}

void GameScene::onEnter() {
  Layer::onEnter();
  EventListenerKeyboard *keyboardEvent = EventListenerKeyboard::create();
  keyboardEvent->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event*){
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
      Director::getInstance()->replaceScene(HomeScene::createHomeScene());
    }
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEvent, this);
}

void GameScene::onExit() {
  Layer::onExit();
  Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

bool GameScene::handleTouchBegan(Touch* mTouch, Event* pEvent) {
  Vec2 touchPositionInGameBoardCoordinator = Vec2(mTouch->getLocation().x - gameBoard->getPosition().x,
                                                  mTouch->getLocation().y - gameBoard->getPosition().y);
  touchObject = gameBoard->getGameObjectAtPosition(touchPositionInGameBoardCoordinator);
  if(touchObject == NULL) { return false; }
  status = RUNNING;
  previousHeadMatrix = touchObject->getHeadMatrix();
  touchBeginPosition = mTouch->getLocation();
  return true;
}

void GameScene::handleTouchMove(Touch* mTouch, Event* pEvent) {
  if (touchObject == NULL) { return; }
  
  Vec2 touchPosition = mTouch->getLocation();
  float distanceX = touchPosition.x - touchBeginPosition.x;
  float distanceY = touchPosition.y - touchBeginPosition.y;
  touchBeginPosition = touchPosition;
  
  Vec2 currentPosition = touchObject->getPosition();
  Vec2 movePosition;
  if (touchObject->getTypeDirection() == HORIZONTAL) {
    float newPositionX = fmin(fmax(currentPosition.x + distanceX, 0.0), (MAX_COLUMN_ROW_GAMEBOARD - touchObject->getLengthObject()) * WIDTH_HEIGHT_OBJECT_PIXEL);
    movePosition = Vec2(newPositionX, currentPosition.y);
  } else {
    float newPositionY = fmin(fmax(currentPosition.y + distanceY, 0.0), (MAX_COLUMN_ROW_GAMEBOARD - touchObject->getHeightObject()) * WIDTH_HEIGHT_OBJECT_PIXEL);
    movePosition = Vec2(currentPosition.x, newPositionY);
  }
  
  if (gameBoard->checkGameObjectCanMove(touchObject, movePosition)) {
    touchObject->setPositionObject(movePosition);
  }
}

void GameScene::handleTouchEnd(Touch* mTouch, Event* pEvent) {
  if (touchObject == NULL) { return;}
  
  this->updateGameBoardWhenTouchEnd(touchObject, touchObject->getPosition());
  
  /** count  how many move times **/
  if(previousHeadMatrix.x != touchObject->getHeadMatrix().x ||
     previousHeadMatrix.y != touchObject->getHeadMatrix().y) {
    countMove ++;
    lbMoveGame->setString(std::to_string(countMove));
  }
  
  /// Check game win
  gameBoard->checkWinGame();
  //  gameBoard->printGameBoard();
  
  touchObject = NULL;
  previousHeadMatrix = Vec2(-1, -1);
}

void GameScene::updateGameBoardWhenTouchEnd(GameObject* gameObject, Vec2 newPosition) {
  Vec2 newHeadMatrix = gameBoard->convertPositionToMatrix(newPosition);
  Vec2 newLastMatrix;
  if (gameObject->getTypeDirection() == HORIZONTAL) {
    newLastMatrix = Vec2(newHeadMatrix.x + gameObject->getLengthObject(),
                         newHeadMatrix.y);
  } else {
    newLastMatrix = Vec2(newHeadMatrix.x,
                         newHeadMatrix.y + gameObject->getHeightObject());
  }
  gameBoard->updateGameBoardMatrixValue(gameObject, INVISIBLE);
  gameObject->movePosition(gameBoard->convertMatrixToPosition(newHeadMatrix), newHeadMatrix, newLastMatrix);
  gameBoard->updateGameBoardMatrixValue(gameObject, VISIBLE);
}

void GameScene::showFullScreenAdvertisement(const char* key, int frequency) {
  int currentCount = UserDefault::getInstance()->getIntegerForKey(key, 0);
  if(currentCount == frequency) {
    AdmobManager::getInstance()->showInterstitial();
    UserDefault::getInstance()->setIntegerForKey(key, 0);
  } else {
    UserDefault::getInstance()->setIntegerForKey(key, currentCount + 1);
  }
}
