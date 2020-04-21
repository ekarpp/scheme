#ifndef _EXEC_H
#define _EXEC_H

#include "env.h"
#include "cons.h"

typedef struct {
    char *op;
    cons_t *args;
} expr_t;

value_t *exec_expr(env_t *env, expr_t *expr);
void expr_free(expr_t *expr);


#endif
