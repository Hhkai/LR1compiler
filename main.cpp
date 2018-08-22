#include <cstdio>
#include <iostream>

#include "keywords.h"
#include "lexer.h"
#include "parser.h"
#include "symbol.h"

Word* words[1000];
SymbolTable symboltable;

int main()
{
    kwords::init_keywords("keywords.txt");
    int words_n = getWords(words, "source.txt", symboltable); // source file
    
    symboltable.show();
    return 0;
    
    CompilerTable cpltb;
    if (cpltb.initGoto("pro.txt", 'S')) {
        std::cout << "conflict" << std::endl;
        return 0;
    }
    //
    cpltb.read_source(words, words_n);
    return 0;
}