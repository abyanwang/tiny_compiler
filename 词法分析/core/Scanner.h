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
        
	    inline vector<Token> getTokens() {
        	return list;
    	}
    	
    	static vector<Token> scan(ifstream& fin, Log& log);
    
	private:
    // temporary variables
    	string word;
    	Token::Type type = Token::Type::NONE;
    	int line_number = 1;
    	int char_number = -1;
    	bool isSign = false;
    
    
        void new_line();
    	void close_word();
    	void analyse();
        
};

#endif //TINY_COMPILE_TOKEN_SCAN_H
