#include "TreeNode.h"

TreeNode* TreeNode::create(NodeType type) {
    auto* node= new TreeNode();
    for (auto &i : node->children)
        i = nullptr;
    node->type = type;
    node->token = nullptr;
    node->varType = VarType::VT_VOID;
    return node;
}

void TreeNode::print(std::ostream& out) const {
    printHelper(out, this, 0);
}

void TreeNode::printHelper(std::ostream& out, const TreeNode* node, int indent) {
    for (int i = 0; i < indent; ++i)
        out << " ";
    switch (node->type) {
        case TreeNode::NodeType::STMT_SEQUENCE:
            out << "stmt_sequence" << std::endl;
            break;
        case TreeNode::NodeType::PROGRAM:
            out << "program" << std::endl;
            break;
        case TreeNode::NodeType::IF_STMT:
            out << "if" << std::endl;
            break;
        case TreeNode::NodeType::REPEAT_STMT:
            out << "repeat" << std::endl;
            break;
        case TreeNode::NodeType::ASSIGN_STMT:
            out << "assign" << std::endl;
            break;
        case TreeNode::NodeType::READ_STMT:
            out << "read" << std::endl;
            break;
        case TreeNode::NodeType::WRITE_STMT:
            out << "write" << std::endl;
            break;
        case TreeNode::NodeType::WHILE_STMT:
            out << "while" << std::endl;
            break;
        case TreeNode::NodeType::GE_EXP:
            out << ">=" << std::endl;
            break;
        case TreeNode::NodeType::GT_EXP:
            out << ">" << std::endl;
            break;
        case TreeNode::NodeType::LT_EXP:
            out << "<" << std::endl;
            break;
        case TreeNode::NodeType::LE_EXP:
            out << "<=" << std::endl;
            break;
        case TreeNode::NodeType::EQ_EXP:
            out << "=" << std::endl;
            break;
        case TreeNode::NodeType::SUB_EXP:
            out << "sub" << std::endl;
            break;
        case TreeNode::NodeType::PLUS_EXP:
            out << "plus" << std::endl;
            break;
        case TreeNode::NodeType::MUL_EXP:
            out << "mul" << std::endl;
            break;
        case TreeNode::NodeType::DIV_EXP:
            out << "div" << std::endl;
            break;
        case TreeNode::NodeType::OR_EXP:
            out << "or" << std::endl;
            break;
        case TreeNode::NodeType::AND_EXP:
            out << "and" << std::endl;
            break;
        case TreeNode::NodeType::NOT_EXP:
            out << "not" << std::endl;
            break;
        case TreeNode::NodeType::FACTOR:
            out << std::string("factor:var=") + node->token->token << std::endl;
            break;
        default:
            out << "unknown NodeType: " + node->token->token << std::endl;
            break;
    }
    for (auto i : node->children) {
        if (i)
            printHelper(out, i, indent + 2);
    }
}
