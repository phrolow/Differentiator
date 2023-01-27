#include "../differentiator.h"

void TreeCtor(tree *tree, node *root) {
    tree->root = root;
}

static void nodedtor_(node *node) {
    if(CheckChildren(node)) {
        nodedtor_(node->children[0]);
        nodedtor_(node->children[1]);
    }

    NodeDtor(node);
}

void SubTreeDtor(node *node) {
    nodedtor_(node);
}

void TreeDtor(tree *tree) {
    SubTreeDtor(tree->root);
}