#include "MidCode.h"


string MidCode::statement() {
    switch (type) {
        case MidType::NONE:break;
        case MidType::START:
            return "START";
        case MidType::END:
            return "END";
        case MidType::READ:
            return "read " + param3;
        case MidType::WRITE:
            return "write " + param3;
        case MidType::ASSIGN:
            return param3 + " := " + param1;
        case MidType::JUMP:
            return "goto line " + param3;
        case MidType::JUMP_LSS:
            return "if " + param1 + " < " + param2 + " goto line " + param3;
        case MidType::JUMP_GRT:
            return "if " + param1 + " > " + param2 + " goto line " + param3;
        case MidType::JUMP_EQ:
            return "if " + param1 + " = " + param2 + " goto line " + param3;
        case MidType::JUMP_LEQ:
            return "if " + param1 + " <= " + param2 + " goto line " + param3;
        case MidType::JUMP_GEQ:
            return "if " + param1 + " >= " + param2 + " goto line " + param3;
        case MidType::ADD:
            return param3 + " := " + param1 + " + " + param2;
        case MidType::SUB:
            return param3 + " := " + param1 + " - " + param2;
        case MidType::MUL:
            return param3 + " := " + param1 + " * " + param2;
        case MidType::DIV:
            return param3 + " := " + param1 + " / " + param2;
        case MidType::AND:
            return param3 + " := " + param1 + " and " + param2;
        case MidType::OR:
            return param3 + " := " + param1 + " or " + param2;
        case MidType::NOT:
            return param3 + " := " + " not " + param1;
    }
    return "";
}

MidCodeGenerator::MidCodeGenerator(TreeNode* root) {
	//start 
    codes.push_back(link(MidType::START, "", "", ""));
    
	//中间代码生成 
	generateMidCode(root);
	
	//end 
    codes.push_back(link(MidType::END, "", "", ""));
    
    //处理jump 
    for (MidCode* code : codes) {
        if (code->jump != -1)code->param3 = to_string(code->jump);
    }
}

//析构 
MidCodeGenerator::~MidCodeGenerator() {
    for (auto* code : codes)
        delete code;
}

//处理单行中间代码生成 
MidCode* MidCodeGenerator::link(MidType type, const string &param1,
                                const string &param2, const string &param3) {
    MidCode* code = new MidCode;
    code->pos = codes.size();
    code->type = type;
    code->param1 = param1;
    code->param2 = param2;
    code->param3 = param3;
    return code;
}


//链上的code的jump改变 
void MidCodeGenerator::backPatch(int a, int b) {
    MidCode* p = codes[a];
    while (p) {
        p->jump = b;
        p = p->next;
    }
}

int MidCodeGenerator::merge(int a, int b) {
    MidCode* p1 = codes[a];
	MidCode* p2 = codes[b];
    MidCode* p = p2;
    while (p2->next)p2 = p2->next;
    p2->next = p1;
    return p->pos;
}

void MidCodeGenerator::opJump(TreeNode* node, MidType type, const string &param1, const string &param2) {
    node->T = codes.size();
    node->B = node->T;
    node->F = node->T + 1;
    codes.push_back(link(type, param1, param2, ""));
    codes.push_back(link(MidType::JUMP, "", "", ""));
}


//dfs
string MidCodeGenerator::generateMidCode(TreeNode* node) {
    if (node == nullptr)
        return "";
    string name;
    switch (node->type) {
    	//if-stmt -> if  logical-or-exp then stmt-sequence [else stmt-sequence] end
        case NodeType::IF_STMT: {
            node->children[0]->B = codes.size();
            generateMidCode(node->children[0]);
            
            //then 
            int then_b = codes.size();
            generateMidCode(node->children[1]);
            MidCode* code = link(MidType::JUMP, "", "", "");
            codes.push_back(code);
            
            //else 
            int else_b = codes.size();
            generateMidCode(node->children[2]);
            code->jump = codes.size();
            backPatch(node->children[0]->F, else_b);
            backPatch(node->children[0]->T, then_b);
            break;
        }
        case NodeType::READ_STMT:
            codes.push_back(link(MidType::READ, "", "", node->children[0]->token->token));
            break;
        case NodeType::WRITE_STMT: {
            codes.push_back(link(MidType::WRITE, "", "", node->children[0]->token->token));
            break;
        }
        
        //repeat-stmt -> repeat stmt-sequence until logical-or-exp 
        case NodeType::REPEAT_STMT: {
            int repeat_b = codes.size();
            //stmt
            generateMidCode(node->children[0]);
            node->children[1]->B = codes.size();
            
            //logical-or-exp
            generateMidCode(node->children[1]);
            backPatch(node->children[1]->T, repeat_b);
            backPatch(node->children[1]->F, codes.size());
            break;
        }
        case NodeType::WHILE_STMT: {
            node->children[0]->B = codes.size();
            generateMidCode(node->children[0]); // 循环条件
            int while_t = codes.size();
            generateMidCode(node->children[1]); // 循环体
            backPatch(node->children[0]->T, while_t);
            backPatch(node->children[0]->F, codes.size() + 1);
            codes.push_back(link(MidType::JUMP, "", "", to_string(node->children[0]->B)));
            break;
        }
        case NodeType::STMT_SEQUENCE: {
            generateMidCode(node->children[0]);
            generateMidCode(node->children[1]);
            break;
        }
        case NodeType::ASSIGN_STMT: {
            name = generateMidCode(node->children[1]);
            codes.push_back(link(MidType::ASSIGN, name, "", node->children[0]->token->token));
            break;
        }
        case NodeType::GT_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            opJump(node, MidType::JUMP_GRT, t1, t2);
            break;
        }
        case NodeType::GE_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            opJump(node, MidType::JUMP_GEQ, t1, t2);
            break;
        }
        case NodeType::LT_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            opJump(node, MidType::JUMP_LSS, t1, t2);
            break;
        }
        case NodeType::LE_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            opJump(node, MidType::JUMP_LEQ, t1, t2);
            break;
        }
        case NodeType::EQ_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            opJump(node, MidType::JUMP_EQ, t1, t2);
            break;
        }
        case NodeType::AND_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            if (node->stmtType == StatementType::CONDITION) {
                backPatch(node->children[0]->T, node->children[1]->B);
                node->B = node->children[0]->B;
                node->T = node->children[1]->T;
                node->F = merge(node->children[0]->F, node->children[1]->F);
            } else {
                name = tmpVar();
                codes.push_back(link(MidType::AND, t1, t2, name));
            }
            break;
        }
        case NodeType::OR_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            if (node->stmtType == StatementType::CONDITION) {
                backPatch(node->children[0]->F, node->children[1]->B);
                node->B = node->children[0]->B;
                node->T = merge(node->children[0]->T, node->children[1]->T);
                node->F = node->children[1]->F;
            } else {
                name = tmpVar();
                codes.push_back(link(MidType::OR, t1, t2, name));
            }
            break;
        }
        case NodeType::NOT_EXP: {
            string t = generateMidCode(node->children[0]);
            if (node->stmtType == StatementType::CONDITION) {
                node->B = node->children[0]->B;
                node->T = node->children[0]->F;
                node->F = node->children[0]->T;
            } else {
                name = tmpVar();
                codes.push_back(link(MidType::NOT, t, "", name));
            }
            break;
        }
        case NodeType::PLUS_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            name = tmpVar();
            codes.push_back(link(MidType::ADD, t1, t2, name));
            break;
        }
        case NodeType::SUB_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            name = tmpVar();
            codes.push_back(link(MidType::SUB, t1, t2, name));
            break;
        }
        case NodeType::MUL_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            name = tmpVar();
            codes.push_back(link(MidType::MUL, t1, t2, name));
            break;
        }
        case NodeType::DIV_EXP: {
            string t1 = generateMidCode(node->children[0]);
            string t2 = generateMidCode(node->children[1]);
            name = tmpVar();
            codes.push_back(link(MidType::DIV, t1, t2, name));
            break;
        }
        case NodeType::FACTOR: {
            name = node->token->token;
            if (node->varType == VarType::VT_BOOL && node->stmtType == StatementType::CONDITION) {
                // 如果原子类型是bool类型而且是条件语句，需要进行跳转判断
                node->T = codes.size();
                node->B = node->T;
                node->F = node->T + 1;
                codes.push_back(link(MidType::JUMP_EQ, node->token->token, "true", ""));
                codes.push_back(link(MidType::JUMP, "", "", ""));
            }
            break;
        }
        default:
            break;
    }
    return name;
}
