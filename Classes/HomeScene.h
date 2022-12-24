//
//  HomeScene.h
//  SlideBlock
//
//  Created by Harry Nguyen on 12/9/16.
//
//

#ifndef HomeScene_h
#define HomeScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class HomeScene : public Layer {
public:
  Size winSize;
  ui::Button* playButton;
  ui::Button* choseModeButton;
  ui::Button* moreGameButton;
  ui::Button* helpButton;
public:
  HomeScene();
  virtual bool init();
  virtual ~HomeScene();
  static Scene* createHomeScene();
  CREATE_FUNC(HomeScene);
  void createButton();
  void handleClickButtonHome(Ref* pSender);
};
#endif /* HomeScene_h */
