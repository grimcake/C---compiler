#include "symtable.h"
#include <malloc.h>
#include <string.h>

void SymtableCreate(Symtable *s, char name[], int kind, int level, int offset){
    s = (Symtable *)malloc(sizeof(Symtable));
    if(s == NULL){
        printf("symtable malloc error\n");
        return;
    }
    strcpy(s->s_name, name);
    s->s_kind = kind;
    s->s_level = level;
    s->s_offset = offset;
}
