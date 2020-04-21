#include <stdlib.h>

#include "exec.h"
#include "env.h"
#include "cons.h"

value_t *exec_expr(env_t *env, expr_t *expr)
{
    // make env_get
    // goes through all envs that are stored in linked list
    // first make sure it is how envs work
    // (latest can overwrite latter)
    value_t *ret = NULL;
    value_t *f = env_get(env, expr->op);
    if (f)
        ret = f->bif(expr->args);
    free(expr->op);
    expr_free(expr);
    return ret;
}

void expr_free(expr_t *expr)
{
    cons_free(expr->args);
    free(expr);
}
