#ifndef _STACK_H
#define _STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "symtable.h"


typedef struct StackElem{
    Symtable* elem;
    struct StackElem* pre;
    struct StackElem* next;
}StackElem;

typedef struct Stack{
    StackElem* elem;
    StackElem* top;
    int elemNum;
}Stack;

Stack* stackCreate();
void stackDestroy(Stack* s);
void stackPush(Stack* s, Symtable* element);
Symtable* stackPop(Stack* s);
void stackOutput(Stack *s);


#endif

