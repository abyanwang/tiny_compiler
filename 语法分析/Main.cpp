#include <iostream>
#include <fstream>
#include <vector>
#include "core/Scanner.cpp"
#include "core/Helper.cpp"
#include "core/Log.h"
#include "core/Parser.cpp"
#include "core/TokenStream.h"
#include "core/TreeNode.cpp"
using namespace std;

int main	() {
    Log log(std::cerr);
    ifstream fin("experiment2_test1_in.txt");
    try {
        auto list = Scanner::scan(fin, log);
        TokenStream stream(list);
        Parser parser(stream, log);
        // Êä³ö
        cout << "\nAll tokens list:\n";
        for (auto &i : list)
            cout << "(" << Helper::getTokenTypeName(i.type) << ", " << i.token << ")" << endl;
            
        cout << endl << "syntax tree:" << endl;
        parser.getRootTreeNode()->print(cout);
    } catch (const string& err) {
        cerr << endl << err << endl;
    }
}
