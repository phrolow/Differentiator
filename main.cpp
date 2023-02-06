#include "differentiator.h"

int main(int argc, char **argv) {
    if(!argc) {
        printf("Too few arguments!");
        exit(1);
    }

    if(!checkfile(argv[1])) {
        printf("Invalid file");
        exit(2);
    }

    tree *expression;
    FILE *tex  = fopen("output.tex", "w");;

    TexInit(tex);

    struct text txt = textFromFile(argv[1]);

    expression = ReadExpression(txt.content);

    PrintExpression(expression, tex);

    fprintf(tex, "\\right) ' = ");

    expression->root = Diff(expression);

    TreeDump(expression);

    Simplify(expression);

    PrintExpression(expression, tex);

    TexFinish(tex);

    TreeDtor(expression);

    return 0;
}
