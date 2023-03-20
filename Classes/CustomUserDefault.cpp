//
//  CustomUserDefault.cpp
//  SlideBlockPuzzle
//
//  Created by Harry Nguyen on 11/1/23.
//

#include "CustomUserDefault.h"
#include "GameDefine.h"
#include "cocos2d.h"

CustomUserDefault* CustomUserDefault::instance= nullptr;

CustomUserDefault *CustomUserDefault::GetInstance() {
  if(instance == nullptr) { instance = new CustomUserDefault(); }
  return instance;
}

CustomUserDefault::~CustomUserDefault() {}

CustomUserDefault:: CustomUserDefault() {}

int CustomUserDefault::getHighLevel(int modeGame) {
  switch (modeGame) {
    case EASY_LEVEL:
      return cocos2d::UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_EASY, 1);
    case MEDIUM_LEVEL:
      return cocos2d::UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_MEDIUM, 1);
    case HARD_LEVEL:
      return cocos2d::UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_HARD, 1);
    case EXPERT_LEVEL:
      return cocos2d::UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_EXPERT, 1);
  }
}

void CustomUserDefault::setHighLevel(int modeGame, int newValue) {
  if(newValue <= CustomUserDefault::GetInstance()->getHighLevel(modeGame)) { return; }
  switch (modeGame) {
    case EASY_LEVEL:
      cocos2d::UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_EASY, newValue);
      break;
    case MEDIUM_LEVEL:
      cocos2d::UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_MEDIUM, newValue);
      break;
    case HARD_LEVEL:
      cocos2d::UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_HARD, newValue);
      break;
    case EXPERT_LEVEL:
      cocos2d::UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_EXPERT, newValue);
      break;
  }
}

int CustomUserDefault::getBestMove(int modeGame, int level) {
  char bufferMoveBest[512] = {0};
  snprintf(bufferMoveBest, sizeof(bufferMoveBest), "MoveBest-%d-%d", modeGame, level);
  return cocos2d::UserDefault::getInstance()->getIntegerForKey(bufferMoveBest, 0);
}

void CustomUserDefault::setBestMove(int modeGame, int level, int newValue) {
  int previousBestMove = getBestMove(modeGame, level);
  char bufferMoveBest[512] = {0};
  snprintf(bufferMoveBest, sizeof(bufferMoveBest), "MoveBest-%d-%d", modeGame, level);
  if(previousBestMove == 0) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(bufferMoveBest, newValue);
  } else if(newValue < previousBestMove) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(bufferMoveBest, newValue);
  }
}

bool CustomUserDefault::shouldDisplayFullAdMod(const char* key, int value) {
  int currentCount = cocos2d::UserDefault::getInstance()->getIntegerForKey(key, 0);
  if(currentCount == value) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(key, 0);
    return true;
  } else {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(key, currentCount + 1);
    return false;
  }
}

int CustomUserDefault::getCurrentModeGame() {
  return cocos2d::UserDefault::getInstance()->getIntegerForKey(CURRENT_MODE_GAME, EASY_LEVEL);
}

void CustomUserDefault::finishCurrentModeGame() {
  int currentModel = getCurrentModeGame();
  switch (currentModel) {
    case EASY_LEVEL:
      cocos2d::UserDefault::getInstance()->setIntegerForKey(CURRENT_MODE_GAME, MEDIUM_LEVEL);
      break;
    case MEDIUM_LEVEL:
      cocos2d::UserDefault::getInstance()->setIntegerForKey(CURRENT_MODE_GAME, HARD_LEVEL);
      break;
    case HARD_LEVEL:
      cocos2d::UserDefault::getInstance()->setIntegerForKey(CURRENT_MODE_GAME, EXPERT_LEVEL);
      break;
    default: break;
  }
}

void CustomUserDefault::updateCurrentModeGame(int modeGame) {
  cocos2d::UserDefault::getInstance()->setIntegerForKey(CURRENT_MODE_GAME, modeGame);
}
