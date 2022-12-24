//
//  HelpScene.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 3/7/17.
//
//

#include "HelpScene.h"
#include "GameDefine.h"
#include "HomeScene.h"
#include "AdmobManager.h"

HelpScene::~HelpScene() {}

Scene* HelpScene::createHelpScene() {
  auto scene = Scene::create();
  auto layer = HelpScene::create();
  scene->addChild(layer);
  return scene;
}

bool HelpScene::init() {
  if(!Layer::init()) {
    return false;
  }
  winsize = Director::getInstance()->getWinSize();
  Sprite* spBg = Sprite::create("bgHomeScene.png");
  spBg->setPosition(Vec2(winsize.width/2, winsize.height/2));
  this->addChild(spBg,1);
  
  pageView = cocos2d::ui::PageView::create();
  pageView->setTouchEnabled(true);
  pageView->addEventListener(CC_CALLBACK_2(HelpScene::pageViewEvent, this));
  pageView->setContentSize(Size(winsize.width, winsize.height));
  for (int i = 0; i < 3; i++) {
    auto layout = cocos2d::ui::Layout::create();
    layout->setContentSize(Size(winsize.width, winsize.height));
    LayerColor* color = LayerColor::create(COLOR4V, winsize.width, winsize.height);
    color->setAnchorPoint(Vec2::ZERO);
    color->setPosition(Vec2(0,40));
    layout->addChild(color);
    
    std::string bufferSprImg = "help" + std::to_string(i) + ".png";
    auto imgSprite = Sprite::create(bufferSprImg);
    imgSprite->setPosition(Vec2(winsize.width/2, winsize.height/2));
    layout->addChild(imgSprite);
    
    auto buttonNext = ui::Button::create();
    auto buttonPrevious = ui::Button::create();
    if(i ==0) {
      buttonNext->loadTextures("button_next_normal.png","button_next_pressed.png","",ui::Widget::TextureResType::LOCAL);
      buttonPrevious->loadTextures("button_back_end.png","button_back_pressed.png","",ui::Widget::TextureResType::LOCAL);
      buttonPrevious->setEnabled(false);
    } else if(i == 2) {
      buttonNext->loadTextures("button_next_end.png","button_next_pressed.png","",ui::Widget::TextureResType::LOCAL);
      buttonNext->setEnabled(false);
      buttonPrevious->loadTextures("button_back_normal.png","button_back_pressed.png","",ui::Widget::TextureResType::LOCAL);
    } else {
      buttonNext->loadTextures("button_next_normal.png","button_next_pressed.png","",ui::Widget::TextureResType::LOCAL);
      buttonPrevious->loadTextures("button_back_normal.png","button_back_pressed.png","",ui::Widget::TextureResType::LOCAL);
    }
    buttonNext->setTag(TAG_BTN_NEXT);
    buttonPrevious->setTag(TAG_BTN_PREVIOUS);
    buttonNext->addClickEventListener(CC_CALLBACK_1(HelpScene::btnClickButtonNextPrevious,this));
    buttonPrevious->addClickEventListener(CC_CALLBACK_1(HelpScene::btnClickButtonNextPrevious,this));
    
    buttonNext->setPosition(Vec2(imgSprite->getPositionX() + imgSprite->getContentSize().width*0.3,
                                 imgSprite->getPositionY() + imgSprite->getContentSize().height*0.44));
    layout->addChild(buttonNext,(3-i)*12 + 10);
    buttonPrevious->setPosition(Vec2(imgSprite->getPositionX() - imgSprite->getContentSize().width*0.3,
                                     imgSprite->getPositionY() + imgSprite->getContentSize().height*0.44));
    layout->addChild(buttonPrevious,(3-i)*12 + 10);
    pageView->insertPage(layout, i);
  }
  pageView->setCurrentPageIndex(0);
  this->addChild(pageView,2);
  
  /*Button Back*/
  ui::Button* btnBack = ui::Button::create("backscene_normal.png","backscene_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnBack->setPosition(Vec2(winsize.width/2,btnBack->getContentSize().height/2));
  btnBack->setTag(TAG_BTN_BACK);
  btnBack->addClickEventListener(CC_CALLBACK_1(HelpScene::btnClickButtonNextPrevious,this));
  this->addChild(btnBack,1234);
  AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
  return true;
}

void HelpScene::pageViewEvent(Ref *pSender, cocos2d::ui::PageView::EventType type) {}

void HelpScene::btnClickButtonNextPrevious(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  if (tag == TAG_BTN_NEXT) {
    pageView->setCurrentPageIndex(pageView->getCurrentPageIndex() + 1);
  }
  if (tag == TAG_BTN_PREVIOUS) {
    pageView->setCurrentPageIndex(pageView->getCurrentPageIndex() - 1);
  }
  if(tag == TAG_BTN_BACK) {
    Director::getInstance()->replaceScene(HomeScene::createHomeScene());
  }
}

void HelpScene::onEnter() {
  Layer::onEnter();
  EventListenerKeyboard *keyboardEvent = EventListenerKeyboard::create();
  keyboardEvent->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event*){
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
      Director::getInstance()->replaceScene(HomeScene::createHomeScene());
    }
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEvent, this);
}

void HelpScene::onExit() {
  Layer::onExit();
  Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}
