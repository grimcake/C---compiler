enum node_kind{
    ID_NODE,
    INT_NODE,
    LPRP_NODE,
    PLUS_NODE,
    MINUS_NODE,
    STAR_NODE,
    DIV_NODE,
    ASSIGNOP_NODE
};

typedef struct Exp {
    enum node_kind kind;
    char type_id[33];
    int type_int;
    struct Exp* pExp1;
    struct Exp* pExp2;
}*PEXP, BEXP;
