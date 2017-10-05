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
    
    Database(char* filename);
    ~Database();
    
    bool open(char* filename);
    vector<vector<string> > query(char* query);
    void close();
    
private:
    sqlite3 *database;
};

#endif /* Database_hpp */
