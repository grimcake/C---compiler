#include <stdio.h>
#include <stdlib.h>
#ifndef _NODE_H_
#define _NODE_H_
#include "node.h"
#endif

struct node *mknode(int kind, struct node *ptr0, struct node *ptr1, struct node *ptr2){
    struct node *T = (struct node*)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = ptr0;
    T->ptr[1] = ptr1;
    T->ptr[2] = ptr2;
    return T;
}

void display(NODE T, int indent){
    if(T){
        switch(T->kind){
        case EXT_DEF_LIST_NODE:
            display(T->ptr[0], indent);
            display(T->ptr[1], indent);
            break;
        case EXT_VAR_DEF_NODE:
            printf("%*c out variable definition: \n", indent, ' ');
            display(T->ptr[0], indent+3);
            printf("%*c variable name:\n", indent+3, ' ');
            display(T->ptr[1], indent+6);
            break;
        case TYPE_NODE:
            printf("%*c type: %s\n", indent, ' ', T->type_id);
            break;
        case EXT_DEC_LIST_NODE:
            display(T->ptr[0], indent);
            display(T->ptr[1], indent);
            break;
        case FUNC_DEF_NODE:
            printf("%*c function definition:\n", indent, ' ');
            display(T->ptr[0], indent+3);
            display(T->ptr[1], indent+3);
            display(T->ptr[2], indent+3);
            break;

        case FUNC_DEC_NODE:
            printf("%*c function name: %s\n", indent, ' ', T->type_id);
            if(T->ptr[0]){
                printf("%*c function parameters: \n", indent, ' ');
                display(T->ptr[0], indent+3);
            }
            else{
                printf("%*c no-parameter function\n", indent+3, ' ');
            }
            break;
        case PARAM_LIST_NODE:
            display(T->ptr[0], indent);
            display(T->ptr[1], indent);
            break;
        case PARAM_DEC_NODE:
            printf("%*c type: %s name: %s\n", indent, ' ', T->ptr[0]->kind == INT_NODE?"int":"float", T->ptr[1]->type_id);
            break;
        case EXP_STMT_NODE:
            printf("%*c expression sentece: \n", indent, ' ');
            display(T->ptr[0], indent+3);
            break;
        case RETURN_NODE:
            printf("%*c return sentece:\n", indent, ' ');
            display(T->ptr[0], indent+3);
            break;
        case COMP_STM_NODE:
            printf("%*c mix sentences:\n", indent, ' ');
            printf("%*c mix sentences variable definition:\n", indent+3, ' ');
            display(T->ptr[0], indent+6);
            printf("%*c mix sentences senteces:\n", indent+3, ' ');
            display(T->ptr[1], indent+6);
            break;
        case STM_LIST_NODE:
            display(T->ptr[0], indent);
            display(T->ptr[1], indent);
            break;
        case DEF_LIST_NODE:
            display(T->ptr[0], indent);
            display(T->ptr[1], indent);
            break;
        case VAR_DEF_NODE:
            printf("%*c local variable name:\n", indent, ' ');
            display(T->ptr[0], indent+3);
            display(T->ptr[1], indent+3);
            break;
        case ID_NODE:
            printf("%*c ID: %s\n", indent, ' ', T->type_id);
            break;
        case INT_NODE:
            printf("%*c INT: %d\n", indent, ' ', T->type_int);
            break;
        case ASSIGNOP_NODE:
        case AND_NODE:
        case OR_NODE:
        case PLUS_NODE:
        case MINUS_NODE:
        case STAR_NODE:
        case DIV_NODE:
            printf("%*c%s\n", indent, ' ', T->type_id);
            display(T->ptr[0], indent+3);
            display(T->ptr[1], indent+3);
            break;
    
        }

    }
}
