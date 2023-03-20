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
#include "LevelScene.h"
#include "HomeScene.h"
#include "AdmobManager.h"
#include "CustomUserDefault.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

Scene* GameScene::createGameScene(int level, int style) {
  auto scene = Scene::create();
  auto layer = new GameScene();
  layer->level = level;
  layer->modeGame = style;
  layer->initUpdateUI();
  scene->addChild(layer);
  return scene;
}

void GameScene::updateTimeRemaining(float dt) {
  if(status != RUNNING) { return; }
  timeRunGameInSeconds += dt;
  updateCounterTime(timeRunGameInSeconds);
}

void GameScene::updateCounterTime(int timeInSeconds) {
  char charTimes[1024] = {0};
  int minutes = timeInSeconds/60;
  int seconds = timeInSeconds%60;
  if(minutes < 10) {
    snprintf(charTimes, sizeof(charTimes), seconds > 9 ? "0%d:%d" : "0%d:0%d",minutes, seconds);
  } else {
    snprintf(charTimes, sizeof(charTimes), seconds > 9 ? "%d:%d" : "%d:0%d",minutes, seconds);
  }
  timeRunningLabel->setString(charTimes);
}

void GameScene::initUpdateUI() {
  winSize = Director::getInstance()->getWinSize();
  Sprite* backgroundSprite = Sprite::create("bgHomeScene.png");
  backgroundSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
  this->addChild(backgroundSprite);
  
  status = PAUSE;
  countMove = 0;
  timeRunGameInSeconds = 0;
 
  gameBoard = new GameBoard(modeGame, level);
  gameBoard->setPosition(Vec2(winSize.width - gameBoard->getContentSize().width,
                              (winSize.height - gameBoard->getContentSize().height)/2.0 - 24.0));
  this->addChild(gameBoard);
  
  createHeaderUI();
  createFooterUI();
  
  auto lister = EventListenerTouchOneByOne::create();
  lister->setSwallowTouches(true);
  lister->onTouchBegan = CC_CALLBACK_2(GameScene::handleTouchBegan, this);
  lister->onTouchMoved = CC_CALLBACK_2(GameScene::handleTouchMove, this);
  lister->onTouchEnded = CC_CALLBACK_2(GameScene::handleTouchEnd, this);
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lister, this);
  this->scheduleUpdate();
  
  // Show AdMod full screen
  if(CustomUserDefault::GetInstance()->shouldDisplayFullAdMod(OPEN_GAME_SCREEN_KEY, OPEN_GAME_SCREEN_FREQUENCY)) {
    AdmobManager::getInstance()->showInterstitial();
  }
  AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
  this->schedule(schedule_selector(GameScene::updateTimeRemaining), 1.0);
}

void GameScene::createHeaderUI() {
  Sprite* spBgHeader = Sprite::create("box_time.png");
  spBgHeader->setPosition(Vec2(winSize.width/2, winSize.height - spBgHeader->getContentSize().height/2));
  this->addChild(spBgHeader,2);
  
  /*pauseButton*/
  ui::Button* pauseButton = ui::Button::create("pause_normal.png","pause_pressed.png","",ui::Widget::TextureResType::LOCAL);
  pauseButton->setPosition(Vec2(spBgHeader->getPositionX() + spBgHeader->getContentSize().width/2 - pauseButton->getContentSize().width*0.8, spBgHeader->getPositionY() + pauseButton->getContentSize().height*0.4));
  pauseButton->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  pauseButton->setTag(TAG_BTN_PAUSE_GAME_SCENE);
  this->addChild(pauseButton,3);
  
  ui::Button* replayButton = ui::Button::create("replay_normal.png","replay_pressed.png","",ui::Widget::TextureResType::LOCAL);
  replayButton->setPosition(Vec2(pauseButton->getPositionX() - replayButton->getContentSize().width*1.1, pauseButton->getPositionY()));
  replayButton->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  replayButton->setTag(TAG_BTN_REPLAY_GAME_SCENE);
  this->addChild(replayButton,3);
  
  auto labelMoveBest = Label::createWithTTF("MoveBest : --", "HKABEL.TTF", 30);
  labelMoveBest->setPosition(Vec2(replayButton->getPositionX() - replayButton->getContentSize().width/2, replayButton->getPositionY() - replayButton->getContentSize().height*0.66));
  labelMoveBest->setAnchorPoint(Vec2(0, 0.5f));
  this->addChild(labelMoveBest,3);
  int countMoveBestPrevious = CustomUserDefault::GetInstance()->getBestMove(modeGame, level);
  if(countMoveBestPrevious != 0) {
    std::string bufferCountMoveBest = "MoveBest : " + std::to_string(countMoveBestPrevious);
    labelMoveBest->setString(bufferCountMoveBest);
    labelMoveBest->setVisible(true);
  } else {
    labelMoveBest->setVisible(false);
  }
  
  Sprite* spLevelTheme = Sprite::create("box_level.png");
  spLevelTheme->setPosition(Vec2(replayButton->getPositionX() - replayButton->getContentSize().width*1.9,
                                 replayButton->getPositionY() + spLevelTheme->getContentSize().height/4));
  this->addChild(spLevelTheme,3);
  
  Sprite* spMoveTheme = Sprite::create("box_moves.png");
  spMoveTheme->setPosition(Vec2(spLevelTheme->getPositionX(), spLevelTheme->getPositionY() - spLevelTheme->getContentSize().height*1.1));
  this->addChild(spMoveTheme,3);
  
  
  levelGameLabel = Label::createWithTTF(std::to_string(level), "HKABEL.TTF", 40);
  levelGameLabel->setPosition(Vec2(spLevelTheme->getPositionX() + spLevelTheme->getContentSize().width*0.2,
                                spLevelTheme->getPositionY() + + levelGameLabel->getContentSize().height/10));
  this->addChild(levelGameLabel,4);
  
  
  countMoveLabel = Label::createWithTTF(std::to_string(countMove), "HKABEL.TTF", 40);
  countMoveLabel->setPosition(Vec2(spMoveTheme->getPositionX() + spMoveTheme->getContentSize().width*0.2,
                               spMoveTheme->getPositionY() + countMoveLabel->getContentSize().height/10));
  this->addChild(countMoveLabel,4);
  
  timeRunningLabel = Label::createWithTTF("00:00", "HKABEL.TTF", 36);
  timeRunningLabel->setPosition(Vec2(spBgHeader->getPositionX(),
                               spBgHeader->getPositionY() - spBgHeader->getContentSize().height*0.36));
  this->addChild(timeRunningLabel,4);
}

void GameScene::createFooterUI() {
  backButton = ui::Button::create("backscene_normal.png",
                                   "backscene_pressed.png","",
                                   ui::Widget::TextureResType::LOCAL);
  backButton->setPosition(Vec2(backButton->getContentSize().height/2,
                                backButton->getContentSize().height/2));
  backButton->setTag(TAG_BTN_BACK_GAME_SCENE);
  backButton->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  this->addChild(backButton,100);
  
  previousLevelButton = ui::Button::create();
  nextLevelButton = ui::Button::create();
  if(level == 1) {
    previousLevelButton->loadTextures("back_end.png","","",ui::Widget::TextureResType::LOCAL);
    previousLevelButton->setEnabled(false);
    if(level == CustomUserDefault::GetInstance()->getHighLevel(modeGame)) {
      nextLevelButton->loadTextures("next_end.png","","",ui::Widget::TextureResType::LOCAL);
      nextLevelButton->setEnabled(false);
    } else {
      nextLevelButton->loadTextures("next_normal.png","next_pressed.png","",ui::Widget::TextureResType::LOCAL);
      nextLevelButton->setEnabled(true);
    }
  } else {
    previousLevelButton->loadTextures("back_normal.png","back_pressed.png","",ui::Widget::TextureResType::LOCAL);
    nextLevelButton->setEnabled(true);
    
    if(level == CustomUserDefault::GetInstance()->getHighLevel(modeGame)) {
      nextLevelButton->loadTextures("next_end.png","","",ui::Widget::TextureResType::LOCAL);
      nextLevelButton->setEnabled(false);
    } else {
      nextLevelButton->loadTextures("next_normal.png","next_pressed.png","",ui::Widget::TextureResType::LOCAL);
      nextLevelButton->setEnabled(true);
    }
  }
  
  previousLevelButton->setPosition(Vec2(winSize.width/2 - previousLevelButton->getContentSize().width*0.6, backButton->getPositionY()));
  previousLevelButton->setTag(TAG_BTN_PREVIOUS_LEVEL_GAME_SCENE);
  previousLevelButton->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  this->addChild(previousLevelButton,100);
  
  nextLevelButton->setPosition(Vec2(winSize.width/2 + nextLevelButton->getContentSize().width*0.6,backButton->getPositionY()));
  nextLevelButton->setTag(TAG_BTN_NEXT_LEVEL_GAME_SCENE);
  nextLevelButton->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  this->addChild(nextLevelButton,100);
}

void GameScene::buttonClickGameScene(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  switch (tag) {
    case TAG_BTN_BACK_GAME_SCENE:
      Director::getInstance()->replaceScene(LevelScene::createLevelGameScene(modeGame, DIRECTOR_TO_HOME_SCENE));
      break;
    case TAG_BTN_REPLAY_GAME_SCENE:
      Director::getInstance()->replaceScene(GameScene::createGameScene(level, modeGame));
      break;
    case TAG_BTN_PAUSE_GAME_SCENE:
      this->createPopUpPause();
      break;
    case TAG_POPUP_BACK_TO_PREVIOUS:
      Director::getInstance()->replaceScene(LevelScene::createLevelGameScene(modeGame, DIRECTOR_TO_HOME_SCENE));
      break;
    case TAG_POPUP_REPLAY:
      Director::getInstance()->replaceScene(GameScene::createGameScene(level, modeGame));
      break;
    case TAG_POPUP_RESUME:
      status = RUNNING;
      if(this->getChildByTag(TAG_POPUP_PAUSE)) { this->removeChildByTag(TAG_POPUP_PAUSE); }
      break;
    case TAG_BTN_PREVIOUS_LEVEL_GAME_SCENE:
      if(level == 1) { return; }
      Director::getInstance()->replaceScene(GameScene::createGameScene(level-1, modeGame));
      break;
    case TAG_BTN_NEXT_LEVEL_GAME_SCENE:
      if(level > CustomUserDefault::GetInstance()->getHighLevel(modeGame)) { return; }
      Director::getInstance()->replaceScene(GameScene::createGameScene(level+1, modeGame));
      break;
    default: break;
  }
}

void GameScene::createPopUpPause() {
  status = PAUSE;
  LayerColor* pauseLayer  = LayerColor::create(COLOR4V, winSize.width, winSize.height);
  pauseLayer->setPosition(Vec2(0,0));
  pauseLayer->setTag(TAG_POPUP_PAUSE);
  this->addChild(pauseLayer,1000);
  
  auto lister = EventListenerTouchOneByOne::create();
  lister->setSwallowTouches(true);
  lister->onTouchBegan = [=](Touch* pTouch, Event* mEvent) { return true; };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lister, pauseLayer);
  
  Sprite* bgPause = Sprite::create("bgHomeScene.png");
  bgPause->setPosition(Vec2(winSize.width/2, winSize.height/2));
  pauseLayer->addChild(bgPause,-1);
  
  Sprite* thumbPause = Sprite::create("bg_pause.png");
  thumbPause->setPosition(Vec2(winSize.width/2, winSize.height/2));
  pauseLayer->addChild(thumbPause,1);
  
  ui::Button* resumeButton = ui::Button::create("resume_normal.png","resume_pressed.png","",ui::Widget::TextureResType::LOCAL);
  resumeButton->setPosition(Vec2(thumbPause->getPositionX(), thumbPause->getPositionY() - resumeButton->getContentSize().height*0.56));
  resumeButton->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  resumeButton->setTag(TAG_POPUP_RESUME);
  pauseLayer->addChild(resumeButton,3);
  
  ui::Button* replayGameButton = ui::Button::create("replay_normal.png","replay_pressed.png","",ui::Widget::TextureResType::LOCAL);
  replayGameButton->setPosition(Vec2(resumeButton->getPositionX() + replayGameButton->getContentSize().width*1.1, resumeButton->getPositionY()));
  replayGameButton->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  replayGameButton->setTag(TAG_POPUP_REPLAY);
  pauseLayer->addChild(replayGameButton,3);
  
  ui::Button* backToPreviousButton = ui::Button::create("menu_normal.png","menu_pressed.png","",ui::Widget::TextureResType::LOCAL);
  backToPreviousButton->setPosition(Vec2(resumeButton->getPositionX() - backToPreviousButton->getContentSize().width*1.1, resumeButton->getPositionY()));
  backToPreviousButton->addClickEventListener(CC_CALLBACK_1(GameScene::buttonClickGameScene,this));
  backToPreviousButton->setTag(TAG_POPUP_BACK_TO_PREVIOUS);
  pauseLayer->addChild(backToPreviousButton,3);
}

void GameScene::displayGameWinScene() {
  CustomUserDefault::GetInstance()->setBestMove(modeGame, level, countMove);
  int nextLevel = level + 1;
  nextLevel = std::min(TOTAL_PAGE_LEVEL * TOTAL_ROW * TOTAL_COLUM, nextLevel);
  if (nextLevel > TOTAL_PAGE_LEVEL * TOTAL_ROW * TOTAL_COLUM) {
    nextLevel = 1;
    CustomUserDefault::GetInstance()->finishCurrentModeGame();
    modeGame = CustomUserDefault::GetInstance()->getCurrentModeGame();
  } else {
    CustomUserDefault::GetInstance()->setHighLevel(modeGame, nextLevel);
  }
  Director::getInstance()->replaceScene(GameScene::createGameScene(nextLevel, modeGame));
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
    float newPositionX = fmin(fmax(currentPosition.x + distanceX, 0.0), (MAX_COLUMN_ROW_GAME_BOARD - touchObject->getLengthObject()) * WIDTH_HEIGHT_OBJECT_PIXEL);
    movePosition = Vec2(newPositionX, currentPosition.y);
  } else {
    float newPositionY = fmin(fmax(currentPosition.y + distanceY, 0.0), (MAX_COLUMN_ROW_GAME_BOARD - touchObject->getHeightObject()) * WIDTH_HEIGHT_OBJECT_PIXEL);
    movePosition = Vec2(currentPosition.x, newPositionY);
  }
  
  if (gameBoard->checkGameObjectCanMove(touchObject, movePosition)) { touchObject->setPositionObject(movePosition); }
}

void GameScene::handleTouchEnd(Touch* mTouch, Event* pEvent) {
  if (touchObject == NULL) { return;}
  
  this->updateGameBoardWhenTouchEnd(touchObject, touchObject->getPosition());
  
  /** count  how many move times **/
  if(previousHeadMatrix.x != touchObject->getHeadMatrix().x || previousHeadMatrix.y != touchObject->getHeadMatrix().y) {
    countMove ++;
    countMoveLabel->setString(std::to_string(countMove));
  }
  
  if(gameBoard->checkReachToWinPoint(touchObject)) {
    Vec2 winPos = Vec2(touchObject->getPosition().x + (touchObject->getLengthObject() + 1) * WIDTH_HEIGHT_OBJECT_PIXEL,
                       touchObject->getPosition().y);
    touchObject->runWinAnimation(winPos, [this]{ this->displayGameWinScene(); });
  }
  touchObject = NULL;
  previousHeadMatrix = Vec2(INVALID_MATRIX_VALUE, INVALID_MATRIX_VALUE);
}

void GameScene::updateGameBoardWhenTouchEnd(GameObject* gameObject, Vec2 newPosition) {
  Vec2 newHeadMatrix = gameBoard->convertPositionToMatrix(newPosition);
  Vec2 newLastMatrix;
  if (gameObject->getTypeDirection() == HORIZONTAL) {
    newLastMatrix = Vec2(newHeadMatrix.x + gameObject->getLengthObject(), newHeadMatrix.y);
  } else {
    newLastMatrix = Vec2(newHeadMatrix.x, newHeadMatrix.y + gameObject->getHeightObject());
  }
  gameBoard->updateGameBoardMatrixValue(gameObject, INVISIBLE);
  gameObject->movePosition(gameBoard->convertMatrixToPosition(newHeadMatrix), newHeadMatrix, newLastMatrix);
  gameBoard->updateGameBoardMatrixValue(gameObject, VISIBLE);
}
