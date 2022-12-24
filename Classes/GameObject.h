//
//  GameObject.h
//  SlideBlock
//
//  Created by Harry Nguyen on 11/29/16.
//
//

#ifndef GameObject_h
#define GameObject_h

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class GameObject: public Sprite {
private:
  int typeDirection;
  int typeObject;
  int lengthObject;
  int heightObject;
  
  Vec2 headMatrix;
  Vec2 lastMatrix;
public:
  GameObject();
  virtual ~GameObject();
  void createWithFileName(const std::string& filename);
  void setPositionObject(const Vec2& pos);
  void setHeadAndLastMatrix(const Vec2& headMatrix, const Vec2& lastMatrix);
  void movePosition(const Vec2& position, const Vec2& newHeadMatrix, const Vec2& newLastMatrix);
  Vec2 getLastMatrix();
  Vec2 getHeadMatrix();
  int getLengthObject();
  int getHeightObject();
  
  void setTypeObject(int type);
  int getTypeObject();
  
  void runActionGameWin(const Vec2& pos);
  int getTypeDirection();
};

#endif /* GameObject_h */
