#include <sys/stat.h>
#include <stdio.h>

extern FILE* yyin;
extern int yylex(void);
extern int yyrestart(FILE*);
extern int yyparse();


int main(int argc, char** argv){
    if(argc > 1){
        if(!(yyin = fopen(argv[1], "r"))){
            perror(argv[1]);
            return 1;
        }
    }
    yyrestart(yyin);
    yyparse();
    return 0;
}
