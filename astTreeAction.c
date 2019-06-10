#include "astTreeAction.h"
#include "symtable.h"
#include "stack.h"
#include "ircode.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#define _SYMTABLETEST

const int DX = 0;
Stack * SymStack;
struct Symtable* ext_symtable; //外部变量符号表
static int LEV;  //当前层
static Symtable* now_symtable; //当前作用域的符号表
int tmp_kind, tmp_level, tmp_offset, tmp_type, tmp_num; //查表时返回的内容
s_data* tmp_res;
int args_num = 0;

void deal_astTree(NODE T){
    SymStack = stackCreate();
    ext_symtable = SymtableCreate(); //外部变量符号表
    stackPush(SymStack, ext_symtable);
    LEV = 0;
    Ast_To_Symtable(T);
    //stackOutput(SymStack);
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
    char tmp_name[33];
    Symtable* tmp_table;
    int op_kind;
    struct opn opn1, opn2, result;

    if(T){
        switch(T->kind){
        case EXT_DEF_LIST_NODE:
            if(!T->ptr[0]) break;
            T->ptr[0]->offset = T->offset;
            Ast_To_Symtable(T->ptr[0]);
            T->code = T->ptr[0]->code;
#ifdef _SYMTABLETEST
           // getchar();
            //stackOutput(SymStack);
#endif

            if(T->ptr[1]){
                T->ptr[1]->offset = T->ptr[0]->offset+T->ptr[0]->width;
                Ast_To_Symtable(T->ptr[1]);
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
            break;
        case EXT_VAR_DEF_NODE: //外部变量定义
            T->type = T->ptr[1]->type = !(strcmp(T->ptr[0]->type_id, "int"))?S_INT:S_FLOAT;
            T->ptr[1]->offset = T->offset;
            T->ptr[1]->width = (T->type == S_INT)? 4:8;
            var_list(T->ptr[1]);
            T->width = T->ptr[1]->num*((T->type == S_INT)? 4:8);
            T->code = NULL;
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
            strcpy(T->ptr[2]->Snext, newLabel()); //函数体语句执行结束后的位置属性
            Ast_To_Symtable(T->ptr[2]);
            T->code = merge(3, T->ptr[1]->code, T->ptr[2]->code, genLabel(T->ptr[2]->Snext));
            break;

        case FUNC_DEC_NODE:
            tmp_res = check_in_symtable(SymStack,T->type_id, &tmp_kind, &tmp_level, &tmp_offset, &tmp_type, &tmp_num);
            if(tmp_res != NULL){
                printf("Error at line %d: function \'%s\' Redefined \n", T->line, T->type_id);
            }
            else{
                T->place = tmp_res;
            }
            
            result.kind = ID;
            strcpy(result.id, T->type_id);
            T->code = genIR(FUNCTION, opn1, opn2, result);

            T->offset = DX;
            LEV++;

            //无论有没有参数都创建一个新的符号表
            Symtable* new_table = SymtableCreate();
            stackPush(SymStack, new_table);
            now_symtable = new_table;
            //有参数
            if(T->ptr[0]){
                /*Symtable* new_table = SymtableCreate();
                stackPush(SymStack, new_table);
                now_symtable = new_table;*/
                T->ptr[0]->offset = T->offset;
                Ast_To_Symtable(T->ptr[0]);
                T->width = T->ptr[0]->width;
                T->num = T->ptr[0]->num;
            }
            else{
                T->width = 0;
                T->num = 0;
            }
            LEV--; //参数和块语句同级，但是在compst才是真正的加
            SymtableInsert(ext_symtable, T->type_id, newAlias(), K_FUNC, LEV, -1, T->type, T->num);
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
                //T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);
            }
            else{
                T->num = T->ptr[0]->num; //不等于1，因为考虑数组
                T->width = T->ptr[0]->width;
                //T->code = T->ptr[0]->code;
            }

            break;
        case PARAM_DEC_NODE:
            T->type = T->ptr[1]->type = !(strcmp(T->ptr[0]->type_id, "int"))?S_INT:S_FLOAT;
            T->ptr[1]->offset = T->offset;
            T->ptr[1]->width = (T->type == S_INT)? 4:8;
            
            local_var_list(T->ptr[1]);
            T->width = T->ptr[1]->num*((T->type == S_INT)? 4:8);
            T->num = 1;
            break;
        case EXP_STMT_NODE:
            T->ptr[0]->offset = T->offset;
            Ast_To_Symtable(T->ptr[0]);
            T->width = T->ptr[0]->width;
            T->code = T->ptr[0]->code;
            break;
        case RETURN_NODE:
            break;
        case COMP_STM_NODE:
            LEV++;
            T->width = 0;
            T->code = NULL;
            if(T->ptr[0]){
                T->ptr[0]->offset = T->offset;
                if(LEV > 1){
                    Symtable * new_table = SymtableCreate(); //创建新作用域符号表
                    stackPush(SymStack, new_table); //将符号表入栈
                    now_symtable = new_table;
                }
                local_var_list(T->ptr[0]);
                T->width += T->ptr[0]->width;
                T->code = T->ptr[0]->code;
            }
            if(T->ptr[1]){
                T->ptr[1]->offset = T->offset+T->width;
                Ast_To_Symtable(T->ptr[1]);
                T->width += T->ptr[1]->width;
                T->code = merge(2, T->code, T->ptr[1]->code);
            }
#ifdef _SYMTABLETEST
            //语句块结束时将当前作用域符号表出栈
            printf("-------------------------------------------------\n");
            stackOutput(SymStack);
            getchar();
#endif
            if(SymStack->top->pre){
                SymStack->top = SymStack->top->pre;
                SymStack->elemNum--;
                now_symtable = SymStack->top->elem;
            }
            else{
                now_symtable = ext_symtable;
            }
#ifdef _SYMTABLETEST
            stackOutput(SymStack);
            getchar();
#endif
            LEV--;
            break;
        case STM_LIST_NODE:
            if(T->ptr[0] == NULL){
                //空语句
                T->width = 0;
                break;
            }
            if(T->ptr[1]){
                strcpy(T->ptr[0]->Snext, newLabel());
            }
            else{
                strcpy(T->ptr[0]->Snext, T->Snext);
            }
            T->ptr[0]->offset = T->offset;
            Ast_To_Symtable(T->ptr[0]);
            T->code = T->ptr[0]->code;
            T->width = T->ptr[0]->width;
            if(T->ptr[1]){
                strcpy(T->ptr[1]->Snext, T->Snext);
                T->ptr[1]->offset = T->offset+T->ptr[0]->width;
                Ast_To_Symtable(T->ptr[1]);
                if(T->ptr[0]->kind == RETURN_NODE || T->ptr[0]->kind == EXP_STMT_NODE || T->ptr[0]->kind == COMP_STM_NODE){
                    T->code = merge(2, T->code, T->ptr[1]->code);
                }
                else{
                    T->code = merge(3, T->code, genLabel(T->ptr[0]->Snext), T->ptr[1]->code);
                }
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
            //进入这里的是表达式语句部分, 从下面的case中进入,可以换成在Exp中处理
            printf("here！！！！！！\n");
            tmp_res = check_in_symtable(SymStack, T->type_id, &tmp_kind, &tmp_level, &tmp_offset, &tmp_type, &tmp_num);
            if(tmp_res == NULL){
                printf("Error at line %d: undefined variable \'%s\' \n", T->line, T->type_id);
            }
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
            //Ast_To_Symtable(T->ptr[0]);
            //Ast_To_Symtable(T->ptr[1]);
            Exp(T);
            printf("Exp ir: \n");
            printIR(T->code);
            break;
        case FUNC_CALL_NODE:
            tmp_res = check_in_symtable(SymStack, T->type_id, &tmp_kind, &tmp_level, &tmp_offset, &tmp_type, &tmp_num);
            if(tmp_res == NULL){
                printf("Error at line %d: undefined function \'%s\'\n", T->line, T->type_id);
                break;
            }
            if(T->ptr[0]!=NULL){
                Ast_To_Symtable(T->ptr[0]);
            }
            if(args_num != tmp_num){
                printf("Error at line %d: arguments num error \'%s\'\n", T->line, T->type_id);
            }
            args_num = 0;

            break;
        case ARGS_NODE:
            while(T){
                struct node* T0 = T->ptr[0];
                T = T->ptr[1];
                args_num++;
            }
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
            SymtableInsert(ext_symtable, T->type_id, newAlias(), K_VAL, LEV, T->offset, T->type, 0);
            T->num = 1;
            break;
        default:
            break;
    }
}

void local_var_list(node* T){
    switch(T->kind){
        case DEF_LIST_NODE:
            T->code = NULL;
            if(!T->ptr[0]) break;
            T->ptr[0]->offset = T->offset;
            local_var_list(T->ptr[0]);
            T->width += T->ptr[0]->width;
            if(T->ptr[1]){
                T->ptr[1]->offset = T->offset+T->ptr[0]->width;
                local_var_list(T->ptr[1]);
            }
            break;
        case VAR_DEF_NODE:
            T->code = NULL;
            T->type = T->ptr[1]->type = !(strcmp(T->ptr[0]->type_id, "int"))?S_INT:S_FLOAT;
            T->ptr[1]->offset = T->offset;
            T->ptr[1]->width = (T->type == S_INT)? 4:8;
            local_var_list(T->ptr[1]);
            T->width = T->ptr[1]->num*((T->type == S_INT)? 4:8);
            break;
        case DEC_LIST_NODE:
            T->code = NULL;
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
            T->code = NULL;
            //函数无参数，直接进入compst时没有创建符号表
            if(now_symtable == NULL || now_symtable == ext_symtable){
                now_symtable = SymtableCreate();
                stackPush(SymStack, now_symtable);
            }
            tmp_res = check_in_now_symtable(SymStack, T->type_id, &tmp_kind, &tmp_level, &tmp_offset, &tmp_type, &tmp_num);
            if(tmp_res != NULL){
                printf("Error at line %d: variable \'%s\' Redefined\n", T->line, T->type_id);
            }
            SymtableInsert(now_symtable, T->type_id, newAlias(), K_VAL, LEV, T->offset, T->type, 0);
            T->num = 1;
            break;
        default:
            break;
    }
}


s_data* check_in_symtable(Stack* SymStack, char *s, int* kind, int* level, int* offset, int* type, int* num){
    StackElem *tmp = SymStack->top;
    while(tmp!=NULL){
        s_data *tmp_date = tmp->elem->head;
        while(tmp_date!=NULL){
            if(!strcmp(s, tmp_date->s_name)){
                *kind = tmp_date->s_kind;
                *level = tmp_date->s_level;
                *offset = tmp_date->s_offset;
                *type = tmp_date->s_type;
                *num = tmp_date->s_num;
                return tmp_date;
            }
            tmp_date = tmp_date->next;
        }
        tmp = tmp->pre;
    }
    return NULL;
}

/*
 * 判断是否在当前作用域符号表中出现，用于判断redefine错误
 */

s_data* check_in_now_symtable(Stack* SymStack, char *s, int* kind, int* level, int* offset, int* type, int* num){
    s_data *tmp_date = SymStack->top->elem->head;
    while(tmp_date!=NULL){
        if(!strcmp(s, tmp_date->s_name)){
            *kind = tmp_date->s_kind;
            *level = tmp_date->s_level;
            *offset = tmp_date->s_offset;
            *type = tmp_date->s_type;
            *num = tmp_date->s_num;
            return tmp_date;
        }
        tmp_date = tmp_date->next;
    }
    return NULL;
}


void Exp(node *T)
{
    s_data* rtn;
    int num, width;
    node *T0;
    opn opn1, opn2, result;
    int find_kind, find_level, find_offset, find_type, find_num;
    int op_kind;
    if(T){
        switch(T->kind){
        case ID_NODE: rtn = check_in_now_symtable(SymStack, T->type_id, &find_kind, &find_level, &find_offset, &find_type, &find_num);
                      if(rtn == NULL){
                          //表达式中出现的标识符不在符号表内
                          printf("EError at line %d: undefined variable \'%s\' \n", T->line, T->type_id);
                      }
                      else{
                          //取出符号表中的内容作为节点属性
                          T->place = rtn;
                          T->code = NULL;
                          T->type = rtn->s_type;
                          T->offset = rtn->s_offset;
                          T->width = 0;
                      }
                      break;
        case INT_NODE: //生成临时变量，并加入符号表中
                       T->type = S_INT;
                       T->place = SymtableInsertTemp(now_symtable, newTemp(), K_TEMP, LEV, T->offset, T->type, 0);
                       opn1.kind = INT;
                       opn1.const_int = T->type_int;
                       result.kind = ID;
                       strcpy(result.id, T->place->alias);
                       result.offset = T->place->s_offset;
                       T->code = genIR(ASSIGNOP, opn1, opn2, result);
                       T->width = 4;
                       break;
        case ASSIGNOP_NODE: if(T->ptr[0]->kind != ID_NODE){
                                printf("Error at line %d: the assignment requires left value\n", T->line);
                            }
                            else{
                                Exp(T->ptr[0]);
                                T->ptr[1]->offset = T->offset;
                                Exp(T->ptr[1]);
                                T->type = T->ptr[0]->type;
                                T->width = T->ptr[1]->width;
                                T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);
                                opn1.kind = ID;
                                strcpy(opn1.id, T->ptr[1]->place->alias);
                                opn1.offset = T->ptr[1]->place->s_offset;
                                result.kind = ID;
                                strcpy(result.id, T->ptr[0]->place->alias);
                                result.offset = T->ptr[0]->place->s_offset;
                                T->code = merge(2, T->code, genIR(ASSIGNOP, opn1, opn2, result));
                            }
                            break;
        case PLUS_NODE:
        case STAR_NODE:
        case DIV_NODE: T->ptr[0]->offset = T->offset;
                       Exp(T->ptr[0]);
                       T->ptr[1]->offset = T->offset + T->ptr[0]->width;
                       Exp(T->ptr[1]);
                       if(T->ptr[0]->type == FLOAT || T->ptr[1]->type == FLOAT){
                           T->type = FLOAT;
                           T->width = T->ptr[0]->width+T->ptr[1]->width+4;
                       }
                       else{
                           T->type = INT;
                           T->width = T->ptr[0]->width+T->ptr[1]->width+2;
                       }
                       T->place = SymtableInsertTemp(now_symtable, newTemp(), K_TEMP, LEV, T->offset+T->ptr[0]->width+T->ptr[1]->width, T->type, 0);
                       opn1.kind = ID;
                       strcpy(opn1.id, T->ptr[0]->place->alias);
                       opn1.type = T->ptr[0]->type; 
                       opn1.offset = T->ptr[0]->place->s_offset;

                       opn2.kind = ID;
                       strcpy(opn2.id, T->ptr[1]->place->alias);
                       opn2.type = T->ptr[1]->type;
                       opn2.offset = T->ptr[1]->place->s_offset;

                       result.kind = ID;
                       strcpy(result.id, T->place->alias);
                       result.type = T->type;
                       result.offset = T->place->s_offset;
                       if(T->kind == PLUS_NODE) op_kind = PLUS;
                       if(T->kind == MINUS_NODE) op_kind = MINUS;
                       if(T->kind == STAR_NODE) op_kind = STAR;
                       if(T->kind == DIV_NODE) op_kind = DIV;
                       T->code = merge(3, T->ptr[0]->code, T->ptr[1]->code, genIR(op_kind, opn1, opn2, result));
                       T->width = T->ptr[0]->width+T->ptr[1]->width+(T->type == INT?4:8);
                       //printIR(T->code);
                       break;
        }
    }
}


void boolExp(node *T){
    opn opn1, opn2, result;
    int op;
    s_data* rtn;
    if(T){
        switch(T->kind){
        case INT_NODE: if(T->type_int != 0){
                           T->code = genGoto(T->Etrue);
                       }
                       else{
                           T->code = genGoto(T->Efalse);
                       }
                       T->width = 0;
                       break;
                    
        }
    }
}
