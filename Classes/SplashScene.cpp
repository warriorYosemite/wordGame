//
//  SplashScene.cpp
//  WordGame
//

#include "SplashScene.hpp"
#include "constant.h"
#include "MainScene.hpp"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    isSplash = true;
    
    return true;
}


void SplashScene::onEnter()
{
    Layer::onEnter();
    
    Sprite* backGround = Sprite::create("ingame_bg.jpg");
    backGround->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
    float scaleFactorW = visibleSize.width / backGround->getContentSize().width;
    float scaleFactorH = visibleSize.height/ backGround->getContentSize().height;
    backGround->setScale(scaleFactorW, scaleFactorH);
    this->addChild(backGround);
    
    m_logo = Sprite::create("logo.png");
    m_logo->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.6));
    m_logo->setOpacityModifyRGB(true);
    m_logo->setScale(0.75);
    m_logo->setOpacity(0);
    this->addChild(m_logo);
    
    
    m_appName = Label::createWithTTF("WORD BOUNCE", FONT_HEADLINE, 60);
    m_appName->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.3));
    m_appName->setOpacityModifyRGB(true);
    m_appName->setColor(Color3B::WHITE);
    m_appName->setOpacity(0);
    this->addChild(m_appName);
    
    runLogoAnimation();
}

void SplashScene::runLogoAnimation(){
    
    CallFunc* callFun1 = CallFunc::create([=]{
        m_logo->runAction(FadeIn::create(4));
        
    });
    
    CallFunc* callFun2 = CallFunc::create([=]{
        m_appName->runAction(FadeIn::create(4));
        
    });
    
    CallFunc* callFun3 = CallFunc::create([=]{
        
        isSplash = false;
        auto mainScene = MainScene::createScene();
        Director::getInstance()->replaceScene(mainScene);
        
    });
    
    //delay is implemented so as the current implementation will complete and then the main scene will populate
    this->runAction(Spawn::create(callFun1, callFun2, NULL));
    this->runAction(Sequence::create(DelayTime::create(6),callFun3, NULL));
    
}

void SplashScene::onExit()
{
    Layer::onExit();
}

void SplashScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
