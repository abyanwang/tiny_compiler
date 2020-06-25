#ifndef TINY_COMPILE_TOKENSTREAM_H
#define TINY_COMPILE_TOKENSTREAM_H

#include <vector>
using namespace std;


class TokenStream{
    const vector<Token>& list;
    int index = -1;

public:
    TokenStream(vector<Token>& tokens) : list(tokens) { }

    Token nextToken(){
        if (++index < (int) list.size()) return list[index];
        
        Token token;
        token.type = Token::Type::NO_MORE_TOKEN;
        token.token = "EOF";
        return token;
    }
};

#endif //TINY_COMPILE_TOKENSTREAM_H
