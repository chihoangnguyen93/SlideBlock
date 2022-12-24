//
//  GameDefine.h
//  SlideBlock
//
//  Created by Harry Nguyen on 11/29/16.
//
//

#ifndef GameDefine_h
#define GameDefine_h

/// Global define
#define INVALID_MATRIX_VALUE -1
#define TOTAL_LEVEL 5
#define TOTAL_ROW 4
#define TOTAL_COLUM 5

#define MAX_COLUMN_ROW_GAMEBOARD  6
#define WIDTH_HEIGHT_OBJECT_PIXEL 80

#define EASY_LEVEL 1
#define MEDIUM_LEVEL 2
#define HARD_LEVEL 3
#define EXPERT_LEVEL 4

#define USER_HIGHT_LEVEL_EASY "EasyLevel"
#define USER_HIGHT_LEVEL_MEDIUM "MediumLevel"
#define USER_HIGHT_LEVEL_HARD "HardLevel"
#define USER_HIGHT_LEVEL_EXPERT "ExpertLevel"

/// Home scene
#define DIRECTOR_HOME 1001
#define DIRECTOR_MODE 1002


/// ModeGame scene
#define TAG_BTN_BACK_MODEGAME 2001
#define TAG_BTN_EASY_MODEGAME 2002
#define TAG_BTN_MEDIUM_MODEGAME 2003
#define TAG_BTN_HARD_MODEGAME 2004
#define TAG_BTN_EXPERT_MODEGAME 2005

/// Level scene
#define TAG_BTN_BACK_LEVEL_SCENE 3001
#define COLOR4V Color4B(6,75,38,0)
#define BUFFER 40


/// Help scene
#define TAG_BTN_BACK 5001
#define TAG_BTN_NEXT 5002
#define TAG_BTN_PREVIOUS 5003

/// Game scene
#define TAG_BTN_PAUSE_GAMESCENE 6001
#define TAG_BTN_REPLAY_GAMESCENE 6002
#define TAG_BTN_BACK_GAMESCENE 6003
#define TAG_BTN_NEXTSLOT_GAMESCENE 6004
#define TAG_BTN_BACKSLOT_GAMESCENE 6005

#define TAG_POPUP_RESUME 6006
#define TAG_POPUP_REPLAY 6007
#define TAG_POPUP_MENU 6008

typedef enum
{
  VERTICAL = 0,
  HORIZONTAL = 1
}OBJECT_DIRECTION;

typedef enum
{
  NORMARL_OBJECT = 0,
  WIN_OBJECT = 1,
}OBJECTTYPE;

typedef enum
{
  INVISIBLE = 0,
  VISIBLE = 1
}MATRIX_VALUE;

typedef enum {
  RUNNING = 0,
  PAUSE,
  WIN,
}GAME_STATUS;

typedef enum
{
  TAG_BTN_PLAY = 1102,
  TAG_BTN_PUZZLE = 1103,
  TAG_BTN_MORE = 1104,
  TAG_BTN_HELP = 1105,
} TAG_BTN_HOME;

/// Admod
#define OPEN_GAME_SCREEN_KEY "OPEN_GAME_SCREEN_KEY"
#define OPEN_GAME_SCREEN_FREQUENCY 8

#endif /* GameDefine_h */
