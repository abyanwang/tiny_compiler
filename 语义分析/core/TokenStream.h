#ifndef TINY_COMPILE_TOKENSTREAM_H
#define TINY_COMPILE_TOKENSTREAM_H

#include <vector>


class TokenStream{
    const vector<Token>& list;
    int index = 0;

public:
    TokenStream(vector<Token>& tokens) : list(tokens) { }

    Token nextToken(){
        if(index++ < list.size()) return list[index-1];
        Token token;
        token.type = TokenType::NO_MORE_TOKEN;
        token.token = "EOF";
        return token;
    }
};

#endif //TINY_COMPILE_TOKENSTREAM_H
