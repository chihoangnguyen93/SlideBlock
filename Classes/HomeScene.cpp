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
#include "CustomUserDefault.h"

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
  winSize = Director::getInstance()->getWinSize();
  Sprite* backgroundSprite = Sprite::create("bgHomeScene.png");
  backgroundSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
  this->addChild(backgroundSprite,1);
  
  /*icon game*/
  Sprite* spIcon = Sprite::create("iconGame.png");
  spIcon->setPosition(Vec2(winSize.width/2, winSize.height*3/4));
  this->addChild(spIcon,1);
  this->createButton();
  AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
  return true;
}

void HomeScene::createButton() {
  /*Button Play*/
  playButton = ui::Button::create("play_normal.png","play_pressed.png","",ui::Widget::TextureResType::LOCAL);
  playButton->setPosition(Vec2(winSize.width/2, winSize.height/2));
  playButton->setTag(TAG_BTN_PLAY);
  playButton->addClickEventListener(CC_CALLBACK_1(HomeScene::handleClickButtonHome,this));
  this->addChild(playButton,2);
  
  /*Button Puzzle*/
  choseModeButton = ui::Button::create("mode_normal.png","mode_pressed.png","",ui::Widget::TextureResType::LOCAL);
  choseModeButton->setPosition(Vec2(playButton->getPositionX(), playButton->getPositionY() - choseModeButton->getContentSize().height*1.2));
  choseModeButton->setTag(TAG_BTN_PUZZLE);
  choseModeButton->addClickEventListener(CC_CALLBACK_1(HomeScene::handleClickButtonHome,this));
  this->addChild(choseModeButton,2);
  
  /*Button More*/
  moreGameButton = ui::Button::create("more_normal.png","more_pressed.png","",ui::Widget::TextureResType::LOCAL);
  moreGameButton->setPosition(Vec2(playButton->getPositionX(), choseModeButton->getPositionY() - moreGameButton->getContentSize().height*1.2));
  moreGameButton->setTag(TAG_BTN_MORE);
  moreGameButton->addClickEventListener(CC_CALLBACK_1(HomeScene::handleClickButtonHome,this));
  this->addChild(moreGameButton,2);
  
  /*Button Help*/
  helpButton = ui::Button::create("help_normal.png","help_pressed.png","",ui::Widget::TextureResType::LOCAL);
  helpButton->setPosition(Vec2(winSize.width - helpButton->getContentSize().width*0.7, helpButton->getContentSize().height*0.7));
  helpButton->setTag(TAG_BTN_HELP);
  helpButton->addClickEventListener(CC_CALLBACK_1(HomeScene::handleClickButtonHome,this));
  this->addChild(helpButton,2);
}

void HomeScene::handleClickButtonHome(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  switch (tag) {
    case TAG_BTN_PLAY:
      Director::getInstance()->pushScene(LevelScene::createLevelGameScene(CustomUserDefault::GetInstance()->getCurrentModeGame(),
                                                                          DIRECTOR_TO_HOME_SCENE));
      break;
    case TAG_BTN_HELP:
      Director::getInstance()->pushScene(HelpScene::createHelpScene());
      break;
    case TAG_BTN_PUZZLE:
      Director::getInstance()->pushScene(ModeGameScene::createModeGameScene());
      break;
    case TAG_BTN_MORE:
      #if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
      Application::getInstance()->openURL("https://itunes.apple.com/vn/developer/nguyen-chi-hoang/id1208114740?mt=8");
      #endif
      #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
      Application::getInstance()->openURL("https://play.google.com/store/apps/developer?id=Nguyen+Chi+Hoang");
      #endif
      break;
    default: break;
  }
}
