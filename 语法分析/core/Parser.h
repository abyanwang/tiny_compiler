#ifndef TINY_COMPILE_PARSE_H
#define TINY_COMPILE_PARSE_H

#include <sstream>
#include "Token.h"
#include "TreeNode.h"
#include "TokenStream.h" 
#include "Log.h"
#include <unordered_map> 

struct Symbol{
	VarType type;
    vector<int> lines;
};


class Parser {
	private:
	    TokenStream& _interface;
	    unordered_map<string,Symbol> table;
	    TreeNode* root = nullptr;
	    Log& log;
	
	
	    // temporary variable
	    Token token, last_token;
	
	public:
	    TreeNode* getRootTreeNode() {
	        return root;
	    }
//	    const SymbolTable* getSymbolTable() const {
//	        return &symbol_table;
//	    }
	    ~Parser() {
	        delete root;
	    }
	
	    Parser(TokenStream& token_interface, Log& log) : _interface(token_interface), log(log) {
	        parse();
	    }
	
	    void parse();
	    bool match(Token::Type expected, bool flag = false); 
	    void checkType(TreeNode* node);
	    void checkStatementType(TreeNode* node, TreeNode::StatementType parentStmtType = TreeNode::StatementType::NONE);
	
	    //ÎÄ·¨ 
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

};

#endif //TINY_COMPILE_PARSE_H
