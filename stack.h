#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct{
    void* element;
    int elemSize;
    int allocLength;
    int elemNum;
}Stack;

void stackCreate(Stack* s, int elemSize);
void stackDestroy(Stack* s);
void stackPush(Stack* s, void* element);
void stackPop(Stack* s, void* element);



