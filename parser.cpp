#include "parser.h"
#include <fstream>
#include <map>
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include <cstring>
#include <queue>


std::map<char, Production*> wfgMap;

std::map<std::string, int> termMap;
/*
int table_id;


*/
#define debug
void Node::show()
{
    if (iskey) {
        std::cout << kwords::getStr(val.code) << " ";
    }
    else {
        std::cout << val.prod << " ";
    }
}
bool Node::equal(Node* oth)
{
    if (iskey != oth->iskey) return false;
    if (iskey) {
        if (val.code != oth->val.code) return false;
    }
    else {
        if (val.prod != oth->val.prod) return false;
    }
    return true;
}

NodeChain::NodeChain(char ch = 0)
{
    left = ch;
    Node* _head = new Node();
    head = _head;
    tail = _head;
    nxt = NULL;
    len = 0;
}
void NodeChain::addNode(Node* node)
{
    tail->nxt = node;
    tail = node;
    len++;
}
void NodeChain::show()
{
    
    Node* p;
    std::cout << '\n' << "  :";
    for (p = head->nxt; p; p = p->nxt) {
        p->show();
    }
}

Production::Production(char x)
{
    left = x;
    NodeChain* _nodechain = new NodeChain();
    head = _nodechain;
    tail = _nodechain;
    firstInited = false;
    firstIniting = false;
}
void Production::addChain(NodeChain* nodechain)
{
    tail->nxt = nodechain;
    tail = nodechain;
}
void Production::showChains()
{
    std::cout << left;
    NodeChain* p;
    for (p = head->nxt; p; p = p->nxt) {
        p->show();
    }
    std::cout << "\n";
}
void Production::initFirst()
{
    if (firstInited) return;
    firstIniting = true;
    
    firstSet.clear();
    NodeChain* p_nodechain;
    Node* p_node;
    Production* p_production;
    std::set<int>::iterator it;
    for (p_nodechain = head->nxt; p_nodechain; p_nodechain = p_nodechain->nxt) {
        for (p_node = p_nodechain->head->nxt; p_node; p_node = p_node->nxt) {
            if (p_node->iskey) {
                firstSet.insert(p_node->val.code);
                break;
            }
            char nxtPro = p_node->val.prod;
            p_production = findPro(nxtPro);
            bool haveNull = false;
            if (p_production->firstIniting == false) {
                p_production->initFirst();
                for (it = p_production->firstSet.begin(); 
                    it != p_production->firstSet.end(); it++) {
                    firstSet.insert(*it);
                    if (*it == kwords::getInt("nullprod")) {
                        haveNull = true;
                    }
                }
            }
            if (haveNull == false) {
                break;
            }
        }
    }
    firstIniting = false;
    firstInited = true;
}
void Production::showFirst()
{
    std::cout << left << "  :first set:\n    ";
    for (std::set<int>::iterator it = firstSet.begin(); it != firstSet.end(); it++) {
        std::cout << kwords::getStr(*it) << " ";
    }
    std::cout << std::endl;
}

Node::Node(char* ch)
{
    int code;
    nxt = NULL;
    code = kwords::getInt(std::string(ch));
    // dot = false;
    // std::cout << ch << ":" << code << std::endl;
    if (code == kwords::getInt(std::string("id"))) {
        // suppose that ch is not a key word or temination
        // in other word, so it is something like "E"
        iskey = false;
        val.prod = ch[0];
    }
    else {
        iskey = true;
        val.code = code;
    }
}

void readinput(std::string filename)
{
    std::ifstream read_file;
    read_file.open(filename);
    char buffer[256];
    if (!read_file) {
        std::cout << "file open failed!^" << std::endl;
        return;
    }
    while (!read_file.eof()) {
        char word[20];
        int word_id = 0;
        memset(word, 0, sizeof(char) * 20);
        read_file.getline(buffer, 255);
        // std::cout << "get buffer:" << buffer << std::endl;
        char left = buffer[0];
        if (left == '/' || left == '\n' || left == '\r' || left == '\0') continue;
        
        Production* p = findPro(left);
        NodeChain* nodechain = new NodeChain(left);
        for (int i = 3; ; i++) {
            if (buffer[i] == ' ' || buffer[i] == 0 || buffer[i] == '\n') {
                // std::cout << "word:" << word << std::endl;
                // split is right
                if (word_id == 0) {
                    if (buffer[i] == ' ') continue;
                    break;
                }
                Node* new_node = new Node(word);
                nodechain->addNode(new_node);
                
                // prepare to read the next
                word_id = 0;
                memset(word, 0, sizeof(char) * 20);
                if (buffer[i] == 0) break;
                continue;
            }
            word[word_id++] = buffer[i];
        }
        p->addChain(nodechain);
    }
    std::cout << "end read:" << wfgMap.size() << std::endl;
}

Production* findPro(char x) {
    std::map<char, Production*>::iterator itr = wfgMap.find(x);

    if (itr != wfgMap.end()) {
        return itr->second;
    }
    // not find
    Production* pro = new Production(x);
    wfgMap[x] = pro;
    return pro;
}

void showinputwfg()
{
    std::cout << "showinputwfg" << std::endl;
    for (std::map<char, Production*>::iterator itr = wfgMap.begin(); 
        itr != wfgMap.end(); itr++) {
        Production* p = itr->second;
        p->showChains();
    }
}

void initFirst()
{
    for (std::map<char, Production*>::iterator itr = wfgMap.begin(); 
        itr != wfgMap.end(); itr++) {
        Production* p = itr->second;
        p->initFirst();
    }
}

void showFirst()
{
    for (std::map<char, Production*>::iterator itr = wfgMap.begin();
        itr != wfgMap.end(); itr++) {
        Production* p = itr->second;
        p->showFirst();
    }
}


// CompilerTable cpltb;

LRitem::LRitem(char x = '0', NodeChain* _nodechain = NULL)
{
    nodechain = _nodechain;
    cur = 0;
    left = x;
    for (int i = 0; i < MAX_SYM; i++) {
        symbol[i] = false;
    }
    curNode = nodechain->head->nxt;
}
bool LRitem::isSub(LRitem* oth)
{
    if (oth->nodechain != nodechain || oth->cur != cur 
    || oth->left != left) {
        return false;
    }
    for (int i = 0; i < MAX_SYM; i++) {
        if (symbol[i] == true && oth->symbol[i] == false) {
            return false;
        }
    }
    return true;
}
void LRitem::show()
{
    std::cout << left << "->";
    int dot = 0;
    Node* outnode = nodechain->head->nxt;
    while (outnode) {
        if (dot++ == cur) {
            std::cout << ". ";
        }
        outnode->show();
        outnode = outnode->nxt;
    }
    std::cout << "\t\t\tsymbolSet: ";
    for (int i = 0; i < MAX_SYM; i++) {
        if (symbol[i]) {
            std::cout << kwords::getStr(i) << " ";
        }
    }
    std::cout<<"\n";
}
NodeChain* LRitem::checkReduce(SymStack &stk, int word_code)
{
    //std::cout << "checkReduce:" << word_code << std::endl;
    if (symbol[word_code] == false) return NULL;
    int len = nodechain->len;
    Node* cmpNode = nodechain->head->nxt;
    Node* getNode;
    //std::cout << "ccccccccc" << std::endl;
    for (int i = len; i > 0; i--) {
        getNode = stk.get(i);
        if (getNode->equal(cmpNode) == false) return NULL;
        cmpNode = cmpNode->nxt;
    }
    //std::cout << "rr:" << nodechain << std::endl;
    return nodechain;
}

Closure::Closure()
{
    items.clear();
}
int Closure::addItem(LRitem* item){
    bool add = false;
    bool beContained = false;
    std::set<LRitem*>::iterator it;
    for (it = items.begin(); it != items.end(); it++) {
        if ((*it)->nodechain == item->nodechain  
        && (*it)->cur == item->cur && (*it)->left == item->left) {
            beContained = true;
            for (int i = 0; i < MAX_SYM; i++) {
                if (item->symbol[i]) {
                    if ((*it)->symbol[i] == false) {
                        add = true;
                        beContained = false;
                    }
                    (*it)->symbol[i] = true;
                }
            }
        }
        if (add || beContained) break;
    }
    if (beContained) return 0;
    if (add) return 1;
    items.insert(item);
    return 1;
}
Closure::~Closure()
{
    std::set<LRitem*>::iterator it;
    for (it = items.begin(); it != items.end(); it++) {
        delete (*it);
    }
}
int Closure::make_red()
{
    memset(reduce, 0, sizeof(reduce));
    std::set<LRitem*>::iterator it;
    LRitem* tp;
    for (int i = 0; i < MAX_SYM; i++) {
        for (it = items.begin(); it != items.end(); it++) {
            tp = *it;
            if (tp->symbol[i] && tp->cur == tp->nodechain->len) {
                if (reduce[i]) {
                    std::cout << "reduce conflict" << std::endl;
                    return 1;
                }
                reduce[i] = tp->nodechain;
            }
        }
    }
    return 0;
}    
bool Closure::isSub(Closure* otherClo)
{
//  if (otherClo->items.size() == 0) std::cout << "attention !!!!!!!!" << std::endl;
   // if (otherClo->items.size() == 0 || items.size() == 0) return false;
    std::set<LRitem*>::iterator it;
    std::set<LRitem*>::iterator it2;
    for (it = items.begin(); it != items.end(); it++) {
        LRitem* cur = *it;
        bool curIsContained = false;
        for (it2 = otherClo->items.begin(); it2 != otherClo->items.end(); it2++) {
            LRitem* cur2 = *it2;
            if (otherClo->items.size() == 0) while(1) std::cout << "sd";
            if (cur->isSub(cur2)) {
                curIsContained = true;
                break;
            }
        }
        /*
        if (otherClo->items.size() == 0) {
            std::cout << "curis.." << curIsContained;
            std::cout << std::endl;
        }
        */
        if (curIsContained == false) {
            return false;
        }
    }
//    if (otherClo->items.size() == 0) 
//        std::cout << "attention end return true!!!!!!!!" << std::endl;
    return true;
}
void Closure::show()
{
    for (std::set<LRitem*>::iterator it = items.begin(); it != items.end(); it++) {
        (*it)->show();
    }
}
NodeChain* Closure::checkReduce(SymStack &stk, int word_code)
{
    NodeChain* res;
    //std::cout << "sss: closure begin" << std::endl;
    for (std::set<LRitem*>::iterator it = items.begin(); it != items.end(); it++) {
        if ((res = (*it)->checkReduce(stk, word_code)) != NULL) {
            //std::cout << "gua+++++++" << std::endl;
            return res;
        }
    }
    //std::cout << "gua----------" << std::endl;
    return NULL;
}

void makeClosure(Closure* clo)
{
    // for every item ``[A->alpha . B beta, a]'' in I, 
    // every production [B -> gama], and every b in FirstSet(beta a)
    // add the item : [B->. gama, b]
    std::queue<LRitem*> Q;
    for (std::set<LRitem*>::iterator it = clo->items.begin();
    it != clo->items.end(); it++) {
        Q.push(*it);
    }
    
    // std::cout << "?" ;
    while (!Q.empty()) {
        LRitem* tmpitem = Q.front();
        Q.pop();
        Node* curnode = tmpitem->curNode;
        if (!curnode || curnode->iskey) continue;
        
        // for every terminal in FirstSet(beta symbol), 
        // and for every production 'prod', 
        // we add [prod -> .gama, b] in this closure
        
        // find production 
        char prod = curnode->val.prod;
        Production* pro = findPro(prod);
        // find FirstSet
        bool tmpset[MAX_SYM];
        for (int i = 0; i < MAX_SYM; i++) {
            tmpset[i] = false;
        }
        Node* findFirstNode = curnode->nxt;
        bool goNext = true; // to find the firstSet of a sentence
        while (goNext && findFirstNode) {
            goNext = false;
            if (findFirstNode->iskey) {
                tmpset[findFirstNode->val.code] = true;
                break;
            }
            char curProd = findFirstNode->val.prod;
            Production* curPro = findPro(curProd);
            for (std::set<int>::iterator it = curPro->firstSet.begin();
            it != curPro->firstSet.end(); it++) {
                tmpset[*it] = true;
                if ((*it) == kwords::getInt("nullprod")) goNext = true;
            }
            findFirstNode = findFirstNode->nxt;
        }
        if (goNext) {
            for (int i = 0; i < MAX_SYM; i++) {
                if (tmpitem->symbol[i]) {
                    tmpset[i] = true;
                }
            }
        }
        // now get the production and symbol
        
        // add the new item
        NodeChain* tmpNodeChain = pro->head->nxt;
        /*
        if (tmpNodeChain) {
            std::cout<<"-----debug----";
            tmpNodeChain->show();
            std::cout<<"---------"<<std::endl;
        }
        */
        int res;
        while (tmpNodeChain) {
            LRitem* additem = new LRitem(prod, tmpNodeChain);
            for (int i = 0; i < MAX_SYM; i++) {
                if (tmpset[i]) {
                    additem->symbol[i] = true;
                }
            }
            if ((res = clo->addItem(additem)) == 1) {
                Q.push(additem);
            }
            tmpNodeChain = tmpNodeChain->nxt;
        }
        /*
        std::cout << "---" << std::endl;
        clo->show();
        */
    }
    //
}

int CompilerTable::initGoto(std::string filename, char s)
{
    readinput(filename);
    showinputwfg();
    initFirst();
    showFirst();
    //
    std::cout << "======" << std::endl;
    // showClosures();
    Production* firstPro = findPro(s);
    
    memset(charGoto, -1, sizeof(int) * MAX_CLOSURE * MAX_SYM);
    memset(symGoto, -1, sizeof(int) * MAX_CLOSURE * MAX_SYM);
    cur_closure = 0;
    // make first item 
    NodeChain* ndc = firstPro->head->nxt;
    if (ndc->nxt) {
        std::cout << "not S'" << std::endl;
        return 1;
    }
    LRitem* firstItem = new LRitem(firstPro->left, ndc);
    firstItem->symbol[kwords::getInt("endw")] = true;
    // first Closure
    Closure* firstClo = new Closure();
    firstClo->addItem(firstItem);
    makeClosure(firstClo);
    //
    closures[cur_closure++] = firstClo;
    // Init Gotofunction
    int makeGoto = 0;
    while (makeGoto < cur_closure) {
        Closure* clo = closures[makeGoto];
        std::set<LRitem*>::iterator it;
        bool haveSame;
        int gotores;
        Closure* newclo;
        for (int charorsym = 0; charorsym < MAX_SYM; charorsym++) {
            newclo = new Closure();
            for (it = clo->items.begin(); it != clo->items.end(); it++) {
                LRitem* tmp = *it;
                if (!tmp->curNode) continue;
                if (tmp->curNode->iskey) {
                    continue;
                }
                if ((int)(tmp->curNode->val.prod) != charorsym) {
                    continue;
                }
                
                LRitem* newitem = new LRitem(tmp->left, tmp->nodechain);
                newitem->cur = tmp->cur + 1;
                newitem->curNode = tmp->curNode->nxt;
                memcpy(newitem->symbol, tmp->symbol, sizeof(bool) * MAX_SYM);
                
                newclo->addItem(newitem);
            }
            if (newclo->items.size() == 0) {
                //std::cout << charorsym << std::endl;
                continue;
            }
            makeClosure(newclo);
            
            haveSame = false;
            gotores = -1;
            for (int i = 0; i < cur_closure; i++) {
                if ((newclo->isSub(closures[i])) && (closures[i]->isSub(newclo))) {
                    delete newclo;
                    //std::cout << "delete" << std::endl;
                    haveSame = true;
                    gotores = i;
                    break;
                }
            }
            //std::cout << haveSame << std::endl;
            //std::cout << gotores << std::endl;
            if (haveSame) {
                charGoto[makeGoto][charorsym] = gotores;
            } else {
                closures[gotores = cur_closure++] = newclo;
                charGoto[makeGoto][charorsym] = gotores;
            }
            //std::cout << cur_closure << std::endl;
            //std::cout << "haveSame:" << haveSame << std::endl;
            
            if (cur_closure >= MAX_CLOSURE) {
                std::cout << "closure too much" << std::endl;
                break;
            }
        }
        ///////// 
        // the symGoto
        for (int charorsym = 0; charorsym < MAX_SYM; charorsym++) {
            newclo = new Closure();
            for (it = clo->items.begin(); it != clo->items.end(); it++) {
                LRitem* tmp = *it;
                if (!tmp->curNode) continue;
                if (!tmp->curNode->iskey) {
                    continue;
                }
                if (tmp->curNode->val.code != charorsym) {
                    continue;
                }
                
                LRitem* newitem = new LRitem(tmp->left, tmp->nodechain);
                newitem->cur = tmp->cur + 1;
                newitem->curNode = tmp->curNode->nxt;
                memcpy(newitem->symbol, tmp->symbol, sizeof(bool) * MAX_SYM);
                
                newclo->addItem(newitem);
            }
            if (newclo->items.size() == 0) {
                //std::cout << charorsym << std::endl;
                continue;
            }
            makeClosure(newclo);
            
            haveSame = false;
            gotores = -1;
            for (int i = 0; i < cur_closure; i++) {
                if ((newclo->isSub(closures[i])) && (closures[i]->isSub(newclo))) {
                    delete newclo;
                    haveSame = true;
                    gotores = i;
                    break;
                }
            }
            if (haveSame) {
                symGoto[makeGoto][charorsym] = gotores;
            } else {
                closures[gotores = cur_closure++] = newclo;
                symGoto[makeGoto][charorsym] = gotores;
            }
            
            if (cur_closure >= MAX_CLOSURE) {
                std::cout << "closure too much" << std::endl;
                break;
            }
        }
        makeGoto++;
        if (cur_closure >= MAX_CLOSURE) {
            std::cout << "closure too much" << std::endl;
            break;
        }
    }
    std::cout << "init goto end" << std::endl;
    showClosures();
    std::cout << "make table begin" << std::endl;
    for (int i = 0; i < cur_closure; i++) {
        if (closures[i]->make_red()) return 1;
    }
    return 0;
}

CompilerTable::CompilerTable()
{
    memset(charGoto, -1, sizeof(int) * MAX_CLOSURE * MAX_SYM);
    memset(symGoto, -1, sizeof(int) * MAX_CLOSURE * MAX_SYM);
}
void CompilerTable::showClosures()
{
    for (int i = 0; i < cur_closure; i++) {
        std::cout << "closure" << i << std::endl;
        closures[i]->show();
    }
    std::cout << "---------" << std::endl;
    int gotoj;
    for (int i = 0; i < cur_closure; i++) {
        std::cout << "closure" << i << std::endl;
        for (int j = 0; j < MAX_SYM; j++) {
            if ((gotoj = symGoto[i][j]) != -1) {
                std::cout << "  " << kwords::getStr(j) << " to " << gotoj << std::endl;
            }
            if ((gotoj = charGoto[i][j]) != -1) {
                std::cout << "  " << char(j) << " to " << gotoj << std::endl;
            }
        }
    }
}

void SymStack::push(Node node)
{
    nodes[cur++] = node;
    if (cur >= MAX_STACK) {
        std::cout << "MAX_STACK" << std::endl;
    }
}
void SymStack::pop(int n = 1)
{
    while (n--)
        cur--;
}
Node* SymStack::get(int n)
{
    return nodes + cur - n;
}
void SymStack::show()
{
    for (int i = 0; i < cur; i++) {
        nodes[i].show();
    }
    std::cout << std::endl;
}

void StateStack::push(int x) {
    s[cur++] = x;
    if (cur >= MAX_STACK) {
        std::cout << "MAX_STACK" << std::endl;
    }
}
void StateStack::pop(int n = 1)
{
    while (n--)
        cur--;
}
int StateStack::top()
{
    return s[cur - 1];
}
void StateStack::show()
{
    for (int i = 0; i < cur; i++) {
        std::cout << s[i] << " ";
    }
    std::cout << std::endl;
}
void CompilerTable::read_source(Word* words[], int words_n)
{
    stateStack.cur = 0;
    symStack.cur = 0;
    stateStack.push(0);
    
    Word* curword;
    int word_code;
    int cur_state;
    int nxt_state;
    Node pushnode = Node();
    NodeChain* ndc = 0;
    char left;
    int i = 0;
    while (i < words_n) {
        curword = words[i];
        cur_state = stateStack.top();
        
        std::cout << "cur_state:" << cur_state << std::endl;
        std::cout << "scan a code:" << kwords::getStr(curword->code) << std::endl;
        
        if ((nxt_state = symGoto[cur_state][curword->code]) != -1) {
            stateStack.push(nxt_state);
            pushnode.iskey = true;
            pushnode.val.code = curword->code;
            symStack.push(pushnode);
            i++;
        }
        else {
            ndc = closures[cur_state]->reduce[curword->code];
            if (!ndc) {
                std::cout << "no reduce" << std::endl;
                return;
            }
            // pop and goto
            std::cout << "reduce:" << ndc->left;
            ndc->show();
            std::cout << std::endl;
            stateStack.pop(ndc->len);
            symStack.pop(ndc->len);
            left = ndc->left;
            pushnode.iskey = false;
            pushnode.val.prod = ndc->left;
            symStack.push(pushnode);
            nxt_state = charGoto[stateStack.top()][ndc->left];
            stateStack.push(nxt_state);
            //
            if (ndc && ndc->left == 'S') {
                std::cout << "acc" << std::endl;
                break;
            }
        }
        std::cout << "-------" << std::endl;
        stateStack.show();
        symStack.show();
        std::cout << "-------" << std::endl;
    }
}
/*
void CompilerTable::read_source(Word* words[], int words_n)
{
    stateStack.cur = 0;// clear stateStack
    symStack.cur = 0; // clear symStack
    // 
    stateStack.push(0);
    
    int word_code;
    Closure* p_clo;
    int cur_state;
    int nxt_state;
    NodeChain* ndc=0;
    int len;
    Node pushNode = Node();
    std::cout << "==========" << std::endl;
    Node* showNode;
    
    int cur_word = 0;
    Word* word;
    while (cur_word < words_n) {
        std::cout << "cur_word:" << cur_word << std::endl;
        word = words[cur_word];
        cur_word++;
        if ((word_code = word->code) == WORD_NOTINGEND) continue;
        if (word_code == 0) word_code = kwords::getInt("endw");
        if (word->code == kwords::getInt("id") || word->code == kwords::getInt("num")) word_code = kwords::getInt("var");
        // std::cout << "word code:" << word->code << std::endl;
        cur_state = stateStack.top();
        
        std::cout << "word_code:" << word_code << std::endl;
#ifdef debug
        std::cout << "scan a code:" << word_code << std::endl;
        std::cout << "-------" << std::endl;
        stateStack.show();
        symStack.show();
        std::cout << "-------" << std::endl;
#endif
        while ((nxt_state = symGoto[cur_state][word_code]) == -1) {
            p_clo = closures[cur_state];
            if ((ndc = p_clo->reduce[word_code]) == 0) {
                // std::cout << "cur state:" << cur_state << std::endl;
                // std::cout << "word_code:" << word_code << std::endl;
                //stateStack.show();
                //symStack.show();
                std::cout << "error:no reduce" << std::endl;
                break;
            }
            // reduce 
            std::cout << "--------------------\n";
            std::cout << "symbol stack:\t\t";
            symStack.show();
            std::cout << "-------------------- next code:\t\t";
            std::cout << kwords::getStr(word_code) << std::endl;
            std::cout << "reduce:\t\t" << ndc->left << "->";
            showNode = ndc->head->nxt;
            while (showNode) {
                showNode->show();
                showNode = showNode->nxt;
            }
            std::cout << std::endl;
            //
            len = ndc->len;
            stateStack.pop(len);
            symStack.pop(len);
#ifdef debug
            std::cout << "-------" << std::endl;
            stateStack.show();
            symStack.show();
            std::cout << "-------" << std::endl;
#endif
            // goto
            if (ndc->left == 'S') {
                std::cout << "acc" << std::endl;
                break;
            }
            cur_state = stateStack.top();
            // std::cout << "want goto:" << ndc->left << std::endl;
            cur_state = charGoto[cur_state][ndc->left];
            // std::cout << "goto:" << cur_state << std::endl;
            stateStack.push(cur_state);
            pushNode.iskey = false;
            pushNode.val.prod = ndc->left;
            symStack.push(pushNode);
#ifdef debug
            std::cout << "-------" << std::endl;
            stateStack.show();
            symStack.show();
            std::cout << "-------" << std::endl;
#endif
        }
        
        if (ndc) {
            std::cout << "+++++++++ = = ++++++++++" << std::endl;
            ndc->show();
            if (ndc->left == 'S') {
                break;
            }
        }
        // jump
        std::cout << "jump begin" << std::endl;
        if ((nxt_state = symGoto[cur_state][word_code]) != -1) {
            stateStack.push(nxt_state);
            pushNode.iskey = true;
            pushNode.val.code = word_code;
            symStack.push(pushNode);
#ifdef debug
            std::cout << "push:" << nxt_state << std::endl;
            std::cout << "-------" << std::endl;
            stateStack.show();
            symStack.show();
            std::cout << "-------" << std::endl;
#endif
            continue;
        }
        else {
            std::cout << "tran error" << std::endl;
            break;
        }
        if (word->code == 0) break;
    }
    // look_symboltable(table);
}
*/
/*
void Compiler::look_symboltable(TableEntry* table)
{
    for (int i = 0; i < table_id; i++) {
        table[i].look();
    }
    return;
}
*/