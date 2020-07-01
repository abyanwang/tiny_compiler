#include "Utils.h"

string Utils::tokenToName(TokenType type) {
    switch (type) {
        //关键字
        case TokenType::KEY_TRUE:
            return "KEY_TRUE";
        case TokenType::KEY_FALSE:
            return "KEY_FALSE";
        case TokenType::KEY_OR:
            return "KEY_OR";
        case TokenType::KEY_AND:
            return "KEY_AND";
        case TokenType::KEY_NOT:
            return "KEY_NOT";
        case TokenType::KEY_INT:
            return "KEY_INT";
        case TokenType::KEY_BOOL:
            return "KEY_BOOL";
        case TokenType::KEY_STRING:
            return "KEY_STRING";
        case TokenType::KEY_DO:
            return "KEY_DO";
        case TokenType::KEY_WHILE:
            return "KEY_WHILE";
        case TokenType::KEY_REPEAT:
            return "KEY_REPEAT";
        case TokenType::KEY_UNTIL:
            return "KEY_UNTIL";
        case TokenType::KEY_IF:
            return "KEY_IF";
        case TokenType::KEY_THEN:
            return "KEY_THEN";
        case TokenType::KEY_ELSE:
            return "KEY_ELSE";
        case TokenType::KEY_END:
            return "KEY_END";
        case TokenType::KEY_READ:
            return "KEY_READ";
        case TokenType::KEY_WRITE:
            return "KEY_WRITE";
        
        //运算符
        case TokenType::OP_GTR:
            return "OP_GTR";
        case TokenType::OP_LSS:
            return "OP_LSS";
        case TokenType::OP_LEQ:
            return "OP_LEQ";
        case TokenType::OP_GEQ:
            return "OP_GEQ";
        case TokenType::OP_EQU:
            return "OP_EQU";
        case TokenType::OP_COMMA:
            return "OP_COMMA";
        case TokenType::OP_SEMICOLON:
            return "OP_SEMICOLON";
        case TokenType::OP_ASSIGN:
            return "OP_ASSIGN";
        case TokenType::OP_ADD:
            return "OP_ADD";
        case TokenType::OP_SUB:
            return "OP_SUB";
        case TokenType::OP_MUL:
            return "OP_MUL";
        case TokenType::OP_DIV:
            return "OP_DIV";
        case TokenType::OP_LP:
            return "OP_LP";
        case TokenType::OP_RP:
            return "OP_RP";

        //变量 
        case TokenType::ID:
            return "ID";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::STRING:
            return "STRING";

        case TokenType::NONE:
            break;
        case TokenType::ANNOTATION:
            break;
        case TokenType::NO_MORE_TOKEN:
            break;
    }
    return "NONE";
};

TokenType Utils::nameToToken(const string &token) {
    if (token == "true")
        return TokenType::KEY_TRUE;
    else if (token == "false")
        return TokenType::KEY_FALSE;
    else if (token == "or")
        return TokenType::KEY_OR;
    else if (token == "and")
        return TokenType::KEY_AND;
    else if (token == "not")
        return TokenType::KEY_NOT;
    else if (token == "int")
        return TokenType::KEY_INT;
    else if (token == "bool")
        return TokenType::KEY_BOOL;
    else if (token == "string")
        return TokenType::KEY_STRING;
    else if (token == "while")
        return TokenType::KEY_WHILE;
    else if (token == "do")
        return TokenType::KEY_DO;
    else if (token == "if")
        return TokenType::KEY_IF;
    else if (token == "then")
        return TokenType::KEY_THEN;
    else if (token == "else")
        return TokenType::KEY_ELSE;
    else if (token == "end")
        return TokenType::KEY_END;
    else if (token == "repeat")
        return TokenType::KEY_REPEAT;
    else if (token == "until")
        return TokenType::KEY_UNTIL;
    else if (token == "read")
        return TokenType::KEY_READ;
    else if (token == "write")
        return TokenType::KEY_WRITE;

    else if (token == ">")
        return TokenType::OP_GTR;
    else if (token == "<")
        return TokenType::OP_LSS;
    else if (token == "<=")
        return TokenType::OP_LEQ;
    else if (token == ">=")
        return TokenType::OP_GEQ;
    else if (token == "=")
        return TokenType::OP_EQU;
    else if (token == ",")
        return TokenType::OP_COMMA;
    else if (token == ";")
        return TokenType::OP_SEMICOLON;
    else if (token == ":=")
        return TokenType::OP_ASSIGN;
    else if (token == "+")
        return TokenType::OP_ADD;
    else if (token == "-")
        return TokenType::OP_SUB;
    else if (token == "*")
        return TokenType::OP_MUL;
    else if (token == "/")
        return TokenType::OP_DIV;
    else if (token == "(")
        return TokenType::OP_LP;
    else if (token == ")")
        return TokenType::OP_RP;

    return TokenType::NONE;
};
