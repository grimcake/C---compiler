#ifndef _NODE_H
#define _NODE_H
#include "symtable.h"

enum node_kind{
    ID_NODE,
    INT_NODE,
    LPRP_NODE,
    PLUS_NODE,
    MINUS_NODE,
    STAR_NODE,
    DIV_NODE,
    ASSIGNOP_NODE,
    AND_NODE,
    OR_NODE,

    EXT_DEF_LIST_NODE,
    EXT_VAR_DEF_NODE,
    TYPE_NODE,
    EXT_DEC_LIST_NODE,
    FUNC_DEF_NODE,
    FUNC_DEC_NODE,
    PARAM_LIST_NODE,
    PARAM_DEC_NODE,
    EXP_STMT_NODE,
    RETURN_NODE,
    COMP_STM_NODE,
    STM_LIST_NODE,
    WHILE_NODE,
    IF_THEN_NODE,
    IF_THEN_ELSE_NODE,
    DEF_LIST_NODE,
    VAR_DEF_NODE,
    DEC_LIST_NODE,
    RELOP_NODE,
    NOT_NODE,
    FUNC_CALL_NODE,
    ARGS_NODE,

};

typedef struct node {
    enum node_kind kind;
    char type_id[33];
    int type_int;
    struct node* ptr[3];

    int offset;
    int width;
    int type;

    int num; //一次定义的数量，用于EXT_VAR_DEF_NODE,或函数参数列表的数量
    int level;

    int line; //行号，主要用于静态语义分析表达式和变量

    struct s_data* place;
    char Etrue[15], Efalse[15];
    char Snext[15];
    struct codenode* code;
    char op[10];
}node, *NODE;

NODE ASTroot;







#endif
