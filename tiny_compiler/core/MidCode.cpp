#include "MidCode.h"

std::string MidCode::quaternary() {
    std::string op;
    switch (type) {
        case Type::NONE:break;
        case Type::START:
            op = "start";
            break;
        case Type::END:
            op = "end";
            break;
        case Type::READ:
            op = "read";
            break;
        case Type::WRITE:
            op = "write";
            break;
        case Type::ASSIGN:
            op = ":=";
            break;
        case Type::JUMP:
            op = "JMP";
            break;
        case Type::JUMP_LSS:
            op = "J<";
            break;
        case Type::JUMP_GRT:
            op = "J>";
            break;
        case Type::JUMP_EQ:
            op = "J=";
            break;
        case Type::JUMP_LEQ:
            op = "J<=";
            break;
        case Type::JUMP_GEQ:
            op = "J>=";
            break;
        case Type::ADD:
            op = "+";
            break;
        case Type::SUB:
            op = "-";
            break;
        case Type::MUL:
            op = "*";
            break;
        case Type::DIV:
            op = "/";
            break;
        case Type::AND:
            op = "AND";
            break;
        case Type::OR:
            op = "OR";
            break;
        case Type::NOT:
            op = "NOT";
            break;
    }
    return "(" + op + ", " + param1 + ", " + param2 + ", " + param3 + ")";
}

std::string MidCode::statement() {
    switch (type) {
        case Type::NONE:break;
        case Type::START:
            return "start";
        case Type::END:
            return "end";
        case Type::READ:
            return "read " + param3;
        case Type::WRITE:
            return "write " + param3;
        case Type::ASSIGN:
            return param3 + " := " + param1;
        case Type::JUMP:
            return "goto " + param3;
        case Type::JUMP_LSS:
            return "if " + param1 + " < " + param2 + " goto " + param3;
        case Type::JUMP_GRT:
            return "if " + param1 + " > " + param2 + " goto " + param3;
        case Type::JUMP_EQ:
            return "if " + param1 + " = " + param2 + " goto " + param3;
        case Type::JUMP_LEQ:
            return "if " + param1 + " <= " + param2 + " goto " + param3;
        case Type::JUMP_GEQ:
            return "if " + param1 + " >= " + param2 + " goto " + param3;
        case Type::ADD:
            return param3 + " := " + param1 + " + " + param2;
        case Type::SUB:
            return param3 + " := " + param1 + " - " + param2;
        case Type::MUL:
            return param3 + " := " + param1 + " * " + param2;
        case Type::DIV:
            return param3 + " := " + param1 + " / " + param2;
        case Type::AND:
            return param3 + " := " + param1 + " and " + param2;
        case Type::OR:
            return param3 + " := " + param1 + " or " + param2;
        case Type::NOT:
            return param3 + " := " + " not " + param1;
    }
    return "";
}

MidCodeGenerator::MidCodeGenerator(TreeNode* root) {
    codes.push_back(_emit(MidCode::Type::START, "-", "-", "-"));
    generate(root);
    codes.push_back(_emit(MidCode::Type::END, "-", "-", "-"));
    for (auto code : codes) {
        if (code->jump != -1)
            code->param3 = std::to_string(code->jump);
    }
}

MidCodeGenerator::~MidCodeGenerator() {
    for (auto* code : codes)
        delete code;
}

MidCode* MidCodeGenerator::_emit(MidCode::Type type, const std::string &param1,
                                const std::string &param2, const std::string &param3) {
    auto* code = new MidCode;
    code->pos = static_cast<int>(codes.size());
    code->type = type;
    code->param1 = param1;
    code->param2 = param2;
    code->param3 = param3;
    return code;
}

void MidCodeGenerator::back_patch(int a, int b) {
    auto p = codes[a];
    while (p) {
        p->jump = b;
        p = p->next;
    }
}

int MidCodeGenerator::merge(int a, int b) {
    auto p1 = codes[a], p2 = codes[b];
    auto p = p2;
    while (p2->next != nullptr)
        p2 = p2->next;
    p2->next = p1;
    return p->pos;
}

void MidCodeGenerator::opJump(TreeNode* node, MidCode::Type type, const std::string &param1, const std::string &param2) {
    node->T = static_cast<int>(codes.size());
    node->B = node->T;
    node->F = node->T + 1;
    codes.push_back(_emit(type, param1, param2, ""));
    codes.push_back(_emit(MidCode::Type::JUMP, "-", "-", ""));
}

std::string MidCodeGenerator::generate(TreeNode* node) {
    if (node == nullptr)
        return "";
    std::string name;
    switch (node->type) {
        case TreeNode::NodeType::IF_STMT: {
            node->children[0]->B = static_cast<int>(codes.size());
            generate(node->children[0]);
            int then_b = static_cast<int>(codes.size());
            generate(node->children[1]);
            MidCode* code = _emit(MidCode::Type::JUMP, "-", "-", "");
            codes.push_back(code);
            int else_b = static_cast<int>(codes.size());
            generate(node->children[2]);
            code->jump = static_cast<int>(codes.size());
            back_patch(node->children[0]->F, else_b);
            back_patch(node->children[0]->T, then_b);
            break;
        }
        case TreeNode::NodeType::READ_STMT:
            codes.push_back(_emit(MidCode::Type::READ, "-", "-", node->children[0]->token->token));
            break;
        case TreeNode::NodeType::WRITE_STMT: {
            codes.push_back(_emit(MidCode::Type::WRITE, "-", "-", node->children[0]->token->token));
            break;
        }
        case TreeNode::NodeType::REPEAT_STMT: {
            int repeat_b = static_cast<int>(codes.size());
            generate(node->children[0]);
            node->children[1]->B = static_cast<int>(codes.size());
            generate(node->children[1]);
            back_patch(node->children[1]->T, repeat_b);
            back_patch(node->children[1]->F, static_cast<int>(codes.size()));
            break;
        }
        case TreeNode::NodeType::WHILE_STMT: {
            node->children[0]->B = static_cast<int>(codes.size());
            generate(node->children[0]); // 循环条件
            int while_t = static_cast<int>(codes.size());
            generate(node->children[1]); // 循环体
            back_patch(node->children[0]->T, while_t);
            back_patch(node->children[0]->F, static_cast<int>(codes.size() + 1));
            codes.push_back(_emit(MidCode::Type::JUMP, "-", "-", std::to_string(node->children[0]->B)));
            break;
        }
        case TreeNode::NodeType::STMT_SEQUENCE: {
            generate(node->children[0]);
            generate(node->children[1]);
            break;
        }
        case TreeNode::NodeType::ASSIGN_STMT: {
            name = generate(node->children[1]);
            codes.push_back(_emit(MidCode::Type::ASSIGN, name, "-", node->children[0]->token->token));
            break;
        }
        case TreeNode::NodeType::GT_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            opJump(node, MidCode::Type::JUMP_GRT, t1, t2);
            break;
        }
        case TreeNode::NodeType::GE_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            opJump(node, MidCode::Type::JUMP_GEQ, t1, t2);
            break;
        }
        case TreeNode::NodeType::LT_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            opJump(node, MidCode::Type::JUMP_LSS, t1, t2);
            break;
        }
        case TreeNode::NodeType::LE_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            opJump(node, MidCode::Type::JUMP_LEQ, t1, t2);
            break;
        }
        case TreeNode::NodeType::EQ_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            opJump(node, MidCode::Type::JUMP_EQ, t1, t2);
            break;
        }
        case TreeNode::NodeType::AND_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            if (node->stmtType == TreeNode::StatementType::CONDITION) {
                back_patch(node->children[0]->T, node->children[1]->B);
                node->B = node->children[0]->B;
                node->T = node->children[1]->T;
                node->F = merge(node->children[0]->F, node->children[1]->F);
            } else {
                name = new_tmp_var();
                codes.push_back(_emit(MidCode::Type::AND, t1, t2, name));
            }
            break;
        }
        case TreeNode::NodeType::OR_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            if (node->stmtType == TreeNode::StatementType::CONDITION) {
                back_patch(node->children[0]->F, node->children[1]->B);
                node->B = node->children[0]->B;
                node->T = merge(node->children[0]->T, node->children[1]->T);
                node->F = node->children[1]->F;
            } else {
                name = new_tmp_var();
                codes.push_back(_emit(MidCode::Type::OR, t1, t2, name));
            }
            break;
        }
        case TreeNode::NodeType::NOT_EXP: {
            auto t = generate(node->children[0]);
            if (node->stmtType == TreeNode::StatementType::CONDITION) {
                node->B = node->children[0]->B;
                node->T = node->children[0]->F;
                node->F = node->children[0]->T;
            } else {
                name = new_tmp_var();
                codes.push_back(_emit(MidCode::Type::NOT, t, "-", name));
            }
            break;
        }
        case TreeNode::NodeType::PLUS_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            name = new_tmp_var();
            codes.push_back(_emit(MidCode::Type::ADD, t1, t2, name));
            break;
        }
        case TreeNode::NodeType::SUB_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            name = new_tmp_var();
            codes.push_back(_emit(MidCode::Type::SUB, t1, t2, name));
            break;
        }
        case TreeNode::NodeType::MUL_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            name = new_tmp_var();
            codes.push_back(_emit(MidCode::Type::MUL, t1, t2, name));
            break;
        }
        case TreeNode::NodeType::DIV_EXP: {
            auto t1 = generate(node->children[0]);
            auto t2 = generate(node->children[1]);
            name = new_tmp_var();
            codes.push_back(_emit(MidCode::Type::DIV, t1, t2, name));
            break;
        }
        case TreeNode::NodeType::FACTOR: {
            name = node->token->token;
            if (node->varType == VarType::VT_BOOL && node->stmtType == TreeNode::StatementType::CONDITION) {
                // 如果原子类型是bool类型而且是条件语句，需要进行跳转判断
                node->T = codes.size();
                node->B = node->T;
                node->F = node->T + 1;
                codes.push_back(_emit(MidCode::Type::JUMP_EQ, node->token->token, "true", ""));
                codes.push_back(_emit(MidCode::Type::JUMP, "-", "-", ""));
            }
            break;
        }
        default:
            break;
    }
    return name;
}
