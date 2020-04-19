#include "problem_st_2.h"

int main() {
    int res;
    char inp[MAXLEN] = {0};

    res = scanf("%1023c", inp);
    if (res != 1) {
        printf ("ERROR: file is empty\n");
        exit (0);
    }

    parsing(inp);
    return 0;
}
