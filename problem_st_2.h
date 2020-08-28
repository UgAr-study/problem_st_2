#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>


int end_of_expr;

//char print[] = "print";

enum { VAR_NUM = 100 };

// lexem is: operation | brace | number | end of file |
//  name of variable | function
enum lexem_kind_t { OP, BRACE, NUM, END, VAR_NAME, FUNC };

// operation is: + | - | * | / | scanf() | printf()
enum operation_t { ADD, SUB, MUL, DIV, APP, SCAN, PRINT };

// braces are: ( | )
enum braces_t { LBRAC, RBRAC };



// lexem is one of lexem kind entities
// if (l.kind == BRACE)
//   assert(l.lex.b == LBRAC || l.lex.b == RBRAC)


struct lexem_t {
    enum lexem_kind_t kind;
    union {
        char* var_name;
        enum operation_t op;
        enum braces_t b;
        int num;
    } lex;
};


struct node_t {
    struct lexem_t lex;
    struct node_t *left, *right;
};

struct hash_node {
    char flag;
    int val;
};

//variable value table
struct hash_node hash_table[VAR_NUM];

//array of variables and their expressions
struct node_t var_arr[VAR_NUM];


//variables
unsigned hash_func (const char* str);
struct node_t *create (struct lexem_t lex);


//lexems
struct lexem_t get_cur_lexem (char** str);
int is_end (char c);
int is_add_sub (struct lexem_t l, char** str);
int is_mul_div (struct lexem_t l, char** str);
int is_left_brace (struct lexem_t l);
int is_right_brace (struct lexem_t l);
struct node_t* build_syntax_tree_(char** str);
void print_lexem (struct lexem_t l);

//code analysis
char* read_file (char* f_name);
void parsing(char** str);

//calculating the result
int calc_result(struct node_t *top);

//print tree
void print_tree (struct node_t* top);
//print node
void print_node (struct lexem_t lex);
//free tree
void free_syntax_tree(struct node_t * top);
//free var_arr
void free_var_arr (struct node_t* arr_of_var);