//
//  AndroidScene.h
//  SlideBlock
//
//  Created by Harry Nguyen on 12/10/16.
//
//

#ifndef AndroidScene_h
#define AndroidScene_h

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class AndroidScene: public Layer {
public:
  static Scene* CreateScene();
  virtual bool init();
  CREATE_FUNC(AndroidScene);
  void update(float dt);
};
#endif /* AndroidScene_h */
