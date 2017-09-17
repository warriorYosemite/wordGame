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
    fillSpriteNameVector();
    
    createGridElements();
    
    createTextItems();
    createPlayButton();
    
    createPlayerProfileItem();
    
    Node::onEnter();
}


void MainScene::createTextItems(){
    
    Label* gameName = Label::createWithTTF("WORD BOUNCE", FONT_HEADLINE, 60);
    gameName->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height - gameName->getContentSize().height * 0.85));
    gameName->setColor(Color3B::WHITE);
    this->addChild(gameName);
}

void MainScene::fillSpriteNameVector() {
    
    m_spriteName.push_back("mainscreen_blue.png");
    m_spriteName.push_back("mainscreen_cyan.png");
    m_spriteName.push_back("mainscreen_green.png");
    m_spriteName.push_back("mainscreen_orange.png");
}

void MainScene::createGridElements() {
    
    Scale9Sprite* gridBg = Scale9Sprite::create("mainscreen_gridbg.png");
    gridBg->setContentSize(Size(visibleSize.width * 0.8, visibleSize.height * 0.1));
    gridBg->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.7));
    this->addChild(gridBg);

    
    
}

void MainScene::createPlayerProfileItem() {
    
    Sprite* playerProfileBg = Sprite::create("mainscreen_playerbg.png");
    playerProfileBg->setAnchorPoint(Vec2(0.5, 0));
    playerProfileBg->setPosition(Vec2(visibleSize.width * 0.5, 0));
    this->addChild(playerProfileBg);
    
    m_playerProfile = Sprite::create("mainscreen_playe_mask.png");
    m_playerProfile->setPosition(Vec2(m_playerProfile->getContentSize().width * 0.65, m_playerProfile->getContentSize().height * 0.65));
    playerProfileBg->addChild(m_playerProfile);
    
}

void MainScene::updateHighScoreAfterGame(){
    
    UserDefault* userDef =  UserDefault::getInstance();
    int value = userDef->getIntegerForKey(GAME_HIGH_SCORE_KEY, 0);
    std::string highScore = "HIGH SCORE " + std::to_string(value);
    highScoreStr->setString(highScore);
}

void MainScene::createPlayButton(){
    
    Scale9Sprite* playBg = Scale9Sprite::create("mainscreen_playbtn.png");
    playBg->setContentSize(Size(visibleSize.width * 0.75, visibleSize.height * 0.15));
    playBg->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.4));
    this->addChild(playBg);
    
    Label* playLabel = Label::createWithTTF("PLAY NOW", FONT_HEADLINE, 60);
    playLabel->setColor(Color3B::BLACK);
    playLabel->setOpacity(180);
    MenuItemLabel* playButton = MenuItemLabel::create(playLabel, CC_CALLBACK_1(MainScene::playButtonCallback, this));
    playButton->setAnchorPoint(Vec2(0.5,0.5));
    playButton->setPosition(Vec2(playBg->getContentSize().width* 0.5, playBg->getContentSize().height * 0.55));
    auto playMenu = Menu::create(playButton, NULL);
    playMenu->setPosition(Vec2(0,0));
    playBg->addChild(playMenu);
    
}

void MainScene::playButtonCallback(Ref* pSender){

    CCLOG("Inside play button callback");
    auto gameScene = GameLayer::createScene();
    Director::getInstance()->replaceScene(gameScene);
    
}

void MainScene::createBackgroundWithAnimation(){
    
    backGround = Sprite::create("ingame_bg.jpg");
    backGround->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
    float scaleFactorW = visibleSize.width / backGround->getContentSize().width;
    float scaleFactorH = visibleSize.height/ backGround->getContentSize().height;
    backGround->setScale(scaleFactorW, scaleFactorH);
    this->addChild(backGround);
}

void MainScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
