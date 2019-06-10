#ifndef _AST_TREE_ACTION
#define _AST_TREE_ACTION
#include "node.h"
#include "symtable.h"
#include "stack.h"

void deal_astTree(NODE T);
void Ast_To_Symtable(NODE T);
void var_list(node * T);
void local_var_list(node* T);

s_data* check_in_symtable(Stack *SymStack, char *s, int* kind, int* level, int* offset, int* type, int* num);
s_data* check_in_now_symtable(Stack *SymStack, char *s, int* kind, int* level, int* offset, int* type, int* num);
#endif
