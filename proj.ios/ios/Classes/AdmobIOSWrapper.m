//
//  AdmobIOSWrapper.m
//  SlideBlock
//
//  Created by Harry Nguyen on 22/8/22.
//

#import "AdmobIOSWrapper.h"
#import "AdmobIOS.h"

static AdmobIOSWrapper _sAdmobIOSWrapper;

AdmobIOSWrapper* AdmobIOSWrapper::getInstance() {
  return &_sAdmobIOSWrapper;
}

void AdmobIOSWrapper::init(const std::string &bannerId, const std::string &interstitialId) {
  NSString *banner = [NSString stringWithUTF8String:bannerId.data()];
  NSString* interstitial = [NSString stringWithUTF8String:interstitialId.data()];
  [[AdmobIOS sharedAdmob] setBannerId:banner];
  [[AdmobIOS sharedAdmob] setInterstitialId:interstitial];
}

void AdmobIOSWrapper::showBanner(int position) {
  [[AdmobIOS sharedAdmob] showBannerInPosition: position];
}

void AdmobIOSWrapper::hideBanner() {
  [[AdmobIOS sharedAdmob] hideBanner];
}

void AdmobIOSWrapper::showInterstitial() {
  [[AdmobIOS sharedAdmob] showInterstitial];
}
