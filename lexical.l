%{
#include "syntax.tab.h"
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

{SEMI} {
    printf("SEMI\n");
    return SEMI;
}

{COMMA} {
    printf("COMMA\n");
    return COMMA;
}

{ASSIGNOP} {
    printf("ASSIGN\n");
    return ASSIGNOP;
}

{RELOP} {
    printf("RELOP\n");
    return RELOP;
}

{PLUS} {
    printf("PLUS\n");
    return PLUS;
}

{MINUS} {
    printf("MINUS\n");
    return MINUS;
}

{STAR} {
    printf("STAR\n");
    return STAR;
}

{DIV} {
    printf("DIV\n");
    return DIV;
}

{AND} {
    printf("AND\n");
    return AND;
}

{OR} {
    printf("OR\n");
    return OR;
}

{DOT} {
    printf("DOT\n");
    return DOT;
}

{NOT} {
    printf("NOT\n");
    return NOT;
}

{LP} {
    printf("LP\n");
    return LP;
}

{RP} {
    printf("RP\n");
    return RP;
}

{LB} {
    printf("LB\n");
    return LB;
}

{RB} {
    printf("RB\n");
    return RB;
}

{LC} {
    printf("LC\n");
    return LC;
}

{RC} {
    printf("RC\n");
    return RC;
}

{TYPE} {
    printf("TYPE\n");
    return TYPE;
}


{INT} {
    yylval = atoi(yytext);
    printf("INT\n");
    return INT;
}

{STRUCT} {
    printf("STRUCT\n");
    return STRUCT;
}

{IF} {
    printf("IF\n");
    return IF;
}

{ELSE} {
    printf("ELSE\n");
    return ELSE;
}

{WHILE} {
    printf("WHILE\n");
    return WHILE;
}

{RETURN} {
    printf("RETURN\n");
    return RETURN;
}

{ID} {
    printf("ID\n");
    return ID;
}

\n {
}

[ \t]+ {

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
