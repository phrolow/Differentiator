#include "differentiator.h"

void Simplify(tree *expression) {
    TreeVerify(expression);

    while(CalcConst(expression) || NeutralElems(expression));
}

int CalcConst(tree *expression) {
    TreeVerify(expression);

    return 0;
}

static int neutr_elts(node *nod) {
    if(!nod || nod->type != OP)
        return 0;

    if(CheckDiv0(nod)) {
        //наебнуть
    }

    return FixMul0(nod) + FixMul1(nod) + FixAdd0(nod) + FixSub0(nod) + Fix0Div(nod) + neutr_elts(nod->children[LEFT]) + neutr_elts(nod->children[RIGHT]);
}

int NeutralElems(tree *expression) {
    TreeVerify(expression);

    return neutr_elts(expression->root);
}

int FixMul0(node *node) {
    if(node->value.op != MUL)
        return 0;

    for(int i = 0; i < 2; i++) {
        if(node->children[i]->type == CONST && node->children[i]->value.val == 0) {
            SubTreeDtor(node->children[!i]);
            NodeDtor(node->children[i]);

            node->type = CONST;
            node->value.val = 0;

            return 1;
        }
    }

    return 0;
}

int FixMul1(node *node) {
    if(node->value.op != MUL)
        return 0;

    for(int i = 0; i < 2; i++) {
        if(node->children[i]->type == CONST && node->children[i]->value.val == 1) {
            node->children[!i]->parent = node->parent;
            node->parent->children[node->children[!i]->side] = node->children[!i];

            NodeDtor(node->children[i]);
            NodeDtor(node);

            return 1;
        }
    }

    return 0;
}

int FixAdd0(node *node) {
    if(node->value.op != ADD)
        return 0;


    for(int i = 0; i < 2; i++) {
        if(node->children[i]->type == CONST && node->children[i]->value.val == 0) {
            node->children[!i]->parent = node->parent;
            node->parent->children[node->children[!i]->side] = node->children[!i];

            NodeDtor(node->children[i]);
            NodeDtor(node);

            return 1;
        }
    }

    return 0;
}

int FixSub0(node *node) {
    if(node->value.op != SUB)
        return 0;


    for(int i = 0; i < 2; i++) {
        if(node->children[i]->type == CONST && node->children[i]->value.val == 0) {
            node->children[!i]->parent = node->parent;
            node->parent->children[node->children[!i]->side] = node->children[!i];

            NodeDtor(node->children[i]);
            NodeDtor(node);

            return 1;
        }
    }

    return 0;
}

int Fix0Div(node *node) {
    if(node->value.op != DIV)
        return 0;

    if(node->children[LEFT]->type == CONST && node->children[LEFT]->value.val == 0) {
        SubTreeDtor(node->children[RIGHT]);
        NodeDtor(node->children[RIGHT]);

        node->type = CONST;
        node->value.val = 0;

        return 1;
    }

    return 0;
}

int CheckDiv0(node *node) {
    if(node->value.op != DIV)
        return 0;

    if(node->children[RIGHT]->type == CONST && node->children[RIGHT]->value.val == 0) {
        return 1;
    }

    return 0;
}