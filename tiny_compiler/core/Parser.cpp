#include "Parser.h"
#include "Helper.h"

void Parser::parse() {
    root = program();
    checkType(root);
    checkStatementType(root);
    if (log.hasError()) {
        std::stringstream msg;
        msg << "You have " << log.getErrorCount() << " errors.";
        throw msg.str();
    }
}

bool Parser::match(Token::Type expected, bool flag) {
    if (token.type == expected) {
        last_token = token;
        token = _interface.nextToken();
        return true;
    } else {
        if (flag) {
            if (token.type == Token::Type::NO_MORE_TOKEN) {
                log.parse_error("expected token " + Helper::getTokenTypeName(expected)
                    + ", but there is no more token", last_token.line, last_token.offset);
            } else {
                log.parse_error("unexpected token: " + token.token + ", "
                    + Helper::getTokenTypeName(expected) + " expected", token.line, token.offset);
            }
        }
        return false;
    }
}

TreeNode* Parser::program() {
    token = _interface.nextToken();
    declarations();
    TreeNode* body = stmt_sequence();
    if (token.type != Token::Type::NO_MORE_TOKEN)
        log.parse_error("无效的符号，之后的输入将被忽略", token.line, token.offset);
    return body;
}

void Parser::declarations() {
    VarType type = VarType::VT_VOID;
    while (match(Token::Type::KEY_INT) || match(Token::Type::KEY_BOOL) || match(Token::Type::KEY_STRING)) {
        switch (last_token.type) {
            case Token::Type::KEY_INT:
                type = VarType::VT_INT;
                break;
            case Token::Type::KEY_BOOL:
                type = VarType::VT_BOOL;
                break;
            case Token::Type::KEY_STRING:
                type = VarType::VT_STRING;
                break;
            default:
                log.parse_error("the token can not be parsed to a type: " + last_token.token, last_token.line, last_token.offset);
                break;
        }
        do {
            // 期望获取一个标识符
            if (!match(Token::Type::ID, true))
                break;

            // 插入符号表
            const SymbolTable::Symbol& symbol = symbol_table.lookup(last_token.token);
            if (&symbol == &SymbolTable::Symbol::NONE)
                symbol_table.insert(last_token.token, type, last_token.line);
            else {
                std::stringstream msg;
                msg << "the variable " << last_token.token << " has already declared in line " << symbol.lines[0];
                log.parse_error(msg.str(), last_token.line, last_token.offset);
            }
        } while(match(Token::Type::OP_COMMA));
        match(Token::Type::OP_SEMICOLON); // 分号可有可无
    }
}

TreeNode* Parser::stmt_sequence() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::STMT_SEQUENCE);
    node->children[0] = statement();
    if (node->children[0] == nullptr) {
        delete node;
        return nullptr;
    }
    match(Token::Type::OP_SEMICOLON); // 分号可有可无
    node->children[1] = stmt_sequence();
    if (node->children[1] == nullptr) {
        TreeNode* tmp = node->children[0];
        node->children[0] = nullptr;
        delete node;
        node = tmp;
    }
    return node;
}

TreeNode* Parser::statement() {
    TreeNode* node = nullptr;
    switch (token.type) {
        case Token::Type::KEY_IF:
            node = if_stmt();
            break;
        case Token::Type::KEY_READ_:
            node = read_stmt();
            break;
        case Token::Type::KEY_REPEAT:
            node = repeat_stmt();
            break;
        case Token::Type::KEY_WRITE_:
            node = write_stmt();
            break;
        case Token::Type::KEY_WHILE:
            node = while_stmt();
            break;
        case Token::Type::ID:
            node = assign_stmt();
            break;
        default:
            break;
    }
    return node;
}

TreeNode* Parser::if_stmt() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::IF_STMT);
    match(Token::Type::KEY_IF, true);
    node->children[0] = or_exp();
    match(Token::Type::KEY_THEN, true);
    node->children[1] = stmt_sequence();
    if (match(Token::Type::KEY_ELSE))
        node->children[2] = stmt_sequence();
    match(Token::Type::KEY_END, true);
    return node;
}

TreeNode* Parser::repeat_stmt() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::REPEAT_STMT);
    match(Token::Type::KEY_REPEAT, true);
    node->children[0] = stmt_sequence();
    match(Token::Type::KEY_UNTIL, true);
    node->children[1] = or_exp();
    return node;
}

TreeNode* Parser::assign_stmt() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::ASSIGN_STMT);
    if (token.type == Token::Type::ID)
        node->children[0] = factor();
    match(Token::Type::OP_ASSIGN, true);
    node->token = new Token(last_token);
    node->children[1] = or_exp();
    return node;
}

TreeNode* Parser::read_stmt() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::READ_STMT);
    match(Token::Type::KEY_READ_, true);
    node->token = new Token(last_token);
    if (token.type == Token::Type::ID)
        node->children[0] = factor();
    else if (token.type == Token::Type::NO_MORE_TOKEN)
        log.parse_error("unexpected token: " + token.token, last_token.line, last_token.offset);
    else
        log.parse_error("unexpected token: " + token.token, token.line, token.offset);
    return node;
}

TreeNode* Parser::write_stmt() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::WRITE_STMT);
    match(Token::Type::KEY_WRITE_, true);
    node->token = new Token(last_token);
    node->children[0] = or_exp();
    return node;
}

TreeNode* Parser::while_stmt() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::WHILE_STMT);
    match(Token::Type::KEY_WHILE, true);
    node->children[0] = or_exp();
    match(Token::Type::KEY_DO, true);
    node->children[1] = stmt_sequence();
    match(Token::Type::KEY_END, true);
    return node;
}

TreeNode* Parser::or_exp() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::OR_EXP);
    node->children[0] = and_exp();
    if (match(Token::Type::KEY_OR)) {
        node->token = new Token(last_token);
        node->children[1] = or_exp();
    }
    else {
        TreeNode* tmp = node->children[0];
        node->children[0] = nullptr;
        delete node;
        node = tmp;
    }
    return node;
}

TreeNode* Parser::and_exp() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::AND_EXP);
    node->children[0] = comparison_exp();
    if (match(Token::Type::KEY_AND)) {
        node->token = new Token(last_token);
        node->children[1] = and_exp();
    }
    else {
        TreeNode* tmp = node->children[0];
        node->children[0] = nullptr;
        delete node;
        node = tmp;
    }
    return node;
}

TreeNode* Parser::comparison_exp() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::NONE);
    node->children[0] = add_sub_exp();
    switch (token.type) {
        case Token::Type::OP_LSS:
            match(Token::Type::OP_LSS, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::LT_EXP;
            node->children[1] = comparison_exp();
            break;
        case Token::Type::OP_LEQ:
            match(Token::Type::OP_LEQ, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::LE_EXP;
            node->children[1] = comparison_exp();
            break;
        case Token::Type::OP_GTR:
            match(Token::Type::OP_GTR, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::GT_EXP;
            node->children[1] = comparison_exp();
            break;
        case Token::Type::OP_GEQ:
            match(Token::Type::OP_GEQ, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::GE_EXP;
            node->children[1] = comparison_exp();
            break;
        case Token::Type::OP_EQU:
            match(Token::Type::OP_EQU, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::EQ_EXP;
            node->children[1] = comparison_exp();
            break;
        default: {
            TreeNode* tmp = node->children[0];
            node->children[0] = nullptr;
            delete node;
            node = tmp;
            break;
        }
    }
    return node;
}

TreeNode* Parser::add_sub_exp() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::NONE);
    node->children[0] = mul_div_exp();
    switch (token.type) {
        case Token::Type::OP_ADD:
            match(Token::Type::OP_ADD, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::PLUS_EXP;
            node->children[1] = add_sub_exp();
            break;
        case Token::Type::OP_SUB:
            match(Token::Type::OP_SUB, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::SUB_EXP;
            node->children[1] = add_sub_exp();
            break;
        default: {
            TreeNode* tmp = node->children[0];
            node->children[0] = nullptr;
            delete node;
            node = tmp;
            break;
        }
    }
    return node;
}

TreeNode* Parser::mul_div_exp() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::NONE);
    node->children[0] = factor();
    switch (token.type) {
        case Token::Type::OP_MUL:
            match(Token::Type::OP_MUL, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::MUL_EXP;
            node->children[1] = mul_div_exp();
            break;
        case Token::Type::OP_DIV:
            match(Token::Type::OP_DIV, true);
            node->token = new Token(last_token);
            node->type = TreeNode::NodeType::DIV_EXP;
            node->children[1] = mul_div_exp();
            break;
        default: {
            TreeNode* tmp = node->children[0];
            node->children[0] = nullptr;
            delete node;
            node = tmp;
            break;
        }
    }
    return node;
}

TreeNode* Parser::factor() {
    TreeNode* node = TreeNode::create(TreeNode::NodeType::FACTOR);
    switch (token.type) {
        case Token::Type::ID: {
            node->token = new Token(token);
            const SymbolTable::Symbol& symbol = symbol_table.lookup(token.token);
            if(&symbol == &SymbolTable::Symbol::NONE)
                log.parse_error("the symbol " + token.token + " is not declared", token.line, token.offset);
            else {
                node->varType = symbol.type;
                symbol_table.insert(token.token, node->varType, token.line);
            }
            match(Token::Type::ID, true);
            break;
        }
        case Token::Type::NUMBER:
            node->token = new Token(token);
            node->varType = VarType::VT_INT;
            match(Token::Type::NUMBER, true);
            break;
        case Token::Type::STRING:
            node->token = new Token(token);
            node->varType = VarType::VT_STRING;
            match(Token::Type::STRING, true);
            break;
        case Token::Type::KEY_TRUE:
            node->token = new Token(token);
            node->varType = VarType::VT_BOOL;
            match(Token::Type::KEY_TRUE, true);
            break;
        case Token::Type::KEY_FALSE:
            node->token = new Token(token);
            node->varType = VarType::VT_BOOL;
            match(Token::Type::KEY_FALSE, true);
            break;
        case Token::Type::OP_LP:
            node->type = TreeNode::NodeType::OR_EXP;
            match(Token::Type::OP_LP, true);
            node = or_exp();
            match(Token::Type::OP_RP, true);
            break;
        case Token::Type::KEY_NOT:
            node->type = TreeNode::NodeType::NOT_EXP;
            match(Token::Type::KEY_NOT, true);
            node->token = new Token(last_token);
            node->children[0] = factor();
            break;
        default:
            if (token.type == Token::Type::NO_MORE_TOKEN)
                log.parse_error("unexpected token: " + token.token, last_token.line, last_token.offset);
            else
                log.parse_error("unexpected token: " + token.token, token.line, token.offset);
            delete node;
            node = nullptr;
            break;
    }
    return node;
}

void Parser::checkType(TreeNode *node) {
    if (!node)
        return;
    for (TreeNode* child : node->children)
        if (child)
            checkType(child);
    switch (node->type) {
        case TreeNode::NodeType::LT_EXP:
        case TreeNode::NodeType::LE_EXP:
        case TreeNode::NodeType::GT_EXP:
        case TreeNode::NodeType::GE_EXP:
            if (node->children[0] == nullptr || node->children[1] == nullptr)
                log.type_error("缺少操作数", node->token->line, node->token->offset);
            else if (node->children[0]->varType != VarType::VT_INT)
                log.type_error("比较运算符的操作数不是int类型", node->children[0]->token->line, node->children[0]->token->offset);
            else if (node->children[1]->varType != VarType::VT_INT)
                log.type_error("比较运算符的操作数不是int类型", node->children[1]->token->line, node->children[1]->token->offset);
            node->varType = VarType::VT_BOOL;
            if (!node->token) {
                node->token = new Token;
                node->token->line = node->children[1]->token->line;
                node->token->offset = node->children[1]->token->offset;
            }
            break;
        case TreeNode::NodeType::EQ_EXP:
            if (node->children[0] == nullptr || node->children[1] == nullptr)
                log.type_error("缺少操作数", node->token->line, node->token->offset);
            else if (node->children[0]->varType != node->children[1]->varType)
                log.type_error("相等运算类型不一致", node->children[1]->token->line, node->children[1]->token->offset);
            node->varType = VarType::VT_BOOL;
            if (!node->token) {
                node->token = new Token;
                node->token->line = node->children[1]->token->line;
                node->token->offset = node->children[1]->token->offset;
            }
            break;
        case TreeNode::NodeType::OR_EXP:
        case TreeNode::NodeType::AND_EXP:
            if (node->children[0] == nullptr || node->children[1] == nullptr)
                log.type_error("缺少操作数", node->token->line, node->token->offset);
            else if (node->children[0]->varType != VarType::VT_BOOL)
                log.type_error("逻辑运算符的操作数不是bool类型", node->children[0]->token->line, node->children[0]->token->offset);
            else if (node->children[1]->varType != VarType::VT_BOOL)
                log.type_error("逻辑运算符的操作数不是bool类型", node->children[1]->token->line, node->children[1]->token->offset);
            node->varType = VarType::VT_BOOL;
            if (!node->token) {
                node->token = new Token;
                node->token->line = node->children[1]->token->line;
                node->token->offset = node->children[1]->token->offset;
            }
            break;
        case TreeNode::NodeType::NOT_EXP:
            if (node->children[0] == nullptr)
                log.type_error("缺少操作数", node->token->line, node->token->offset);
            else if (node->children[0]->varType != VarType::VT_BOOL)
                log.type_error("逻辑运算符的操作数不是bool类型", node->children[0]->token->line, node->children[0]->token->offset);
            node->varType = VarType::VT_BOOL;
            if (!node->token) {
                node->token = new Token;
                node->token->line = node->children[0]->token->line;
                node->token->offset = node->children[0]->token->offset;
            }
            break;
        case TreeNode::NodeType::PLUS_EXP:
        case TreeNode::NodeType::SUB_EXP:
        case TreeNode::NodeType::MUL_EXP:
        case TreeNode::NodeType::DIV_EXP:
            if (node->children[0] == nullptr || node->children[1] == nullptr)
                log.type_error("缺少操作数", node->token->line, node->token->offset);
            else if (node->children[0]->varType != VarType::VT_INT)
                log.type_error("算术运算符的操作数不是int类型", node->children[0]->token->line, node->children[0]->token->offset);
            else if (node->children[1]->varType != VarType::VT_INT)
                log.type_error("算术运算符的操作数不是int类型", node->children[1]->token->line, node->children[1]->token->offset);
            node->varType = VarType::VT_INT;
            if (!node->token) {
                node->token = new Token;
                node->token->line = node->children[1]->token->line;
                node->token->offset = node->children[1]->token->offset;
            }
            break;
        case TreeNode::NodeType::IF_STMT:
        case TreeNode::NodeType::WHILE_STMT:
            if (node->children[0]->varType != VarType::VT_BOOL)
                log.type_error("IF/WHILE语句的判别式不是bool类型", node->children[0]->token->line, node->children[0]->token->offset);
            node->varType = VarType::VT_BOOL;
            break;
        case TreeNode::NodeType::ASSIGN_STMT:
            if (node->children[0] == nullptr || node->children[1] == nullptr)
                log.type_error("缺少操作数", node->token->line, node->token->offset);
            else if (node->children[0]->varType != node->children[1]->varType)
                log.type_error("赋值运算类型不一致", node->children[1]->token->line, node->children[1]->token->offset);
            node->varType = node->children[0]->varType;
            break;
        default:
            break;
    }
}

void Parser::checkStatementType(TreeNode *node, TreeNode::StatementType parentStmtType) {
    if (!node)
        return;
    switch (node->type) {
        case TreeNode::NodeType::ASSIGN_STMT:
            node->stmtType = TreeNode::StatementType::ASSIGN;
            break;
        case TreeNode::NodeType::IF_STMT:
        case TreeNode::NodeType::WHILE_STMT:
            node->stmtType = TreeNode::StatementType::CONDITION;
            break;
        default:
            node->stmtType = parentStmtType;
            break;
    }
    for (TreeNode* child : node->children)
        if (child)
            checkStatementType(child, node->stmtType);
}
