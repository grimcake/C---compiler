%{
#include "node.h"
#include "symtable.h"
#include <stdlib.h>
%}

%union {
    int type_int;
    char type_id[33];
    struct node *pnode;
};

%type <pnode> program ExtDefList ExtDef Specifier ExtDecList FunDec CompSt VarList VarDec ParamDec Stmt StmtList DefList Def DecList Dec Exp Args 
%token <type_id> TYPE
%token <type_id> PLUS MINUS STAR DIV ASSIGNOP RELOP
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
Program : ExtDefList {display($1, 0); ASTroot = $1; }
    ;
ExtDefList : ExtDef ExtDefList {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = EXT_DEF_LIST_NODE; $$->ptr[0] = $1; $$->ptr[1] = $2;}
    | {$$=NULL;}
    ;
ExtDef : Specifier ExtDecList SEMI {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = EXT_VAR_DEF_NODE; $$->ptr[0] = $1; $$->ptr[1] = $2;}
    | Specifier SEMI
    | Specifier FunDec CompSt {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = FUNC_DEF_NODE; $$->ptr[0] = $1; $$->ptr[1] = $2; $$->ptr[2] = $3;}
    ;
ExtDecList : VarDec {$$=$1;}
    | VarDec COMMA ExtDecList {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = EXT_DEC_LIST_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3;}
    ;

/*
 * Specifiers
 */
Specifier : TYPE {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = TYPE_NODE; strcpy($$->type_id, $1); $$->type = (strcmp($$->type_id, "int") == 0)?S_INT:S_FLOAT; printf("hi %d\n", $$->type);}
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
VarDec : ID {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = ID_NODE; strcpy($$->type_id, $1); }
    | VarDec LB INT RB
    ;
FunDec : ID LP VarList RP {$$ = (struct node*)malloc(sizeof(struct node)); $$->ptr[0] = $3; $$->kind = FUNC_DEC_NODE; strcpy($$->type_id, $1);}
    | ID LP RP {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = FUNC_DEC_NODE; strcpy($$->type_id, $1);}
    ;
VarList : ParamDec COMMA VarList {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = PARAM_LIST_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3;}
    | ParamDec {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = PARAM_LIST_NODE; $$->ptr[0] = $1;}
    ;
ParamDec : Specifier VarDec {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = PARAM_DEC_NODE; $$->ptr[0] = $1; $$->ptr[1] = $2;}
    ;

/*
 * Statements
 */
CompSt : LC DefList StmtList RC {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = COMP_STM_NODE; $$->ptr[0] = $2; $$->ptr[1] = $3;}
    ;
StmtList : Stmt StmtList {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = STM_LIST_NODE; $$->ptr[0] = $1; $$->ptr[1] = $2;}
    | {$$=NULL;}
    ;
Stmt : Exp SEMI {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = EXP_STMT_NODE; $$->ptr[0] = $1;}
    | CompSt {$$=$1;}
    | RETURN Exp SEMI {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = RETURN_NODE; $$->ptr[0] = $2;}
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = IF_THEN_ELSE_NODE; $$->ptr[0] = $3; $$->ptr[1] = $5; $$->ptr[2] = $7;}
    | WHILE LP Exp RP Stmt {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = WHILE_NODE; $$->ptr[0] = $3; $$->ptr[1] = $5;}
    ;

/*
 * Local Definitions
 */
DefList : Def DefList {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = DEF_LIST_NODE; $$->ptr[0] = $1; $$->ptr[1] = $2;}
    | {$$=NULL;}
    ;
Def : Specifier DecList SEMI {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = VAR_DEF_NODE; $$->ptr[0] = $1; $$->ptr[1] = $2;}
    ;
DecList : Dec {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = DEC_LIST_NODE; $$->ptr[0] = $1;}
    | Dec COMMA DecList {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = DEC_LIST_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3;}
    ;
Dec : VarDec {$$=$1;}
    | VarDec ASSIGNOP Exp {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = ASSIGNOP_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3; }
    ;

/*
 * Expressions
 */
Exp : Exp ASSIGNOP Exp {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = ASSIGNOP_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3;strcpy($$->type_id, $2);}
    | Exp AND Exp 
    | Exp OR Exp
    | Exp RELOP Exp {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = RELOP_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3; strcpy($$->type_id, $2);}
    | Exp PLUS Exp {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = PLUS_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3;strcpy($$->type_id, $2);} 
    | Exp MINUS Exp {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = MINUS_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3;strcpy($$->type_id, $2);} 
    | Exp STAR Exp {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = STAR_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3; strcpy($$->type_id, $2);} 
    | Exp DIV Exp {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = DIV_NODE; $$->ptr[0] = $1; $$->ptr[1] = $3;strcpy($$->type_id, $2);} 
    | LP Exp RP 
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = ID_NODE; strcpy($$->type_id, $1);} 
    | INT {$$ = (struct node*)malloc(sizeof(struct node)); $$->kind = INT_NODE; $$->type_int=$1;}
    ;
Args : Exp COMMA Args
    | Exp
    ;

%%

#include "lex.yy.c"









































