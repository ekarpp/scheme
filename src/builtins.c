#include <stdlib.h>

#include "exec.h"
#include "env.h"
#include "hashmap.h"
#include "builtins.h"

value_t *builtins_add(cons_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_LONG;
    ret->lng = 0L;

    while (args)
    {
        value_t *arg = args->car;
        if (arg->type != V_LONG)
            break;// error

        ret->lng += arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_multiply(cons_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_LONG;
    ret->lng = 1L;

    while (args)
    {
        value_t *arg = args->car;
        if (arg->type != V_LONG)
            break;// error

        ret->lng *= arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_subtract(cons_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_LONG;

    if (args == NULL || args->car->type != V_LONG)
        return NULL;// error

    ret->lng = args->car->lng;
    if (args->cdr == NULL)
        ret->lng = -ret->lng;

    args = args->cdr;
    while (args)
    {
        value_t *arg = args->car;
        if (arg->type != V_LONG)
            break;// error
        ret->lng -= arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_divide(cons_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_LONG;

    if (args == NULL || args->car->type != V_LONG)
        return NULL;// error

    ret->lng = args->car->lng;
    if (args->cdr == NULL)
        ret->lng = 0L;

    args = args->cdr;
    while (args)
    {
        ret->lng /= args->car->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_abs(cons_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;
    ret->type = V_LONG;

    if (args == NULL || args->car->type != V_LONG)
        return NULL;// error
    ret->lng = args->car->lng < 0 ? -args->car->lng : args->car->lng;
    return ret;
}

value_t *builtins_eqv(cons_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

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

value_t *builtins_cmp(cons_t *args, char op)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_BOOL;
    ret->b = 1;

    if (args == NULL || args->car->type != V_LONG
        || args->cdr == NULL)
        return NULL;// error

    value_t *prev = args->car;
    args = args->cdr;
    while (args && ret->b)
    {
        value_t *curr = args->car;
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

value_t *builtins_equal(cons_t *args)
{
    return builtins_cmp(args, '=');
}

value_t *builtins_increasing(cons_t *args)
{
    return builtins_cmp(args, '<');
}

value_t *builtins_non_increasing(cons_t *args)
{
    return builtins_cmp(args, 'i');
}

value_t *builtins_decreasing(cons_t *args)
{
    return builtins_cmp(args, '>');
}

value_t *builtins_non_decreasing(cons_t *args)
{
    return builtins_cmp(args, 'd');
}

value_t *builtins_lambda(cons_t *args)
{
    return NULL;
}

void builtins_add_f(hashmap_t *hm, char *key, builtin_t f)
{
    value_t *val = malloc(sizeof(value_t));
    if (val == NULL)
        return;
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
}
