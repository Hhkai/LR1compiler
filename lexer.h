#pragma once

#include <cstring>
#include <map>

#define BUFFER_SIZE 100

#define AUTO_BEGIN 0
#define AUTO_LETTER 1
#define AUTO_DIGIT 2
#define AUTO_ERROR -1
#define AUTO_OPERATOR 3
#define NEXT_ROUND 100
#define RET_BREAK 101
#define RET_MULTINOTE 102
#define RET_SINGLENOTE 103
#define AUTO_NOTING 4
#define RET_NOTINGEND 104
//
#define WORD_NOTINGEND -10

#define MAXWORDS 1000

#include "keywords.h"

struct Word
{
    int code;
    int val;
};

struct Automaton
{
    Automaton():state(AUTO_BEGIN), buffer_id(0)
    {
        memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
    };
    int state;
    char buffer[BUFFER_SIZE];
    int buffer_id;
    int scanChar(char c);
    Word* getWord();
};

int getWords(Word* words[], std::string filename);

Word* word_scanner(std::ifstream& file);
int isKeyWord(std::string x);