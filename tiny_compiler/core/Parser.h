#ifndef TINY_COMPILE_PARSE_H
#define TINY_COMPILE_PARSE_H

#include <sstream>
#include "Token.h"
#include "TreeNode.h"
#include "SymbolTable.h"
#include "Log.h"

class Parser {
public:
    class TokenInterface;

private:
    TokenInterface& _interface;
    SymbolTable symbol_table;
    TreeNode* root = nullptr;
    Log& log;

private:
    // temporary variable
    Token token, last_token;

public:
    inline TreeNode* getRootTreeNode() {
        return root;
    }
    inline const SymbolTable* getSymbolTable() const {
        return &symbol_table;
    }
    inline ~Parser() {
        delete root;
    }

public:
    inline explicit Parser(TokenInterface& token_interface, Log& log) : _interface(token_interface), log(log) {
        parse();
    }

private:
    void parse();
    bool match(Token::Type expected, bool flag = false /* 是否强制匹配 */);
    void checkType(TreeNode* node);
    void checkStatementType(TreeNode* node, TreeNode::StatementType parentStmtType = TreeNode::StatementType::NONE);

private:
    TreeNode* program();
    void declarations();
    TreeNode* stmt_sequence();
    TreeNode* statement();
    TreeNode* if_stmt();
    TreeNode* repeat_stmt();
    TreeNode* assign_stmt();
    TreeNode* read_stmt();
    TreeNode* write_stmt();
    TreeNode* while_stmt();

    TreeNode* or_exp();
    TreeNode* and_exp();
    TreeNode* comparison_exp();
    TreeNode* add_sub_exp();
    TreeNode* mul_div_exp();
    TreeNode* factor();

public:
    class TokenInterface {
    public:
        virtual Token nextToken() = 0;
    };
};

#endif //TINY_COMPILE_PARSE_H
