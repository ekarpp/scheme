#include <stdlib.h>
#include <stdio.h>

#include "exec.h"
#include "env.h"
#include "cons.h"

value_t *exec_expr(cons_t *expr, env_t *env)
{
    if (expr->car == NULL && expr->cdr == NULL)
        return empty_list();

    value_t *ret = NULL;
    char *op = NULL;
    value_t *f = NULL;

    switch (expr->car->type)
    {
    case V_STRING: case V_IDENTIFIER:
        op = expr->car->str;
        f = env_get(env, op);
        break;
    case V_PROCEDURE:
        f = expr->car;
        break;
    case V_EXPRESSION:
        f = exec_expr(expr->car->cons, env);
        break;
    }

    if (f)
    {
        switch (f->type)
        {
        case V_BUILTIN:
            ret = f->bif(expr->cdr, env);
            break;
        case V_PROCEDURE:
            ret = exec_procedure(f->proc, expr->cdr, env);
            value_free(f);
            break;
        }
    }

    return ret;
}

value_t *exec_procedure(procedure_t *proc, cons_t *args, env_t *env)
{
    env = env_add(env);
    cons_t *formals = proc->formals;
    cons_t *arg = args;

    while (formals)
    {
        value_t *formal = formals->car;
        if (formal->type != V_IDENTIFIER || arg->car == NULL)
            return NULL; // error
        exec_eval(arg, env);
        hashmap_put(env->hm, formal->str, arg->car);
        arg = arg->cdr;
        formals = formals->cdr;
    }

    value_t *ret = exec_expr(proc->body, env);
    env = env_pop(env);
    // env_pop free'd these
    args->car = NULL;
    args->cdr = NULL;
    return ret;
}

void exec_eval(cons_t *arg, env_t *env)
{
    if (arg->car->type == V_EXPRESSION)
    {
        value_t *val = exec_expr(arg->car->cons, env);
        value_free(arg->car);
        arg->car = val;
    }
}
