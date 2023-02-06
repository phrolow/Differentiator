#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H

#include "Text/onega.h"
#include "Tree/tree.h"

#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>

#define DEBUG
//#define FATAL_VERIFY

const size_t BUFSIZE = 0x100;
const size_t NAMESIZE = 0x10;

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

int FixMul1(node *nod);

int FixAdd0(node *node);

int FixSub0(node *node);

int Fix0Div(node *node);

int CheckDiv0(node *node);

void TexInit(FILE *tex);

void PrintExpression(tree *expression, FILE *tex);

void TexFinish(FILE *tex);

#ifdef DEBUG

#include "Debug/debug.h"

#endif //DEBUG

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
