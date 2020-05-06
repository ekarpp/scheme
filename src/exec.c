#include <stdlib.h>
#include <stdio.h>

#include "exec.h"
#include "env.h"
#include "cons.h"

int exec_expr(expression_t *expr, env_t *env)
{
    if (expr->val != NULL)
        value_free(expr->val);

    cons_t *body = expr->body;
    if (body->car == NULL && body->cdr == NULL)
    {
        expr->val = empty_list();
        return 1;
    }

    char *op = NULL;
    value_t *f = NULL;

    switch (body->car->type)
    {
    case V_STRING: case V_IDENTIFIER:
        op = body->car->str;
        f = env_get(env, op);
        break;
    case V_PROCEDURE:
        f = body->car;
        break;
    case V_EXPRESSION:
        exec_expr(body->car->expr, env);
        f = body->car->expr->val;
        // env_get here ??
        break;
    }

    if (f)
    {
        switch (f->type)
        {
        case V_BUILTIN:
            expr->val = f->bif(body->cdr, env);
            break;
        case V_PROCEDURE:
            expr->val = exec_procedure(f->proc, body->cdr, env);
            value_free(f);// when does this get free'd ??
            break;
        }
    }

    return 1;
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
        value_t *tmp = arg->car;
        if (tmp->type == V_EXPRESSION)
        {
            exec_expr(tmp->expr, env);
            tmp = tmp->expr->val;
        }
        // what is happening here
        value_t *v = value_get(formal, env);
        if (v == NULL || v == tmp)
            hashmap_put(env->hm, formal->str, tmp);

        arg = arg->cdr;
        formals = formals->cdr;
    }
    exec_expr(proc->expr, env);
    value_t *ret = proc->expr->val;
    env = env_pop(env);
    // env_pop free'd these
    args->car = NULL;
    args->cdr = NULL;
    return ret;
}

value_t *exec_eval(value_t *arg, env_t *env)
{
    if (arg->type == V_EXPRESSION)
    {
        exec_expr(arg->expr, env);
        arg = arg->expr->val;
    }
    return value_get(arg, env);
}
