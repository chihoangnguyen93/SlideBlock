//
//  AdmobManager.c
//  SlideBlock
//
//  Created by Harry Nguyen on 22/8/22.
//

#include "AdmobManager.h"
#include "cocos2d.h"
using namespace std;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AdmobIOSWrapper.h"
#endif

static AdmobManager _sAdmobManager;

AdmobManager* AdmobManager::getInstance() {
  return &_sAdmobManager;
}

void AdmobManager::init(const std::string &bannerId, const std::string &interstitialId) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  AdmobIOSWrapper::getInstance()->init(bannerId, interstitialId);
#endif
}

void AdmobManager::showBanner(AdmobPosition position) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  AdmobIOSWrapper::getInstance()->showBanner((int)position);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  cocos2d::JniMethodInfo minfo;
  if (cocos2d::JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","showBanner","()V")) {
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
  }
#endif
}

void AdmobManager::hideBanner() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  AdmobIOSWrapper::getInstance()->hideBanner();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  cocos2d::JniMethodInfo minfo;
  if (cocos2d::JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","hideBanner","()V")) {
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
  }
#endif
}

void AdmobManager::showInterstitial() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
  AdmobIOSWrapper::getInstance()->showInterstitial();
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  cocos2d::JniMethodInfo minfo;
  if (cocos2d::JniHelper::getStaticMethodInfo(minfo,"org/cocos2dx/cpp/AppActivity","showInterstitial","()V")) {
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
  }
#endif
}
