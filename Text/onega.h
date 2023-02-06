#ifndef DIFFERENTIATOR_ONEGA_H
#define DIFFERENTIATOR_ONEGA_H

#include <ctype.h>

struct text {
    char *content;
    char **ptrs;
    size_t  nChar,
            nLine,
            maxLine;
};

struct text maketext(char *content, char** ptrs, size_t nChar, size_t nLine, size_t maxLine);

struct text textFromFile(const char *path);

#endif //DIFFERENTIATOR_ONEGA_H
