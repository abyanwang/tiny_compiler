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
	    TokenStream& Stream;
	    unordered_map<string,Symbol> table;
	    TreeNode* root = nullptr;
	    Log& log;
	
	
	    // temporary variable
	    Token token, last_token;
	
	public:
	    TreeNode* getRootTreeNode() {
	        return root;
	    }

	    ~Parser() {
	        delete root;
	    }
	
	    Parser(TokenStream& tokenInterface, Log& log) : Stream(tokenInterface), log(log) {
	        parse();
	    }
	
	    void parse();
	    bool match(TokenType expected, bool flag = false); 
	    void checkType(TreeNode* node);
	    void checkStatementType(TreeNode* node, StatementType parentStmtType = StatementType::NONE);
	
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
