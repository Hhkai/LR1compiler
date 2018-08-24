#pragma once
#include <fstream>

#include "symbol.h"

class TreeNode
{
    public:
    int type;
    int var;
    
    int codeline;
    int first; // the first code
    int last; // the last code
    char name[80] = "";
};

class Code
{
    public:
    char content[80] = "";
    int next = -1;
    int prev;
};

class ThreeAddCode
{
    public:
    Code codes[10000];
    int cur = 0;
    int temp = 0;
    int output();
    int first = 0;
    int last = 0;
    int lable = 0;
};

int ndcReduce(int id, int pos, int, SymbolTable&);