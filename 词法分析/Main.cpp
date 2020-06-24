#include <iostream>
#include <fstream>
#include <vector>
#include "core/Scanner.cpp"
#include "core/Helper.cpp"
#include "core/Log.h"

using namespace std;

int main() {
    Log log(std::cerr);
    ifstream fin("test.txt");
    freopen("test_out.txt","w",stdout);
    try {
        auto list = Scanner::scan(fin, log);
        // Êä³ö
        cout << "\nAll tokens list:\n";
        for (auto &i : list)
            cout << "(" << Helper::getTokenTypeName(i.type) << ", " << i.token << ")" << endl;
    } catch (const string& err) {
        cerr << endl << err << endl;
    }
}
