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
    K_FUNC,
    K_TEMP
};

typedef struct s_data{    
    char s_name[33];
    int s_kind;
    int s_level;
    int s_offset;
    int s_type;
    int s_num; //参数个数，用于函数
    char alias[33]; //别名
    struct s_data* next;
}s_data;

typedef struct Symtable{
    struct s_data* data;
    struct s_data* head;
    struct s_data* tail;
}Symtable;

struct Symtable* SymtableCreate();
s_data* SymtableInsert(struct Symtable *s,  char name[], char *alias, int kind, int level, int offset, int type, int num);
s_data* SymtableInsertTemp(struct Symtable *s,  char *alias, int kind, int level, int offset, int type, int num);
void SymtableOutput(struct Symtable *s);
#endif
