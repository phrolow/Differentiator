#include "differentiator.h"

#define TEX(str) fprintf(tex, str)

void TexInit(FILE *tex) {
    if(!tex) {
        fprintf(stderr, "TEX: file didn't open");
    }

    TEX("\\input{header.tex}     \n\n");
    TEX("\\usepackage{upgreek} \n\n\n");
    TEX("\\begin{document}     \n\n\n");
    TEX("$\\left( ");
}

static const char* GetOpFormat(op op) {
    const char *format = NULL;

    switch (op) {
        case ADD:
            format = "%s + %s ";
            break;
        case SUB:
            format = "%s - %s ";
            break;
        case MUL:
            format = "%s %s ";
            break;
        case DIV:
            format = "\\frac{ %s } { %s } ";
            break;
        case POW:
            format = "{ %s }^{ %s } ";
            break;
        case SIN:
            format = "sin \\left( %s \\right) ";
            break;
        case COS:
            format = "cos \\left( %s \\right) ";
            break;
        case LN:
            format = "ln \\left( %s \\right) ";
            break;
        default:
            fprintf(stderr, "TEX: unknown operation");
            format = "";
            break;
    }

    return format;
}

static const char *GetMathConst(mconst m) {
    const char *math_const = NULL;

    switch(m) {
        case E:
            math_const = " e ";
            break;
        case PI:
            math_const = " \\pi ";
            break;
    }

    return math_const;
}

static char* PrintNode(node *nod) {
    assert(nod);

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

void PrintExpression(tree *expression, FILE *tex) {
    assert(expression);
    assert(tex);

    fputs(PrintNode(expression->root), tex);
}

void TexFinish(FILE *tex) {
    assert(tex);

    fprintf(tex, "$\n \\end{document}");

    fclose(tex);
}