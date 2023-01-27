#include "../differentiator.h"

struct text maketext(char *content, char **ptrs, size_t nChar, size_t nLine, size_t maxLine) {
    struct text newText = { NULL, NULL, 0, 0, 0 };

    newText.content = content;
    newText.ptrs = ptrs;
    newText.nChar = nChar;
    newText.nLine = nLine;
    newText.maxLine = maxLine;

    return newText;
}

struct text textFromFile(const char *path) {
    assert(path);

    FILE *fp = NULL;
    size_t  currentLine = 0,
            i = 0,
            iLine = 0,
            nChar = 0,
            nLine = 0,
            maxLine = 0;
    char **ptrs = NULL;
    char *content = NULL;
    struct stat stats;

    fp = fopen(path, "rb");

    fstat(fileno(fp), &stats);

    nChar = stats.st_size / sizeof(char);

    content = (char*)calloc(nChar + 1, sizeof(char));

    nChar = fread(content, sizeof(char), nChar, fp);

    fclose(fp);

    content = (char*)realloc(content, nChar + 1);

    for(i = 0; i < nChar; i++)
        if(content[i] == '\n')
            nLine++;

    nLine++;

    content[i + 1] = '\0';

    ptrs = (char**)calloc(nLine + 1, sizeof(char*));

    ptrs[0] = &(content[0]);
    iLine++;

    for(i = 0; i < nChar; i++) {
        currentLine++;

        if(content[i] == '\n') {
            ptrs[iLine] = &(content[i + 1]);

            iLine++;

            if(currentLine > maxLine)
                maxLine = currentLine;

            currentLine = 0;
        }
    }

    return maketext(content, ptrs, nChar, nLine, maxLine);
}

int checkfile(char* path) {
    assert(path);

    char* ptr = NULL;

    ptr = strchr(path, '.');

    if(ptr) {
        ptr++;

        if(!stricmp(ptr, "txt") || !stricmp(ptr, "docx") || !stricmp(ptr, "doc") || !stricmp(ptr, "rtf") || !stricmp(ptr, "odt"))
            return 1;
    }

    return 0;
}

int streq(const char *txt, const char *str) {
    int len = strlen(str);

    for(int i = 0; i < len; i++) {
        if(*(txt + i) != *(str + i)) {
            return 0;
        }
    }

    return 1;
}