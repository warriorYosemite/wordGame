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
    
    m_gridCount.push_back("4");
    m_gridCount.push_back("5");
    m_gridCount.push_back("6");
    m_gridCount.push_back("7");
}

void MainScene::createGridElements() {
    
    Sprite* temp = Sprite::create("mainscreen_green.png");
    
    Scale9Sprite* gridBg = Scale9Sprite::create("mainscreen_gridbg.png");
    gridBg->setContentSize(Size(temp->getContentSize().width * 5, temp->getContentSize().height * 1.05));
    gridBg->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.7));
    this->addChild(gridBg);

    Label* gridText = Label::createWithTTF("SELECT GRID", FONT_HEADLINE, 30);
    gridText->setPosition(Vec2(gridBg->getContentSize().width * 0.5, gridBg->getContentSize().height * 1.5));
    gridBg->addChild(gridText);
    
    for (int i=0; i < MAX_ALPHABET_COUNT; i++){
        std::string spriteName = m_spriteName.at(i);
        Sprite* alphabetBg = Sprite::create(spriteName);
        alphabetBg->setTag(TAG_SPRITE_BG);
        MenuItemSprite* alphabetE = MenuItemSprite::create(alphabetBg, alphabetBg, CC_CALLBACK_1(MainScene::selectGridCallback, this));
        alphabetE->setPosition(getXPositionAccordingToIndex(i), gridBg->getContentSize().height * 0.4);
        alphabetE->setTag(i);
        Menu* menu = Menu::create(alphabetE, NULL);
        menu->setPosition(Vec2(0,0));
        gridBg->addChild(menu);
        
        Label* alphabetLabel = Label::createWithTTF(m_gridCount.at(i), FONT_HEADLINE, 70);
        alphabetLabel->setPosition(Vec2(alphabetBg->getContentSize().width * 0.5, alphabetBg->getContentSize().height * 0.6));
        alphabetLabel->setColor(Color3B::BLACK);
        alphabetLabel->setOpacity(180);
        alphabetLabel->setTag(TAG_ALPHABET);
        alphabetBg->addChild(alphabetLabel);
        
        Sprite* checkIcon = Sprite::create("mainscreen_check.png");
        checkIcon->setPosition(Vec2(alphabetBg->getContentSize().width - checkIcon->getContentSize().width * 0.15, alphabetBg->getContentSize().height - checkIcon->getContentSize().height * 0.25));
        checkIcon->setTag(CHECK_ICON_TAG);
        checkIcon->setVisible(false);
        alphabetBg->addChild(checkIcon);
        
        if (i == 0) {
            checkIcon->setVisible(true);
        }
        
        m_gridElemVector.push_back(alphabetBg);
    }
}

float MainScene::getXPositionAccordingToIndex(int index) {
    Sprite* temp = Sprite::create("mainscreen_green.png");
    float baseX = temp->getContentSize().width * 0.765;
    float xPos = baseX + ((temp->getContentSize().width * 1.2) * index);
    return xPos;
}

void MainScene::resetCheckOnGrid() {

    for (int i=0; i < m_gridElemVector.size(); i++) {
        Sprite* image = m_gridElemVector.at(i);
        image->getChildByTag(CHECK_ICON_TAG)->setVisible(false);
    }
}

void MainScene::selectGridCallback(cocos2d::Ref* pSender) {

    CCLOG("inside select grid callback");
    resetCheckOnGrid();
    
    MenuItemSprite* gridElem = (MenuItemSprite*)pSender;
    Sprite* gridSprite = (Sprite*)gridElem->getChildByTag(TAG_SPRITE_BG);
    gridSprite->getChildByTag(CHECK_ICON_TAG)->setVisible(true);
    
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
