%{
#ifndef _NODE_H_
#define _NODE_H_
#include "node.h"
#endif
#include <stdlib.h>
%}

%union {
    int type_int;
    char type_id[33];
    struct Exp *pExp;
};

%type <pExp> Exp
%token TYPE
%token PLUS MINUS STAR DIV ASSIGNOP RELOP
%token AND OR NOT
%token SEMI COMMA DOT
%token LP RP LB RB LC RC
%token STRUCT IF ELSE WHILE RETURN
%token <type_int> ID
%token <type_int> INT

%%
/*
 * High-level Definitions
 */
Program : ExtDefList
    ;
ExtDefList : ExtDef ExtDefList
    |
    ;
ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDecList : VarDec
    | VarDec COMMA ExtDecList
    ;

/*
 * Specifiers
 */
Specifier : TYPE
    | StructSpecifier
    ;
StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;
OptTag : ID
    |
    ;
Tag :ID
    ;

/*
 * Declarators
 */
VarDec : ID
    | VarDec LB INT RB
    ;
FunDec : ID LP VarList RP
    | ID LP RP
    ;
VarList : ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec : Specifier VarDec
    ;

/*
 * Statements
 */
CompSt : LC DefList StmtList RC
    ;
StmtList : Stmt StmtList
    |
    ;
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;

/*
 * Local Definitions
 */
DefList : Def DefList
    |
    ;
Def : Specifier DecList SEMI
    ;
DecList : Dec
    | Dec COMMA DecList
    ;
Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;

/*
 * Expressions
 */
Exp : Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=PLUS_NODE; $$->pExp1=$1; $$->pExp2=$3;}
    | Exp MINUS Exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=MINUS_NODE; $$->pExp1=$1; $$->pExp2=$3;}
    | Exp STAR Exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=STAR_NODE; $$->pExp1=$1; $$->pExp2=$3;}
    | Exp DIV Exp {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=DIV_NODE; $$->pExp1=$1; $$->pExp2=$3;}
    | LP Exp RP {$$=(PEXP)$2;}
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID  {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=ID_NODE; strcpy($$->type_id, $1);} 
    | INT {$$=(PEXP)malloc(sizeof(struct Exp)); $$->kind=INT_NODE; $$->type_int=$1;} 
    ;
Args : Exp COMMA Args
    | Exp
    ;

%%

#include "lex.yy.c"









































