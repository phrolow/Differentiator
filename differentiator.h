#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H

#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>

#define DEBUG
//#define FATAL_VERIFY

#define DEF_CMD(name, num, sign, ...) name = num,

typedef enum OP {
    #include "op.h"
} op;

#undef DEF_CMD

typedef enum TYPE {
    NOT_DEFINED,
    OP,
    VAR,
    CONST,
    MATH_CONST,
} type;

typedef enum MCONST {
    IDK = 0,
    E,
    PI,
} mconst;

typedef enum SIDE {
    ROOT = -1,
    LEFT = 0,
    RIGHT = 1,
} side;

union value {
    op op;
    int val;
    char name;
    mconst math_const;
    void* no_val;
};

enum TREE_ERROR {
    TREE_OK = 0,
    NULL_NODE,
    INVALID_LINK,
    UNDEFINED_TYPE,
    UNDEFINED_OP,
    NULL_NAME
};

enum EXPRESSION_ERROR {
    EXPRESSION_OK = 0,
    INVALID_EXPRESSION,
};

const size_t BUFSIZE = 0x100;
const size_t NAMESIZE = 0x10;

typedef struct Node {
    struct Node *parent;
    struct Node *children[2];
    type type;
    value value;
    side side;
} node;

typedef struct Tree {
    node *root;
    size_t depth;
    size_t size;
} tree;

struct text {
    char *content;
    char **ptrs;
    size_t  nChar,
            nLine,
            maxLine;
};

struct text maketext(char *content, char** ptrs, size_t nChar, size_t nLine, size_t maxLine);

struct text textFromFile(const char *path);

void TreeCtor(tree *tree, node *node);

node* NodeCopy(node *original);

void TreeDtor(tree *tree);

void SubTreeDtor(node *node);

int NodeCtor(node *new_node, node *parent, type node_type, value node_value, side side);

void NodeDtor(node *nod);

int NodeCheck(node *nod);

int CheckChildren(const node *nod);

void NodeConnect(node *parent, node *child);

int checkfile(char *path);

int streq(const char *txt, const char *str);

tree * ReadExpression(const char *txt);

node* Diff(tree *expression);

void Simplify(tree *expression);

int CalcConst(tree *expression);

int NeutralElems(tree *expression);

int FixMul0(node *node);

int FixMul1(node **nod);

int FixAdd0(node **node);

int FixSub0(node **node);

int Fix0Div(node *node);

int CheckDiv0(node *node);

void TexInit(FILE *tex);

void PrintExpression(tree *expression, FILE *tex);

void TexFinish(FILE *tex);

#ifdef DEBUG

#define PRINT(n) printf("%d %d\n", (n)->type, (n)->value)

const int WRONG = 0xBAD;

const char* const    PNGPATH = "tree.png";
const char* const    HTMLPATH = "log.html";
const char* const    DOTPATH = "tree";

const size_t COLORLEN = 8;

const char  OP_COLOR[COLORLEN] = "#F4CF72",
        CONST_COLOR[COLORLEN] = "#71DCC1",
        VAR_COLOR[COLORLEN] = "#F49872",
        MCONST_COLOR[COLORLEN] = "#B0C4DE",
        ERR_COLOR[COLORLEN] = "#808080",
        RIGHT_COLOR[COLORLEN] = "#7173DC",
        LEFT_COLOR[COLORLEN] = "#CD5C5C";

int NodeVerify(node *node);

void TreeDump(tree *tree);

TREE_ERROR TreeVerify(tree *tree);

#endif //DEBUG

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
