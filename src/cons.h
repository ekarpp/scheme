#ifndef _CONS_H
#define _CONS_H


#include "lexer.h"
/*

  cons and value

 */

typedef struct cons_t cons_t;
typedef struct value_t value_t;

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

cons_t *cons_init(void);
void cons_free(cons_t *cons);

long long str_to_long(char *str);
value_t *token_to_value(token_t *t);
void value_output(value_t *val);
void value_free(value_t *val);

#endif
