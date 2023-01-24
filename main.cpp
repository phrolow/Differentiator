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
    FILE *tex = NULL;

    TexInit(tex);

    struct text txt = textFromFile(argv[1]);

    expression = ReadExpression(txt.content);

    //PrintExpression(expression);

    expression->root = Diff(expression);

    Simplify(expression);

    TreeDump(expression);

//    PrintExpression(expression);

    TreeDtor(expression);

    return 0;
}
