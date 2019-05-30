#include "symtable.h"
#include <malloc.h>
#include <string.h>
#include "display.h"
#include "stack.h"

const int DX = 0;
Stack * SymStack;
struct Symtable* ext_symtable; //外部变量符号表
static int LEV;  //当前层
static Symtable* now_symtable; //当前作用域的符号表
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
void SymtableInsert(struct Symtable *s,  char name[], int kind, int level, int offset, int type){
    struct s_data* newnode;
    newnode = (struct s_data*)malloc(sizeof(struct s_data));
    strcpy(newnode->s_name, name);
    newnode->s_kind = kind;
    newnode->s_level = level;
    newnode->s_offset = offset;
    newnode->s_type = type;
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
    printf("name\tkind\tlevel\toffset\ttype\n");
    while(p)
    {
        printf("%s\t%d\t%d\t%d\t%d\n", p->s_name, p->s_kind, p->s_level, p->s_offset, p->s_type);
        if(p == s->tail){
            break;
        }
        else p = p->next; 
    }
    printf("\n");
}

/*
 * 销毁符号表
 */
void SymtableDestroy(Symtable *s){

}

/*
 * 遍历抽象语法树，生成符号表
 */

void deal_astTree(NODE T){
    SymStack = stackCreate();
    ext_symtable = SymtableCreate(); //外部变量符号表
    stackPush(SymStack, ext_symtable);
    LEV = 0;
    Ast_To_Symtable(T);
    stackOutput(SymStack);
    //SymtableOutput(ext_symtable);
    /*while(SymStack->elemNum){
        Symtable* tmp_symtable;
        tmp_symtable = stackPop(SymStack);
        SymtableOutput(tmp_symtable);
    }*/
    
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
            var_list(T->ptr[1]);
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
            T->offset += T->ptr[1]->width; //函数体内的偏移为DX加上参数的宽度
            T->ptr[2]->offset = T->offset;
            Ast_To_Symtable(T->ptr[2]);

            break;

        case FUNC_DEC_NODE:
            SymtableInsert(ext_symtable, T->type_id, K_FUNC, LEV, -1, T->type);
            T->offset = DX;
            LEV++;
            //有参数
            if(T->ptr[0]){
                Symtable* new_table = SymtableCreate();
                stackPush(SymStack, new_table);
                now_symtable = new_table;
                T->ptr[0]->offset = T->offset;
                Ast_To_Symtable(T->ptr[0]);
                T->width = T->ptr[0]->width;
            }
            else{
                T->width = 0;
            }
            LEV--; //参数和块语句同级，但是在compst才是真正的加
            break;
        case PARAM_LIST_NODE:
            T->ptr[0]->offset = T->offset;
            Ast_To_Symtable(T->ptr[0]);
            if(T->ptr[1]){
                //如果有多个参数
                T->ptr[1]->offset = T->ptr[0]->offset+T->ptr[0]->width;
                Ast_To_Symtable(T->ptr[1]);
                T->num = T->ptr[0]->num+T->ptr[1]->num;
                T->width = T->ptr[0]->width+T->ptr[1]->width;

            }
            else{
                T->num = T->ptr[0]->num; //不等于1，因为考虑数组
                T->width = T->ptr[0]->width;
            }

            break;
        case PARAM_DEC_NODE:
            T->type = T->ptr[1]->type = !(strcmp(T->ptr[0]->type_id, "int"))?S_INT:S_FLOAT;
            T->ptr[1]->offset = T->offset;
            T->ptr[1]->width = (T->type == S_INT)? 4:8;
            
            local_var_list(T->ptr[1]);
            T->width = T->ptr[1]->num*((T->type == S_INT)? 4:8);
            break;
        case EXP_STMT_NODE:
            T->ptr[0]->offset = T->offset;
            Ast_To_Symtable(T->ptr[0]);
            T->width = T->ptr[0]->width;
            break;
        case RETURN_NODE:
            break;
        case COMP_STM_NODE:
            LEV++;
            T->width = 0;
            if(T->ptr[0]){
                T->ptr[0]->offset = T->offset;
                if(LEV > 1){
                    Symtable * new_table = SymtableCreate(); //创建新作用域符号表
                    stackPush(SymStack, new_table); //将符号表入栈
                    now_symtable = new_table;
                }
                local_var_list(T->ptr[0]);
                T->width += T->ptr[0]->width;
            }
            if(T->ptr[1]){
                T->ptr[1]->offset = T->offset+T->width;
                Ast_To_Symtable(T->ptr[1]);
                T->width += T->ptr[1]->width;
            }

            LEV--;
            break;
        case STM_LIST_NODE:
            if(T->ptr[0] == NULL){
                //空语句
                T->width = 0;
                break;
            }
            T->ptr[0]->offset = T->offset;
            Ast_To_Symtable(T->ptr[0]);
            T->width = T->ptr[0]->width;
            if(T->ptr[1]){
                T->ptr[1]->offset = T->offset+T->ptr[0]->width;
                Ast_To_Symtable(T->ptr[1]);
            }
            break;
        case WHILE_NODE:
            T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
            T->width = T->ptr[0]->width;
            Ast_To_Symtable(T->ptr[1]);
            break;
        case IF_THEN_ELSE_NODE:
            T->ptr[1]->offset = T->offset;
            T->ptr[2]->offset = T->offset;
            Ast_To_Symtable(T->ptr[1]);
            Ast_To_Symtable(T->ptr[2]);
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

void var_list(node * T){
    switch(T->kind){
        case EXT_DEC_LIST_NODE:
            T->ptr[0]->type = T->type;
            T->ptr[0]->offset = T->offset;
            var_list(T->ptr[0]);
            if(T->ptr[1]){
                T->ptr[1]->type = T->type;
                T->ptr[1]->offset = T->offset+T->width;
                T->ptr[1]->width = T->width;
                var_list(T->ptr[1]);
                T->num = T->ptr[1]->num+1;
            }
            else{
                T->num = T->ptr[0]->num;
            }
            break;
        case ID_NODE:
            SymtableInsert(ext_symtable, T->type_id, K_VAL, LEV, T->offset, T->type);
            T->num = 1;
            break;
        default:
            break;
    }
}

void local_var_list(node* T){
    switch(T->kind){
        case DEF_LIST_NODE:
            if(!T->ptr[0]) break;
            T->ptr[0]->offset = T->offset;
            local_var_list(T->ptr[0]);
            T->width += T->ptr[0]->width;
            if(T->ptr[1]){
                T->ptr[1]->offset = T->ptr[0]->offset+T->ptr[0]->width;
                local_var_list(T->ptr[1]);
            }
            break;
        case VAR_DEF_NODE:
            T->type = T->ptr[1]->type = !(strcmp(T->ptr[0]->type_id, "int"))?S_INT:S_FLOAT;
            T->ptr[1]->offset = T->offset;
            T->ptr[1]->width = (T->type == S_INT)? 4:8;
            local_var_list(T->ptr[1]);
            T->width = T->ptr[1]->num*((T->type == S_INT)? 4:8);
            break;
        case DEC_LIST_NODE:
            T->ptr[0]->type = T->type;
            T->ptr[0]->offset = T->offset;
            local_var_list(T->ptr[0]);
            if(T->ptr[1]){
                T->ptr[1]->type = T->type;
                T->ptr[1]->offset = T->offset+T->width;
                T->ptr[1]->width = T->width;
                local_var_list(T->ptr[1]);
                T->num = T->ptr[1]->num+1;
            }
            else{
                T->num = T->ptr[0]->num;
            }
            

            break;
        case ID_NODE:
            //函数无参数，直接进入compst时没有创建符号表
            if(now_symtable == NULL){
                now_symtable = SymtableCreate();
                stackPush(SymStack, now_symtable);
            }
            SymtableInsert(now_symtable, T->type_id, K_VAL, LEV, T->offset, T->type);
            T->num = 1;
            break;
        default:
            break;
    }
}

Symtable* create_new_symtable(){
    Symtable *newtable = (Symtable*)malloc(sizeof(Symtable));
    stackPush(SymStack, newtable);
    return newtable;
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
