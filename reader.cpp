#include "differentiator.h"

#define DEF_CMD(name, num, ch, ...)     \
    case ch:                            \
        nod->value.op = name;           \
        break;

static int getConstOrVar(node *nod, const char *start, node *parent, side side) {
    value value;

    start++;

    if(isalpha(*start)) {
        size_t len = strchr(start, ')') - start;

        char *name = (char*)calloc(len + 1, sizeof(char));

        strncpy(name, start, len);

        value.name = name;

        NodeCtor(nod, parent, VAR, value, side);
    }
    else if(isdigit(*start)) {
        const char *ptr = start;
        int val = 0;

        while(*ptr != ')') {
            val = val * 10 + *ptr - '0';
            ptr++;
        }

        value.val = val;

        NodeCtor(nod, parent, CONST, value, side);
    }
    else
        return INVALID_EXPRESSION;

    return EXPRESSION_OK;
}

static const char* searchclosep(const char *ptr) {
    int parenthesis = 1;

    ptr++;

    while(parenthesis) {
        if(*ptr == '(') {
            parenthesis++;
        }
        else if(*ptr == ')') {
            parenthesis--;
        }

        ptr++;
    }

    return ptr - 1;
}

static node *getNode(const char *start, const char *stop, node *parent, side side) {
    node *nod = (node*) malloc(sizeof(node));

    const char *closep = searchclosep(start + 1);

    if(closep == stop) {
        getConstOrVar(nod, start, parent, side);
    }
    else {
        value op;
        op.op = NOT_DEFINED_OP;

        NodeCtor(nod, parent, OP, op, side);

        switch (*(closep + 1)) {
            #include "op.h"
            default:
                return NULL;
        }

        #undef DEF_CMD

        printf("%d %d\n", nod->type, nod->value);

        nod->children[LEFT] = getNode(start + 1, closep, nod, LEFT);
        nod->children[RIGHT] = getNode(closep + 2, stop - 1, nod, RIGHT);
    }

    return nod;
}

void ReadExpression(tree *expression, const char *txt) {
    expression->root = getNode(txt, txt + strlen(txt) - 1, NULL, ROOT);
}