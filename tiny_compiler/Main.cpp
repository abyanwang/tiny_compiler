#include <iostream>
#include <fstream>
#include <vector>
#include "core/Scanner.cpp"
#include "core/Helper.cpp"
#include "core/Log.h"
#include "core/TokenStream.h"
#include "core/TreeNode.cpp"
#include "core/MidCode.cpp"
#include "core/Parser.cpp"
#include "core/SymbolTable.cpp"
using namespace std;

int main() {
    Log log(std::cerr);
    ifstream fin("experiment2_test2_in.txt");
    try {
        auto list = Scanner::scan(fin, log);
        TokenStream stream(list);
        Parser parser(stream, log);
        MidCodeGenerator coder(parser.getRootTreeNode());

        // 输出
        cout << "\nAll tokens list:\n";
        for (auto &i : list)
            cout << "(" << Helper::getTokenTypeName(i.type) << ", " << i.token << ")" << endl;

        cout << endl << "symbol table:" << endl;
        parser.getSymbolTable()->print(cout);

        cout << endl << "syntax tree:" << endl;
        parser.getRootTreeNode()->print(cout);

        cout << endl << "mid codes(statement):" << endl;
        vector<MidCode*> codes = coder.getCodes();
        for (int i = 0; i < codes.size(); ++i)
            cout << i << ") " << codes[i]->statement() << endl;
        cout << endl << "mid codes(quaternary):" << endl;
        for (int i = 0; i < codes.size(); ++i)
            cout << i << ") " << codes[i]->quaternary() << endl;
    } catch (const string& err) {
        cerr << endl << err << endl;
    }
}
