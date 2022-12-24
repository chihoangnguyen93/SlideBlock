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
  if(modeGame == EASY_LEVEL) {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_EASY, 1);
  } else if(modeGame == MEDIUM_LEVEL) {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_MEDIUM, 1);
  } else if(modeGame == HARD_LEVEL) {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_HARD, 1);
  } else {
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_EXPERT, 1);
  }
}

void CustomUserDefault::setHighLevel(int modeGame, int newValue) {
  if(newValue <= CustomUserDefault::GetInstance()->getHighLevel(modeGame)) { return; }
  if(modeGame == EASY_LEVEL) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_EASY, newValue);
  } else if (modeGame == MEDIUM_LEVEL) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_MEDIUM, newValue);
  } else if (modeGame == HARD_LEVEL) {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_HARD, newValue);
  } else {
    cocos2d::UserDefault::getInstance()->setIntegerForKey(USER_HIGHT_LEVEL_EXPERT, newValue);
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
