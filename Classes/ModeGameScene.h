//
//  ModeGameScene.h
//  SlideBlock
//
//  Created by Harry Nguyen on 3/7/17.
//
//

#ifndef ModeGameScene_h
#define ModeGameScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ModeGameScene : public Layer
{
public:
  Size winsize;
public:
  virtual bool init();
  static Scene* createModeGameScene();
  virtual ~ModeGameScene();
  CREATE_FUNC(ModeGameScene);
  void createButtonModeGame();
  
  void btnClickButtonModeGame(Ref* pSender);
  void onEnter();
  void onExit();
};

#endif /* ModeGameScene_h */
