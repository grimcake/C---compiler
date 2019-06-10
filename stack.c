#include "stack.h"
#include <string.h>


Stack* stackCreate(){
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->elem = NULL;
    s->top = NULL;
    s->elemNum = 0;
    return s;
}

void stackDestroy(Stack* s){
    if(s == NULL) return;
    StackElem* p = s->elem;
    while(p){
        StackElem* tmp = p;
        p = p->next;
        free(tmp);
    }
    free(s);
}

void stackPush(Stack* s, Symtable* element){
    if(s->elem == NULL){
        s->elem = (StackElem*)malloc(sizeof(StackElem));
        s->top = s->elem;
        s->top->elem = element;
        s->top->pre = NULL;
        s->top->next = NULL;
    }
    else{
        s->top->next = (StackElem*)malloc(sizeof(StackElem));
        s->top->next->pre = s->top;
        s->top = s->top->next;
        s->top->elem = element;
        s->top->next = NULL;
    }
    s->elemNum++;
}

Symtable* stackPop(Stack* s){
    if(s->elemNum <= 0){
        printf("stack is empty!\n");
        return NULL;
    }
    StackElem* topelem = s->top;
    s->top = s->top->pre;
    s->elemNum--;
    return topelem->elem;
}

/*
 * 从栈底到栈顶打印内容
 */

void stackOutput(Stack *s){
    StackElem *p = s->elem;
    while(p!=NULL){
       SymtableOutput(p->elem);
       if(p == s->top) break;
       p = p->next; 
    }
}

/*
typedef struct testnode{
    void *elem;
    int k;
}testnode;


int main(){
    int i;
    char c;
    i = 333;
    c = 'x';

    void *f;
    f = &i;
    printf("%d\n", *(int*)f);

    f = &c;
    printf("%c\n", *(char*)f);
    printf("%p \n", &i);
    printf("%p \n", &c);
    int g = 10, h = 12;

    Stack* s = NULL;
    s = stackCreate(sizeof(testnode));
    testnode *p;
    p = (testnode*)malloc(sizeof(testnode));
    p->k = 333;
    p->elem = (int *)malloc(sizeof(int));
    p->elem = &g;
    stackPush(s, p);

    testnode* top = (testnode*)malloc(sizeof(testnode));
    stackPop(s, top);
    top->elem = &h;

    printf("%d %d\n", top->k, *(int *)top->elem);
    stackPop(s, top);

}*/
