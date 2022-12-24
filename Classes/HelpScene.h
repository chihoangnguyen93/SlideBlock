//
//  HelpScene.h
//  SlideBlock
//
//  Created by Harry Nguyen on 3/7/17.
//
//

#ifndef HelpScene_h
#define HelpScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class HelpScene : public Layer {
public:
  Size winsize;
  cocos2d::ui::PageView* pageView;
public:
  virtual bool init();
  virtual ~HelpScene();
  static Scene* createHelpScene();
  void pageViewEvent(Ref *pSender, cocos2d::ui::PageView::EventType type);
  void btnClickButtonNextPrevious(Ref* pSender);
  CREATE_FUNC(HelpScene);
  void onEnter();
  void onExit();
};

#endif /* HelpScene_h */
