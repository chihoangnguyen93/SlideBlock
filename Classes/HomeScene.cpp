//
//  HomeScene.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 12/9/16.
//
//

#include "HomeScene.h"
#include "LevelScene.h"
#include "HelpScene.h"
#include "ModeGameScene.h"
#include "GameDefine.h"
#include "AdmobManager.h"

HomeScene::HomeScene() {}

HomeScene::~HomeScene() {}

Scene* HomeScene::createHomeScene() {
  auto scene = Scene::create();
  auto layer = HomeScene::create();
  scene->addChild(layer);
  return scene;
}

bool HomeScene::init() {
  if(!Layer::init()) { return false; }
  winsize = Director::getInstance()->getWinSize();
  Sprite* spBg = Sprite::create("bgHomeScene.png");
  spBg->setPosition(Vec2(winsize.width/2, winsize.height/2));
  this->addChild(spBg,1);
  
  /*icon game*/
  Sprite* spIcon = Sprite::create("iconGame.png");
  spIcon->setPosition(Vec2(winsize.width/2, winsize.height*3/4));
  this->addChild(spIcon,1);
  this->createButton();
  AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
  return true;
}

void HomeScene::createButton() {
  /*Button Play*/
  btnPlay = ui::Button::create("play_normal.png","play_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnPlay->setPosition(Vec2(winsize.width/2, winsize.height/2));
  btnPlay->setTag(TAG_BTN_PLAY);
  btnPlay->addClickEventListener(CC_CALLBACK_1(HomeScene::btnClickButtonHome,this));
  this->addChild(btnPlay,2);
  
  /*Button Puzzle*/
  btnPuzzle = ui::Button::create("mode_normal.png","mode_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnPuzzle->setPosition(Vec2(btnPlay->getPositionX(), btnPlay->getPositionY() - btnPuzzle->getContentSize().height*1.2));
  btnPuzzle->setTag(TAG_BTN_PUZZLE);
  btnPuzzle->addClickEventListener(CC_CALLBACK_1(HomeScene::btnClickButtonHome,this));
  this->addChild(btnPuzzle,2);
  
  /*Button More*/
  btnMore = ui::Button::create("more_normal.png","more_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnMore->setPosition(Vec2(btnPlay->getPositionX(), btnPuzzle->getPositionY() - btnMore->getContentSize().height*1.2));
  btnMore->setTag(TAG_BTN_MORE);
  btnMore->addClickEventListener(CC_CALLBACK_1(HomeScene::btnClickButtonHome,this));
  this->addChild(btnMore,2);
  
  /*Button Help*/
  btnHelp = ui::Button::create("help_normal.png","help_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnHelp->setPosition(Vec2(winsize.width - btnHelp->getContentSize().width*0.7, btnHelp->getContentSize().height*0.7));
  btnHelp->setTag(TAG_BTN_HELP);
  btnHelp->addClickEventListener(CC_CALLBACK_1(HomeScene::btnClickButtonHome,this));
  this->addChild(btnHelp,2);
}

void HomeScene::btnClickButtonHome(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  if(tag == TAG_BTN_PLAY) {
    Director::getInstance()->pushScene(LevelScene::createLevelGameScene(EASY_LEVEL, DIRECTOR_HOME));
  }
  if(tag == TAG_BTN_HELP) {
    Director::getInstance()->pushScene(HelpScene::createHelpScene());
  }
  if(tag == TAG_BTN_PUZZLE) {
    Director::getInstance()->pushScene(ModeGameScene::createModeGameScene());
  }
  if(tag == TAG_BTN_MORE)
  {
   #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    Application::getInstance()->openURL("https://itunes.apple.com/vn/developer/nguyen-chi-hoang/id1208114740?mt=8");
   #endif
   #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    Application::getInstance()->openURL("https://play.google.com/store/apps/developer?id=Nguyen+Chi+Hoang");
   #endif
  }
}

void HomeScene::onEnter() {
  Layer::onEnter();
  EventListenerKeyboard *keyboardEvent = EventListenerKeyboard::create();
  keyboardEvent->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event*){
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
      Director::getInstance()->end();
    }
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEvent, this);
}

void HomeScene::onExit() {
  Layer::onExit();
  Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}
