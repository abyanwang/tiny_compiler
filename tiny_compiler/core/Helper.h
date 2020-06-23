#ifndef TINY_COMPILE_TOKEN_HELPER_H
#define TINY_COMPILE_TOKEN_HELPER_H

#include <string>
#include "Token.h"
#include "TreeNode.h"

namespace Helper {
    std::string getTokenTypeName(Token::Type type);
    Token::Type getTokenTypeByName(const std::string &token);

    inline bool isLetter(char ch) {
        if (ch >= 'A' && ch <= 'Z')
            return true;
        return ch >= 'a' && ch <= 'z';
    }

    inline bool isDigit(char ch) {
        return ch >= '0' && ch <= '9';
    }

    inline bool isValidSign(char ch) {
        static const std::string signs = "><=,;:+-*/()";
        return std::string::npos != signs.find(ch);
    }

    inline bool isSeparator(char ch) {
        static const std::string signs = " \t\r\n";
        return std::string::npos != signs.find(ch);
    }
}

#endif //TINY_COMPILE_TOKEN_HELPER_H
