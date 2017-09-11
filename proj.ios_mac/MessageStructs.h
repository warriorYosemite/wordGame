//
//  MessageStructs.h
//  WordGame
//
//

#ifndef MessageStructs_h
#define MessageStructs_h

struct AlphabetDetail {
    char m_alphabet;
    bool m_isSelected;
    
    AlphabetDetail() {
        m_isSelected = false;
    }
};

struct QuestionBlock {
    
    std::vector<AlphabetDetail* > m_question;
    std::vector<std::string> m_answer;                  //vector for multiple correct answer
    
    int m_points;
    int m_level;
    int m_category;             //we will change this in future
    
    QuestionBlock() {
        m_points = 0;
        m_level = 0;
        m_category = 0;
    }
};

struct WordStruct {
    std::string m_question;
    std::string m_answer;
    bool m_isSelected;
    
    WordStruct() {
        m_question = "";
        m_answer = "";
        m_isSelected = false;
    }
};


#endif /* MessageStructs_h */
