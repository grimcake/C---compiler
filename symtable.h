#ifndef _NODE_H_
#define _NODE_H_
#include "node.h"
#include "stdlib.h"
#include <stdio.h>
#endif


enum Symtable_kind{
    INT,
    FLOAT,
    FUNCTION
};

struct s_data{    
    char s_name[33];
    int s_kind;
    int s_level;
    int s_offset;
    struct s_data* next;
};

struct Symtable{
    struct s_data* data;
    struct s_data* head;
    struct s_data* tail;
};

struct Symtable* SymtableCreate();
void SymtableInsert(struct Symtable *s,  char name[], int kind, int level, int offset);
void SymtableOutput(struct Symtable *s);


