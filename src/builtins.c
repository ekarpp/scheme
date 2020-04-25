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
        value_t *arg = value_get(args, env);
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
        value_t *arg = value_get(args, env);
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

    value_t *arg = value_get(args, env);
    if (arg->type != V_LONG)
        return NULL; // error

    ret->lng = arg->lng;
    if (args->cdr == NULL)
        ret->lng = -ret->lng;

    args = args->cdr;
    while (args)
    {
        arg = value_get(args, env);
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

    value_t *arg = value_get(args, env);

    ret->lng = arg->lng;
    if (args->cdr == NULL)
        ret->lng = 0L;

    args = args->cdr;
    while (args)
    {
        arg = value_get(args, env);
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

    value_t *arg = value_get(args, env);

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

// todo: fix identifier
value_t *builtins_cmp(cons_t *args, char op)
{
    value_t *ret = value_init();

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

value_t *builtins_equal(cons_t *args, env_t *env)
{
    return builtins_cmp(args, '=');
}

value_t *builtins_increasing(cons_t *args, env_t *env)
{
    return builtins_cmp(args, '<');
}

value_t *builtins_non_increasing(cons_t *args, env_t *env)
{
    return builtins_cmp(args, 'i');
}

value_t *builtins_decreasing(cons_t *args, env_t *env)
{
    return builtins_cmp(args, '>');
}

value_t *builtins_non_decreasing(cons_t *args, env_t *env)
{
    return builtins_cmp(args, 'd');
}

value_t *builtins_lambda(cons_t *args, env_t *env)
{
    return NULL;
}

value_t *builtins_define(cons_t *args, env_t *env)
{
    value_t *ret = value_init();

    if (args->car->type != V_IDENTIFIER)
        return NULL; // error

    char *key = args->car->str;// this fails maybe??

    hashmap_put(env->hm, args->car->str, args->cdr->car);
    args->car->str = NULL; // memory leak here
    // maybe malloc all hashmap keys ??
    // or set flag that free this str ??
    memcpy(ret, args->cdr->car, sizeof(value_t));

    args->cdr->car = NULL;
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
