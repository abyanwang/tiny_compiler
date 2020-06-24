#ifndef TINY_COMPILE_SYMBOL_TABLE_H
#define TINY_COMPILE_SYMBOL_TABLE_H

#include <vector>
#include <string>
#include <ostream>
#include <unordered_map>
#include "VarType.h"

class SymbolTable {
public:
    struct Symbol {
        int location;
        VarType type;
        std::vector<int> lines;

        static const Symbol NONE;
    };

private:
    int location = 0;
    std::unordered_map<std::string, Symbol> table;

public:
    void insert(std::string name, VarType type, int line);
    const Symbol& lookup(std::string name);
    void print(std::ostream &out) const;
};

#endif //TINY_COMPILE_SYMBOL_TABLE_H
