#ifndef TINY_COMPILE_TOKEN_HELPER_H
#define TINY_COMPILE_TOKEN_HELPER_H

#include <string>
#include "Token.h"
#include "TreeNode.h"
#include <ctype.h> 
using namespace std;

namespace Helper {
    string getTokenTypeName(Token::Type type);
    Token::Type getTokenTypeByName(const std::string &token);

    inline bool isLetter(char ch) {
    	return isalpha(ch);
    }

    inline bool isDigit(char ch) {
    	return isdigit(ch);
//        return ch >= '0' && ch <= '9';
    }

    inline bool isValidSign(char ch) {
        static const string signs = "><=,;:+-*/()";
        return string::npos != signs.find(ch);
    }

    inline bool isSeparator(char ch) {
        static const string signs = " \t\r\n";
        return string::npos != signs.find(ch);
    }
}

#endif //TINY_COMPILE_TOKEN_HELPER_H
