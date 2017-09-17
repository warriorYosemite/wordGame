//
//  MainScene.hpp
//  WordGame
//


#ifndef MainScene_hpp
#define MainScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "InfiniteParallaxNode.hpp"

using namespace cocos2d::ui;
USING_NS_CC;


class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void onEnter();
    
    Size visibleSize;
    Sprite* backGround;
    Sprite* backGround2;
    InfiniteParallaxNode *parallaxNode;
    Label* highScoreStr;
    std::vector<std::string> m_spriteName;
    Sprite* m_playerProfile;
    
    void createBackgroundWithAnimation();
    void createTextItems();
    void createPlayButton();
    void updateHighScoreAfterGame();
    void createPlayerProfileItem();
    void createGridElements();
    void fillSpriteNameVector();
    
    void updateBgPosition(float dt);
    
    void playButtonCallback(Ref* pSender);
    void playStageTwoCallback(Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif /* MainScene_hpp */
