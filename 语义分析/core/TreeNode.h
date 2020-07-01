#ifndef TINY_COMPILE_TREE_NODE_H
#define TINY_COMPILE_TREE_NODE_H

#include <ostream>
#include "Token.h"
// #include "VarType.h"
using namespace std;

enum class VarType {
    VT_VOID, VT_INT, VT_BOOL, VT_STRING
};

	//定义常量 
enum class NodeType {
    PROGRAM, STMT_SEQUENCE, IF_STMT, REPEAT_STMT, ASSIGN_STMT,
    READ_STMT, WRITE_STMT, WHILE_STMT,
    GT_EXP, GE_EXP, LT_EXP, LE_EXP, EQ_EXP, // > >= < <= =
    OR_EXP, AND_EXP, NOT_EXP,
    PLUS_EXP, SUB_EXP, MUL_EXP, DIV_EXP,
    FACTOR,
    NONE
};

enum class StatementType {
    NONE, CONDITION, ASSIGN
};

struct TreeNode {
    NodeType type = NodeType::NONE;
    StatementType stmtType = StatementType::NONE;
    VarType varType = VarType::VT_VOID;
    TreeNode *children[3] {NULL};
    Token* token = NULL;

    // temporary variables
    // T => TRUE chain
    // F => FALSE chain
    // B => code begin
    int T = 0, F = 0, B = 0;


    static TreeNode* createNode(NodeType type);

    void print(ostream& out) const{
    	printHelper(out, this, 0);
    };

    static void printHelper(ostream& out,const TreeNode* node, int indent);
    
    
    ~TreeNode() {
        delete token;
        for (auto* child : children){
			delete child;	
		}
    }
};

#endif //TINY_COMPILE_TREE_NODE_H
