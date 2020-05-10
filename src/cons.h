#ifndef _CONS_H
#define _CONS_H

typedef struct cons_t cons_t;
typedef struct value_t value_t;

#include "lexer.h"
#include "env.h"
/*

  cons and value

 */

typedef enum {
    V_LONG,
    V_IDENTIFIER,
    V_STRING,
    V_BOOL,
    V_LIST,
    V_PROCEDURE,
    V_BUILTIN,
    V_EXPRESSION
} value_type;

typedef value_t *(*builtin_t)(cons_t *, env_t *);

struct cons_t {
    value_t *car;
    cons_t *cdr;
};

typedef struct {
    cons_t *body;
    value_t *val;
} expression_t;

typedef struct {
    cons_t *formals;
    expression_t *expr;
} procedure_t;

struct value_t {
    value_type type;
    char free;
    union {
        long long lng;
        char *str;
        char b;
        builtin_t bif;
        cons_t *cons;
        procedure_t *proc;
        expression_t *expr;
    };
};

char *type_to_string(value_type t);

cons_t *cons_init(void);
void cons_output(cons_t *cons, env_t *env);
void cons_free(cons_t *cons);
cons_t *cons_copy(cons_t *cons);

expression_t *expression_init(void);
void expression_free(expression_t *expr);

long long str_to_long(char *str);
value_t *value_init(void);
value_t *value_copy(value_t *val);
value_t *empty_list(void);
value_t *token_to_value(token_t *t);
value_t *value_get(value_t *val, env_t *env);
void value_output(value_t *val, env_t *env);
void value_free(value_t *val);

procedure_t *procedure_init(void);
procedure_t *procedure_make(value_t *formals, value_t *body);
void procedure_free(procedure_t *proc);

#endif
