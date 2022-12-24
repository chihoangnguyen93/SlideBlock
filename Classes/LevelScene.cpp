//
//  LevelScene.cpp
//  SlideBlock
//
//  Created by Harry Nguyen on 12/9/16.
//
//

#include "LevelScene.h"
#include "GameScene.h"
#include "GameDefine.h"
#include "ModeGameScene.h"
#include "HomeScene.h"
#include "AdmobManager.h"

LevelScene::LevelScene() {}

LevelScene::~LevelScene() {}

Scene* LevelScene::createLevelGameScene(int typeLevel, int director) {
  auto scene = Scene::create();
  auto layer = new LevelScene();
  layer->typeLevel = typeLevel;
  layer->director = director;
  layer->initUI();
  scene->addChild(layer);
  return scene;
}

void LevelScene::initUI() {
  int adLevel = UserDefault::getInstance()->getIntegerForKey("ADFULLLEVEL", 1);
  UserDefault::getInstance()->setIntegerForKey("ADFULLLEVEL", adLevel + 1);
  winsize = Director::getInstance()->getWinSize();
  
  Sprite* spBg = Sprite::create("bgHomeScene.png");
  spBg->setPosition(Vec2(winsize.width/2, winsize.height/2));
  this->addChild(spBg,1);
  
  ui::Button* btnBack = ui::Button::create("backscene_normal.png", "backscene_pressed.png", "", ui::Widget::TextureResType::LOCAL);
  btnBack->setPosition(Vec2(btnBack->getContentSize().height/2,btnBack->getContentSize().height/2));
  btnBack->setTag(TAG_BTN_BACK_LEVEL_SCENE);
  btnBack->addClickEventListener(CC_CALLBACK_1(LevelScene::handleClickOnBackButton,this));
  this->addChild(btnBack,1234);
  
  Sprite* titleSpr;
  if(typeLevel == EASY_LEVEL) {
    titleSpr = Sprite::create("box_easy.png");
    hightLevel = UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_EASY, 1);
  }
  if(typeLevel == MEDIUM_LEVEL) {
    titleSpr = Sprite::create("box_normal.png");
    hightLevel = UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_MEDIUM, 1);
  }
  if(typeLevel == HARD_LEVEL) {
    titleSpr = Sprite::create("box_hard.png");
    hightLevel = UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_HARD, 1);
  }
  if(typeLevel == EXPERT_LEVEL) {
    titleSpr = Sprite::create("box_expert.png");
    hightLevel = UserDefault::getInstance()->getIntegerForKey(USER_HIGHT_LEVEL_EXPERT, 1);
  }
  
  titleSpr->setPosition(Vec2(winsize.width/2, winsize.height*0.8));
  this->addChild(titleSpr,2);

  std::string stringValue = std::to_string(TOTAL_LEVEL*TOTAL_ROW*TOTAL_COLUM);
  stringValue= "1-"  + stringValue;
  
  Label* lbLevel = Label::createWithTTF(stringValue, "HKABEL.TTF", 36);
  lbLevel->setPosition(Vec2(titleSpr->getPositionX(), titleSpr->getPositionY() - titleSpr->getContentSize().height*0.3f));
  this->addChild(lbLevel,3);
  
  /* btn Next and Previous */
  currentPage = hightLevel/(TOTAL_COLUM*TOTAL_ROW);
  btnPrevious = ui::Button::create("button_back_normal.png","button_back_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnPrevious->setEnabled(false);

  btnNext =  ui::Button::create("button_next_normal.png","button_next_pressed.png","",ui::Widget::TextureResType::LOCAL);
  btnNext->setEnabled(false);
  
  btnPrevious->setPosition(Vec2(titleSpr->getPositionX() - titleSpr->getContentSize().width*0.8, titleSpr->getPositionY() + titleSpr->getContentSize().height*0.2));
  btnNext->setPosition(Vec2(titleSpr->getPositionX() + titleSpr->getContentSize().width*0.8, titleSpr->getPositionY() + titleSpr->getContentSize().height*0.2));
  this->addChild(btnPrevious,1235);
  this->addChild(btnNext,1235);
  
  /* CreateLevelGame */
  pageView = cocos2d::ui::PageView::create();
  pageView->setTouchEnabled(true);
  pageView->setContentSize(Size(winsize.width, winsize.height));
  pageView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
  pageView->removeAllItems();
  
  for (int i = 0; i < TOTAL_LEVEL + 1 ; i++) {
    auto layout = cocos2d::ui::Layout::create();
    layout->setContentSize(Size(winsize.width, winsize.height));
    LayerColor* color = LayerColor::create(COLOR4V, winsize.width, winsize.height);
    color->setAnchorPoint(Vec2::ZERO);
    color->setPosition(Vec2(0,0));
    layout->addChild(color);
    
    Sprite* imgSprite = Sprite::create("bg_level.png");
    imgSprite->setPosition(Vec2(winsize.width/2, winsize.height*0.42));
    layout->addChild(imgSprite);
    
    for (int j = 0; j < TOTAL_COLUM; j++) {
      for (int k = 0; k < TOTAL_ROW; k++) {
        int index = (i - 1)*TOTAL_ROW*TOTAL_COLUM + (k + j*TOTAL_ROW) + 1;
        auto buttonLevel = ui::Button::create();
        if(index <= hightLevel) {
          buttonLevel->loadTextures("level_open.png","","",ui::Widget::TextureResType::LOCAL);
          auto label = Label::createWithTTF("", "HKABEL.TTF", 32);
          label->setString(std::to_string(index));
          label->setPosition(Vec2(buttonLevel->getContentSize().width/2, buttonLevel->getContentSize().height/2));
          buttonLevel->addChild(label,100);
          buttonLevel->setEnabled(true);
        } else {
          buttonLevel->loadTextures("level_lock.png","","",ui::Widget::TextureResType::LOCAL);
          buttonLevel->setEnabled(false);
        }
        buttonLevel->addClickEventListener(CC_CALLBACK_1(LevelScene::handleClickOnLevelButton,this));
        buttonLevel->setTag(index + 1000);
        float height = winsize.height/2 + (TOTAL_ROW/2*buttonLevel->getContentSize().height/3*2) - j*(buttonLevel->getContentSize().height + 8.0f);
        buttonLevel->setPosition(Vec2(buttonLevel->getContentSize().width*1.5 + (buttonLevel->getContentSize().width + BUFFER)*k,height));
        layout->addChild(buttonLevel);
      }
    }
    pageView->insertPage(layout, i);
  }
  pageView->removeItem(0);
  pageView->scrollToItem(TOTAL_LEVEL - 2);
  pageView->setCurrentPageIndex(currentPage);
  pageView->addEventListener(CC_CALLBACK_2(LevelScene::pageViewEventCustom, this));
  this->addChild(pageView,4);
  AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
}

void LevelScene::pageViewEventCustom(Ref *pSender, cocos2d::ui::PageView::EventType type) {
  switch(type) {
    case cocos2d::ui::PageView::EventType::TURNING: break;
    default: break;
  }
}

void LevelScene::handleClickOnLevelButton(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  AdmobManager::getInstance()->hideBanner();
  Director::getInstance()->replaceScene(GameScene::createGameScene(tag-1000, typeLevel));
}

void LevelScene::handleClickOnBackButton(Ref* pSender){
  int tag = ((ui::Button*)pSender)->getTag();
  if(tag == TAG_BTN_BACK_LEVEL_SCENE) {
    if(director == DIRECTOR_HOME) {
      Director::getInstance()->replaceScene(HomeScene::createHomeScene());
    } else {
      Director::getInstance()->replaceScene(ModeGameScene::createModeGameScene());
    }
  }
}

void LevelScene::onEnter() {
  Layer::onEnter();
  EventListenerKeyboard *keyboardEvent = EventListenerKeyboard::create();
  keyboardEvent->onKeyReleased = [](EventKeyboard::KeyCode keyCode, Event*){
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK){
      Director::getInstance()->popScene();
    }
  };
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEvent, this);
}

void LevelScene::onExit() {
  Layer::onExit();
  Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

