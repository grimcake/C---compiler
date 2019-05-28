#include "symtable.h"
#include <malloc.h>
#include <string.h>
#include "display.h"
#include "stack.h"

const int DX = 0;
Stack * SymStack;
struct Symtable* ext_symtable; //外部变量符号表

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

/*
 * 输出符号表
 */
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
 * 遍历抽象语法树，生成符号表
 */

void deal_astTree(NODE T){
    SymStack = stackCreate(4);
    ext_symtable = SymtableCreate(); //外部变量符号表
    stackPush(SymStack, ext_symtable);
    //SymtableInsert(ext_symtable, (char *)"ss", 0, 0, 0);
    Ast_To_Symtable(T);
    SymtableOutput(ext_symtable);
}

/*
 * 根据文法处理各层节点属性
 */
void Ast_To_Symtable(NODE T){

    if(T){
        switch(T->kind){
        case EXT_DEF_LIST_NODE:
            if(!T->ptr[0]) break;
            T->ptr[0]->offset = T->offset;
            Ast_To_Symtable(T->ptr[0]);
            if(T->ptr[1]){
                T->ptr[1]->offset = T->ptr[0]->offset+T->ptr[0]->width;
                Ast_To_Symtable(T->ptr[1]);
            }
            break;
        case EXT_VAR_DEF_NODE: //外部变量定义
            T->type = T->ptr[1]->type = !(strcmp(T->ptr[0]->type_id, "int"))?S_INT:S_FLOAT;
            T->ptr[1]->offset = T->offset;
            T->ptr[1]->width = (T->type == S_INT)? 4:8;
            ext_var_list(T->ptr[1]);
            T->width = T->ptr[1]->num*((T->type == S_INT)? 4:8);
            break;
        case TYPE_NODE:
            break;
        case EXT_DEC_LIST_NODE:
            break;
        case FUNC_DEF_NODE: //函数体定义
            T->ptr[1]->type = !(strcmp(T->ptr[0]->type_id, "int"))?S_INT:S_FLOAT;
            T->width = 0;
            T->offset = DX;
            Ast_To_Symtable(T->ptr[1]);
            T->offset += T->ptr[1]->offset;
            T->ptr[2]->offset = T->offset;
            Ast_To_Symtable(T->ptr[2]);


            break;

        case FUNC_DEC_NODE:
            break;
        case PARAM_LIST_NODE:
            break;
        case PARAM_DEC_NODE:
            break;
        case EXP_STMT_NODE:
            break;
        case RETURN_NODE:
            break;
        case COMP_STM_NODE:
            break;
        case STM_LIST_NODE:
            break;
        case WHILE_NODE:
            break;
        case IF_THEN_ELSE_NODE:
            break;
        case DEF_LIST_NODE:
            break;
        case VAR_DEF_NODE:
            break;
        case DEC_LIST_NODE:
            break;
        case ID_NODE:
            break;
        case INT_NODE:
            break;
        case RELOP_NODE:
        case ASSIGNOP_NODE:
        case AND_NODE:
        case OR_NODE:
        case PLUS_NODE:
        case MINUS_NODE:
        case STAR_NODE:
        case DIV_NODE:
            break;
        default:
            break;
        }

    }
}

void ext_var_list(node * T){
    int num = 1;
    switch(T->kind){
        case EXT_DEC_LIST_NODE:
            T->ptr[0]->type = T->type;
            T->ptr[0]->offset = T->offset;
            T->ptr[1]->type = T->type;
            T->ptr[1]->offset = T->offset+T->width;
            T->ptr[1]->width = T->width;
            ext_var_list(T->ptr[0]);
            ext_var_list(T->ptr[1]);
            T->num = T->ptr[1]->num+1;
            break;
        case ID_NODE:
            SymtableInsert(ext_symtable, T->type_id, T->type, 0, T->offset);
            T->num = 1;
            break;
        default:
            break;
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
