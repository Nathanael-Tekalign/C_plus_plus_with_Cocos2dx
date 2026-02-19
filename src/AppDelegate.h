#pragma once
#include "application/AppDelegate.h" // or correct header path for your cocos2d-x version
class AppDelegate : public cocos2d::Application {
public:
    AppDelegate();
    virtual ~AppDelegate();
    virtual void initGLContextAttrs() override;
    virtual bool applicationDidFinishLaunching() override;
    virtual void applicationDidEnterBackground() override;
    virtual void applicationWillEnterForeground() override;
};
