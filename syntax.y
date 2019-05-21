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
    struct node *pnode;
};

%type <pnode> Exp
%token TYPE
%token PLUS MINUS STAR DIV ASSIGNOP RELOP
%token AND OR NOT
%token SEMI COMMA DOT
%token LP RP LB RB LC RC
%token STRUCT IF ELSE WHILE RETURN
%token <type_id> ID
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
    | Exp PLUS Exp 
    | Exp MINUS Exp 
    | Exp STAR Exp 
    | Exp DIV Exp 
    | LP Exp RP 
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID  
    | INT 
    ;
Args : Exp COMMA Args
    | Exp
    ;

%%

#include "lex.yy.c"









































