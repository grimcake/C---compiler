%x COMMENT

%{
#include "syntax.tab.h"
#ifndef _NODE_H_
#define _NODE_H_
#include "node.h"
#endif

typedef union{
    int type_int;
    char type_id[33];
    NODE *pnode;
}YYLVAL;
#define YYSTYPE YYLVAL
%}

INT [0-9]+
ID [_a-zA-Z][a-zA-Z0-9]*
SEMI ";"
COMMA ","
ASSIGNOP "="
RELOP ">"|"<"|">="|"<="|"=="|"!="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
DOT "."
NOT "!"
TYPE "int"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT "struct"
RETURN "return"
IF "if"
ELSE "else"
WHILE "while"

%%
"//".* {}
"/*" {BEGIN COMMENT;}
<COMMENT>"*/" {BEGIN INITIAL;}


{SEMI} {
    printf("%s ", yytext);
    printf("SEMI\n");
    return SEMI;
}

{COMMA} {
    printf("%s ", yytext);
    printf("COMMA\n");
    return COMMA;
}

{ASSIGNOP} {
    printf("%s ", yytext);
    printf("ASSIGN\n");
    strcpy(yylval.type_id, yytext);
    return ASSIGNOP;
}

{RELOP} {
    strcpy(yylval.type_id, yytext);
    printf("%s ", yytext);
    printf("RELOP\n");
    return RELOP;
}

{PLUS} {
    printf("%s ", yytext);
    printf("PLUS\n");
    strcpy(yylval.type_id, yytext);
    return PLUS;
}

{MINUS} {
    printf("%s ", yytext);
    printf("MINUS\n");
    strcpy(yylval.type_id, yytext);
    return MINUS;
}

{STAR} {
    printf("%s ", yytext);
    printf("STAR\n");
    strcpy(yylval.type_id, yytext);
    return STAR;
}

{DIV} {
    printf("%s ", yytext);
    printf("DIV\n");
    strcpy(yylval.type_id, yytext);
    return DIV;
}

{AND} {
    printf("%s ", yytext);
    printf("AND\n");
    return AND;
}

{OR} {
    printf("%s ", yytext);
    printf("OR\n");
    return OR;
}

{DOT} {
    printf("%s ", yytext);
    printf("DOT\n");
    return DOT;
}

{NOT} {
    printf("%s ", yytext);
    printf("NOT\n");
    return NOT;
}

{LP} {
    printf("%s ", yytext);
    printf("LP\n");
    return LP;
}

{RP} {
    printf("%s ", yytext);
    printf("RP\n");
    return RP;
}

{LB} {
    printf("%s ", yytext);
    printf("LB\n");
    return LB;
}

{RB} {
    printf("%s ", yytext);
    printf("RB\n");
    return RB;
}

{LC} {
    printf("%s ", yytext);
    printf("LC\n");
    return LC;
}

{RC} {
    printf("%s ", yytext);
    printf("RC\n");
    return RC;
}

{TYPE} {
    strcpy(yylval.type_id, yytext);
    printf("%s ", yytext);
    printf("TYPE\n");
    return TYPE;
}


{INT} {
    yylval.type_int = atoi(yytext);
    printf("%s ", yytext);
    printf("INT\n");
    return INT;
}

{STRUCT} {
    printf("%s ", yytext);
    printf("STRUCT\n");
    return STRUCT;
}

{IF} {
    printf("%s ", yytext);
    printf("IF\n");
    return IF;
}

{ELSE} {
    printf("%s ", yytext);
    printf("ELSE\n");
    return ELSE;
}

{WHILE} {
    printf("%s ", yytext);
    printf("WHILE\n");
    return WHILE;
}

{RETURN} {
    printf("%s ", yytext);
    printf("RETURN\n");
    return RETURN;
}

{ID} {
    strcpy(yylval.type_id, yytext);
    printf("%s ", yytext);
    printf("ID\n");
    return ID;
}

\n {
}


[ \r\t]+ {

}

. {
    printf("Error type at line %d: characters \'%s\'\n", yylineno, yytext);
}

%%

/*
int main(int argc, char** argv){
    if(argc > 1){
        if(!(yyin = fopen(argv[1], "r"))){
            perror(argv[1]);
            return 1;
        }
    }
    yylex();
    return 0;
}
*/
