#include "differentiator.h"
#define COMMA ,

#define DEF_CMD(name, num, ch, ...) \
    case name:                      \
        __VA_ARGS__                 \
        break;

static void diff(node *nod) {
    node    *old = nod,
            *rch = NULL,
            *lch = NULL;

    if(!nod) {
        return;
    }

    switch (nod->type) {
        case CONST:
            nod->value.val = 0;
            break;
        case VAR:
            nod->type = CONST;
            nod->value.val = 1;
            break;
        case OP:
            switch (nod->value.op) {
                #include "op.h"
                #undef DEF_CMD
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void Diff(tree *expression) {
    TreeVerify(expression);

    diff(expression->root);
}