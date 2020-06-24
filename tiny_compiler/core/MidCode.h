#ifndef TINY_COMPILE_CODE_H
#define TINY_COMPILE_CODE_H

#include <string>
#include <vector>
#include "TreeNode.h"

struct MidCode {
    enum class Type {
        NONE, START, END,
        READ, WRITE, ASSIGN,
        JUMP, JUMP_LSS, JUMP_GRT,
        JUMP_EQ, JUMP_LEQ, JUMP_GEQ,
        ADD, SUB, MUL, DIV,
        AND, OR, NOT
    };

    int jump = -1;
    int pos = 0;
    MidCode* next = nullptr;

    Type type = Type::NONE;
    std::string param1, param2, param3;

    std::string quaternary();
    std::string statement();
};

class MidCodeGenerator {
    std::vector<MidCode*> codes;

    // temporary variable
    // to create a temporary variable
    // @see MidCodeGenerator::new_tmp_var()
    int t_id = 0;

private:
    inline std::string new_tmp_var() {
        return "_t" + std::to_string(t_id++);
    }

private:
    MidCode* _emit(MidCode::Type type, const std::string& param1, const std::string& param2, const std::string& param3);
    void back_patch(int a, int b);
    int merge(int a, int b);
    void opJump(TreeNode *node, MidCode::Type type, const std::string &param1, const std::string &param2);
    std::string generate(TreeNode* node);

public:
    explicit MidCodeGenerator(TreeNode* root);
    ~MidCodeGenerator();
    inline std::vector<MidCode*> getCodes() {
        return codes;
    }
};

#endif //TINY_COMPILE_CODE_H
