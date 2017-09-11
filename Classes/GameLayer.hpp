//
//  GameLayer.hpp
//  WordGame
//

#ifndef GameLayer_hpp
#define GameLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "MessageStructs.h"

using namespace cocos2d::ui;
USING_NS_CC;


class GameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void onEnter();
    
    Size m_visibleSize;
    Label* m_scoreStr;
    LayerColor* m_background;
    Label* m_timerLabel;
    int m_timerCount;
    int m_curQuestion;
    Scale9Sprite* m_popupBg;
    Label* m_messageLabel;
    std::string m_questionString;
    
    std::vector<Sprite*> m_questionSpriteVector;
    std::vector<Sprite*> m_answerSpriteVector;
    std::vector<WordStruct *> m_questions;
    std::vector<QuestionBlock* > m_questionsNew;
    std::string m_answerString;
    
    
    void createBackground();
    void createAlphabetsBlocks();
    Vec2 getPositionAccordingToIndex(int index);
    void createAnswerBlocks();
    void updateQuestionBlocks(WordStruct* wordStruct);
    void updateAnswerBlock(std::string ansStr);
    void removeLetterFromAnswer(char letter);
    void resetAnswerBlock();
    void fillQuestionsVector();
    void createTimer();
    void prepareNextQuestion();
    void handleGameOver();
    bool checkAnswer();
    void updateOnCorrectAnswer();
    void correctAnswerAnimation();
    void wrongAnswerAnimation();
    void showPopUp(std::string message);
    void createPopUp();
    void okButtonCallback(Ref* pSender);
    
    void reduceTimer(float dt);
    
    
    void questionAlphabetCallback(Ref* pSender);
    void answerAlphabetCallback(Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
};



#endif /* GameLayer_hpp */
