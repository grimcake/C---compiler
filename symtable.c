#include "symtable.h"
#include <malloc.h>
#include <string.h>

struct Symtable* SymtableCreate(){
    struct Symtable* s = (struct Symtable *)malloc(sizeof(struct Symtable));
    if(s == NULL){
        printf("symtable malloc error\n");
        return NULL;
    }
    s->data = NULL;
    s->head = NULL;
    s->tail = NULL;
    return s;
}


void SymtableInsert(struct Symtable *s,  char name[], int kind, int level, int offset){
    struct s_data* newnode;
    newnode = (struct s_data*)malloc(sizeof(struct s_data));
    strcpy(newnode->s_name, name);
    newnode->s_kind = kind;
    newnode->s_level = level;
    newnode->s_offset = offset;
    if(s->tail == NULL){
        s->head = s->tail = newnode;
    }
    else{
        s->tail->next = newnode;
        s->tail = s->tail->next;
    }
}


void SymtableOutput(struct Symtable *s){
    struct s_data* p = NULL;
    if(s == NULL) return;
    p = s->head;
    while(1)
    {
        printf("%s %d %d %d\n", p->s_name, p->s_kind, p->s_level, p->s_offset);
        if(p == s->tail){
            break;
        }
        else p = p->next; 
    }
}

/*
int main(){
    struct Symtable* s = NULL;
    s = SymtableCreate();
    char str[3]={"dx"};
    SymtableInsert(s, str, 1,1,1);
    SymtableInsert(s, (char*)"dd", 1,1,1);
    SymtableInsert(s, str, 1,1,1);
    SymtableOutput(s);
    return 0;
}
*/
