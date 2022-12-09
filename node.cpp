#include "differentiator.h"

int NodeCtor(node *new_node, node *parent, type node_type, value node_value, side side) {
    new_node->parent = parent;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->type = node_type;
    new_node->value = node_value;
    new_node->side = side;

    return 0;
}

void NodeDtor(node *node) {
    assert(node);
PRINT(node);
    if(node->parent)
        node->parent->children[node->side] = NULL;

    node->parent = NULL;
    node->type = NOT_DEFINED;
    node->value.no_val = NULL;
    node->children[0] = NULL;
    node->children[1] = NULL;

    free(node);
}

static size_t numchildren(node *node) {
    if(node->children[0]) {
        return 2 + numchildren(node->children[0]) + numchildren(node->children[1]);
    }
    else
        return 0;
}

//static node *nodcpy(const node *orig, node **arr) {
//    node *cur = *arr;
//
//    NodeCtor(cur, orig->parent, orig->type, orig->value, orig->side);
//
//    if(orig->children[0]) {
//        (*arr)++;
//        cur->children[0] = nodcpy(orig->children[0], arr);
//
//        (*arr)++;
//        cur->children[1] = nodcpy(orig->children[1], arr);
//    }
//
//    return cur;
//}
//
//node *NodeCopy(node *original) {
//    node *copy = (node*) malloc((1 + numchildren(original)) * sizeof(node));
//
//    nodcpy(original, &copy);
//
//    return copy - numchildren(original);
//}

static node *nodcpy(const node *orig, node *parent) {
    node *nod = (node*) malloc(sizeof(node));

    NodeCtor(nod, parent, orig->type, orig->value, orig->side);

    if(orig->children[0]) {
        nod->children[LEFT] = nodcpy(orig->children[LEFT], nod);
        nod->children[RIGHT] = nodcpy(orig->children[RIGHT], nod);
    }

    return nod;
}

node *NodeCopy(node *original) {
    return nodcpy(original, NULL);
}