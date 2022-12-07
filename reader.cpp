#include "differentiator.h"

#define DEF_CMD(name, num, ch, ...)     \
    case ch:                            \
        nod->value.op = name;           \
        break;

static int getConstOrVar(node *nod, const char *start) {
    start++;

    if(isalpha(*start)) {
        nod->type = VAR;

        size_t len = strchr(start, ')') - start;

        char *name = (char*)calloc(len + 1, sizeof(char));

        strncpy(name, start, len);

        nod->value.name = name;
    }
    else if(isdigit(*start)) {
        nod->type = CONST;

        const char *ptr = start;
        int val = 0;

        while(*ptr != ')') {
            val = val * 10 + *ptr - '0';
            ptr++;
        }

        nod->value.val = val;
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

static node *getNode(const char *start, const char *stop) {
    node *nod = (node*) malloc(sizeof(node));

    const char *closep = searchclosep(start + 1);

    if(closep == stop) {
        getConstOrVar(nod, start);
    }
    else {
        nod->type = OP;

        switch (*(closep + 1)) {
            #include "op.h"
            default:
                return NULL;
        }

        #undef DEF_CMD

        printf("%d %d\n", nod->type, nod->value);

        nod->children[LEFT] = getNode(start + 1, closep);
        nod->children[RIGHT] = getNode(closep + 2, stop - 1);
    }

    return nod;
}

void ReadExpression(tree *expression, const char *txt) {
    expression->root = getNode(txt, txt + strlen(txt) - 1);
}