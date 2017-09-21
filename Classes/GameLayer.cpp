//
//  GameLayer.cpp
//  WordGame
//


#include "GameLayer.hpp"
#include "extensions/cocos-ext.h"
#include "constant.h"
#include "MainScene.hpp"

USING_NS_CC;

#define MAX_TIMER_COUNT                         15
#define TIME_DELTA                              6

Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    m_visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    m_wordSize = 4; //default
    m_isPause = false;
    m_pauseTime = 1;
    m_answerState = NO_RESULT;
    return true;
}

void GameLayer::onEnter()
{
    
    CCLOG("In on enter main Scene");

    Node::onEnter();
    m_timerCount = MAX_TIMER_COUNT;
    m_curQuestion = 0;
    m_answerString = "";
    m_questionString = "";
    m_popupBg = nullptr;
    m_currentScore = 10;
    m_bestScore = 20;
    
    
    fillSpriteNameVector();
    fillQuestionsVector();
    
    createBackground();
    
    createScoreLabel();
    createAlphabetsBlocks();
    createAnswerBlocks();
    createTimer();
    prepareNextQuestion();
    createPopUp();
    this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::reduceTimer), 1);

}

void GameLayer::updateQuestionBlocks(WordStruct* wordStruct) {

    std::string wordStr = wordStruct->m_question;
    
    for (int i=0; i < wordStr.size(); i++) {
        
        char letter = wordStr.at(i);
        Sprite* letterSprite = m_questionSpriteVector.at(i);
        
        Label* letterLabel = (Label*)letterSprite->getChildByTag(TAG_ALPHABET);
        if (letterLabel != nullptr) {
            std::string cToS(1, letter);
            letterLabel->setString(cToS);
        }
    }
}

void GameLayer::createAlphabetsBlocks(){
    
    for (int i=0; i < MAX_ALPHABET_COUNT; i++){
        std::string spriteName = m_spriteName.at(i);
        Sprite* alphabetBg = Sprite::create(spriteName);
        alphabetBg->setTag(TAG_SPRITE_BG);
        MenuItemSprite* alphabetE = MenuItemSprite::create(alphabetBg, alphabetBg, CC_CALLBACK_1(GameLayer::questionAlphabetCallback, this));
        alphabetE->setPosition(getPositionAccordingToIndex(i));
        alphabetE->setTag(i);
        Menu* menu = Menu::create(alphabetE, NULL);
        menu->setPosition(Vec2(0,0));
        this->addChild(menu);
        
        Label* alphabetLabel = Label::createWithTTF("", FONT_HEADLINE, 70);
        alphabetLabel->setPosition(Vec2(alphabetBg->getContentSize().width * 0.5, alphabetBg->getContentSize().height * 0.6));
        alphabetLabel->setColor(Color3B::BLACK);
        alphabetLabel->setOpacity(180);
        alphabetLabel->setTag(TAG_ALPHABET);
        alphabetBg->addChild(alphabetLabel);
        m_questionSpriteVector.push_back(alphabetBg);
    }
}

void GameLayer::updateTimeOnCorrectAnswer() {
    int timeDelta = TIME_DELTA;
    m_timerCount = m_timerCount + timeDelta;
}

void GameLayer::createTimer() {
    m_timerLabel = Label::createWithTTF(std::to_string(m_timerCount),FONT_HEADLINE,75);
    m_timerLabel->setPosition(Vec2(m_visibleSize.width * 0.5, m_visibleSize.height * 0.6));
    m_timerLabel->setColor(Color3B::WHITE);
    this->addChild(m_timerLabel);
}

void GameLayer::prepareNextQuestion() {
    resetAnswerBlock();
    m_questionString.clear();
    m_answerString.clear();
    m_answerState = NO_RESULT;
    
    QuestionBlock* curQuestion = m_questionsNew.at(m_curQuestion);
    
    for (int i=0; i < curQuestion->m_question.size(); i++) {
        
        char letter = curQuestion->m_question.at(i)->m_alphabet;
        Sprite* letterSprite = m_questionSpriteVector.at(i);
        Label* letterLabel = (Label*)letterSprite->getChildByTag(TAG_ALPHABET);
        if (letterLabel != nullptr) {
            std::string cToS(1, letter);
            letterLabel->setString(cToS);
            m_questionString = m_questionString + cToS;
        }
    }
}
void GameLayer::checkAnswerAndAnimate(){
    
    if (m_answerString.size() == getGameWordSize()) {
        bool isCorrect = checkAnswer();
        if (isCorrect){
            m_answerState = CORRECT_ANSWER;
            correctAnswerAnimation();
        }else{
            m_answerState = WRONG_ANSWER;
            wrongAnswerAnimation();
        }
        pauseOnAnimation(1);
    }
}

bool GameLayer::checkAnswer() {
    
    bool isAnswerCorrect = false;
    
    QuestionBlock* currQuestionStruct = m_questionsNew.at(m_curQuestion);
    std::string rightAnswer = currQuestionStruct->m_answer.at(0); //TODO::checking one answer only for now
    if (m_answerString.compare(rightAnswer) == 0) {
        CCLOG("correct answer");
        isAnswerCorrect = true;
    }else {
        CCLOG("wrong answer");
        isAnswerCorrect = false;
    }
    return isAnswerCorrect;
}

void GameLayer::answerAlphabetCallback(Ref* pSender) {
    
    if (!m_isPause){
        CCLOG("inside answer alphabet callback");
        MenuItemSprite* answerMenu = (MenuItemSprite*)pSender;
        Sprite* answerSprite = (Sprite*)answerMenu->getChildByTag(TAG_SPRITE_BG);
        Label* alphaLabel = (Label*)answerSprite->getChildByTag(TAG_ALPHABET);
        std::string ansLetter = alphaLabel->getString();
        
        QuestionBlock* curQuestion = m_questionsNew.at(m_curQuestion);
        for (int i=0; i < curQuestion->m_question.size(); i++){
            char letter = curQuestion->m_question.at(i)->m_alphabet;
            std::string cToS(1, letter);
            if (cToS == ansLetter){
                bool isSelected = curQuestion->m_question.at(i)->m_isSelected;
                if (isSelected) {
                    removeLetterFromAnswer(letter);
                    curQuestion->m_question.at(i)->m_isSelected = false;
                    updateAnswerBlock(m_answerString);
                }
            }
        }
    }
}

void GameLayer::questionAlphabetCallback(Ref* pSender) {

    if (!m_isPause){
        CCLOG("Inside question alphabet callback");
        MenuItemSprite* questionMenu = (MenuItemSprite*)pSender;
        int tag = questionMenu->getTag();
        if (!m_questionString.empty()){
            char selectedChar = m_questionString[tag];
            QuestionBlock* curQuestion = m_questionsNew.at(m_curQuestion);
            for (int i=0; i < curQuestion->m_question.size(); i++){
                char letter = curQuestion->m_question.at(i)->m_alphabet;
                if (letter == selectedChar){
                    std::string cToS(1, letter);
                    bool isSelected = curQuestion->m_question.at(i)->m_isSelected;
                    if (!isSelected) {
                        m_answerString = m_answerString + cToS;
                        updateAnswerBlock(m_answerString);
                        curQuestion->m_question.at(i)->m_isSelected = true;
                    }else{
                        removeLetterFromAnswer(letter);
                        curQuestion->m_question.at(i)->m_isSelected = false;
                        updateAnswerBlock(m_answerString);
                    }
                }
            }
        }
        checkAnswerAndAnimate();
    }
}

void GameLayer::pauseOnAnimation(float dt) {
    m_isPause = true;
    m_pauseTime = dt;
}

int GameLayer::getGameWordSize() {
    return m_wordSize;
}

void GameLayer::setGameWordSize(int size) {
    m_wordSize = size;
}

void GameLayer::removeLetterFromAnswer(char letter) {
    
    for (int i = 0; i < m_answerString.size(); i++){
        
        char temp = m_answerString[i];
        if (temp == letter) {
            m_answerString.erase(m_answerString.begin() + i);
            break;
        }
    }
}

void GameLayer::updateAnswerBlock(std::string ansStr) {
    
    resetAnswerBlock();
    for (int i=0; i < m_answerSpriteVector.size(); i++) {
       
        Sprite* letterSprite = m_answerSpriteVector.at(i);
        
        Label* letterLabel = (Label*)letterSprite->getChildByTag(TAG_ALPHABET);
        if (letterLabel != nullptr) {
            if (i < ansStr.size()){
                char letter = ansStr[i];
                std::string cToS(1, letter);
                std::string str = cToS;
                letterLabel->setString(str);
            }
        }
    }
}
void GameLayer::resetSelectedQuestionAlphabet() {

    QuestionBlock* curQuestion = m_questionsNew.at(m_curQuestion);
    for (int i=0; i < curQuestion->m_question.size(); i++){
        curQuestion->m_question.at(i)->m_isSelected = false;
    }
}

void GameLayer::resetAnswerBlock() {
    for (int i=0; i < m_answerSpriteVector.size(); i++) {
        Sprite* letterSprite = m_answerSpriteVector.at(i);
        Label* letterLabel = (Label*)letterSprite->getChildByTag(TAG_ALPHABET);
        if (letterLabel != nullptr) {
            letterLabel->setString("");
            letterLabel->setColor(Color3B::WHITE);
        }
    }
}

void GameLayer::handleGameOver() {
    CCLOG("inside game over");
    this->unschedule(CC_SCHEDULE_SELECTOR(GameLayer::reduceTimer));
    showPopUp("Your Game is Over, \n wanna try again ?");
}

void GameLayer::createPopUp(){

    LayerColor* bgLayer = LayerColor::create(Color4B(0,0,0,180), m_visibleSize.width, m_visibleSize.height);
    bgLayer->setPosition(Vec2(0,0));
    
    
    m_popupBg = Scale9Sprite::create("blackbg.png");
    m_popupBg->setContentSize(Size(m_visibleSize.width * 0.75, m_visibleSize.height * 0.4));
    m_popupBg->setPosition(Vec2(m_visibleSize.width * 0.5, m_visibleSize.height * 0.55));
    m_popupBg->setVisible(false);
    this->addChild(m_popupBg);
    
    m_messageLabel = Label::createWithTTF("", FONT_HEADLINE, 40);
    m_messageLabel->setPosition(Vec2(m_popupBg->getContentSize().width * 0.5,
                                     m_popupBg->getContentSize().height * 0.5));
    m_messageLabel->setContentSize(Size(m_popupBg->getContentSize().width * 0.8,
                                        m_popupBg->getContentSize().height * 0.75));
    m_messageLabel->setAlignment(cocos2d::TextHAlignment::CENTER);
    m_popupBg->addChild(m_messageLabel);
    
    Label* okLabel = Label::createWithTTF("OK", FONT_HEADLINE, 40);
    MenuItemLabel* okStr = MenuItemLabel::create(okLabel, CC_CALLBACK_1(GameLayer::okButtonCallback, this));
    okStr->setAnchorPoint(Vec2(0.5,0.5));
    auto okMenu = Menu::create(okStr, NULL);
    okMenu->setPosition(Vec2(m_popupBg->getContentSize().width * 0.5, m_popupBg->getContentSize().height * 0.25));
    m_popupBg->addChild(okMenu);
}

void GameLayer::okButtonCallback(Ref* pSender){
    
    CCLOG("OK button pressed");
    unscheduleAllCallbacks();
    
    auto mainScene = MainScene::createScene();
    Director::getInstance()->replaceScene(mainScene);
}

void GameLayer::showPopUp(std::string message) {
    m_messageLabel->setString(message);
    m_popupBg->setVisible(true);
}

void GameLayer::updateOnCorrectAnswer() {
    
    updateTimeOnCorrectAnswer();
    m_curQuestion++;
    if (m_curQuestion > m_questionsNew.size() - 1){
        handleGameOver();
    }else{
        prepareNextQuestion();
        this->schedule(CC_SCHEDULE_SELECTOR(GameLayer::reduceTimer), 1);
    }
    m_timerLabel->setString(std::to_string(m_timerCount));
}

void GameLayer::correctAnswerAnimation() {
    
    for (int i=0; i < m_answerSpriteVector.size(); i++) {
        Sprite* letterSprite = m_answerSpriteVector.at(i);
        Label* letterLabel = (Label*)letterSprite->getChildByTag(TAG_ALPHABET);
        if (letterLabel != nullptr) {
            letterLabel->setColor(Color3B::GREEN);
        }
    }
}

void GameLayer::wrongAnswerAnimation() {

    for (int i=0; i < m_answerSpriteVector.size(); i++) {
        Sprite* letterSprite = m_answerSpriteVector.at(i);
        Label* letterLabel = (Label*)letterSprite->getChildByTag(TAG_ALPHABET);
        if (letterLabel != nullptr) {
            letterLabel->setColor(Color3B::RED);
        }
    }
}

void GameLayer::proceedGameAccordingToGameState() {

    if (m_answerState == NO_RESULT) {
        
    }else if (m_answerState == CORRECT_ANSWER) {
        updateOnCorrectAnswer();
        m_answerState = NO_RESULT;
    }else if (m_answerState == WRONG_ANSWER) {
        resetAnswerBlock();
        m_answerString.clear();
        resetSelectedQuestionAlphabet();
        m_answerState = NO_RESULT;
    }
}

void GameLayer::reduceTimer(float dt) {
    
    if (!m_isPause){
        if (m_timerCount == 0){
            handleGameOver();
        }else{
            m_timerCount--;
            proceedGameAccordingToGameState();
        }
        m_timerLabel->setString(std::to_string(m_timerCount));
    }else{
        if (m_pauseTime != 0){
            m_pauseTime--;
        }else{
            m_isPause = false;
            m_pauseTime = 1;
        }
    }
}


void GameLayer::createAnswerBlocks() {

    float xPos = m_visibleSize.width * 0.2;
    float yPos = m_visibleSize.height * 0.75;
    
    for (int i=0; i < MAX_ALPHABET_COUNT; i++){
        Sprite* alphabetBg = Sprite::create("ingame_emptybox.png");
        std::string spriteName = m_spriteName.at(i);
        Sprite* colouredBg = Sprite::create(spriteName);
        alphabetBg->setTag(TAG_SPRITE_BG);
        MenuItemSprite* alphabetE = MenuItemSprite::create(alphabetBg, alphabetBg, CC_CALLBACK_1(GameLayer::answerAlphabetCallback, this));
        alphabetE->setPosition(Vec2(xPos +  (i * alphabetBg->getContentSize().width * 1.25) , yPos));
        Menu* menu = Menu::create(alphabetE, NULL);
        menu->setPosition(Vec2(0,0));
        this->addChild(menu);
        
        Label* alphabetLabel = Label::createWithTTF("", FONT_HEADLINE, 50);
        alphabetLabel->setPosition(Vec2(alphabetBg->getContentSize().width * 0.5, alphabetBg->getContentSize().height * 0.5));
        alphabetLabel->setTag(TAG_ALPHABET);
        alphabetBg->addChild(alphabetLabel);
        m_answerSpriteVector.push_back(alphabetBg);
    }
}

Vec2 GameLayer::getPositionAccordingToIndex(int index) {
    //TODO::For now we are considering the case for 4 words only
    float xPos;
    float yPos;
    float basePosX = m_visibleSize.width * 0.5;
    float basePosY = m_visibleSize.height * 0.3;
    
    Sprite* alphabetBg = Sprite::create("blackbg.png");
    float width = alphabetBg->getContentSize().width;
    
    if (index % 2 == 0) {
        xPos = basePosX - width * 0.7;
    }else{
        xPos = basePosX + width * 0.7;
    }
    
    if (index < 2) {
        yPos = basePosY + width * 0.7;
    }else{
        yPos = basePosY - width * 0.7;
    }
    
    return Vec2(xPos, yPos);
}

void GameLayer::createScoreLabel() {
    
    Label* currentText = Label::createWithTTF("CURRENT SCORE", FONT_HEADLINE, 18);
    currentText->setPosition(Vec2(m_visibleSize.width * 0.5, m_visibleSize.height * 0.975));
    this->addChild(currentText);
    
    m_currentScoreLabel = Label::createWithTTF(std::to_string(m_currentScore), FONT_HEADLINE, 40);
    m_currentScoreLabel->setPosition(Vec2(m_visibleSize.width * 0.5, m_visibleSize.height * 0.93));
    this->addChild(m_currentScoreLabel);

    
    Label* bestScoreText = Label::createWithTTF("BEST SCORE", FONT_HEADLINE, 18);
    bestScoreText->setAnchorPoint(Vec2(1,0.5));
    bestScoreText->setPosition(Vec2(m_visibleSize.width * 0.975, m_visibleSize.height * 0.975));
    this->addChild(bestScoreText);
    
    m_bestScoreLabel = Label::createWithTTF(std::to_string(m_bestScore), FONT_HEADLINE, 40);
    m_bestScoreLabel->setAnchorPoint(Vec2(1,0.5));
    m_bestScoreLabel->setPosition(Vec2(m_visibleSize.width * 0.975, m_visibleSize.height * 0.93));
    this->addChild(m_bestScoreLabel);
    
}

void GameLayer::createBackground() {

    m_background = Sprite::create("ingame_bg.jpg");
    m_background->setPosition(Vec2(m_visibleSize.width * 0.5, m_visibleSize.height * 0.5));
    
    float scaleFactorW = m_visibleSize.width / m_background->getContentSize().width;
    float scaleFactorH = m_visibleSize.height/ m_background->getContentSize().height;
    
    m_background->setScale(scaleFactorW, scaleFactorH);
    this->addChild(m_background);
}

void GameLayer::fillSpriteNameVector() {

    m_spriteName.push_back("mainscreen_blue.png");
    m_spriteName.push_back("mainscreen_cyan.png");
    m_spriteName.push_back("mainscreen_green.png");
    m_spriteName.push_back("mainscreen_orange.png");
}

void GameLayer::fillQuestionsVector() {
    
    QuestionBlock* question1 = new QuestionBlock();
    
    AlphabetDetail* a1 = new AlphabetDetail();
    a1->m_alphabet = 'E';
    question1->m_question.push_back(a1);
    
    AlphabetDetail* a2 = new AlphabetDetail();
    a2->m_alphabet = 'O';
    question1->m_question.push_back(a2);
    
    AlphabetDetail* a3 = new AlphabetDetail();
    a3->m_alphabet = 'L';
    question1->m_question.push_back(a3);
    
    AlphabetDetail* a4 = new AlphabetDetail();
    a4->m_alphabet = 'V';
    question1->m_question.push_back(a4);
    
    question1->m_answer.push_back("LOVE");
    
    m_questionsNew.push_back(question1);
    
    
    QuestionBlock* question2 = new QuestionBlock();
    
    AlphabetDetail* b1 = new AlphabetDetail();
    b1->m_alphabet = 'R';
    question2->m_question.push_back(b1);
    
    AlphabetDetail* b2 = new AlphabetDetail();
    b2->m_alphabet = 'O';
    question2->m_question.push_back(b2);
    
    AlphabetDetail* b3 = new AlphabetDetail();
    b3->m_alphabet = 'W';
    question2->m_question.push_back(b3);
    
    AlphabetDetail* b4 = new AlphabetDetail();
    b4->m_alphabet = 'D';
    question2->m_question.push_back(b4);
    
    question2->m_answer.push_back("WORD");
    
    m_questionsNew.push_back(question2);
    
    
    QuestionBlock* question3 = new QuestionBlock();
    AlphabetDetail* c1 = new AlphabetDetail();
    c1->m_alphabet = 'K';
    question3->m_question.push_back(c1);
    
    AlphabetDetail* c2 = new AlphabetDetail();
    c2->m_alphabet = 'I';
    question3->m_question.push_back(c2);
    
    AlphabetDetail* c3 = new AlphabetDetail();
    c3->m_alphabet = 'N';
    question3->m_question.push_back(c3);
    
    AlphabetDetail* c4 = new AlphabetDetail();
    c4->m_alphabet = 'W';
    question3->m_question.push_back(c4);
    
    question3->m_answer.push_back("WINK");
    m_questionsNew.push_back(question3);
    
    
    QuestionBlock* question4 = new QuestionBlock();
    AlphabetDetail* d1 = new AlphabetDetail();
    d1->m_alphabet = 'E';
    question4->m_question.push_back(d1);
    
    AlphabetDetail* d2 = new AlphabetDetail();
    d2->m_alphabet = 'P';
    question4->m_question.push_back(d2);
    
    AlphabetDetail* d3 = new AlphabetDetail();
    d3->m_alphabet = 'R';
    question4->m_question.push_back(d3);
    
    AlphabetDetail* d4 = new AlphabetDetail();
    d4->m_alphabet = 'U';
    question4->m_question.push_back(d4);
    
    question4->m_answer.push_back("PURE");
    m_questionsNew.push_back(question4);
    
    
    QuestionBlock* question5 = new QuestionBlock();
    AlphabetDetail* e1 = new AlphabetDetail();
    e1->m_alphabet = 'E';
    question5->m_question.push_back(e1);
    
    AlphabetDetail* e2 = new AlphabetDetail();
    e2->m_alphabet = 'A';
    question5->m_question.push_back(e2);
    
    AlphabetDetail* e3 = new AlphabetDetail();
    e3->m_alphabet = 'B';
    question5->m_question.push_back(e3);
    
    AlphabetDetail* e4 = new AlphabetDetail();
    e4->m_alphabet = 'L';
    question5->m_question.push_back(e4);
    
    question5->m_answer.push_back("ABLE");
    m_questionsNew.push_back(question5);
    
    
//    QuestionBlock* question3 = new QuestionBlock();
//    AlphabetDetail* c1 = new AlphabetDetail();
//    c1->m_alphabet = 'K';
//    question3->m_question.push_back(c1);
//    
//    AlphabetDetail* c2 = new AlphabetDetail();
//    c2->m_alphabet = 'I';
//    question3->m_question.push_back(c2);
//    
//    AlphabetDetail* c3 = new AlphabetDetail();
//    c3->m_alphabet = 'N';
//    question3->m_question.push_back(c3);
//    
//    AlphabetDetail* c4 = new AlphabetDetail();
//    c4->m_alphabet = 'W';
//    question3->m_question.push_back(c4);
//    
//    question3->m_answer.push_back("WINK");
//    m_questionsNew.push_back(question3);
//    
//    
//    QuestionBlock* question3 = new QuestionBlock();
//    AlphabetDetail* c1 = new AlphabetDetail();
//    c1->m_alphabet = 'K';
//    question3->m_question.push_back(c1);
//    
//    AlphabetDetail* c2 = new AlphabetDetail();
//    c2->m_alphabet = 'I';
//    question3->m_question.push_back(c2);
//    
//    AlphabetDetail* c3 = new AlphabetDetail();
//    c3->m_alphabet = 'N';
//    question3->m_question.push_back(c3);
//    
//    AlphabetDetail* c4 = new AlphabetDetail();
//    c4->m_alphabet = 'W';
//    question3->m_question.push_back(c4);
//    
//    question3->m_answer.push_back("WINK");
//    m_questionsNew.push_back(question3);
//    
//    QuestionBlock* question3 = new QuestionBlock();
//    AlphabetDetail* c1 = new AlphabetDetail();
//    c1->m_alphabet = 'K';
//    question3->m_question.push_back(c1);
//    
//    AlphabetDetail* c2 = new AlphabetDetail();
//    c2->m_alphabet = 'I';
//    question3->m_question.push_back(c2);
//    
//    AlphabetDetail* c3 = new AlphabetDetail();
//    c3->m_alphabet = 'N';
//    question3->m_question.push_back(c3);
//    
//    AlphabetDetail* c4 = new AlphabetDetail();
//    c4->m_alphabet = 'W';
//    question3->m_question.push_back(c4);
//    
//    question3->m_answer.push_back("WINK");
//    m_questionsNew.push_back(question3);
//    
//    QuestionBlock* question3 = new QuestionBlock();
//    AlphabetDetail* c1 = new AlphabetDetail();
//    c1->m_alphabet = 'K';
//    question3->m_question.push_back(c1);
//    
//    AlphabetDetail* c2 = new AlphabetDetail();
//    c2->m_alphabet = 'I';
//    question3->m_question.push_back(c2);
//    
//    AlphabetDetail* c3 = new AlphabetDetail();
//    c3->m_alphabet = 'N';
//    question3->m_question.push_back(c3);
//    
//    AlphabetDetail* c4 = new AlphabetDetail();
//    c4->m_alphabet = 'W';
//    question3->m_question.push_back(c4);
//    
//    question3->m_answer.push_back("WINK");
//    m_questionsNew.push_back(question3);
//    
//    QuestionBlock* question3 = new QuestionBlock();
//    AlphabetDetail* c1 = new AlphabetDetail();
//    c1->m_alphabet = 'K';
//    question3->m_question.push_back(c1);
//    
//    AlphabetDetail* c2 = new AlphabetDetail();
//    c2->m_alphabet = 'I';
//    question3->m_question.push_back(c2);
//    
//    AlphabetDetail* c3 = new AlphabetDetail();
//    c3->m_alphabet = 'N';
//    question3->m_question.push_back(c3);
//    
//    AlphabetDetail* c4 = new AlphabetDetail();
//    c4->m_alphabet = 'W';
//    question3->m_question.push_back(c4);
//    
//    question3->m_answer.push_back("WINK");
//    m_questionsNew.push_back(question3);
//    
//    
//    WordStruct* word4 = new WordStruct();
//    word4->m_question = "EPRU";
//    word4->m_answer = "PURE";
//    m_questions.push_back(word4);
//    
//    WordStruct* word5 = new WordStruct();
//    word5->m_question = "TNAE";
//    word5->m_answer = "NEAT";
//    m_questions.push_back(word5);
//    
//    WordStruct* word6 = new WordStruct();
//    word6->m_question = "CEIN";
//    word6->m_answer = "NICE";
//    m_questions.push_back(word6);
//    
//    WordStruct* word7 = new WordStruct();
//    word7->m_question = "EABL";
//    word7->m_answer = "ABLE";
//    m_questions.push_back(word7);
//    
//    WordStruct* word8 = new WordStruct();
//    word8->m_question = "OOFD";
//    word8->m_answer = "FOOD";
//    m_questions.push_back(word8);
//    
//    WordStruct* word9 = new WordStruct();
//    word9->m_question = "AIGN";
//    word9->m_answer = "GAIN";
//    m_questions.push_back(word9);
//    
//    WordStruct* word10 = new WordStruct();
//    word10->m_question = "EKAW";
//    word10->m_answer = "WAKE";
//    m_questions.push_back(word10);
//    
//    WordStruct* word11 = new WordStruct();
//    word11->m_question = "ECKD";
//    word11->m_answer = "DECK";
//    m_questions.push_back(word11);
//    
//    WordStruct* word12 = new WordStruct();
//    word12->m_question = "EEKW";
//    word12->m_answer = "WEEK";
//    m_questions.push_back(word12);
//    
//    WordStruct* word13 = new WordStruct();
//    word13->m_question = "NOGS";
//    word13->m_answer = "SONG";
//    m_questions.push_back(word13);
//    
//    WordStruct* word14 = new WordStruct();
//    word14->m_question = "ZUIQ";
//    word14->m_answer = "QUIZ";
//    m_questions.push_back(word14);
//    
//    WordStruct* word15 = new WordStruct();
//    word15->m_question = "LASM";
//    word15->m_answer = "SLAM";
//    m_questions.push_back(word15);
//    
//    WordStruct* word16 = new WordStruct();
//    word16->m_question = "MILS";
//    word16->m_answer = "SLIM";
//    m_questions.push_back(word16);
//    
//    WordStruct* word17 = new WordStruct();
//    word17->m_question = "DSIA";
//    word17->m_answer = "SAID";
//    m_questions.push_back(word17);
//    
//    WordStruct* word18 = new WordStruct();
//    word18->m_question = "EAPL";
//    word18->m_answer = "PALE";
//    m_questions.push_back(word18);
//    
//    WordStruct* word19 = new WordStruct();
//    word19->m_question = "DOLG";
//    word19->m_answer = "GOLD";
//    m_questions.push_back(word19);
//    
//    WordStruct* word20 = new WordStruct();
//    word20->m_question = "EBRE";
//    word20->m_answer = "BEER";
//    m_questions.push_back(word20);
//    
//    WordStruct* word21 = new WordStruct();
//    word21->m_question = "FULG";
//    word21->m_answer = "GULF";
//    m_questions.push_back(word21);
//    
//    WordStruct* word22 = new WordStruct();
//    word22->m_question = "RBEH";
//    word22->m_answer = "HERB";
//    m_questions.push_back(word22);
//    
//    WordStruct* word23 = new WordStruct();
//    word23->m_question = "EPOR";
//    word23->m_answer = "ROPE";
//    m_questions.push_back(word23);
//    
//    WordStruct* word24 = new WordStruct();
//    word24->m_question = "ROTN";
//    word24->m_answer = "TORN";
//    m_questions.push_back(word24);
//    
//    WordStruct* word25 = new WordStruct();
//    word25->m_question = "MZOO";
//    word25->m_answer = "ZOOM";
//    m_questions.push_back(word25);
    
}
