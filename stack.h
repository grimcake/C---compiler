#ifndef _STACK_H
#define _STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Stack{
    void* element;
    int elemSize;
    int allocLength;
    int elemNum;
}Stack;

Stack* stackCreate(int elemSize);
void stackDestroy(Stack* s);
void stackPush(Stack* s, void* element);
void stackPop(Stack* s, void* element);

#endif

