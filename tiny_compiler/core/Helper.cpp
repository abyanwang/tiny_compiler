#include "Helper.h"

namespace Helper {

    std::string getTokenTypeName(Token::Type type) {
        switch (type) {
            case Token::Type::KEY_TRUE:
                return "KEY_TRUE";
            case Token::Type::KEY_FALSE:
                return "KEY_FALSE";
            case Token::Type::KEY_OR:
                return "KEY_OR";
            case Token::Type::KEY_AND:
                return "KEY_AND";
            case Token::Type::KEY_NOT:
                return "KEY_NOT";
            case Token::Type::KEY_INT:
                return "KEY_INT";
            case Token::Type::KEY_BOOL:
                return "KEY_BOOL";
            case Token::Type::KEY_STRING:
                return "KEY_STRING";
            case Token::Type::KEY_DO:
                return "KEY_DO";
            case Token::Type::KEY_WHILE:
                return "KEY_WHILE";
            case Token::Type::KEY_REPEAT:
                return "KEY_REPEAT";
            case Token::Type::KEY_UNTIL:
                return "KEY_UNTIL";
            case Token::Type::KEY_IF:
                return "KEY_IF";
            case Token::Type::KEY_THEN:
                return "KEY_THEN";
            case Token::Type::KEY_ELSE:
                return "KEY_ELSE";
            case Token::Type::KEY_END:
                return "KEY_END";
            case Token::Type::KEY_READ_:
                return "KEY_READ_";
            case Token::Type::KEY_WRITE_:
                return "KEY_WRITE_";

            case Token::Type::OP_GTR:
                return "OP_GTR";
            case Token::Type::OP_LSS:
                return "OP_LSS";
            case Token::Type::OP_LEQ:
                return "OP_LEQ";
            case Token::Type::OP_GEQ:
                return "OP_GEQ";
            case Token::Type::OP_EQU:
                return "OP_EQU";
            case Token::Type::OP_COMMA:
                return "OP_COMMA";
            case Token::Type::OP_SEMICOLON:
                return "OP_SEMICOLON";
            case Token::Type::OP_ASSIGN:
                return "OP_ASSIGN";
            case Token::Type::OP_ADD:
                return "OP_ADD";
            case Token::Type::OP_SUB:
                return "OP_SUB";
            case Token::Type::OP_MUL:
                return "OP_MUL";
            case Token::Type::OP_DIV:
                return "OP_DIV";
            case Token::Type::OP_LP:
                return "OP_LP";
            case Token::Type::OP_RP:
                return "OP_RP";

            case Token::Type::ID:
                return "ID";
            case Token::Type::NUMBER:
                return "NUMBER";
            case Token::Type::STRING:
                return "STRING";

            case Token::Type::NONE:
                break;
            case Token::Type::ANNOTATION:
                break;
            case Token::Type::NO_MORE_TOKEN:
                break;
        }
        return "NONE";
    }

    Token::Type getTokenTypeByName(const std::string &token) {
        if (token == "true")
            return Token::Type::KEY_TRUE;
        else if (token == "false")
            return Token::Type::KEY_FALSE;
        else if (token == "or")
            return Token::Type::KEY_OR;
        else if (token == "and")
            return Token::Type::KEY_AND;
        else if (token == "not")
            return Token::Type::KEY_NOT;
        else if (token == "int")
            return Token::Type::KEY_INT;
        else if (token == "bool")
            return Token::Type::KEY_BOOL;
        else if (token == "string")
            return Token::Type::KEY_STRING;
        else if (token == "while")
            return Token::Type::KEY_WHILE;
        else if (token == "do")
            return Token::Type::KEY_DO;
        else if (token == "if")
            return Token::Type::KEY_IF;
        else if (token == "then")
            return Token::Type::KEY_THEN;
        else if (token == "else")
            return Token::Type::KEY_ELSE;
        else if (token == "end")
            return Token::Type::KEY_END;
        else if (token == "repeat")
            return Token::Type::KEY_REPEAT;
        else if (token == "until")
            return Token::Type::KEY_UNTIL;
        else if (token == "read")
            return Token::Type::KEY_READ_;
        else if (token == "write")
            return Token::Type::KEY_WRITE_;

        else if (token == ">")
            return Token::Type::OP_GTR;
        else if (token == "<")
            return Token::Type::OP_LSS;
        else if (token == "<=")
            return Token::Type::OP_LEQ;
        else if (token == ">=")
            return Token::Type::OP_GEQ;
        else if (token == "=")
            return Token::Type::OP_EQU;
        else if (token == ",")
            return Token::Type::OP_COMMA;
        else if (token == ";")
            return Token::Type::OP_SEMICOLON;
        else if (token == ":=")
            return Token::Type::OP_ASSIGN;
        else if (token == "+")
            return Token::Type::OP_ADD;
        else if (token == "-")
            return Token::Type::OP_SUB;
        else if (token == "*")
            return Token::Type::OP_MUL;
        else if (token == "/")
            return Token::Type::OP_DIV;
        else if (token == "(")
            return Token::Type::OP_LP;
        else if (token == ")")
            return Token::Type::OP_RP;

        return Token::Type::NONE;
    }
}
