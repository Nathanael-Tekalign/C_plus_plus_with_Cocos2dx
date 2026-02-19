#include "MainScene.h"
#include "2d/CCLabel.h"
#include "base/CCDirector.h"
USING_NS_CC;

Scene* MainScene::createScene() { return MainScene::create(); }

bool MainScene::init() {
    if (!Scene::init()) return false;

    auto vs = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithSystemFont("Hello, Cocos2d-x!", "Arial", 36);
    label->setPosition(origin + Vec2(vs.width/2, vs.height/2));
    this->addChild(label);

    return true;
}
