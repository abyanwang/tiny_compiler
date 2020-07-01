#ifndef TINY_COMPILE_TOKEN_H
#define TINY_COMPILE_TOKEN_H

#include <string>
using namespace std;

enum class TokenType {
	NO_MORE_TOKEN,
	NONE, ANNOTATION,
	KEY_TRUE, KEY_FALSE, // true false
	KEY_OR, KEY_AND, KEY_NOT, // or and not
	KEY_INT, KEY_BOOL, KEY_STRING, // int bool string
	KEY_DO, KEY_WHILE, // do while
	KEY_REPEAT, KEY_UNTIL, // repeat until
	KEY_IF, KEY_THEN, KEY_ELSE, KEY_END, // if then else end
	KEY_READ, KEY_WRITE, 

	OP_GTR, OP_LSS, // > <
	OP_LEQ, OP_GEQ, OP_EQU, // <= >= =
	OP_COMMA, OP_SEMICOLON, OP_ASSIGN, // , ; :=
	OP_ADD, OP_SUB, OP_MUL, OP_DIV, // + - * /
	OP_LP, OP_RP, // ( )

	ID, NUMBER, STRING
};

struct Token {
	TokenType type;
	string token;
	int line = 1, offset = 0;
};

#endif //TINY_COMPILE_TOKEN_H
