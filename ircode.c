#include "node.h"
#include "ircode.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "symtable.h"


char* strcat0(char* s1, char* s2){
    static char result[10];
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* newAlias(){
    static int no = 1;
    char s[10];
    sprintf(s, "%d", no++);
    return strcat0("v", s);
}

char* newLabel(){
    static int no = 1;
    char s[10];
    sprintf(s, "%d", no++);
    return strcat0("label", s);
}

char* newTemp(){
    static int no = 1;
    char s[10];
    sprintf(s, "%d", no++);
    return strcat0("temp", s);
}

struct codenode* genIR(int op, opn opn1, opn opn2, opn result){
    struct codenode* h = (struct codenode*)malloc(sizeof(struct codenode));
    h->op = op;
    h->opn1 = opn1;
    h->opn2 = opn2;
    h->result = result;
    h->next = h->pre = h;
    return h;
}

struct codenode* genLabel(char* label){
    struct codenode* h = (struct codenode*)malloc(sizeof(struct codenode));
    h->op = LABEL;
    strcpy(h->result.id, label);
    h->next = h->pre = h;
    return h;
}

struct codenode* merge(int num, ...){
    struct codenode *h1, *h2, *p, *t1, *t2;
    va_list ap;
    va_start(ap, num);
    h1 = va_arg(ap, struct codenode *);
    while(--num>0){
        h2 = va_arg(ap, struct codenode*);
        if(h1 == NULL) h1 = h2;
        else if(h2){
            t1 = h1->pre;
            t2 = h2->pre;
            t1->next = h2;
            t2->next = h1;
            h1->pre = t2;
            h2->pre = t1;
        }
    }
    va_end(ap);
    return h1;
}



void printIR(struct codenode* head){
    char opnstr1[32], opnstr2[32], resultstr[32];
    struct codenode* h = head;
    do{
        if(h->opn1.kind == INT){
            sprintf(opnstr1, "#%d", h->opn1.const_int);
        }
        if(h->opn1.kind == FLOAT){
            sprintf(opnstr1, "#%f", h->opn1.const_float);
        }
        if(h->opn1.kind == ID){
            sprintf(opnstr1, "%s", h->opn1.id);
        }
        if(h->opn2.kind == INT){
            sprintf(opnstr2, "#%d", h->opn2.const_int);
        }
        if(h->opn2.kind == FLOAT){
            sprintf(opnstr2, "#%f", h->opn2.const_float);
        }
        if(h->opn2.kind == ID){
            sprintf(opnstr2, "%s", h->opn2.id);
        }

        switch(h->op){
        case ASSIGNOP:printf(" %s:=%s\n", resultstr, opnstr1);
                      break;
        case PLUS:
        case MINUS:
        case STAR:
        case DIV:printf(" %s:=%s %c %s\n", resultstr, opnstr1, \
                        h->op == PLUS?'+':h->op == MINUS?'-':h->op == STAR?'*':'\\',opnstr2);
                 break;
        case FUNCTION:printf("\nFUNCTION %s:\n", h->result.id);
                      break;
        case PARAM:printf(" PARAM %s\n", h->result.id);
                   break;
        case LABEL:printf(" LABEL %s:\n", h->result.id);
                   break;
        case GOTO:printf(" GOTO %s\n", h->result.id);
                  break;
        
        case JLE:printf(" IF %s <= %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                 break;
        case JLT:printf(" IF %s < %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                 break;
        case JGE:printf(" IF %s >= %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                 break;
        case JGT:printf(" IF %s > %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                 break;
        case EQ:printf(" IF %s == %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                break;
        case NEQ:printf(" IF %s != %s GOTO %s\n", opnstr1, opnstr2, resultstr);
                 break;
        case ARG:printf(" ARG %s\n", h->result.id);
                 break;
        case CALL:printf(" %s := CALL %s\n", resultstr, opnstr1);
                  break;
        case RETURN:if(h->result.kind){
                        printf("RETURN %s\n", resultstr);
                    }
                    else{
                        printf(" RETURN\n");
                    }
                    break;
        }
        h = h->next;
    }while(h != head);
}







