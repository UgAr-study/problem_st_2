#include "problem_st_2.h"

void parsing(char* str) {
    struct lexem_t cur_lex;
    while (*str != '\0') {
        cur_lex = get_cur_lexem(&str);

        if (cur_lex.kind != VAR_NAME) {
            printf ("Wrong format: expected variable in the beginning of the string\n");
            exit (0);
        }

        if (strcmp(cur_lex.lex.var_name, print) == 0) {
            printf("%d\n", calc_result(build_syntax_tree_(str)));
            str++;
            continue;
        }

        if (*str != '=') {
            printf ("Wrong format: expected '=' after variable\n");
            exit (0);
        }

        unsigned hash = hash_func(cur_lex.lex.var_name);
        var_arr[hash].left = create (cur_lex);
        var_arr[hash].lex.kind = OP;
        var_arr[hash].lex.lex.op = APP;
        var_arr[hash].right = build_syntax_tree_(str);
    }
}