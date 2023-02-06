#include "../differentiator.h"

int NodeCtor(node *new_node, node *parent, type node_type, value node_value, side side) {
    new_node->parent = parent;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->type = node_type;
    new_node->value = node_value;
    new_node->side = side;

    return 0;
}

void NodeDtor(node *nod) {
    //assert(nod);

    printf("DTOR says:\n");
    PRINT(nod);

    if(nod->parent)
        nod->parent->children[nod->side] = NULL;

    nod->parent = NULL;
    nod->type = NOT_DEFINED;
    nod->value.no_val = NULL;
    nod->children[0] = NULL;
    nod->children[1] = NULL;

    free(nod);
}

static size_t numchildren(node *node) {
    if(CheckChildren(node)) {
        return 2 + numchildren(node->children[0]) + numchildren(node->children[1]);
    }
    else
        return 0;
}

static node *nodcpy(const node *orig, node *parent) {
    node *nod = (node*) malloc(sizeof(node));

    NodeCtor(nod, parent, orig->type, orig->value, orig->side);

    if(orig->children[LEFT])
        nod->children[LEFT] = nodcpy(orig->children[LEFT], nod);
    if(orig->children[RIGHT])
        nod->children[RIGHT] = nodcpy(orig->children[RIGHT], nod);

    return nod;
}

node *NodeCopy(node *original) {
    return nodcpy(original, NULL);
}

int NodeCheck(node *nod) {
    return nod
        && (nod->value.no_val != NULL)
        && (nod->type != NOT_DEFINED);
}

int CheckChildren(const node *nod) {
    return nod->children[0] && nod->children[1];
}

void NodeConnect(node *parent, node *child) {
    child->parent = parent;

    if(parent)
        parent->children[child->side] = child;
}