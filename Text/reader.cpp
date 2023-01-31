#include "../differentiator.h"

#define DEF_CMD(name, num, sign, ...)   \
    if(streq(*ptr, sign)) {             \
        op = name;                      \
        (*ptr) += strlen(sign);         \
    } else

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

    NodeCtor(newnode, NULL, CONST, {.val = val}, side);

    return newnode;
}

static node* getV(const char **ptr, side side) {
    assert(ptr && *ptr);

    if(isalpha(**ptr)) {
        char name = **ptr;
        (*ptr)++;

        node *newnode = (node*) malloc(sizeof(node));
        NodeCtor(newnode, NULL, VAR, {.name = name}, side);

        return newnode;
    }

    return getN(ptr, side);
}

static node* getC(const char **ptr, side side) {
    assert(ptr && *ptr);

    mconst m;

    if(streq(*ptr, "e")) {
        m = E;

        (*ptr)++;
    } else if(streq(*ptr, "pi")) {
        m = PI;

        (*ptr) += 2;
    }
    else
        return getV(ptr, side);

    node *nod = (node*) malloc(sizeof(node));
    NodeCtor(nod, NULL, MATH_CONST, {.math_const = m}, side);

    return nod;
}

static node* getF(const char **ptr, side side) {
    assert(ptr && *ptr);

    op op = WTF;

    #include "../op.h"

        ;
    #undef DEF_CMD

    node *left = getP(ptr, side);

    if(op == WTF)
        return left;

    left->side = LEFT;

    node *right = (node*) malloc(sizeof(node));
    node *nod = (node*) malloc(sizeof(node));

    NodeCtor(right, nod, NOT_DEFINED, {.no_val = NULL}, RIGHT);

    NodeCtor(nod, NULL, OP, {.op = op}, side);
    NodeConnect(nod, left);
    NodeConnect(nod, right);

    return nod;
}

static node* getPow(const char **ptr, side side) {
    assert(ptr && *ptr);

    node *left = getF(ptr, side);

    while(**ptr == '^') {
        left->side = LEFT;

        (*ptr)++;

        node *right = getF(ptr, RIGHT);

        node *nod = (node*) malloc(sizeof(node));

        NodeCtor(nod, NULL, OP, {.op = POW}, side);
        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
    }

    return left;
}

static node* getT(const char **ptr, side side) {
    assert(ptr && *ptr);

    node *left = getPow(ptr, side);

    while(**ptr == '*' || **ptr == '/') {
        op op;

        left->side = LEFT;

        if(**ptr == '*')
            op = MUL;
        else
            op = DIV;

        (*ptr)++;

        node *right = getPow(ptr, RIGHT);

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
        return getC(ptr, side);
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