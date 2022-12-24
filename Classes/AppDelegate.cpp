#include "AppDelegate.h"
#include "HomeScene.h"
#include "AndroidScene.h"
#include "GameDefine.h"
#include "AdmobManager.h"
USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(640, 960);


AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
  //set OpenGL context attributions,now can only set six attributions:
  //red,green,blue,alpha,depth,stencil
  GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
  GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages() {
  return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
  // initialize director
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();
  if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    glview = GLViewImpl::createWithRect("SlideBlock", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
    glview = GLViewImpl::create("SlideBlock");
#endif
    director->setOpenGLView(glview);
  }
  FileUtils::getInstance()->addSearchPath("imageHomeScene/");
  FileUtils::getInstance()->addSearchPath("imageHelpScene/");
  FileUtils::getInstance()->addSearchPath("imageSettingScene/");
  FileUtils::getInstance()->addSearchPath("imageModeGameScene/");
  FileUtils::getInstance()->addSearchPath("imageLevelScene/");
  FileUtils::getInstance()->addSearchPath("imageInGameScene/");
  FileUtils::getInstance()->addSearchPath("imagePopUp-Pause/");
  FileUtils::getInstance()->addSearchPath("iconGame/");
  FileUtils::getInstance()->addSearchPath("data/EasyLevel/");
  FileUtils::getInstance()->addSearchPath("data/MediumLevel/");
  FileUtils::getInstance()->addSearchPath("data/HardLevel/");
  FileUtils::getInstance()->addSearchPath("data/ExpertLevel/");
  FileUtils::getInstance()->addSearchPath("fonts/");
  
  // turn on display FPS
  director->setDisplayStats(false);
  
  // set FPS. the default value is 1.0/60 if you don't call this
  director->setAnimationInterval(1.0 / 60);
  
  // Set the design resolution
  glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
  register_all_packages();
  
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  AdmobManager::getInstance()->init("ca-app-pub-2381211844174227/8228738299", "ca-app-pub-2381211844174227/6468934067");
  auto scene = HomeScene::createHomeScene();
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
  auto scene = AndroidScene::CreateScene();
#endif
  
  // run
  director->runWithScene(scene);
  
  return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
  Director::getInstance()->stopAnimation();
  
  // if you use SimpleAudioEngine, it must be pause
  // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
  Director::getInstance()->startAnimation();
  
  // if you use SimpleAudioEngine, it must resume here
  // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
