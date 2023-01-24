#include "differentiator.h"

#define TEX(str) fprintf(tex, str)

int TexInit(FILE *tex) {
    tex = fopen("output.tex", "w");

    TEX("\\input{header.tex}     \n\n");
    TEX("\\usepackage{upgreek} \n\n\n");
    TEX("\\begin{document}     \n\n\n");
}

static const char* GetOpFormat(op op) {
    const char *format = NULL;

    switch (op) {
        case ADD:
            format = "%s + %s";
            break;
        case SUB:
            format = "%s - %s";
            break;
        case MUL:
            format = "%s %s";
            break;
        case DIV:
            format = "\frac{%s}{%s}";
            break;
        case POW:
            format = "{%s}^{%s}";
            break;
        case SIN:
            format = "sin(%s)";
            break;
    }
}

static char* PrintNode(node *nod) {
    char *tex = (char*) calloc(BUFSIZE, sizeof(char));

    char    *left = NULL,
            *right = NULL;

    switch (nod->type) {
        case OP:
            left = PrintNode(nod->children[LEFT]);
            right = PrintNode(nod->children[RIGHT]);

            sprintf(tex, GetOpFormat(nod->value.op), left, right);

            free(left);
            free(right);

            break;
        case VAR:
            sprintf(tex, "%c\n", nod->value.name);
            break;
        case CONST:
            sprintf(tex, "%d\n", nod->value.val);
            break;
        case MATH_CONST:
            sprintf(tex, GetMathConst(nod->value.math_const));
            break;
        default:
            break;
    }

    return tex;
}

void PrintExpression(tree *expression) {
    FILE *tex = fopen("output.tex", "w");


}