#include "TreeNode.h"

TreeNode* TreeNode::createNode(NodeType type) {
    TreeNode* node= new TreeNode();
    for (TreeNode* &i : node->children){
    	i = NULL;
	}
    node->type = type;
    node->token = NULL;
    node->varType = VarType::VT_VOID;
    return node;
}


void TreeNode::printHelper(ostream& out,const TreeNode* node, int indent) {
    for (int i = 0;i < indent;i++)out << " ";
    switch (node->type) {
        case NodeType::STMT_SEQUENCE:
            out << "stmt_sequence" << endl;
            break;
        case NodeType::PROGRAM:
            out << "program" << endl;
            break;
        case NodeType::IF_STMT:
            out << "if" << endl;
            break;
        case NodeType::REPEAT_STMT:
            out << "repeat" << endl;
            break;
        case NodeType::ASSIGN_STMT:
            out << "assign" << endl;
            break;
        case NodeType::READ_STMT:
            out << "read" << endl;
            break;
        case NodeType::WRITE_STMT:
            out << "write" << endl;
            break;
        case NodeType::WHILE_STMT:
            out << "while" << endl;
            break;
        case NodeType::GE_EXP:
            out << ">=" << endl;
            break;
        case NodeType::GT_EXP:
            out << ">" << endl;
            break;
        case NodeType::LT_EXP:
            out << "<" << endl;
            break;
        case NodeType::LE_EXP:
            out << "<=" << endl;
            break;
        case NodeType::EQ_EXP:
            out << "=" << endl;
            break;
        case NodeType::SUB_EXP:
            out << "sub" << endl;
            break;
        case NodeType::PLUS_EXP:
            out << "plus" << endl;
            break;
        case NodeType::MUL_EXP:
            out << "mul" << endl;
            break;
        case NodeType::DIV_EXP:
            out << "div" << endl;
            break;
        case NodeType::OR_EXP:
            out << "or" << endl;
            break;
        case NodeType::AND_EXP:
            out << "and" << endl;
            break;
        case NodeType::NOT_EXP:
            out << "not" << endl;
            break;
        case NodeType::FACTOR:
            out << "factor:var=" + node->token->token << endl;
            break;
        default:
            out << "unknown NodeType: " + node->token->token << endl;
            break;
    }
    for (TreeNode* treeNode : node->children) {
        if(treeNode)printHelper(out, treeNode, indent + 2);
    }
}
