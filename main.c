#include "problem_st_2.h"

int main() {
    int res;
    char f_name[20] = {0};
    char* inp;

    res = scanf ("%s", f_name);

    if (res != 1) {
        printf ("ERROR: file is empty\n");
        exit (0);
    }

    inp = read_file (f_name);
    parsing(&inp);
    return 0;
}
