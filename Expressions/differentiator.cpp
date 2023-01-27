#include "../differentiator.h"
#define COMMA ,

#define DEF_CMD(name, num, sign, ...) \
    case name:                      \
        __VA_ARGS__                 \
        break;

static node* diff(node *nod) {
    if(!nod) {
        return NULL;
    }

    switch (nod->type) {
        case MATH_CONST:
            nod->type = CONST;
        case CONST:
            nod->value.val = 0;
            break;
        case VAR:
            nod->type = CONST;
            nod->value.val = 1;
            break;
        case OP:
            switch (nod->value.op) {
                #include "../op.h"

#undef DEF_CMD
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return nod;
}

node* Diff(tree *expression) {
    TreeVerify(expression);

    return diff(expression->root);
}