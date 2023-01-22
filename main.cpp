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

    struct text txt = textFromFile(argv[1]);

    expression = ReadExpression(txt.content);

//    Diff(expression);

//    Simplify(&expression);

//    CalcConst(&expression);

    TreeDump(expression);

//    PrintExpression(&res);

    TreeDtor(expression);

    return 0;
}
