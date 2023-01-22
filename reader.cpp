#include "differentiator.h"

#define DEF_CMD(name, num, ch, ...)     \
    case ch:                            \
        nod->value.op = name;           \
        break;

#define KILL {                                                      \
    printf("INVALID EXPRESSION: %c must not be there\n", **ptr);    \
                                                                    \
    assert(0);                                                      \
}

static node* getP(const char **ptr, side side);

static node* getN(const char **ptr, side side) {
    assert(ptr && *ptr);

    if(!isdigit(**ptr)) KILL

    int val = 0;
    node *newnode = (node*) malloc(sizeof(node));

    while(isdigit(**ptr)) {
        val = val * 10 + **ptr - '0';

        (*ptr)++;
    }

    NodeCtor(newnode, NULL, CONST, {.val = (double) val}, side);

    return newnode;
}

static node* getT(const char **ptr, side side) {
    assert(ptr && *ptr);

    op op;
    node *left = getP(ptr, side);

    while(**ptr == '*' || **ptr == '/') {
        left->side = LEFT;

        if(**ptr == '*')
            op = MUL;
        else
            op = DIV;

        (*ptr)++;

        node *right = getP(ptr, RIGHT);

        node *nod = (node*) malloc(sizeof(node));

        NodeCtor(nod, NULL, OP, {.op = op}, side);
        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
    }

    return left;
}

static node* getE(const char **ptr, side side) {
    assert(ptr && *ptr);

    op op;
    node *left = getT(ptr, side);

    while(**ptr == '+' || **ptr == '-') {
        left->side = LEFT;

        if(**ptr == '+')
            op = ADD;
        else
            op = SUB;

        (*ptr)++;

        node *right = getT(ptr, RIGHT);

        node *nod = (node*) malloc(sizeof(node));

        NodeCtor(nod, NULL, OP, {.op = op}, side);
        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
    }

    return left;
}

static node* getP(const char **ptr, side side) {
    assert(ptr && *ptr);

    if(**ptr == '(') {
        (*ptr)++;

        node *nod = getE(ptr, side);

        if(**ptr != ')') KILL

        (*ptr)++;

        return nod;
    }
    else
        return getN(ptr, side);
}

static node* getG(const char *expression) {
    assert(expression);

    const char **ptr = &expression;

    node *root = getE(ptr, ROOT);
    assert(root);

    if(*expression != '\0') KILL

    return root;
}

tree* ReadExpression(const char *txt) {
    tree *expression = (tree*) malloc(sizeof(tree));

    TreeCtor(expression, getG(txt));

    return expression;
}