#ifndef TINY_COMPILE_TOKEN_UTILS_H
#define TINY_COMPILE_TOKEN_UTILS_H

#include <string>
#include "Token.h"
#include <ctype.h> 

class Utils {
	public:
	    static string tokenToName(TokenType type);
	    static TokenType nameToToken(const string &token);
		
	    static bool isValidSign(char ch) {
	        static const string signs = "><=,;:+-*/()";
	        return string::npos != signs.find(ch);
	    }
	
	    static bool isSeparator(char ch) {
	        static const string signs = " \t\r\n";
	        return string::npos != signs.find(ch);
	    }
};

#endif //TINY_COMPILE_TOKEN_UTILS_H
