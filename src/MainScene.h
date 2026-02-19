#pragma once
#include "2d/CCScene.h"
#include "2d/CCLayer.h"
#include "base/CCRef.h"

class MainScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(MainScene);
};
