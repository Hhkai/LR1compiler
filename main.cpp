#include <cstdio>
#include <iostream>

#include "keywords.h"
#include "lexer.h"
#include "parser.h"

Word* words[1000];

int main(int argc, char* argv[])
{
    kwords::init_keywords("keywords.txt");
    int words_n = getWords(words, argv[2]);
    // test lexer:
    for (int i = 0; i < words_n; i++) {
        std::cout << kwords::getStr(words[i]->code) << std::endl;
    }
    // test end 
    CompilerTable cpltb;
    if (cpltb.initGoto(argv[1], 'S')) {
        std::cout << "conflict" << std::endl;
        return 0;
    }
    //
    cpltb.read_source(words, words_n);
    return 0;
}