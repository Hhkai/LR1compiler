#include "symbol.h"
#include <cstring>
#include <iostream>

int SymbolTable::signIn(int type, int value)
{
    SymbolItem tp = SymbolItem(type, value);
    symbols.push_back(tp);
    return symbols.size() - 1;
}

int SymbolTable::signInId(char* buffer) {
    int id = 0;
    for (auto x : symbols) {
        if (x.type == 2 && strcmp(buffer, x.name) == 0) {
            return id;
        }
        ++id;
    }
    SymbolItem tp;
    tp.type = 2;
    strcpy(tp.name, buffer);
    symbols.push_back(tp);
    return symbols.size() - 1;
}

int SymbolTable::signInNum(int val) {
    int id = 0;
    for (auto x : symbols) {
        if (x.type == 1 && x.value == val) {
            return id;
        }
        ++id;
    }
    SymbolItem tp;
    tp.type = 1;
    tp.value = val;
    symbols.push_back(tp);
    return symbols.size() - 1;
}

void SymbolTable::show() {
    int id = 0;
    for (auto x : symbols) {
        std::cout << id << ":  \t" << "type: " << x.type;
        if (x.type == 2) {
            std::cout << "  \t name: " << x.name;
        }
        std::cout << "  \t value: " << x.value << std::endl;
        ++id;
    }
}