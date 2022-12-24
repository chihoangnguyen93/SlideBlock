//
//  AdmobManager.h
//  SlideBlock
//
//  Created by Harry Nguyen on 22/8/22.
//

#ifndef AdmobManager_h
#define AdmobManager_h

#include <string>

enum class AdmobPosition {
  TopCenter = 1,
  BottomCenter = 2
};

class AdmobManager {
public:
  static AdmobManager* getInstance();
  void init(const std::string &bannerId, const std::string &interstitialId);
  void showBanner(AdmobPosition position);
  void hideBanner();
  void showInterstitial();
};

#endif /* AdmobManager_h */
