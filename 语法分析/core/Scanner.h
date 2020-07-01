#ifndef TINY_COMPILE_TOKEN_SCAN_H
#define TINY_COMPILE_TOKEN_SCAN_H

#include <fstream>
#include <vector>
#include "Token.h"
#include "Log.h"
using namespace std;

class Scanner {
	public:
		ifstream& in;
        vector<Token> list;
        Log& log;
        
        Scanner(ifstream& fin, Log& log) : in(fin), log(log) {
        	analyse();
    	}
        
	    vector<Token> getTokens() {
        	return list;
    	}
    	
    	static vector<Token> scan(ifstream& fin, Log& log);
    
	private:
    // temporary variables
    	string word;
    	TokenType type = TokenType::NONE;
    	int lineNumber = 1;
    	int charNumber = -1;
    	bool isSign = false;
    
    
        void newLine();
    	void figureWord();
    	void analyse();
        
};

#endif //TINY_COMPILE_TOKEN_SCAN_H
