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
    V_BUILTIN
} value_type;
// maybe need V_EXPRESSION when we have lists implemented ??
typedef value_t *(*builtin_t)(cons_t *, env_t *);

struct cons_t {
    value_t *car;
    cons_t *cdr;
};

typedef struct {
    cons_t *formals;
    cons_t *body;
} procedure_t;

struct value_t {
    value_type type;
    union {
        long long lng;
        char *str;
        int b;
        builtin_t bif;
        cons_t *cons;
        procedure_t *proc;
    };
};

cons_t *cons_init(void);
void cons_output(cons_t *cons, env_t *env);
void cons_free(cons_t *cons);

long long str_to_long(char *str);
value_t *value_init(void);
value_t *empty_list(void);
value_t *token_to_value(token_t *t);
value_t *value_get(cons_t *cons, env_t *env);
void value_output(value_t *val, env_t *env);
void value_free(value_t *val);

procedure_t *procedure_init(void);
procedure_t *procedure_make(cons_t *args);
void procedure_free(procedure_t *proc);

#endif
