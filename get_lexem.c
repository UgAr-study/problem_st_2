#include "problem_st_2.h"

int is_end (char c) { // return 1 if we get end of file, else 0
    if (c == ';' || c == '\0') {
        end_of_expr = 1;
        return 1;
    }
    if (c == '\n') {
        printf ("ERROR: expected ';'\n");
        exit(0);
    }
    return 0;
}

int is_add_sub (struct lexem_t l, char** str) {
    if (l.kind == OP) {
        if (l.lex.op == ADD || l.lex.op == SUB)
            return 1;
    }
    (*str)--;
    return 0;
}

int is_mul_div (struct lexem_t l, char** str) {
    if (l.kind == OP) {
        if (l.lex.op == MUL || l.lex.op == DIV)
            return 1;
    }
    (*str)--;
    return 0;
}

int is_left_brace (struct lexem_t l) {
    if (l.kind == BRACE) {
        if (l.lex.b == LBRAC)
            return 1;
        exit(0);
    }
    return 0;
}

int is_right_brace (struct lexem_t l) {
    if (l.kind == BRACE){
        if (l.lex.b == RBRAC)
            return 1;
    }
    return 0;
}

struct lexem_t get_cur_lexem (char** str) {
    struct lexem_t lexem;
    int number = 0, flag = 0;

    while (isspace(**str) != 0)
        ++(*str);


    if (is_end(**str) == 1) {
        lexem.kind = END;
        lexem.lex.num = 0;
        return lexem;
    }

    if (isalpha(**str) != 0) {
        int i = 0;
        char *name = (char*) calloc(11, sizeof(char));

        for (i = 0; isalpha(**str) != 0; ++i)
            name[i] = *(*str)++;
        name[i] = '\0';

        if (strcmp(name, "print") == 0) {
            lexem.kind = FUNC;
            lexem.lex.op = PRINT;
        } else {
            lexem.kind = VAR_NAME;
            lexem.lex.var_name = name;
        }

        while (isspace(**str) != 0)
            ++(*str);

        --(*str);
        return lexem;
    }

    while (isdigit(**str) != 0) {
        number = 10 * number + (int)(**str - '0');
        flag = 1;
        ++(*str);
    }

    if (flag == 1) {
        lexem.kind = NUM;
        lexem.lex.num = number;
        (*str)--;
        return lexem;
    }

    switch (**str) {
        case '+':
            lexem.kind = OP;
            lexem.lex.op = ADD;
            break;
        case '-':
            lexem.kind = OP;
            lexem.lex.op = SUB;
            break;
        case '*':
            lexem.kind = OP;
            lexem.lex.op = MUL;
            break;
        case '/':
            lexem.kind = OP;
            lexem.lex.op = DIV;
            break;
        case '(':
            lexem.kind = BRACE;
            lexem.lex.b = LBRAC;
            break;
        case ')':
            lexem.kind = BRACE;
            lexem.lex.b = RBRAC;
            break;
        case '?':
            lexem.kind = FUNC;
            lexem.lex.op = SCAN;
            break;
        default:
            printf("%d(%c) is unknown symbol\n", **str, **str);
            exit(1);
    }
    return lexem;
}

void print_lexem (struct lexem_t l) {
    switch (l.kind) {
        case OP:
            switch (l.lex.op) {
                case ADD:
                    printf ("OP:_ADD ");
                    break;
                case SUB:
                    printf ("OP:_SUB ");
                    break;
                case MUL:
                    printf ("OP:_MUL ");
                    break;
                case DIV:
                    printf ("OP:_DIV ");
                    break;
                case APP:
                    printf ("OP:_APP ");
                    break;
                default:
                    exit(0);
            }
            break;
        case BRACE:
            switch (l.lex.b) {
                case LBRAC:
                    printf ("BRACE:_LBRACE ");
                    break;
                case RBRAC:
                    printf ("BRACE:_RBRACE ");
                    break;
                default:
                    exit(0);
            }
            break;
        case NUM:
            printf ("NUM:_%d ", l.lex.num);
            break;
        case END:
            printf("END_OF_LINE\n");
            break;
        case VAR_NAME:
            printf("VAR:_%s ", l.lex.var_name);
            break;
        case FUNC:
            switch (l.lex.op) {
                case SCAN:
                    printf ("FUNC:_SCAN ");
                    break;
                case PRINT:
                    printf ("FUNC:_PRINT ");
                    break;
                default:
                    exit(0);
            }
            break;
        default:
            exit(0);
    }
}








