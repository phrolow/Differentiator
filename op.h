DEF_CMD(ADD, 1, "+", {
    diff(nod->children[0]);
    diff(nod->children[1]);
})

DEF_CMD(SUB, 2, "-", {
    diff(nod->children[0]);
    diff(nod->children[1]);
})

DEF_CMD(MUL, 3, "*", {
    node    *right = NULL COMMA
            *left = NULL COMMA
            *old = NULL;

    right = nod->children[RIGHT];
    left = nod->children[LEFT];
    node *cpy = NodeCopy(nod);
    node *denom_mul = (node *) malloc(sizeof(node));
    old = nod;

    NodeCtor(denom_mul COMMA nod COMMA OP COMMA {.op = MUL} COMMA RIGHT);

    NodeConnect(denom_mul COMMA left);
    NodeConnect(denom_mul COMMA right);

    nod->value.op = ADD;

    cpy->side = LEFT;
    NodeConnect(nod COMMA cpy);
    NodeConnect(nod COMMA denom_mul);

    diff(cpy->children[0]);
    diff(denom_mul->children[1]);
})

DEF_CMD(DIV, 4, "/", {
    node    *left = NULL,
            *right = NULL;

    right = nod->children[RIGHT];
    left = nod->children[LEFT];

    node *denom_mul = (node *) malloc(sizeof(node));
    node *sub = (node *) malloc(sizeof(node));
    node *new_mul = (node *) malloc(sizeof(node));
    node *cpy = (node *) malloc(sizeof(node));

    cpy = NodeCopy(nod);
    cpy->value.op = MUL;

    nod->children[RIGHT] = denom_mul;

    NodeCtor(denom_mul COMMA nod COMMA OP COMMA {.op = MUL} COMMA RIGHT);

    denom_mul->children[0] = NodeCopy(right);
    denom_mul->children[1] = NodeCopy(right);

    nod->children[LEFT] = sub;

    NodeCtor(sub COMMA nod COMMA OP COMMA {.op = SUB} COMMA LEFT);

    sub->children[LEFT] = cpy;
    sub->children[RIGHT] = new_mul;

    NodeCtor(new_mul COMMA sub COMMA OP COMMA {.op = MUL} COMMA RIGHT);

    NodeConnect(new_mul COMMA left);
    NodeConnect(new_mul COMMA right);

    diff(right);
    diff(cpy->children[LEFT]);
})

DEF_CMD(POW, 5, "^", {
    node *top_mul = (node*) malloc(sizeof(node));
    NodeCtor(top_mul COMMA nod->parent COMMA OP COMMA {.op = MUL} COMMA nod->side);
    if(nod->parent)
        nod->parent->children[nod->side] = top_mul;

    nod->side = LEFT;
    NodeConnect(top_mul COMMA nod);

    node *right_mul = (node*) malloc(sizeof(node));
    NodeCtor(right_mul COMMA top_mul COMMA OP COMMA {.op = MUL} COMMA RIGHT);
    NodeConnect(top_mul COMMA right_mul);

    node *right_copy = NodeCopy(nod->children[RIGHT]);
    right_copy->side = LEFT;
    NodeConnect(right_mul COMMA right_copy);

    node *ln = (node*) malloc(sizeof(node));
    NodeCtor(ln COMMA right_mul COMMA OP COMMA {.op = LN} COMMA RIGHT);
    NodeConnect(right_mul COMMA ln);

    node *left_copy = NodeCopy(nod->children[LEFT]);
    left_copy->side = LEFT;
    NodeConnect(ln COMMA left_copy);

    node *empty = (node*) malloc(sizeof(node));
    NodeCtor(empty COMMA ln COMMA NOT_DEFINED COMMA {.no_val = NULL} COMMA RIGHT);
    NodeConnect(ln, empty);

    nod = top_mul;

    diff(right_mul);
})

DEF_CMD(SIN, 6, "sin", {
    node *right = NULL;

    node *top_mul = (node*) malloc(sizeof(node));
    NodeCtor(top_mul COMMA nod->parent COMMA OP COMMA {.op = MUL} COMMA nod->side);
    NodeConnect(nod->parent COMMA top_mul);

    nod->side = LEFT;
    NodeConnect(top_mul COMMA nod);
    nod->value.op = COS;

    right = NodeCopy(nod->children[LEFT]);
    right->side = RIGHT;
    NodeConnect(top_mul COMMA right);

    nod = top_mul;

    diff(right);
})

DEF_CMD(COS, 7, "cos", {
    node *right = NULL;

    node *mul_minus = (node*) malloc(sizeof(node));
    NodeCtor(mul_minus COMMA nod->parent COMMA OP COMMA {.op = MUL} COMMA nod->side);
    NodeConnect(nod->parent COMMA mul_minus);

    node *minus = (node*) malloc(sizeof(node));
    NodeCtor(minus COMMA mul_minus COMMA CONST COMMA {.val = (-1)} COMMA RIGHT);
    NodeConnect(mul_minus COMMA minus);

    node *top_mul = (node*) malloc(sizeof(node));
    NodeCtor(top_mul COMMA mul_minus COMMA OP COMMA {.op = MUL} COMMA LEFT);
    NodeConnect(mul_minus COMMA top_mul);

    nod->side = LEFT;
    NodeConnect(top_mul COMMA nod);

    node *cpy = NodeCopy(nod->children[LEFT]);
    cpy->side = RIGHT;
    NodeConnect(top_mul COMMA cpy);

    nod = mul_minus;

    diff(cpy);
})

DEF_CMD(LN, 8, "ln", {
    NodeDtor(nod->children[RIGHT]);

    node *cpy = NodeCopy(nod->children[LEFT]);
    cpy->side = RIGHT;
    NodeConnect(nod COMMA cpy);

    nod->value.op = DIV;

    diff(nod->children[LEFT]);
})

DEF_CMD(WTF, 0, "", {

})