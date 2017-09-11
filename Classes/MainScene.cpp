//
//  MainScene.cpp
//  WordGame
//

#include "MainScene.hpp"
#include "extensions/cocos-ext.h"
#include "constant.h"
#include "GameLayer.hpp"

USING_NS_CC;

using namespace std;

Vec2 infiniteMoveBy = Vec2(-2,0);

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void MainScene::onEnter()
{
    
    CCLOG("In on enter main Scene");
    createBackgroundWithAnimation();
    
    createTextItems();
    createPlayButton();
    Node::onEnter();
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::updateBgPosition), 0.1);
}


void MainScene::createTextItems(){
    
    Label* gameName = Label::createWithTTF("WORD SEARCH", FONT_HEADLINE, 60);
    gameName->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height - gameName->getContentSize().height * 0.85));
    gameName->setColor(Color3B::BLACK);
    this->addChild(gameName);
    
    UserDefault* userDef =  UserDefault::getInstance();
    int value = userDef->getIntegerForKey(GAME_HIGH_SCORE_KEY, 0);
    
    std::string highScore = "HIGH SCORE " + std::to_string(value);
    highScoreStr = Label::createWithTTF(highScore, FONT_HEADLINE, 50);
    highScoreStr->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.25));
    highScoreStr->setColor(Color3B::BLACK);
    this->addChild(highScoreStr);
    
    std::string textStr = "Guess the 4 letter word before timer expires";
    Label* ruleStr = Label::createWithTTF(textStr, FONT_HEADLINE, 22);
    ruleStr->setAlignment(TextHAlignment::CENTER);
    ruleStr->setColor(Color3B::BLACK);
    ruleStr->setPosition(Vec2(visibleSize.width * 0.5, 50));
    this->addChild(ruleStr);
    
}

void MainScene::updateHighScoreAfterGame(){
    
    UserDefault* userDef =  UserDefault::getInstance();
    int value = userDef->getIntegerForKey(GAME_HIGH_SCORE_KEY, 0);
    std::string highScore = "HIGH SCORE " + std::to_string(value);
    highScoreStr->setString(highScore);
}

void MainScene::createPlayButton(){
    
    Label* playLabel = Label::createWithTTF("PLAY", FONT_HEADLINE, 60);
    MenuItemLabel* playButton = MenuItemLabel::create(playLabel, CC_CALLBACK_1(MainScene::playButtonCallback, this));
    playLabel->setColor(Color3B::BLACK);
    playButton->setAnchorPoint(Vec2(0.5,0.5));
    auto playMenu = Menu::create(playButton, NULL);
    playMenu->setPosition(Vec2(visibleSize.width* 0.5, visibleSize.height * 0.55));
    this->addChild(playMenu);
    
}

void MainScene::playButtonCallback(Ref* pSender){

    CCLOG("Inside play button callback");
    auto gameScene = GameLayer::createScene();
    Director::getInstance()->replaceScene(gameScene);
    
}

void MainScene::createBackgroundWithAnimation(){
    
    backGround = Sprite::create("background.jpg");
    backGround2 = Sprite::create("background.jpg");
    
    parallaxNode = InfiniteParallaxNode::create();
    backGround->setAnchorPoint(Point::ZERO);
    backGround2->setAnchorPoint(Point::ZERO);
    
    parallaxNode->addChild(backGround, 1, Vec2(1.0f, 0.0f),
                           Vec2(0.0f, visibleSize.height - backGround->getContentSize().height));
    parallaxNode->addChild(backGround2, 1, Vec2(1.0f, 0.0f),
                           Vec2(backGround->getContentSize().width,
                                visibleSize.height - backGround->getContentSize().height));
    
    addChild(parallaxNode);
}

void MainScene::updateBgPosition(float dt){
    
    parallaxNode->setPosition(parallaxNode->getPosition() + infiniteMoveBy);
    parallaxNode->updatePosition(infiniteMoveBy);
}

void MainScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
