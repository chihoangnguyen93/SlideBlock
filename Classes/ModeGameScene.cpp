//
//  ModeGameScene.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 3/7/17.
//
//

#include "ModeGameScene.h"
#include "HomeScene.h"
#include "LevelScene.h"
#include "GameDefine.h"
#include "AdmobManager.h"

Scene* ModeGameScene::createModeGameScene() {
  auto scene = Scene::create();
  auto layer = ModeGameScene::create();
  scene->addChild(layer);
  return scene;
}

ModeGameScene::~ModeGameScene() {}

bool ModeGameScene::ModeGameScene::init() {
  if(!Layer::init()) { return false; }
  
  int adMode = UserDefault::getInstance()->getIntegerForKey("ADFULLMODE", 1);
  UserDefault::getInstance()->setIntegerForKey("ADFULLMODE", adMode + 1);
  
  winsize = Director::getInstance()->getWinSize();
  Sprite* spBg = Sprite::create("bgHomeScene.png");
  spBg->setPosition(Vec2(winsize.width/2, winsize.height/2));
  this->addChild(spBg,1);
  
  /* create background */
  Sprite* bgMode = Sprite::create("bg_mode.png");
  bgMode->setPosition(Vec2(winsize.width/2, winsize.height*0.55));
  this->addChild(bgMode,2);
  
  this->createButtonModeGame();
  
  ui::Button* btnBack = ui::Button::create("backscene_normal.png","backscene_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnBack->setPosition(Vec2(btnBack->getContentSize().width/2,btnBack->getContentSize().height/2));
  btnBack->setTag(TAG_BTN_BACK_MODEGAME);
  btnBack->addClickEventListener(CC_CALLBACK_1(ModeGameScene::btnClickButtonModeGame,this));
  this->addChild(btnBack,1234);
  AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
  return true;
}

void ModeGameScene::createButtonModeGame() {
  ui::Button* btnEasy = ui::Button::create("easy_normal.png","easy_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnEasy->setPosition(Vec2(winsize.width/2, winsize.height *0.76));
  btnEasy->addClickEventListener(CC_CALLBACK_1(ModeGameScene::btnClickButtonModeGame,this));
  btnEasy->setTag(TAG_BTN_EASY_MODEGAME);
  this->addChild(btnEasy,10);
  
  ui::Button* btnMedium = ui::Button::create("medium_normal.png","medium_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnMedium->setPosition(Vec2(winsize.width/2, btnEasy->getPositionY() - btnMedium->getContentSize().height*1.1));
  btnMedium->addClickEventListener(CC_CALLBACK_1(ModeGameScene::btnClickButtonModeGame,this));
  btnMedium->setTag(TAG_BTN_MEDIUM_MODEGAME);
  this->addChild(btnMedium,10);
  
  ui::Button* btnHard = ui::Button::create("hard_normal.png","hard_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnHard->setPosition(Vec2(winsize.width/2, btnMedium->getPositionY() - btnHard->getContentSize().height*1.1));
  btnHard->addClickEventListener(CC_CALLBACK_1(ModeGameScene::btnClickButtonModeGame,this));
  btnHard->setTag(TAG_BTN_HARD_MODEGAME);
  this->addChild(btnHard,10);
  
  ui::Button* btnExpert = ui::Button::create("expert_normal.png","expert_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnExpert->setPosition(Vec2(winsize.width/2, btnHard->getPositionY() - btnExpert->getContentSize().height*1.1));
  btnExpert->addClickEventListener(CC_CALLBACK_1(ModeGameScene::btnClickButtonModeGame,this));
  btnExpert->setTag(TAG_BTN_EXPERT_MODEGAME);
  this->addChild(btnExpert,10);
  
  Label* lbEasyLevel = Label::createWithTTF("100", "HKABEL.TTF", 36);
  lbEasyLevel->setPosition(Vec2(btnEasy->getPositionX(), btnEasy->getPositionY()));
  lbEasyLevel->setColor(Color3B::ORANGE);
  this->addChild(lbEasyLevel,11);
  
  Label* lbMemdiumLevel = Label::createWithTTF("100", "HKABEL.TTF", 36);
  lbMemdiumLevel->setPosition(Vec2(btnMedium->getPositionX(), btnMedium->getPositionY()));
  lbMemdiumLevel->setColor(Color3B::ORANGE);
  this->addChild(lbMemdiumLevel,11);
  
  Label* lbHardLevel = Label::createWithTTF("100", "HKABEL.TTF", 36);
  lbHardLevel->setPosition(Vec2(btnHard->getPositionX(), btnHard->getPositionY()));
  lbHardLevel->setColor(Color3B::ORANGE);
  this->addChild(lbHardLevel,11);
  
  Label* lbExpertLevel = Label::createWithTTF("100", "HKABEL.TTF", 36);
  lbExpertLevel->setPosition(Vec2(btnExpert->getPositionX(), btnExpert->getPositionY()));
  lbExpertLevel->setColor(Color3B::ORANGE);
  this->addChild(lbExpertLevel,11);
}

void ModeGameScene::btnClickButtonModeGame(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  if(tag == TAG_BTN_BACK_MODEGAME) {
    Director::getInstance()->replaceScene(HomeScene::createHomeScene());
  }
  
  if(tag == TAG_BTN_EASY_MODEGAME) {
    Director::getInstance()->replaceScene(LevelScene::createLevelGameScene(EASY_LEVEL,DIRECTOR_MODE));
  }

  if(tag == TAG_BTN_MEDIUM_MODEGAME) {
    Director::getInstance()->replaceScene(LevelScene::createLevelGameScene(MEDIUM_LEVEL,DIRECTOR_MODE));
  }

  if(tag == TAG_BTN_HARD_MODEGAME) {
    Director::getInstance()->replaceScene(LevelScene::createLevelGameScene(HARD_LEVEL,DIRECTOR_MODE));
  }
  
  if(tag == TAG_BTN_EXPERT_MODEGAME) {
    Director::getInstance()->replaceScene(LevelScene::createLevelGameScene(EXPERT_LEVEL,DIRECTOR_MODE));
  }
}

void ModeGameScene::onEnter() {
  Layer::onEnter();
  EventListenerKeyboard *keyboardEvent = EventListenerKeyboard::create();
  keyboardEvent->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event*){
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
      Director::getInstance()->replaceScene(HomeScene::createHomeScene());
    }
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEvent, this);
}

void ModeGameScene::onExit() {
  Layer::onExit();
  Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}


