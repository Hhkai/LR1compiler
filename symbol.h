#pragma once

#include <vector>

class SymbolItem
{
    public:
    int type;  // num->1 id->2
    int value;
    char name[80];
    SymbolItem(int _type, int _value) : type(_type), value(_value) {}
    SymbolItem() {}
    int var_type;
};

class SymbolTable 
{
    public:
    std::vector<SymbolItem> symbols;
    int signIn(int, int);
    int signInId(char*);
    int signInNum(int);
    void show();
    void showItem(int);
};