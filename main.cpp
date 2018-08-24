#include <cstdio>
#include <iostream>
#include <fstream>

#include "keywords.h"
#include "lexer.h"
#include "parser.h"
#include "symbol.h"

Word* words[1000];
SymbolTable symboltable;



int main()
{
    kwords::init_keywords("keywords.txt");
    int words_n = getWords(words, "source_using.txt", symboltable); // source file
    
    symboltable.show();
    
    CompilerTable cpltb;
    if (cpltb.initGoto("pro_using.txt", 'S')) {
        std::cout << "conflict" << std::endl;
        return 0;
    }
    //
    cpltb.read_source(words, words_n, symboltable);
    return 0;
}