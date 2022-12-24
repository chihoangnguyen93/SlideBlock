/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"
@implementation RootViewController

void openIturn(const char* URL){
  dispatch_async(dispatch_get_main_queue(), ^{
    NSString *msg = [NSString stringWithCString:URL encoding:NSASCIIStringEncoding];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:msg]];
  });
}

static RootViewController* rootView;
static UIViewController* viewController;
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
  return UIInterfaceOrientationMaskAllButUpsideDown;
#endif
}

- (BOOL) shouldAutorotate {
  return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
  [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
  auto glview = cocos2d::Director::getInstance()->getOpenGLView();
  
  if (glview) {
    CCEAGLView *eaglview = (CCEAGLView*) glview->getEAGLView();
    if (eaglview){
      CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
      cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
    }
  }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden {
  return YES;
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
}

- (void)viewDidUnload {
  [super viewDidUnload];
  self = [RootViewController createRootView];
  if(viewController == nil){
    viewController = [self rootViewController];
  }
}

- (void)dealloc {
  [super dealloc];
}

-(UIViewController*) rootViewController{
  UIWindow* window = [[UIApplication sharedApplication] keyWindow];
  if (!window) {
    NSArray* windows = [[UIApplication sharedApplication] windows];
    window = [windows objectAtIndex:0];
  }
  return [window rootViewController];
}

+(RootViewController*) createRootView{
  if(rootView == nil){
    rootView = [[RootViewController alloc] init];
  }
  return rootView;
}

@end
