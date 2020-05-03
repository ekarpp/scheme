#include <stdlib.h>
#include <string.h>

#include "exec.h"
#include "env.h"
#include "hashmap.h"
#include "builtins.h"


value_t *builtins_add(cons_t *args, env_t *env)
{
    value_t *ret = value_init();

    ret->type = V_LONG;
    ret->lng = 0L;

    while (args)
    {
        exec_eval(args, env);
        value_t *arg = value_get(args->car, env);
        if (arg->type != V_LONG)
            break;// error

        ret->lng += arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_multiply(cons_t *args, env_t *env)
{
    value_t *ret = value_init();

    ret->type = V_LONG;
    ret->lng = 1L;

    while (args)
    {
        exec_eval(args, env);
        value_t *arg = value_get(args->car, env);
        if (arg->type != V_LONG)
            break;// error

        ret->lng *= arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_subtract(cons_t *args, env_t *env)
{
    value_t *ret = value_init();

    ret->type = V_LONG;

    if (args == NULL)
        return NULL;// error
    exec_eval(args, env);
    value_t *arg = value_get(args->car, env);
    if (arg->type != V_LONG)
        return NULL; // error

    ret->lng = arg->lng;
    if (args->cdr == NULL)
        ret->lng = -ret->lng;

    args = args->cdr;
    while (args)
    {
        exec_eval(args, env);
        arg = value_get(args->car, env);
        if (arg->type != V_LONG)
            break;// error
        ret->lng -= arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_divide(cons_t *args, env_t *env)
{
    value_t *ret = value_init();

    ret->type = V_LONG;

    if (args == NULL)
        return NULL;// error
    exec_eval(args, env);
    value_t *arg = value_get(args->car, env);

    ret->lng = arg->lng;
    if (args->cdr == NULL)
        ret->lng = 0L;

    args = args->cdr;
    while (args)
    {
        exec_eval(args, env);
        arg = value_get(args->car, env);
        // type check
        ret->lng /= arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_abs(cons_t *args, env_t *env)
{
    value_t *ret = value_init();
    ret->type = V_LONG;

    if (args == NULL)
        return NULL;// error

    exec_eval(args, env);
    value_t *arg = value_get(args->car, env);

    if (arg->type != V_LONG)
        return NULL;// error

    ret->lng = arg->lng < 0 ? -arg->lng : arg->lng;
    return ret;
}


// todo: fix identifier
value_t *builtins_eqv(cons_t *args, env_t *env)
{
    value_t *ret = value_init();

    ret->type = V_BOOL;
    ret->b = 0;

    if (args->car->type != args->cdr->car->type)
        return ret;

    switch (args->car->type)
    {
    case V_LONG:
        ret->b = args->car->lng == args->cdr->car->lng;
        return ret;
    case V_BOOL:
        ret->b = args->car->b == args->cdr->car->b;
        return ret;
    }
}

value_t *builtins_cmp(cons_t *args, char op, env_t *env)
{
    value_t *ret = value_init();

    ret->type = V_BOOL;
    ret->b = 1;

    if (args == NULL || args->cdr == NULL)
        return NULL;//invalid amount of arg error

    exec_eval(args, env);
    value_t *prev = value_get(args->car, env);
    if (prev->type != V_LONG)
        return NULL; //invalid type error

    args = args->cdr;
    while (args && ret->b)
    {
        exec_eval(args, env);
        value_t *curr = value_get(args->car, env);
        if (curr->type != V_LONG)
            break;// error
        switch (op)
        {
        case '<':
            ret->b = prev->lng < curr->lng;
            break;
        case '>':
            ret->b = prev->lng > curr->lng;
            break;
        case '=':
            ret->b = prev->lng == curr->lng;
            break;
        case 'd':
            ret->b = prev->lng >= curr->lng;
            break;
        case 'i':
            ret->b = prev->lng <= curr->lng;
            break;
        }

        prev = curr;
        args = args->cdr;
    }

    return ret;

}

value_t *builtins_equal(cons_t *args, env_t *env)
{
    return builtins_cmp(args, '=', env);
}

value_t *builtins_increasing(cons_t *args, env_t *env)
{
    return builtins_cmp(args, '<', env);
}

value_t *builtins_non_increasing(cons_t *args, env_t *env)
{
    return builtins_cmp(args, 'i', env);
}

value_t *builtins_decreasing(cons_t *args, env_t *env)
{
    return builtins_cmp(args, '>', env);
}

value_t *builtins_non_decreasing(cons_t *args, env_t *env)
{
    return builtins_cmp(args, 'd', env);
}

value_t *builtins_lambda(cons_t *args, env_t *env)
{
    value_t *ret = value_init();
    ret->type = V_PROCEDURE;


    if (args == NULL || args->car == NULL
        || args->cdr == NULL || args->cdr->car == NULL)
        return NULL; // error

    if (args->car->type != V_EXPRESSION || args->cdr->car->type != V_EXPRESSION)
        return NULL; // error for now

    // two lists, first is formals then body
    // formal can be other that list
    // body always list ??
    ret->proc = procedure_make(args);

    return ret;
}

value_t *builtins_define(cons_t *args, env_t *env)
{
    value_t *ret = NULL;

    if (args->car->type != V_IDENTIFIER)
        return NULL; // error

    // maybe make variables here to make easier read
    char *key = args->car->str;
    value_t *val = args->cdr->car;
    //exec_eval(args->cdr, env); // this should replace next lines ??
    // need to separate list and expression
    if (val->type == V_EXPRESSION)
        val = exec_expr(val->cons, env);
    else
        // hashmap will free this
        args->cdr->car = NULL;


    hashmap_put(env->hm, key, val);

    ret = val;
    // this is bad
    // procedure, string, list dont get copied
    // maybe set flag that it hsould not be freed?
    // copying quite complicated

    return ret;
}

void builtins_add_f(hashmap_t *hm, char *key, builtin_t f)
{
    value_t *val = value_init();
    val->type = V_BUILTIN;
    val->bif = f;
    hashmap_put(hm, key, val);
}

void builtins_add_all_fs(env_t *env)
{
    builtins_add_f(env->hm, "+", builtins_add);
    builtins_add_f(env->hm, "-", builtins_subtract);
    builtins_add_f(env->hm, "*", builtins_multiply);
    builtins_add_f(env->hm, "/", builtins_divide);
    builtins_add_f(env->hm, "abs", builtins_abs);
    builtins_add_f(env->hm, "eqv?", builtins_eqv);
    builtins_add_f(env->hm, "=", builtins_equal);
    builtins_add_f(env->hm, "<", builtins_increasing);
    builtins_add_f(env->hm, "<=", builtins_non_increasing);
    builtins_add_f(env->hm, ">", builtins_decreasing);
    builtins_add_f(env->hm, ">=", builtins_non_decreasing);
    builtins_add_f(env->hm, "lambda", builtins_lambda);
    builtins_add_f(env->hm, "define", builtins_define);
}
