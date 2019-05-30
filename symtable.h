#ifndef _SYMTABLE_H
#define _SYMTABLE_H
#include "node.h"
#include "stdlib.h"
#include <stdio.h>


enum Symtable_type{
    S_TEST,
    S_INT,
    S_FLOAT,
    S_FUNCTION
};

enum Symtable_kind{
    K_TEST,
    K_VAL,
    K_FUNC
};

struct s_data{    
    char s_name[33];
    int s_kind;
    int s_level;
    int s_offset;
    int s_type;
    struct s_data* next;
};

typedef struct Symtable{
    struct s_data* data;
    struct s_data* head;
    struct s_data* tail;
}Symtable;

struct Symtable* SymtableCreate();
void SymtableInsert(struct Symtable *s,  char name[], int kind, int level, int offset, int type);
void SymtableOutput(struct Symtable *s);
void SymtableDestroy(Symtable* s);
void deal_astTree(NODE T);
void Ast_To_Symtable(NODE T);
void var_list(node * T);
void local_var_list(node* T);
#endif
