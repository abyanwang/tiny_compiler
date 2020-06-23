#ifndef TINY_COMPILE_TOKEN_H
#define TINY_COMPILE_TOKEN_H

#include <string>

struct Token {
    enum class Type {
        NO_MORE_TOKEN,
        NONE, ANNOTATION,
        KEY_TRUE, KEY_FALSE, // true false
        KEY_OR, KEY_AND, KEY_NOT, // or and not
        KEY_INT, KEY_BOOL, KEY_STRING, // int bool string
        KEY_DO, KEY_WHILE, // do while
        KEY_REPEAT, KEY_UNTIL, // repeat until
        KEY_IF, KEY_THEN, KEY_ELSE, KEY_END, // if then else end
        KEY_READ_, KEY_WRITE_, // read write (后加下划线以避免与windows库中关键字冲突)

        OP_GTR, OP_LSS, // > <
        OP_LEQ, OP_GEQ, OP_EQU, // <= >= =
        OP_COMMA, OP_SEMICOLON, OP_ASSIGN, // , ; :=
        OP_ADD, OP_SUB, OP_MUL, OP_DIV, // + - * /
        OP_LP, OP_RP, // ( )

        ID, NUMBER, STRING
    };

    Type type;
    std::string token;
    int line = 1, offset = 0;
};

#endif //TINY_COMPILE_TOKEN_H
