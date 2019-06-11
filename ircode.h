#ifndef _IRCODE_H
#define _IRCODE_H
#include "node.h"

enum OPN_OP{
    ASSIGNOP,
    PLUS,
    MINUS,
    STAR,
    DIV,
    FUNCTION,
    PARAM,
    LABEL,
    GOTO,
    JLE,
    JLT,
    JGE,
    JGT,
    EQ,
    NEQ,
    ARG,
    CALL,
    RETURN
};

enum OPN_KIND{
    INT,
    FLOAT,
    ID
};

typedef struct opn{
    int kind;
    int type;
    int const_int;
    float const_float;
    char const_char;
    char id[33];
    int level;
    int offset;
}opn;

typedef struct codenode{
    int op;
    struct opn opn1, opn2, result;
    struct codenode *next, *pre;
}codenode;

char* strcat0(char* s1, char* s2);
char* newAlias();
char* newLabel();
char* newTemp();
struct codenode* genIR(int op, opn opn1, opn opn2, opn result);
struct codenode* genLabel(char* label);
struct codenode* genGoto(char* label);
struct codenode* merge(int num, ...);
void printIR(struct codenode* head);

#endif
