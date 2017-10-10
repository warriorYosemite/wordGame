//
//  Database.hpp
//  WordGame
//
//  Created by Vaibhav Gupta on 04/10/17.
//
//

#ifndef Database_hpp
#define Database_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;
class Database {
public:
    
    ~Database();
    vector<vector<string> > query(char* query);
    static void close();
    static sqlite3* getDatabase();
    
    static bool open();
    static bool create(std::string aTableName);
    static bool execute(cocos2d::CCString *aSql);
    
private:
    Database();
    static Database *m_instance;
    sqlite3 *database;
    static Database* getInstance();
};

#endif /* Database_hpp */
