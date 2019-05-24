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

typedef struct{
    char s_name[33];
    int s_kind;
    int s_level;
    int s_offset;
}Symtable;

void SymtableCreate(Symtable *s, char name[], int kind, int level, int offset);


