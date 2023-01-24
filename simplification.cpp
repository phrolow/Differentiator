#include "differentiator.h"

void Simplify(tree *expression) {
    TreeVerify(expression);

    while(CalcConst(expression) + NeutralElems(expression));
}

static void calcnode(node *nod) {
   double   a = nod->children[LEFT]->value.val,
            b = nod->children[RIGHT]->value.val,
            res = 0;

    switch (nod->value.op) {
        case ADD:
            res = a + b;
            break;
        case SUB:
            res = a - b;
            break;
        case MUL:
            res = a * b;
            break;
        case DIV:
            res = a / b;
            break;
        case POW:
            res = pow(a, b);
            break;
        default:
            return;
    }

    NodeDtor(nod->children[0]);
    NodeDtor(nod->children[1]);

    nod->type = CONST;
    nod->value.val = res;
}

static int calcconst(node *nod) {
    if(nod->type == CONST)
        return 1;

    if(nod->type == VAR || nod->type == MATH_CONST || nod->type == NOT_DEFINED)
        return 0;

    if(calcconst(nod->children[0]) * calcconst(nod->children[1])) {
        calcnode(nod);

        return 1;
    }

    return 0;
}

int CalcConst(tree *expression) {
    TreeVerify(expression);

    calcconst(expression->root);

    return 0;
}

static int neutr_elts(node *nod) {
    if(!(NodeCheck(nod) && nod->type == OP))
        return 0;

    if(CheckDiv0(nod)) {
        //наебнуть
    }

    return FixMul0(nod)
        || FixMul1(nod)
        || FixAdd0(nod)
        || FixSub0(nod)
        || Fix0Div(nod)
        || neutr_elts(nod->children[LEFT]) + neutr_elts(nod->children[RIGHT]);
}

int NeutralElems(tree *expression) {
    TreeVerify(expression);

    return neutr_elts(expression->root);
}

int FixMul0(node *node) {
    if(node->value.op != MUL)
        return 0;

    for(int i = 0; i < 2; i++) {
        if(node->children[i]->type == CONST && node->children[i]->value.val == 0 && node->side != ROOT) {
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
        if(node->children[i]->type == CONST && node->children[i]->value.val == 1 && node->side != ROOT) {
            node->children[!i]->parent = node->parent;
            node->parent->children[node->side] = node->children[!i];
            node->children[!i]->side = node->side;

            node->parent = NULL;

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
        if(node->children[i]->type == CONST && node->children[i]->value.val == 0 && node->side != ROOT) { //с корнем пофикшу но потом
            node->children[!i]->side = node->side;
            NodeConnect(node->parent, node->children[!i]);

            node->parent = NULL;

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


    if(node->children[1]->type == CONST && node->children[1]->value.val == 0 && node->side != ROOT) {
        node->children[0]->parent = node->parent;
        node->parent->children[node->side] = node->children[0];
        node->children[0]->side = node->side;

        node->parent = NULL;

        NodeDtor(node->children[1]);
        NodeDtor(node);

        return 1;
    }

    return 0;
}

int Fix0Div(node *node) {
    if(node->value.op != DIV)
        return 0;

    if(CheckChildren(node) && node->children[LEFT]->type == CONST && node->children[LEFT]->value.val == 0) {
        SubTreeDtor(node->children[RIGHT]);
        NodeDtor(node->children[RIGHT]);

        node->type = CONST;
        node->value.val = 0;

        return 1;
    }

    return 0;
}

int CheckDiv0(node *node) {
    if(node->value.op != DIV || !CheckChildren(node))
        return 0;

    if(node->children[RIGHT]->type == CONST && node->children[RIGHT]->value.val == 0 && node->side != ROOT) {
        return 1;
    }

    return 0;
}