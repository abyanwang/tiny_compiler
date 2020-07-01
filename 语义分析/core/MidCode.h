#ifndef TINY_COMPILE_CODE_H
#define TINY_COMPILE_CODE_H

#include <string>
#include <vector>
#include "TreeNode.h"

enum class MidType {
	NONE, START, END,
	READ, WRITE, ASSIGN,
	JUMP, JUMP_LSS, JUMP_GRT,
	JUMP_EQ, JUMP_LEQ, JUMP_GEQ,
	ADD, SUB, MUL, DIV,
	AND, OR, NOT
};

//µ¥ÅÅµÄmidcode 
struct MidCode {
    int jump = -1;
    int pos = 0;
    MidCode* next = NULL;

    MidType type = MidType::NONE;
    string param1, param2, param3;

    string statement();
};

class MidCodeGenerator {
	    vector<MidCode*> codes;
	
	    // temporary variable
	    // to create a temporary variable
	    int tid = 0;
	
	private:
	    string tmpVar() {
	        return "t" + to_string(tid++);
	    }
	
	    MidCode* link(MidType type, const string& param1, const string& param2, const string& param3);
	    void backPatch(int a, int b);
	    int merge(int a, int b);
	    void opJump(TreeNode *node, MidType type, const string &param1, const string &param2);
	    string generateMidCode(TreeNode* node);
	
	public:
	    MidCodeGenerator(TreeNode* root);
	    ~MidCodeGenerator();
	    vector<MidCode*> getCodes() {
	        return codes;
	    }
};

#endif //TINY_COMPILE_CODE_H
