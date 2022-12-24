//
//  AdmobIOS.h
//  SlideBlock
//
//  Created by Harry Nguyen on 22/8/22.
//

#ifndef AdmobIOS_h
#define AdmobIOS_h

#import <Foundation/Foundation.h>

@interface AdmobIOS: NSObject
@property (nonatomic, strong) NSString* bannerId;
@property (nonatomic, strong) NSString* interstitialId;

+ (AdmobIOS*)sharedAdmob;
- (void)showBannerInPosition:(int)position;
- (void)hideBanner;
- (void)showInterstitial;
@end

#endif /* AdmobIOS_h */
