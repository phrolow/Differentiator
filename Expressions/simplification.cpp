#include "../differentiator.h"

void Simplify(tree *expression) {
    TreeVerify(expression);

    while(CalcConst(expression) + NeutralElems(expression));
}

static void calcnode(node *nod) {
   int      a = nod->children[LEFT]->value.val,
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

    return calcconst(expression->root->children[LEFT]);
}

static int neutr_elts(node *nod) {
    if(!(NodeCheck(nod) && nod->type == OP))
        return 0;

    int res = 0;

    if(CheckDiv0(nod)) {
        printf("Zero division\n");
        exit(3);
    }

    if(nod->children[LEFT])
        res |= neutr_elts(nod->children[LEFT]);

    if(nod->children[RIGHT])
        res |= neutr_elts(nod->children[RIGHT]);

     res |= (FixMul1(nod)
        || FixMul0(nod)
        || FixAdd0(nod)
        || FixSub0(nod)
        || Fix0Div(nod)
        );

     return res;
}

int NeutralElems(tree *expression) {
    TreeVerify(expression);

    return neutr_elts(expression->root->children[LEFT]);
}

int FixMul0(node *node) {
    if(node->type != OP || node->value.op != MUL)
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

int FixMul1(node *nod) {
    assert(nod);

    if(nod->type != OP || nod->value.op != MUL) {
        return 0;
    }

    for(int i = 0; i < 2; i++) {
        if(nod->children[i]->type == CONST && nod->children[i]->value.val == 1) {
            node *other_child = nod->children[!i],
                 *parent = nod->parent;

            other_child->side = nod->side;

            NodeDtor(nod->children[i]);
            NodeDtor(nod);

            NodeConnect(parent, other_child);

            return 1;
        }
    }

    return 0;
}

int FixAdd0(node *nod) {
    assert(nod);

    if(nod->value.op != ADD)
        return 0;

    for(int i = 0; i < 2; i++) {
        if(nod->children[i]->type == CONST && nod->children[i]->value.val == 0) {
            node *other_child = nod->children[!i],
                    *parent = nod->parent;

            other_child->side = nod->side;

            NodeDtor(nod->children[i]);
            NodeDtor(nod);

            NodeConnect(parent, other_child);

            return 1;
        }
    }

    return 0;
}

int FixSub0(node *nod) {
    assert(nod);

    if(nod->type != OP || nod->value.op != SUB)
        return 0;

    node *right = nod->children[RIGHT];

    if(right->type == CONST && right->value.val == 0) {
        node *other_child = nod->children[LEFT],
                *parent = nod->parent;

        other_child->side = nod->side;

        NodeDtor(nod->children[RIGHT]);
        NodeDtor(nod);

        NodeConnect(parent, other_child);

        return 1;
    }

    return 0;
}

int Fix0Div(node *node) {
    if(node->type != OP || node->value.op != DIV)
        return 0;

    if(node->children[LEFT]->type == CONST && node->children[LEFT]->value.val == 0) {
        SubTreeDtor(node->children[RIGHT]);

        node->type = CONST;
        node->value.val = 0;

        return 1;
    }

    return 0;
}

int CheckDiv0(node *node) {
    if(node->type != OP || node->value.op != DIV || !CheckChildren(node))
        return 0;

    if(node->children[RIGHT]->type == CONST && node->children[RIGHT]->value.val == 0) {
        return 1;
    }

    return 0;
}