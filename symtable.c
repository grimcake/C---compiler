#include "symtable.h"
#include <malloc.h>
#include <string.h>
#include "display.h"
#include "stack.h"
#include "ircode.h"
#define _SYMTABLETEST

/*
 * 符号表创建
 */
struct Symtable* SymtableCreate(){
    struct Symtable* s = (struct Symtable *)malloc(sizeof(struct Symtable));
    if(s == NULL){
        printf("symtable malloc error\n");
        return NULL;
    }
    //s->data = NULL;
    s->head = NULL;
    s->tail = NULL;
    return s;
}

/*
 * 符号表插入一条记录
 */
s_data* SymtableInsert(struct Symtable *s,  char name[], char *alias, int kind, int level, int offset, int type, int num){
    struct s_data* newnode;
    newnode = (struct s_data*)malloc(sizeof(struct s_data));
    strcpy(newnode->s_name, name);
    strcpy(newnode->alias, alias);
    newnode->s_kind = kind;
    newnode->s_level = level;
    newnode->s_offset = offset;
    newnode->s_type = type;
    newnode->s_num = num;
    if(s->tail == NULL){
        s->head = s->tail = newnode;
    }
    else{
        s->tail->next = newnode;
        s->tail = s->tail->next;
    }
    return newnode;
}


/*
 * 符号表中插入临时变量
 */
s_data* SymtableInsertTemp(struct Symtable *s,  char *alias, int kind, int level, int offset, int type, int num){
    struct s_data* newnode;
    newnode = (struct s_data*)malloc(sizeof(struct s_data));
    strcpy(newnode->s_name, "");
    strcpy(newnode->alias, alias);
    newnode->s_kind = kind;
    newnode->s_level = level;
    newnode->s_offset = offset;
    newnode->s_type = type;
    newnode->s_num = num;
    if(s->tail == NULL){
        s->head = s->tail = newnode;
    }
    else{
        s->tail->next = newnode;
        s->tail = s->tail->next;
    }
    return newnode;
}
/*
 * 输出符号表
 */
void SymtableOutput(struct Symtable *s){
    struct s_data* p = NULL;
    if(s == NULL) return;
    p = s->head;
    printf("name\talias\tkind\tlevel\toffset\ttype\tnum\n");
    while(p)
    {
        printf("%s\t%s\t%d\t%d\t%d\t%d\t%d\n", p->s_name, p->alias, p->s_kind, p->s_level, p->s_offset, p->s_type, p->s_num);
        if(p == s->tail){
            break;
        }
        else p = p->next; 
    }
    printf("\n");
}


/*
 * 遍历抽象语法树，生成符号表
 */



/*
 * 判断表达式中的变量是否在符号表中出现
 */


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
