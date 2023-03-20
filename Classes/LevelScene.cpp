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
#include "CustomUserDefault.h"

LevelScene::LevelScene() {}

LevelScene::~LevelScene() {}

Scene* LevelScene::createLevelGameScene(int modeGame, int director) {
  auto scene = Scene::create();
  auto layer = new LevelScene();
  layer->modeGame = modeGame;
  layer->director = director;
  layer->initUI();
  scene->addChild(layer);
  return scene;
}

void LevelScene::initUI() {
  winSize = Director::getInstance()->getWinSize();
  
  Sprite* backgroundSprite = Sprite::create("bgHomeScene.png");
  backgroundSprite->setPosition(Vec2(winSize.width/2, winSize.height/2));
  this->addChild(backgroundSprite,1);
  
  ui::Button* backButton = ui::Button::create("backscene_normal.png", "backscene_pressed.png", "", ui::Widget::TextureResType::LOCAL);
  backButton->setPosition(Vec2(backButton->getContentSize().height/2,backButton->getContentSize().height/2));
  backButton->setTag(TAG_BTN_BACK_LEVEL_SCENE);
  backButton->addClickEventListener(CC_CALLBACK_1(LevelScene::handleClickOnBackButton,this));
  this->addChild(backButton,1234);
    
  Sprite* headerSprite;
  
  switch (modeGame) {
    case EASY_LEVEL:
      headerSprite = Sprite::create("box_easy.png");
      break;
    case MEDIUM_LEVEL:
      headerSprite = Sprite::create("box_normal.png");
      break;
    case HARD_LEVEL:
      headerSprite = Sprite::create("box_hard.png");
      break;
    case EXPERT_LEVEL:
      headerSprite = Sprite::create("box_expert.png");
      break;
  }
  headerSprite->setPosition(Vec2(winSize.width/2, winSize.height*0.8));
  this->addChild(headerSprite,2);

  int hightLevel = CustomUserDefault::GetInstance()->getHighLevel(modeGame);
  if(hightLevel % (TOTAL_COLUM*TOTAL_ROW) == 0) {
    currentPage = hightLevel/(TOTAL_COLUM*TOTAL_ROW) - 1;
  } else {
    currentPage = hightLevel/(TOTAL_COLUM*TOTAL_ROW);
  }
  
  levelLabel = Label::createWithTTF("", "HKABEL.TTF", 36);
  levelLabel->setPosition(Vec2(headerSprite->getPositionX(), headerSprite->getPositionY() - headerSprite->getContentSize().height*0.3f));
  this->addChild(levelLabel,3);
  
  previousPageButton = ui::Button::create("button_back_normal.png","button_back_pressed.png","",ui::Widget::TextureResType::LOCAL);
  previousPageButton->setTag(TAG_BTN_PREVIOUS_PAGE);
  previousPageButton->addClickEventListener(CC_CALLBACK_1(LevelScene::handleClickOnBackButton,this));

  nextPageButton = ui::Button::create("button_next_normal.png","button_next_pressed.png","",ui::Widget::TextureResType::LOCAL);
  nextPageButton->setTag(TAG_BTN_NEXT_PAGE);
  nextPageButton->addClickEventListener(CC_CALLBACK_1(LevelScene::handleClickOnBackButton,this));

  previousPageButton->setPosition(Vec2(headerSprite->getPositionX() - headerSprite->getContentSize().width*0.8, headerSprite->getPositionY() + headerSprite->getContentSize().height*0.2));
  nextPageButton->setPosition(Vec2(headerSprite->getPositionX() + headerSprite->getContentSize().width*0.8, headerSprite->getPositionY() + headerSprite->getContentSize().height*0.2));
  this->addChild(previousPageButton,1235);
  this->addChild(nextPageButton,1235);
  
  /* CreateLevelGame */
  pageView = cocos2d::ui::PageView::create();
  pageView->setTouchEnabled(true);
  pageView->setContentSize(Size(winSize.width, winSize.height));
  pageView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
  pageView->removeAllItems();
  
  for (int i = 0; i < TOTAL_PAGE_LEVEL + 1 ; i++) {
    auto layout = cocos2d::ui::Layout::create();
    layout->setContentSize(Size(winSize.width, winSize.height));
    LayerColor* layerColor = LayerColor::create(COLOR4V, winSize.width, winSize.height);
    layerColor->setAnchorPoint(Vec2::ZERO);
    layerColor->setPosition(Vec2(0,0));
    layout->addChild(layerColor);
    
    Sprite* imgSprite = Sprite::create("bg_level.png");
    imgSprite->setPosition(Vec2(winSize.width/2, winSize.height*0.42));
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
        float height = winSize.height/2 + (TOTAL_ROW/2*buttonLevel->getContentSize().height/3*2) - j*(buttonLevel->getContentSize().height + 8.0f);
        buttonLevel->setPosition(Vec2(buttonLevel->getContentSize().width*1.5 + (buttonLevel->getContentSize().width + BUFFER)*k,height));
        layout->addChild(buttonLevel);
      }
    }
    pageView->insertPage(layout, i);
  }
  pageView->removeItem(0);
  configUIWhenUpdateCurrentPage();
  pageView->addEventListener(CC_CALLBACK_2(LevelScene::pageViewEventCustom, this));
  this->addChild(pageView,4);
  AdmobManager::getInstance()->showBanner(AdmobPosition::TopCenter);
}

void LevelScene::pageViewEventCustom(Ref *pSender, cocos2d::ui::PageView::EventType type) {
  switch(type) {
    case cocos2d::ui::PageView::EventType::TURNING:
      currentPage = (int)((cocos2d::ui::PageView*)pSender)->getCurrentPageIndex();
      configUIWhenUpdateCurrentPage();
      break;
    default: break;
  }
}

void LevelScene::handleClickOnLevelButton(Ref* pSender) {
  int tag = ((ui::Button*)pSender)->getTag();
  AdmobManager::getInstance()->hideBanner();
  Director::getInstance()->replaceScene(GameScene::createGameScene(tag-1000, modeGame));
}

void LevelScene::handleClickOnBackButton(Ref* pSender){
  int tag = ((ui::Button*)pSender)->getTag();
  switch (tag) {
    case TAG_BTN_BACK_LEVEL_SCENE:
      if(director == DIRECTOR_TO_HOME_SCENE) {
        Director::getInstance()->replaceScene(HomeScene::createHomeScene());
      } else {
        Director::getInstance()->replaceScene(ModeGameScene::createModeGameScene());
      }
      break;
    case TAG_BTN_PREVIOUS_PAGE:
      currentPage -= 1;
      configUIWhenUpdateCurrentPage();
      break;
    case TAG_BTN_NEXT_PAGE:
      currentPage += 1;
      configUIWhenUpdateCurrentPage();
      break;
    default: break;
  }
}

void LevelScene::configUIWhenUpdateCurrentPage() {
  pageView->setCurrentPageIndex(currentPage);
  pageView->scrollToPage(currentPage);
  previousPageButton->setEnabled(currentPage != 0);
  nextPageButton->setEnabled(currentPage < TOTAL_PAGE_LEVEL - 1);
  std::string fromLevel = std::to_string(currentPage * TOTAL_ROW*TOTAL_COLUM + 1);
  std::string toLevel = std::to_string((currentPage + 1) * TOTAL_ROW*TOTAL_COLUM);
  levelLabel->setString(fromLevel + " - " + toLevel);
}
