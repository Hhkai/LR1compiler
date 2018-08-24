#include "reduce.h"

#include <iostream>
#include <cstdio>

TreeNode tree[1000];
ThreeAddCode tac;

int ThreeAddCode::output()
{
    std::ofstream fout;
    fout.open("output3.txt");
    int cnt = 0;
    int i = first;
    while (i != -1 && cnt++ < 100) {
        fout << codes[i].content << std::endl;
        i = codes[i].next;
    }
    fout.close();
    return 0;
}

int ndcReduce(int id, int pos, int entry, SymbolTable& symboltable)
{
    int tp_entry;
    int tp_add;
    int tp_temp;
    Code* p_code;
    SymbolItem* p_item;
    int temp_code_id;
    int temp_lable;
    switch (id) {
        case 1:
        // acc
        tac.output();
        break;
        
        case 6:
        // Y : int
        tree[pos].type = 1; // int
        break;
        
        case 7:
        // Y : int
        tree[pos].type = 4; // bool
        break;
        
        case 9:
        // Z : var
        //symboltable.showItem(entry);
        tree[pos].type = 2; // var
        tree[pos].var = entry;
        break;
        
        case 3:
        // X : Y Z ;
        tp_entry = tree[pos + 1].var;
        //symboltable.showItem(tp_entry);
        symboltable.symbols[tp_entry].var_type = tree[pos].type;
        break;
        
        case 22:
        // F : var
        //symboltable.showItem(entry);
        // 
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        tree[pos].first = tac.cur;
        tree[pos].last = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        tp_temp = tac.temp++;
        p_item = &(symboltable.symbols[entry]);
        //
        tree[pos].type = 3; // T 
        tree[pos].var = tp_temp;
        if (p_item->type == 1) {
            // num 
            sprintf(p_code->content, "T%d = %d", tp_temp, p_item->value);
        }
        else if (p_item->type == 2) {
            // id
            sprintf(p_code->content, "T%d = V%d", tp_temp, entry);
        }
        break;
        
        case 23:
        // F : false 
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        tree[pos].first = tac.cur;
        tree[pos].last = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        tp_temp = tac.temp++;
        sprintf(p_code->content, "T%d = false", tp_temp);
        tree[pos].type = 3; // T
        tree[pos].var = tp_temp;
        break;
        
        case 33:
        // L : var
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        tree[pos].first = tac.cur;
        tree[pos].last = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        tp_temp = tac.temp++;
        p_item = &(symboltable.symbols[entry]);
        //
        tree[pos].type = 3; // T 
        tree[pos].var = tp_temp;
        if (p_item->type == 1) {
            // num 
            sprintf(p_code->content, "T%d = %d", tp_temp, p_item->value);
        }
        else if (p_item->type == 2) {
            // id
            sprintf(p_code->content, "T%d = V%d", tp_temp, entry);
        }
        break;
        
        case 58:
        // D : var
        tree[pos].type = 5; // addr
        tree[pos].var = entry;
        break;
        
        case 19:
        // T : F
        break;
        
        case 15:
        // E : T 
        break;
        
        case 48:
        // Q : D = E 
        tp_add = tree[pos].var;
        tp_temp = tree[pos + 2].var;
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        // tree[pos].first ;
        tree[pos].last = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        sprintf(p_code->content, "V%d = T%d", tp_add, tp_temp);
        break;
        
        case 13:
        // E : E + T 
        tp_temp = tac.temp++;
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        // tree[pos].first ;
        tree[pos].last = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        sprintf(p_code->content, "T%d = T%d + T%d", tp_temp, tree[pos].var, tree[pos + 2].var);
        tree[pos].var = tp_temp;
        break;
        
        case 25:
        // E : L < E
        tp_temp = tac.temp++;
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        // tree[pos].first ;
        tree[pos].last = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        sprintf(p_code->content, "T%d = T%d < T%d", tp_temp, tree[pos].var, tree[pos + 2].var);
        tree[pos].var = tp_temp;
        break;
        
        case 49:
        // Q : D ++
        tp_add = tree[pos].var;
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        tree[pos].last = tac.cur;
        tree[pos].first = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        sprintf(p_code->content, "V%d = V%d + 1", tp_add, tp_add);
        break;
        
        case 52:
        // Q : ++ D
        tp_add = tree[pos + 1].var;
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        tree[pos].first = tac.cur;
        tree[pos].last = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        sprintf(p_code->content, "V%d = V%d + 1", tp_add, tp_add);
        break;
        
        case 34:
        // E : ! E
        tp_temp = tac.temp++;
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        tree[pos].first = tree[pos + 1].first;
        tree[pos].last = tac.cur;
        
        p_code = &(tac.codes[tac.cur++]);
        sprintf(p_code->content, "T%d = ! T%d", tp_temp, tree[pos + 1].var);
        tree[pos].var = tp_temp;
        break;
        
        case 36:
        // A : C A 
        tree[pos].last = tree[pos + 1].last;
        break;
        
        case 45:
        // B : while ( E ) { A }
        
        // L1: 
        // E
        // ifFalse E goto L2
        // A
        // goto L1 
        // L2:
        
        // now we insert ``L1:'' 
        p_code = &(tac.codes[tac.cur]);
        sprintf(p_code->content, "L%d:", tac.lable++);
        
        temp_code_id = tree[pos + 2].first;  // first of E
        temp_code_id = tac.codes[temp_code_id].prev; // last of the block before E
        tac.codes[temp_code_id].next = tac.cur;
        p_code->prev = temp_code_id;
        
        temp_code_id = tree[pos + 2].first; // first of E
        p_code->next = temp_code_id;
        tac.codes[temp_code_id].prev = tac.cur;
        
        tree[pos].first = tac.cur++;
        
        // insert ``ifFalse E goto L2''
        p_code = &(tac.codes[tac.cur]);
        sprintf(p_code->content, "ifFalse T%d goto L%d", tree[pos + 2].var, tac.lable++);
        
        temp_code_id = tree[pos + 2].last;
        tac.codes[temp_code_id].next = tac.cur;
        p_code->prev = temp_code_id;
        
        temp_code_id = tree[pos + 5].first;
        tac.codes[temp_code_id].prev = tac.cur;
        p_code->next = temp_code_id;
        
        ++tac.cur;
        
        // append ``goto L1'' and ``L2:''
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        p_code = &(tac.codes[tac.cur++]);
        sprintf(p_code->content, "goto L%d", tac.lable - 2);
        
        tac.codes[tac.last].next = tac.cur;
        tac.codes[tac.cur].prev = tac.last;
        tac.last = tac.cur;
        p_code = &(tac.codes[tac.cur]);
        sprintf(p_code->content, "L%d:", tac.lable - 1);
        
        tree[pos].last = tac.cur++;
        break;
        
        /*
        case 11:
        // R : Q ;
        case 38:
        case 40:
        break;
        */
        
        default:
        std::cout << ">>>> ndcReduce: " << id << " " << pos << std::endl;
        std::cout << "err: ndcReduce():line: " << __LINE__;
        std::cout << "            ___________" << std::endl;
        return 1;
    }
    return 0;
}