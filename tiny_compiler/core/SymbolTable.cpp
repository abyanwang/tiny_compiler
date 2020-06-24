#include "SymbolTable.h"

const SymbolTable::Symbol SymbolTable::Symbol::NONE { -1, VarType::VT_VOID };

void SymbolTable::insert(std::string name, VarType type, int line) {
    if (table.count(name))
        table[name].lines.push_back(line);
    else {
        auto& symbol = table[name];
        symbol.type = type;
        symbol.location = location ++;
        symbol.lines.push_back(line);
    }
}

const SymbolTable::Symbol& SymbolTable::lookup(std::string name) {
    if (table.count(name))
        return table[name];
    return SymbolTable::Symbol::NONE;
}

void SymbolTable::print(std::ostream &out) const {
    out << "Variable Name      Type     Location    Line Numbers\n";
    out << "-------------    --------   --------    ------------\n";
    out.setf(std::ios::left);
    for (auto p : table){
        out.width(17);
        out << p.first;
        out.width(11);
        switch (p.second.type) {
            case VarType::VT_INT:
                out << "int";
                break;
            case VarType::VT_BOOL:
                out << "bool";
                break;
            case VarType::VT_STRING:
                out << "string";
                break;
            default:
                out << "unknown";
                break;
        }
        out.width(12);
        out << p.second.location;
        out.width();
        for (auto i : p.second.lines)
            out << std::to_string(i) + ", ";
        out << "\n";
    }
}
