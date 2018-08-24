#pragma once

#include <set>
#include <string>
#include <fstream>

#include "keywords.h"
#include "lexer.h"
#include "symbol.h"

#define MAX_STACK 1000
#define MAX_CLOSURE 1000
#define MAX_SYM 256

void readinput(std::string filename);
void initFirst();

int isTermin(std::string str);

class Node
{
    public:
    bool iskey;
    union {
        int code;
        char prod;
    } val;
    Node* nxt;
    Node():nxt(0){}
    Node(char* ch);
    // bool dot;
    // TODO ~Node()
    void show();
    bool equal(Node*);
};

class NodeChain
{
    public:
    static int cnt;
    int id;
    int len;
    char left;
    Node* head;
    Node* tail;
    NodeChain(char);
    NodeChain* nxt;
    // TODO ~NodeChain();
    void addNode(Node* node);
    void show();
};

class Production{
    public:
    char left;
    NodeChain* head;
    NodeChain* tail;
    Production(char x);
    void addChain(NodeChain* nodechain);
    // TODO ~Production()
    void showChains();
    
    bool firstInited;
    bool firstIniting;
    void initFirst();
    void showFirst();
    std::set<int> firstSet;
};

Production* findPro(char x);




class SymStack
{
    public:
    Node nodes[MAX_STACK];
    int cur;
    SymStack():cur(0){}
    int push(Node);
    void pop(int);
    Node* get(int);
    void show();
};
class StateStack
{
    public:
    int s[MAX_STACK];
    int cur;
    StateStack():cur(0){}
    void push(int);
    void pop(int);
    int top();
    void show();
};

class LRitem
{
    public:
    NodeChain* nodechain;
    int cur;
    Node* curNode;
    char left;
    bool symbol[MAX_SYM]; // set for the second of item
    LRitem(char x, NodeChain* _nodechain);
    bool isSub(LRitem*);
    NodeChain* checkReduce(SymStack&, int);
    
    void show();
};

class Closure
{
    public:
    // LRitem chain
    std::set<LRitem*> items;
    Closure();
    int addItem(LRitem*);
    ~Closure();
    bool isSub(Closure*);
    void show();
    NodeChain* checkReduce(SymStack&, int);
    int make_red();
    NodeChain* reduce[MAX_SYM];
};

class CompilerTable
{
    public:
    int cur_closure;
    Closure* closures[MAX_CLOSURE];
    int charGoto[MAX_CLOSURE][MAX_SYM];
    int symGoto[MAX_CLOSURE][MAX_SYM];
    int initGoto(std::string, char);
    void showClosures();
    CompilerTable();
    //
    void read_source(Word* words[], int words_n, SymbolTable&);
    StateStack stateStack;
    SymStack symStack;
};

void makeClosure(Closure* clo);
void initGoto(Production* firstPro);
void showClosures();

