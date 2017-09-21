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
    
private:
    int m_wordSize;
    bool m_isPause;
    int m_pauseTime;
    ResultState m_answerState;
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void onEnter();
    
    Size m_visibleSize;
    Label* m_scoreStr;
    Sprite* m_background;
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
    std::vector<std::string> m_spriteName;
    std::string m_answerString;
    
    int m_bestScore;
    int m_currentScore;
    Label* m_currentScoreLabel;
    Label* m_bestScoreLabel;
    
    
    void createBackground();
    void createScoreLabel();
    void createAlphabetsBlocks();
    Vec2 getPositionAccordingToIndex(int index);
    void createAnswerBlocks();
    void updateQuestionBlocks(WordStruct* wordStruct);
    void updateAnswerBlock(std::string ansStr);
    void removeLetterFromAnswer(char letter);
    void resetAnswerBlock();
    void resetSelectedQuestionAlphabet();
    void fillQuestionsVector();
    void fillSpriteNameVector();
    void createTimer();
    void updateTimeOnCorrectAnswer();
    void prepareNextQuestion();
    void handleGameOver();
    bool checkAnswer();
    void checkAnswerAndAnimate();
    void updateOnCorrectAnswer();
    void correctAnswerAnimation();
    void wrongAnswerAnimation();
    void showPopUp(std::string message);
    void createPopUp();
    int getGameWordSize();
    void setGameWordSize(int size);
    void reduceTimer(float dt);
    void pauseOnAnimation(float dt);
    void proceedGameAccordingToGameState();
    
    
    void questionAlphabetCallback(Ref* pSender);
    void answerAlphabetCallback(Ref* pSender);
    void okButtonCallback(Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
};



#endif /* GameLayer_hpp */
