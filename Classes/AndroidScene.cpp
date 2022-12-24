//
//  AndroidScene.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 12/10/16.
//
//

#include "AndroidScene.h"
#include "HomeScene.h"

Scene* AndroidScene::CreateScene() {
  auto newScene = Scene::create();
  auto layer = AndroidScene::create();
  newScene->addChild(layer);
  return newScene;
}

bool AndroidScene::init() {
  if(!Layer::init()){ return false; }
  auto backgroundSprite = Sprite::create("bgHomeScene.png");
  backgroundSprite->setPosition(Vec2(Director::getInstance()->getWinSize().width/2,
                               Director::getInstance()->getWinSize().height/2 ));
  this->addChild(backgroundSprite);
  
  auto logoGame = Sprite::create("iconGame.png");
  logoGame->setPosition(Vec2(Director::getInstance()->getWinSize().width/2,
                               Director::getInstance()->getWinSize().height/2 ));
  this->addChild(logoGame);
  scheduleOnce(schedule_selector(AndroidScene::update), 1);
  return true;
}

void AndroidScene::update(float dt) {
  Director::getInstance()->replaceScene(HomeScene::createHomeScene());
}
