#include "problem_st_2.h"

char* read_file (char* f_name){
    FILE* f = fopen(f_name, "rb");

    if (f == NULL) {
        printf ("ERROR: file isn't opened\n");
        exit (0);
    }

    fseek(f, 0, SEEK_END);
    unsigned long int f_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* text = (char*) calloc ((++f_size), sizeof(char));

    if (text == NULL) {
        printf ("ERROR: lack of memory\n");
        exit (0);
    }

    unsigned long int result = fread (text, sizeof(char), f_size, f);

    if (result + 1 != f_size) {
        printf ("Reading file error\n");
        exit (0);
    }

    fclose (f);
    return text;
}

void free_var_arr (struct node_t* arr_of_var) {

    for (int i = 0; i < VAR_NUM; ++i) {
        free_syntax_tree (arr_of_var[i].left);
        free_syntax_tree (arr_of_var[i].right);
    }
    free (arr_of_var);
}

void parsing(char** str) {
    struct lexem_t cur_lex;

    while (**str != '\0') {
        cur_lex = get_cur_lexem(str);

#ifdef VISUALIZE
        print_lexem(cur_lex);
        printf ("\n");
#endif

        if (cur_lex.kind == END)
            break;

        if (cur_lex.kind != VAR_NAME && cur_lex.kind != FUNC) {
            printf ("\nWrong format: expected variable or function in the beginning of the string\n");
            exit (0);
        }

        if (cur_lex.kind == FUNC && cur_lex.lex.op == PRINT) {
            struct node_t* result = build_syntax_tree_(str);
            printf("result = %d\n", calc_result(result));
            free_syntax_tree(result);
            (*str)++;
            continue;
        }

        if (*(++(*str)) != '=') {
            printf ("Wrong format: expected '=' instead '%c' after variable\n", **str);
            exit (0);
        }

        (*str)++;
        unsigned hash = hash_func(cur_lex.lex.var_name);
        var_arr[hash].left = create (cur_lex);
        var_arr[hash].lex.kind = FUNC;
        var_arr[hash].lex.lex.op = APP;
        var_arr[hash].right = build_syntax_tree_(str);

#ifdef VISUALIZE
        print_tree(&var_arr[hash]);
        puts (*str);
#endif

        (*str)++;
    }

    //free_var_arr (var_arr); ????
}