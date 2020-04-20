#include <stdlib.h>

#include "exec.h"
#include "env.h"
#include "hashmap.h"
#include "builtins.h"

value_t *builtins_add(value_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_LONG;
    ret->lng = 0L;
    ret->next = NULL;

    while (args)
    {
        if (args->type != V_LONG)
            break;// error

        ret->lng += args->lng;
        args = args->next;
    }

    return ret;
}

value_t *builtins_multiply(value_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_LONG;
    ret->lng = 1L;
    ret->next = NULL;

    while (args)
    {
        if (args->type != V_LONG)
            break;// error

        ret->lng *= args->lng;
        args = args->next;
    }

    return ret;
}

value_t *builtins_subtract(value_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_LONG;
    ret->next = NULL;

    if (args == NULL || args->type != V_LONG)
        return NULL;// error

    ret->lng = args->lng;
    if (args->next == NULL)
        ret->lng = -ret->lng;

    args = args->next;
    while (args)
    {
        if (args->type != V_LONG)
            break;// error
        ret->lng -= args->lng;
        args = args->next;
    }

    return ret;
}

value_t *builtins_divide(value_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_LONG;
    ret->next = NULL;

    if (args == NULL || args->type != V_LONG)
        return NULL;// error

    ret->lng = args->lng;
    if (args->next == NULL)
        ret->lng = 0L;

    args = args->next;
    while (args)
    {
        ret->lng /= args->lng;
        args = args->next;
    }

    return ret;
}

value_t *builtins_abs(value_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;
    ret->next = NULL;
    ret->type = V_LONG;
    if (args == NULL || args->type != V_LONG)
        return NULL;// error
    ret->lng = args->lng < 0 ? -args->lng : args->lng;
    return ret;
}

value_t *builtins_eqv(value_t *args)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_BOOL;
    ret->next = NULL;
    ret->b = 0;

    if (args->type != args->next->type)
        return ret;

    switch (args->type)
    {
    case V_LONG:
        ret->b = args->lng == args->next->lng;
        return ret;
    case V_BOOL:
        ret->b = args->b == args->next->b;
        return ret;
    }
}

value_t *builtins_cmp(value_t *args, char op)
{
    value_t *ret = malloc(sizeof(value_t));
    if (ret == NULL)
        return NULL;

    ret->type = V_BOOL;
    ret->b = 1;

    if (args == NULL || args->type != V_LONG
        || args->next == NULL)
        return NULL;// error

    value_t *prev = args;
    args = args->next;
    while (args && ret->b)
    {
        if (args->type != V_LONG)
            break;// error
        switch (op)
        {
        case '<':
            ret->b = prev->lng < args->lng;
            break;
        case '>':
            ret->b = prev->lng > args->lng;
            break;
        case '=':
            ret->b = prev->lng == args->lng;
            break;
        case 'd':
            ret->b = prev->lng >= args->lng;
            break;
        case 'i':
            ret->b = prev->lng <= args->lng;
            break;
        }

        prev = args;
        args = args->next;
    }

    return ret;

}

value_t *builtins_equal(value_t *args)
{
    return builtins_cmp(args, '=');
}

value_t *builtins_increasing(value_t *args)
{
    return builtins_cmp(args, '<');
}

value_t *builtins_non_increasing(value_t *args)
{
    return builtins_cmp(args, 'i');
}

value_t *builtins_decreasing(value_t *args)
{
    return builtins_cmp(args, '>');
}

value_t *builtins_non_decreasing(value_t *args)
{
    return builtins_cmp(args, 'd');
}

value_t *builtins_lambda(value_t *args)
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
