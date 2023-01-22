DEF_CMD(ADD, 1, "+", {
    diff(nod->children[0]);
    diff(nod->children[1]);
})

DEF_CMD(SUB, 2, "-", {
    diff(nod->children[0]);
    diff(nod->children[1]);
})

DEF_CMD(MUL, 3, "*", {
    rch = nod->children[RIGHT];
    lch = nod->children[LEFT];
    node *cpy = NodeCopy(nod);
    node *denom_mul = (node *) malloc(sizeof(node));
    old = nod;

    value temp;     //костыль ебаный
    temp.op = MUL;

    NodeCtor(denom_mul COMMA nod COMMA OP COMMA temp COMMA RIGHT);

    rch->parent = denom_mul;
    lch->parent = denom_mul;

    denom_mul->children[LEFT] = lch;
    denom_mul->children[RIGHT] = rch;

    nod->value.op = ADD;

    nod->children[LEFT] = cpy;
    nod->children[RIGHT] = denom_mul;

    diff(cpy->children[0]);
    diff(denom_mul->children[1]);
})

DEF_CMD(DIV, 4, "/", {
    rch = nod->children[RIGHT];
    lch = nod->children[LEFT];
    node *denom_mul = (node *) malloc(sizeof(node));
    node *sub = (node *) malloc(sizeof(node));
    node *new_mul = (node *) malloc(sizeof(node));
    node *cpy = (node *) malloc(sizeof(node));

    cpy = NodeCopy(nod);
    cpy->value.op = MUL;

    nod->children[RIGHT] = denom_mul;

    value temp;     //костыль ебаный
    temp.op = MUL;

    NodeCtor(denom_mul COMMA nod COMMA OP COMMA temp COMMA RIGHT);

    denom_mul->children[0] = NodeCopy(rch);
    denom_mul->children[1] = NodeCopy(rch);

    nod->children[LEFT] = sub;

    temp.op = SUB;

    NodeCtor(sub COMMA nod COMMA OP COMMA temp COMMA LEFT);

    sub->children[LEFT] = cpy;
    sub->children[RIGHT] = new_mul;

    temp.op = MUL;

    NodeCtor(new_mul COMMA sub COMMA OP COMMA temp COMMA RIGHT);

    new_mul->children[LEFT] = lch;
    new_mul->children[RIGHT] = rch;

    lch->parent = new_mul;
    rch->parent = new_mul;

    diff(rch);
    diff(cpy->children[LEFT]);
})

DEF_CMD(POW, 5, "^", {
    printf("huipizda");
})

DEF_CMD(SIN, 6, "sin", {
    printf("nahui idi");
})

DEF_CMD(COS, 7, "cos", {
    printf("nahui idi");
})

DEF_CMD(LN, 8, "ln", {
    printf("nahui idi");
})

DEF_CMD(SH, 9, "sh", {
    printf("nahui idi");
})

DEF_CMD(CH, 10, "ch", {
    printf("nahui idi");
})

DEF_CMD(WTF, 0, "\1", {
    printf("nahui idi");
})