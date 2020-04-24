#include <stdlib.h>
#include <stdio.h>
#include "exec.h"
#include "env.h"
#include "cons.h"

value_t *exec_expr(env_t *env, cons_t *expr)
{
    if (expr->cdr == NULL)
        return empty_list();

    value_t *ret = NULL;
    char *op;

    switch (expr->car->type)
    {
    case V_STRING: case V_IDENTIFIER:
        op = expr->car->str;
        expr->car->str = NULL;
        break;
    }

    value_t *f = env_get(env, op);
    if (f)
        ret = f->bif(expr->cdr, env);
    free(op);
    return ret;
}
