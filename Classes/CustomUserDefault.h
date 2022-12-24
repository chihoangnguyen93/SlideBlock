//
//  CustomUserDefault.h
//  SlideBlockPuzzle
//
//  Created by Harry Nguyen on 11/1/23.
//

#ifndef CustomUserDefault_h
#define CustomUserDefault_h

#include <stdio.h>

class CustomUserDefault {
private:
  static CustomUserDefault* instance;
  CustomUserDefault();
public:
  virtual ~CustomUserDefault();
  
  /* Singletons should not be cloneable. */
  CustomUserDefault(CustomUserDefault &other) = delete;
  
  /* Singletons should not be assignable. */
  void operator=(const CustomUserDefault &) = delete;
  
  static CustomUserDefault *GetInstance();
  
  int getHighLevel(int modeGame);
  void setHighLevel(int modeGame, int value);
  
  int getBestMove(int modeGame, int level);
  void setBestMove(int modeGame, int level, int newValue);
  bool shouldDisplayFullAdMod(const char* key, int newValue);
};

#endif /* CustomUserDefault_h */
