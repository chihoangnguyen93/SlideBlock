//
//  LevelScene.h
//  SlideBlock
//
//  Created by Harry Nguyen on 12/9/16.
//
//

#ifndef LevelScene_h
#define LevelScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class LevelScene : public Layer {
private:
  void configUIWhenUpdateCurrentPage();
public:
  int currentPage, modeGame, director;
  Size winSize;
  ui::Button* nextPageButton, *previousPageButton;
  Label* levelLabel;
  cocos2d::ui::PageView* pageView;
  
public:
  LevelScene();
  void initUI();
  virtual ~LevelScene();
  static Scene* createLevelGameScene(int modeGame, int director);
  void pageViewEventCustom(Ref *pSender, cocos2d::ui::PageView::EventType type);
  CREATE_FUNC(LevelScene);
  void handleClickOnBackButton(Ref* pSender);
  void handleClickOnLevelButton(Ref* pSender);
};

#endif /* LevelScene_h */
