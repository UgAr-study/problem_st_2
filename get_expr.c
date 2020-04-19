#include "problem_st_2.h"

// Grammar:
// expr ::= mult {+, -} expr | mult
// mult ::= term {*, /} mult | term
// term ::= ( expr ) | number

unsigned hash_func (const char* str) {
    unsigned hash = 113;
    for (int i = 0; i < strlen (str); ++i)
        hash += (((hash << 5) + hash) + str[i]) % 101;
    return hash % 101 - 1;
}

struct node_t* expr (char** str);

struct node_t *create (struct lexem_t lex) {
    struct node_t* res;
    res = (struct node_t*) calloc (1, sizeof (struct node_t));
    if (res == NULL) {
        printf ("ERROR: calloc() couldn't provide memory\n");
        exit(0);
    }
    res->lex = lex;
    return res;
}

struct node_t* term (char** str) {
#ifdef VISUALIZE
    printf("in term\n");
#endif
    struct node_t* term = NULL;
    struct lexem_t cur_lex;

    cur_lex = get_cur_lexem(str);
    if (cur_lex.kind == END)
        return term;

    if (cur_lex.kind == NUM || cur_lex.kind == FUNC) {
        term = create(cur_lex);
        return term;
    }

    if (cur_lex.kind == VAR_NAME) {
        unsigned hash = hash_func(cur_lex.lex.var_name);
        if (var_arr[hash].right == NULL) {
            printf ("ERROR: unknown variable '%s'\n", cur_lex.lex.var_name);
            exit(0);
        }
        return &var_arr[hash];
    }

    if (is_left_brace(cur_lex) == 1) {
        (*str)++;
        term = expr (str);
        (*str)++;
        cur_lex = get_cur_lexem(str);
        if (is_right_brace(cur_lex) == 0) {
            printf("ERROR: expected ')'");
            exit(0);
        }
        return term;
    } else {
        printf ("Wrong format: expected number or brace\n");
        exit (0);
    }
}

struct node_t* mult (char** str) {
#ifdef VISUALIZE
    printf ("in mult\n");
#endif

    struct node_t *multy, *m_left;
    struct lexem_t cur_lex;

    m_left = term (str);

#ifdef VISUALIZE
    printf ("M: i = %d\n", *i);
#endif

    if (m_left != NULL) {
        ++(*str);
        cur_lex = get_cur_lexem(str);

        while (is_mul_div(cur_lex, str) == 1) {
            multy = create(cur_lex);
            multy->left = m_left;
            (*str)++;
            multy->right = term (str);

            if (multy->right == NULL) {
                printf ("Wrong format: expected a number or expression\n");
                exit(0);
            }

            m_left = multy;
            (*str)++;
            cur_lex = get_cur_lexem(str);
        }
    }

    return m_left;
}

struct node_t* expr (char** str) {
#ifdef VISUALIZE
    printf ("in expr\n");
#endif

    struct node_t *expression, *e_left;
    struct lexem_t cur_lex;

    e_left = mult(str);

#ifdef VISUALIZE
    printf ("E: i = %d\n", *i);
#endif

    if (e_left != NULL) {
        (*str)++;
        cur_lex = get_cur_lexem(str);

        if (cur_lex.kind == END)
            return e_left;

        while (is_add_sub(cur_lex, str) == 1) {
            expression = create(cur_lex);
            expression->left = e_left;
            (*str)++;
            expression->right = mult (str);

            if (expression->right == NULL) {
                printf ("Wrong format: expected a number or expression\n");
                exit(0);
            }

            e_left = expression;
            (*str)++;
            cur_lex = get_cur_lexem(str);
        }
    }

    return e_left;
}

struct node_t* build_syntax_tree_(char* str) {
    struct node_t* tree;
    tree = expr (&str);

    if (end_of_expr != 1) {
        printf ("Not reach end of expression! Check braces\n");
        exit(0);
    }

    if (tree == NULL) {
        printf ("Nothing to be calculated\n");
        exit (0);
    }

    return tree;
}

int calc_result(struct node_t *top) {
    int result, l, r;
    assert (top != NULL);
    //left
    //right
    //visit
    if (top->lex.kind == NUM)
        return top->lex.lex.num;

    if (top->left->lex.kind == VAR_NAME) {
        unsigned hash = hash_func(top->left->lex.lex.var_name);

        if (hash_table[hash].flag == 1) {
            return hash_table[hash].val;
        }

        if (top->right->lex.kind == FUNC) {
            switch (top->right->lex.lex.op) {
                case SCAN:
                    printf ("\nPlease, input this variable: %s = ", top->left->lex.lex.var_name);
                    int res = scanf ("%d", &result);
                    if (res != 1) {
                        printf ("ERROR: wrong type\n");
                        exit (0);
                    }
                    printf ("\n");
                    hash_table[hash].flag = 1;
                    hash_table[hash].val = result;
                    return result;
                default:
                    printf ("ERROR: unknown function\n");
                    exit (0);
            }
        }

        result = calc_result(top->right);
        hash_table[hash].flag = 1;
        hash_table[hash].val = result;
        return result;
    }

    l = calc_result (top->left);
    r = calc_result (top->right);
    switch (top->lex.lex.op) {
        case ADD:
            result = l + r;
            printf ("%d + %d   ", l, r);
            break;
        case SUB:
            result = l - r;
            printf ("%d - %d   ", l, r);
            break;
        case MUL:
            result = l * r;
            printf ("%d * %d   ", l, r);
            break;
        case DIV:
            result = l / r;
            printf ("%d / %d   ", l, r);
            break;
        default:
            printf ("Operation is not found, sorry (\n");
            exit (0);
    }
    return result;
}

void print_node (struct lexem_t lex) {
    switch (lex.kind) {
        case OP:
            switch (lex.lex.op) {
                case ADD:
                    printf ("ADD ");
                    break;
                case SUB:
                    printf ("SUB ");
                    break;
                case MUL:
                    printf ("MUL ");
                    break;
                case DIV:
                    printf ("DIV ");
                    break;
                default:
                    exit(1);
            }
            break;
        case NUM:
            printf ("%d ", lex.lex.num);
            break;
        default:
            exit(1);
    }
}

void print_tree (struct node_t* top) {
    if (top == NULL){
        printf ("Error: top is NULL\n");
        return;
    }
    if (top->left == NULL && top->right == NULL)
        return;

    print_node (top->lex);

    if (top->left == NULL) {
        printf ("\n");
        return;
    }
    print_node (top->left->lex);

    if (top->right == NULL)
        return;
    print_node (top->right->lex);

    printf ("\n");

    print_tree (top->left);
    print_tree (top->right);
}

void free_syntax_tree(struct node_t * top) {

    if (top->left == NULL && top->right == NULL) {
        free (top);
        return;
    }

    if (top->left != NULL)
        free_syntax_tree (top->left);
    if (top->right != NULL)
        free_syntax_tree (top->right);

    free (top);
}

