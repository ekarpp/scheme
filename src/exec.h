#ifndef _EXEC_H
#define _EXEC_H

typedef struct value_t value_t;

#include "hashmap.h"
#include "env.h"
#include "lexer.h"

typedef enum {
    V_LONG,
    V_STRING,
    V_BOOL,
    V_PROCEDURE,
    V_BUILTIN
} value_type;

typedef value_t *(*builtin_t)(value_t *);

struct value_t {
    union {
        long long lng;
        char *str;
        int b;
        builtin_t bif;
    };
    value_type type;
    value_t *next;
};

typedef struct {
    char *op;
    int nargs;
    value_t *args;
    value_t *last;
} expr_t;

value_t *exec_expr(env_t *env, expr_t *expr);
void expr_free(expr_t *expr);

long long str_to_lng(char *str);
value_t *token_to_value(token_t *t);

void value_output(value_t *val);
void value_free(value_t *val);


#endif
