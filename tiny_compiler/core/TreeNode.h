#ifndef TINY_COMPILE_TREE_NODE_H
#define TINY_COMPILE_TREE_NODE_H

#include <ostream>
#include "Token.h"
#include "VarType.h"

struct TreeNode {
    static const int MAX_CHILDREN = 3;
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

    NodeType type = NodeType::NONE;
    StatementType stmtType = StatementType::NONE;
    VarType varType = VarType::VT_VOID;
    TreeNode *children[MAX_CHILDREN] { nullptr };
    Token* token = nullptr;

    // temporary variables
    // T => TRUE chain
    // F => FALSE chain
    // B => code begin
    int T = 0, F = 0, B = 0;

private:
    inline TreeNode() = default;

public:
    static TreeNode* create(NodeType type);

public:
    inline ~TreeNode() {
        delete token;
        for (auto* child : children)
            delete child;
    }
    void print(std::ostream& out) const;

private:
    static void printHelper(std::ostream& out, const TreeNode* node, int indent);
};

#endif //TINY_COMPILE_TREE_NODE_H
