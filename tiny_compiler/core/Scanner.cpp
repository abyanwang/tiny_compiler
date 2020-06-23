#include <string>
#include <sstream>
#include "Scanner.h"
#include "Helper.h"

std::vector<Token> Scanner::scan(std::ifstream& fin, Log& log) {
    Scanner instance(fin, log);
    if (log.hasError()) {
        std::stringstream msg;
        msg << "You have " << log.getErrorCount() << " errors.";
        throw msg.str();
    }
    return instance.getTokens();
}

void Scanner::new_line() {
    line_number ++;
    char_number = 0;
    type = Token::Type::NONE;
    isSign = false;
}

void Scanner::close_word() {
    Token token;
    token.token = word;
    token.type = type;
    token.line = line_number;
    token.offset = char_number - 1;
    Token::Type t = Helper::getTokenTypeByName(word);
    if (token.type == Token::Type::NONE)
        token.type = t;
    else if (token.type == Token::Type::ID && t != Token::Type::NONE)
        token.type = t;
    if (token.type == Token::Type::NONE)
        log.error("unknown token: " + word, line_number, char_number - 1);
    if (token.type != Token::Type::ANNOTATION)
        list.push_back(token);
    word.clear();
    type = Token::Type::NONE;
    isSign = false;
}

void Scanner::analyse() {
    if (!in.is_open()) {
        log.error("can not load file.", 1, 0);
        return;
    }
    char ch;
    line_number = 1;
    char_number = 0;
    isSign = false;
    while (true)
    {
        in.get(ch);
        char_number ++;
        if (in.fail() || in.eof()) {
            if (type == Token::Type::STRING)
                log.error("unclosed string", line_number, char_number);
            if (type == Token::Type::ANNOTATION)
                log.error("unclosed annotation", line_number, char_number);
            if (!word.empty())
                close_word();
            break;
        }

        if (isSign) {
            if (Helper::getTokenTypeByName(word) != Token::Type::NONE &&
                    Helper::getTokenTypeByName(word + ch) == Token::Type::NONE)
                close_word();
            else if (!Helper::isValidSign(ch))
                close_word();
        }

        if (!word.empty() && type != Token::Type::ANNOTATION && type != Token::Type::STRING
            && !isSign && Helper::isValidSign(ch))
            close_word();

        if (word.empty()) {
            if (ch == '{')
                type = Token::Type::ANNOTATION;
            else if (ch == '\'')
                type = Token::Type::STRING;
            else if (Helper::isDigit(ch))
                type = Token::Type::NUMBER;
            else if (Helper::isLetter(ch))
                type = Token::Type::ID;
            else if (Helper::isValidSign(ch))
                isSign = true;
            else if (!Helper::isSeparator(ch))
                log.error(std::string("unknown symbol: ") + ch, line_number, char_number);
        } else {
            if (type == Token::Type::STRING && ch == '\'') {
                word += ch;
                close_word();
                continue;
            }
            if (type == Token::Type::ANNOTATION && ch == '}') {
                word += ch;
                close_word();
                continue;
            }
        }

        if (ch == '\n') {
            if (type == Token::Type::STRING) {
                log.error("unclosed string", line_number, char_number);
                close_word();
                new_line();
                continue;
            }
            if (type == Token::Type::ANNOTATION) {
                log.error("unclosed annotation", line_number, char_number);
                close_word();
                new_line();
                continue;
            }
        }

        if (type != Token::Type::ANNOTATION && type != Token::Type::STRING) {
            if (Helper::isSeparator(ch)) {
                if (!word.empty())
                    close_word();
                if (ch == '\n')
                    new_line();
                continue;
            }
        }

        if (type == Token::Type::NUMBER && !Helper::isDigit(ch))
            log.error(std::string("here must be a digit: ") + ch, line_number, char_number);
        if (type == Token::Type::ID && !Helper::isLetter(ch) && !Helper::isDigit(ch))
            log.error(std::string("here must be a letter: ") + ch, line_number, char_number);

        word += ch;
        if (ch == '\n')
            new_line();
    }
    if (!word.empty())
        close_word();
}