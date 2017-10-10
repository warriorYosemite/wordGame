//
//  Database.cpp
//  WordGame
//
//  Created by Vaibhav Gupta on 04/10/17.
//
//

#include "Database.hpp"
#include <iostream>
USING_NS_CC;

Database* Database::m_instance = NULL;

Database::Database() {
    database = NULL;
}

Database::~Database(){
    
}

Database* Database::getInstance()
{
    if (m_instance == NULL)
        m_instance = new Database();
    return m_instance;
}

sqlite3* Database::getDatabase()
{
    return Database::getInstance()->database;
}

bool Database::open() {
    char* errMsg = NULL;
    std::string sqlstr;
    int result;
    
    std::string dbPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    dbPath.append("wordbounce.sqlite");
    
    result = sqlite3_open(dbPath.c_str(), &Database::getInstance()->database);
    if (result != SQLITE_OK) {
        CCLOG("OPENING WRONG, %d, MSG:%s",result,errMsg);
        return false;
    }else {
        
        CCLOG("result %d",result);
    }
    return true;
}

bool Database::execute(CCString *aSql)
{
    char *errorMsg;
    
    if (sqlite3_exec(Database::getInstance()->database, aSql->getCString(), NULL, NULL, &errorMsg) != SQLITE_OK)
    {
        CCString *status = CCString::createWithFormat("Error executing SQL statement: %s", errorMsg);
        CCLOG("%s", status->getCString());
        return false;
    }
    
    return true;
}

vector<vector<string> > Database::query(char* query) {
    sqlite3_stmt *statement;
    vector<vector<string> > results;
    
    if(sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK) {
        int cols = sqlite3_column_count(statement);
        int result = 0;
        while(true)
        {
            result = sqlite3_step(statement);
            if(result == SQLITE_ROW) {
                vector<string> values;
                for(int col = 0; col < cols; col++){
                    values.push_back((char*)sqlite3_column_text(statement, col));
                }
                results.push_back(values);
            }
            else{
                break;
            }
        }
        sqlite3_finalize(statement);
    }
    string error = sqlite3_errmsg(database);
    if(error != "not an error") cout << query << " " << error << endl;
        return results;
}

void Database::close()
{
    sqlite3_close(Database::getInstance()->database);
}
