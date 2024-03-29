#include "../differentiator.h"

void TreeCtor(tree *tree, node *root) {
    node *root_node = (node *) malloc(sizeof(node));
    NodeCtor(root_node, NULL, ROOT_NODE, {.no_val = NULL}, ROOT);

    NodeConnect(root_node, root);

    tree->root = root_node;
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