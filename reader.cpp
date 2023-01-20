#include "differentiator.h"

#define DEF_CMD(name, num, ch, ...)     \
    case ch:                            \
        nod->value.op = name;           \
        break;



//static node* getP(node *nod, node *parent, side side, const char **ptr);
//
//static int getConstOrVar(node *nod, const char *start, node *parent, side side) {
//    value value;
//
//    start++;
//
//    if(isalpha(*start)) {
//        size_t len = strchr(start, ')') - start;
//
//        char *name = (char*)calloc(len + 1, sizeof(char));
//
//        strncpy(name, start, len);
//
//        value.name = name;
//
//        NodeCtor(nod, parent, VAR, value, side);
//    }
//    else if(isdigit(*start)) {
//        const char *ptr = start;
//        int val = 0;
//
//        while(*ptr != ')') {
//            val = val * 10 + *ptr - '0';
//            ptr++;
//        }
//
//        value.val = val;
//
//        NodeCtor(nod, parent, CONST, value, side);
//    }
//    else
//        return INVALID_EXPRESSION;
//
//    return EXPRESSION_OK;
//}
//
//static const char* searchclosep(const char *ptr) {
//    int parenthesis = 1;
//
//    ptr++;
//
//    while(parenthesis) {
//        if(*ptr == '(') {
//            parenthesis++;
//        }
//        else if(*ptr == ')') {
//            parenthesis--;
//        }
//
//        ptr++;
//    }
//
//    return ptr - 1;
//}
//
//static node *getNode(const char *start, const char *stop, node *parent, side side) {
//    node *nod = (node*) malloc(sizeof(node));
//
//    const char *closep = searchclosep(start + 1);
//
//    if(closep == stop) {
//        getConstOrVar(nod, start, parent, side);
//    }
//    else {
//        value op;
//        op.op = NOT_DEFINED_OP;
//
//        NodeCtor(nod, parent, OP, op, side);
//
//        switch (*(closep + 1)) {
//            #include "op.h"
//            default:
//                return NULL;
//        }
//
//        #undef DEF_CMD
//
//        nod->children[LEFT] = getNode(start + 1, closep, nod, LEFT);
//        nod->children[RIGHT] = getNode(closep + 2, stop - 1, nod, RIGHT);
//    }
//
//    return nod;
//}
//
//static node* getN(node *nod, node *parent, side side, const char **ptr) {
//    type type;
//    value value;
//
//    if(isdigit(**ptr)) {
//        type = CONST;
//
//        value.val = 0;
//
//        while(isdigit(**ptr))
//            value.val = value.val * 10 + *(*ptr++) - '0';
//    }
//    else if(isalpha(**ptr)) {
//        type = VAR;
//
//        value.name = (char*) calloc(NAMESIZE, sizeof(char));
//
//        int len = 0;
//
//        while (isalpha(**ptr)) {
//            value.name[len++] = **ptr;
//
//            (*ptr)++;
//        }
//    }
//    else {
//        printf("INVALID EXPRESSION");
//
//        abort();
//    }
//
//    NodeCtor(nod, parent, type, value, side);
//}
//
//static node* getT(node *nod, node *parent, side side, const char **ptr) {
//    op op;
//
//    node *left = (node *) malloc(sizeof(node));
//
//    left = getP(left, nod, LEFT, ptr);
//
//    switch (**ptr) {
//        case '*':
//            op = MUL;
//            break;
//        case '/':
//            op = DIV;
//            break;
//        default:
//            left->parent = parent;
//            left->side = side;
//            if(parent)
//                parent->children[side] = nod;
//
//            return left;
//    }
//
//    (*ptr)++;
//
//    node *right = (node *) malloc(sizeof(node));
//
//    NodeCtor(nod, parent, OP, {.op = op}, side);
//
//    right = getT(right, nod, RIGHT, ptr);
//
//    nod->children[LEFT] = left;
//    nod->children[RIGHT] = right;
//
//    return right;
//}
//
//static node* getE(node *nod, node *parent, side side, const char **ptr) {
//    op op;
//
//    node *left = (node *) malloc(sizeof(node));
//
//    left = getT(left, nod, LEFT, ptr);
//
//    switch (**ptr) {
//        case '+':
//            op = ADD;
//            break;
//        case '-':
//            op = MUL;
//            break;
//        default:
//            left->parent = parent;
//            left->side = side;
//            if(parent)
//                parent->children[side] = nod;
//
//            return left;
//    }
//
//    (*ptr)++;
//
//    node *right = (node *) malloc(sizeof(node));
//
//    NodeCtor(nod, parent, OP, {.op = op}, side);
//
//    right = getE(right, nod, RIGHT, ptr);
//
//    nod->children[LEFT] = left;
//    nod->children[RIGHT] = right;
//
//    return right;
//}
//
//static node* getP(node *nod, node *parent, side side, const char **ptr) {
//    if(**ptr != '(') {
//        return getN(nod, parent, side, ptr);;
//    }
//
//    (*ptr)++;
//
//    return getE(nod, parent, side, ptr);
//}
//
//static node* getG(node *nod, const char *expression) {
//    nod = (node*) malloc(sizeof(node));
//
//    nod = getE(nod, NULL, ROOT, &expression);
//
//    if(*expression == '\0') {
//        printf("INVALID EXPRESSION");
//
//        abort();
//    }
//
//    return nod;
//}

static node* getN(node *nod, node *parent, side side, const char **ptr) {
    type type;
    value value;

    if(isdigit(**ptr)) {
        type = CONST;

        value.val = 0;

        while(isdigit(**ptr))
            value.val = value.val * 10 + *(*ptr++) - '0';
    }
    else if(isalpha(**ptr)) {
        type = VAR;

        value.name = (char*) calloc(NAMESIZE, sizeof(char));

        int len = 0;

        while (isalpha(**ptr)) {
            value.name[len++] = **ptr;

            (*ptr)++;
        }
    }
    else {
        printf("INVALID EXPRESSION");

        abort();
    }

    NodeCtor(nod, parent, type, value, side);
}

static node* getG(node *nod, const char *expression) {
    assert(nod && expression);

    nod = (node*) malloc(sizeof(node));



    if(*expression != '\0') {
        printf("INVALID EXPRESSION");

        abort();
    }

    return nod;
}

void ReadExpression(tree *expression, const char *txt) {
    //expression->root = getNode(txt, txt + strlen(txt) - 1, NULL, ROOT);

    getG(expression->root, txt);
}