#ifndef DIFFERENTIATOR_TREE_H
#define DIFFERENTIATOR_TREE_H

#define DEF_CMD(name, num, sign, ...) name = num,

typedef enum OP {
#include "../op.h"
} op;

#undef DEF_CMD

typedef enum TYPE {
    NOT_DEFINED = 0,
    OP,
    VAR,
    CONST,
    MATH_CONST,
    ROOT_NODE,
} type;

typedef enum MCONST {
    IDK = 0,
    E,
    PI,
} mconst;

union value {
    op op;
    int val;
    char name;
    mconst math_const;
    void* no_val;
};

enum TREE_ERROR {
    TREE_OK = 0,
    NULL_NODE,
    INVALID_LINK,
    UNDEFINED_TYPE,
    UNDEFINED_OP,
    NULL_NAME
};

enum EXPRESSION_ERROR {
    EXPRESSION_OK = 0,
    INVALID_EXPRESSION,
};

typedef enum SIDE {
    ROOT  = -1,
    LEFT  = 0,
    RIGHT = 1,
} side;

typedef struct Node {
    struct Node *parent;
    struct Node *children[2];
    type type;
    value value;
    side side;
} node;

typedef struct Tree {
    node *root;
} tree;

#endif //DIFFERENTIATOR_TREE_H
