#include <string>
#include <sstream>
#include "Scanner.h"
#include "Utils.h"
using namespace std;

vector<Token> Scanner::scan(ifstream& fin, Log& log) {
    Scanner instance(fin, log);
    if (log.hasError()) {
        stringstream msg;
        msg << "You have " << log.getErrorCount() << " errors.";
        throw msg.str();
    }
    return instance.getTokens();
}

void Scanner::newLine() {
    lineNumber ++;
    charNumber = 0;
    type = TokenType::NONE;
    isSign = false;
}


//当前的word关闭
void Scanner::figureWord() {
    //新建一个临时的Token
    Token token;
    token.token = word;
    token.type = type;
    token.line = lineNumber;
    token.offset = charNumber - 1;

    //enum
    TokenType t = Utils::nameToToken(word);
    
    //转成正常的type 
    if (token.type == TokenType::NONE)token.type = t;
    else if (token.type == TokenType::ID && t != TokenType::NONE)token.type = t;

    //if word is error     
    if (token.type == TokenType::NONE)log.error("unknown token: " + word, lineNumber, charNumber - 1);
    
    //if word is not annotation 放入token list
    if (token.type != TokenType::ANNOTATION)list.push_back(token);
    word.clear();
    type = TokenType::NONE;
    isSign = false;
}



void Scanner::analyse(){
	if (!in.is_open()) {
        log.error("can not load file.", 1, 0);
        return;
    }
    char ch;
    lineNumber = 1;
    charNumber = 0;
    isSign = false;
    while(1){
    	in.get(ch);
    	if (in.fail() || in.eof()) {
            if (!word.empty()){
            	figureWord();
			}
            //跳出while    
            break;
        }
        charNumber++;
        
        //处理当前ch为sign 但是没判断为sign 且有word的情况  单个sign 
        if(word.size() != 0&&type != TokenType::ANNOTATION&& type != TokenType::STRING&&
		isSign == false&&Utils::isValidSign(ch)){
			figureWord();
		}
		
		
		//当前的type已经为sign(前一个字符为sign)  :1 ;  不是单个sign := ; 到下一步的时候处理为:= 
        if(isSign&&Utils::nameToToken(word) != TokenType::NONE &&
                    Utils::nameToToken(word + ch) == TokenType::NONE){
            figureWord();        	
		}
		
		if(word.size() == 0){
			if (ch == '{'){
				type = TokenType::ANNOTATION;
			}else if (ch == '\''){
            	type = TokenType::STRING;
			}else if (isdigit(ch)){
				type = TokenType::NUMBER;
			}else if (isalpha(ch)){
				type = TokenType::ID;
			}else if (Utils::isValidSign(ch)){
				isSign = true;
			}else if (!Utils::isSeparator(ch)){
				log.error("unknown symbol: " + ch, lineNumber, charNumber);
			}
		}else{
			if (type == TokenType::STRING && ch == '\'') {
                word += ch;
                figureWord();
                continue;
            }
            if (type == TokenType::ANNOTATION && ch == '}') {
                word += ch;
                figureWord();
                continue;
            }
		}
		
		//处理分割符 这里去掉了注释多行的问题 
		if (type != TokenType::ANNOTATION && type != TokenType::STRING) {
            if (Utils::isSeparator(ch)) {
                if (!word.empty())figureWord();
                if (ch == '\n')newLine();
                continue;
            }
        }
		
		
		if (type == TokenType::NUMBER && !isdigit(ch))
            log.error("here must be a digit: " + ch, lineNumber, charNumber);
        if (type == TokenType::ID && !isalpha(ch) && !isdigit(ch))
            log.error("here must be a letter: " + ch, lineNumber, charNumber);
            
        word += ch;
	}
	
	if (!word.empty())figureWord();
}
