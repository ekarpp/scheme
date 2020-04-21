#ifndef _EXEC_H
#define _EXEC_H

typedef struct cons_t cons_t;
typedef struct value_t value_t;

#include "hashmap.h"
#include "env.h"
#include "lexer.h"

typedef enum {
    V_LONG,
    V_STRING,
    V_BOOL,
    V_LIST,
    V_PROCEDURE,
    V_BUILTIN
} value_type;

typedef value_t *(*builtin_t)(cons_t *);

struct cons_t {
    value_t *car;
    cons_t *cdr;
};

struct value_t {
    value_type type;
    union {
        long long lng;
        char *str;
        int b;
        builtin_t bif;
        cons_t *cons;
    };
};



typedef struct {
    char *op;
    cons_t *args;
} expr_t;

value_t *exec_expr(env_t *env, expr_t *expr);
void expr_free(expr_t *expr);

long long str_to_lng(char *str);
value_t *token_to_value(token_t *t);

cons_t *cons_init(void);
void cons_free(cons_t *cons);

void value_output(value_t *val);
void value_free(value_t *val);


#endif
