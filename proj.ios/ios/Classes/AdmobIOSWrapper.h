//
//  AdmobIOSWrapper.h
//  SlideBlock
//
//  Created by Harry Nguyen on 22/8/22.
//

#ifndef AdmobIOSWrapper_h
#define AdmobIOSWrapper_h

#include "cocos2d.h"
class AdmobIOSWrapper {
public:
  static AdmobIOSWrapper* getInstance();
  void init(const std::string &bannerId, const std::string &interstitialId);
  void showBanner(int position);
  void hideBanner();
  void showInterstitial();
};

#endif /* AdmobIOSWrapper_h */
