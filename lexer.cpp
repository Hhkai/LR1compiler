#include "lexer.h"

#include <iostream>
#include <fstream>
#include <cstdio>

#include <assert.h>

int Automaton::scanChar(char c)
{
    switch (state) {
    case AUTO_NOTING:
        if (buffer_id == 1 && buffer[0] == '*') {
            if (c == '/') {
                // similar with noting begin 
                // we do not change state there but getWord()
                return RET_NOTINGEND;
            }
        }
        buffer_id = 0;
        if (c == '*') {
            buffer[buffer_id++] = c;
        }
        return 0;
    case AUTO_BEGIN:
        if (isdigit(c)) {
            state = AUTO_DIGIT;
            buffer[buffer_id++] = c;
            return 0;
        }
        if (isalpha(c)) {
            state = AUTO_LETTER;
            buffer[buffer_id++] = c;
            return 0;
        }
        // +-*/=
        buffer[buffer_id++] = c;
        switch (c) {
            case '+':
            case '-':
            case '=':
            case '/':
            case '*':
            case '&':
            case '|':
            case '<':
            case '>':
            case '!':
                state = AUTO_OPERATOR;
                return state;
            case ';':
                state = kwords::getInt(";");
                break;
            case '{':
                state = kwords::getInt("{");
                break;
            case '}':
                state = kwords::getInt("}");
                break;
            case '[':
                state = kwords::getInt("[");
                break;
            case ']':
                state = kwords::getInt("]");
                break;
            case '(':
                state = kwords::getInt("(");
                break;
            case ')':
                state = kwords::getInt(")");
                break;
        }
        return NEXT_ROUND;
    case AUTO_DIGIT:
        if (isdigit(c)) {
            buffer[buffer_id++] = c;
            return 0;
        }
        if (isalpha(c)) {
            state = AUTO_ERROR;
            return 0;
        }
        // digit end
        switch (c) {
            case '+':
            case '-':
            case '=':
            case '{':
            case '}':
            case '[':
            case ']':
            case '(':
            case ')':
            case ';':
            case '*':
            case '/':
            case '&':
            case '|':
            case '!':
                return RET_BREAK;
        }
        state = AUTO_ERROR;
        return 0;
    case AUTO_LETTER:
        if (isdigit(c) || isalpha(c)) {
            buffer[buffer_id++] = c;
            return 0;
        }
        switch (c) {
            case '+':
            case '-':
            case '=':
            case '{':
            case '}':
            case '[':
            case ']':
            case '(':
            case ')':
            case ';':
            case '*':
            case '/':
            case '&':
            case '|':
            case '!':
                return RET_BREAK;
        }
    case AUTO_OPERATOR:
        if (buffer_id == 1 && buffer[0] == '/') {
            // in these two kinds of noting state 
            // we need not change the state of automaton there
            // for we should return the word first
            // then the state will be set by /filescanner.cpp/word_scanner()
            
            // BUT 
            // the word we should return is '/' 
            // that means nothing
            if (c == '*') {
                return RET_MULTINOTE;
            }
            if (c == '/') {
                return RET_SINGLENOTE;
            }
        }
        if (isdigit(c) || isalpha(c)) {
            return RET_BREAK;
        }
        if (buffer_id == 2) return RET_BREAK;
        buffer[buffer_id++] = c;
        return 0;
    default:
        state = AUTO_ERROR;
    }

    return -1;
}

Word* Automaton::getWord()
{
    Word* word = new Word();
    int code;
    switch (state) {
    case AUTO_NOTING:
        word->code = WORD_NOTINGEND;
        return word;
    case AUTO_DIGIT:
        word->code = kwords::getInt("num");
        return word;
    case AUTO_OPERATOR:
    case AUTO_LETTER:
        code = kwords::getInt(buffer);
        word->code = code;
        return word;
    default:
        word->code = state;
        return word;
    }
    return NULL;
}

Word* word_scanner(std::ifstream& file) {
    if (!file) {
        printf("file open failed!^");
        return NULL;
    }
    static char buffer1[256] = "";
    // static char buffer2[256];
    static int id1 = 0;
    char x;
    bool word_has_begin = false;
    static bool multinote = false;
    static bool singlenote = false;
    while (buffer1[id1] == 0 && !file.eof()) {
        file.getline(buffer1, 256);
        std::cout << "get buffer:\t\t" << buffer1 << std::endl;
        id1 = 0;
    }
    x = buffer1[id1];
    Automaton mata;
    if (multinote) {
        mata.state = AUTO_NOTING;
    }
    while ((x=buffer1[id1]) != 0) {
        assert(id1 < 256);
        // if (x == '<') std::cout<<mata.state<<std::endl;
        int temp_scanc;
        int breakloop = false;
        // std::cout << "x=" << int(x) << "id1:" << id1 << std::endl;
        if (isspace(x)) {
            if (word_has_begin) {
                break;
            }
            id1++;
            continue;
        }
        word_has_begin = true;
        // automaton
        switch (temp_scanc = mata.scanChar(x)) {
            case RET_NOTINGEND:
                id1++;
                breakloop = true;
                multinote = false;
                break;
            case RET_BREAK: 
                // there is still one char in buffer1 
                // but the word should return now
                breakloop = true;
                break;
            case NEXT_ROUND: 
                id1++;
                breakloop = true;
                break;
            case RET_MULTINOTE:
                multinote = true;
                breakloop = true;
                id1++;
                break;
            case RET_SINGLENOTE:
                singlenote = true;
                id1 = 255;
                break;
            default:
                id1++;
        }
        if (breakloop) break;
    }
    Word* word = mata.getWord();
    if (singlenote) {
        word->code = WORD_NOTINGEND;
        singlenote = false;
        return word;
    }
    if (multinote) {
        word->code = WORD_NOTINGEND;
        return word;
    }
    
    if (!word || word->code == -1) return NULL;
    /*
    if (word->code == kwords::getInt("id") || word->code == kwords::getInt("num")) {
        word->pos = signIn(mata.buffer, word->code, 0, 0);
    }
    */
    
    return word;
}

int getWords(Word* words[], std::string filename)
{
    std::ifstream read_file;
    read_file.open(filename);
    int cnt = 0;
    Word* word;
    while ((word = word_scanner(read_file)) != 0) {
        if (word->code == WORD_NOTINGEND) continue;
        if (word->code == 0) word->code = kwords::getInt("endw");
        if (word->code == kwords::getInt("id") || word->code == kwords::getInt("num")) word->code = kwords::getInt("var");
        words[cnt++] = word;
        if (word->code == kwords::getInt("endw") || word->code == 0) break;
    }
    return cnt;
}