#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "exec.h"
#include "env.h"
#include "hashmap.h"
#include "builtins.h"


#define ERR_N "\nWrong number of arguments for %s. Expected %s %d, got %d.\n"
#define ERR_T "\nUnexpected value for %s. Expected %s, got %s.\n"

#define _ERR_(...)                                       \
    {                                                    \
        printf(__VA_ARGS__);                             \
        return NULL;                                     \
    }

#define _TYPE_LONG(f)                                    \
    if (arg == NULL)                                     \
    {                                                    \
        value_free(ret);                                 \
        return NULL;                                     \
    }                                                    \
    if (arg->type != V_LONG)                             \
    {                                                    \
        value_free(ret);                                 \
        _ERR_(ERR_T, (f), "long",                        \
              type_to_string(arg->type));                \
    }                                                    \


#define _ATLEAST_(n, f)                                  \
    {                                                    \
        cons_t *TEMP = args;                             \
        int LEN = 0;                                     \
        while (TEMP && LEN < (n))                        \
        {LEN++; TEMP = TEMP->cdr;}                       \
        if (LEN < (n))                                   \
            _ERR_(ERR_N, (f), "atleast", (n), LEN);      \
    }


#define _EXACTLY_(n, f)                                  \
    {                                                    \
        cons_t *TMP = args;                              \
        int LEN = 0;                                     \
        while (TMP)                                      \
        {LEN++; TMP = TMP->cdr;}                         \
        if (LEN != (n))                                  \
            _ERR_(ERR_N, (f), "exactly", (n), LEN);      \
    }


value_t *builtins_add(cons_t *args, env_t *env)
{
    value_t *ret = value_init();

    ret->type = V_LONG;
    ret->lng = 0L;

    while (args)
    {
        value_t *arg = exec_eval(args->car, env);
        _TYPE_LONG("+");

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
        value_t *arg = exec_eval(args->car, env);
        _TYPE_LONG("*");

        ret->lng *= arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_subtract(cons_t *args, env_t *env)
{
    _ATLEAST_(1, "-");
    value_t *ret = value_init();

    ret->type = V_LONG;

    value_t *arg = exec_eval(args->car, env);

    _TYPE_LONG("-");

    ret->lng = arg->lng;
    if (args->cdr == NULL)
        ret->lng = -ret->lng;

    args = args->cdr;
    while (args)
    {
        arg = exec_eval(args->car, env);
        _TYPE_LONG("-");

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
    value_t *arg = exec_eval(args->car, env);

    ret->lng = arg->lng;
    if (args->cdr == NULL)
        ret->lng = 0L;

    args = args->cdr;
    while (args)
    {
        arg = exec_eval(args->car, env);
        // type check
        ret->lng /= arg->lng;
        args = args->cdr;
    }

    return ret;
}

value_t *builtins_abs(cons_t *args, env_t *env)
{
    _EXACTLY_(1, "abs");
    value_t *ret = value_init();
    ret->type = V_LONG;

    if (args == NULL)
        return NULL;// error

    value_t *arg = exec_eval(args->car, env);
    _TYPE_LONG("abs");

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
        return ret;

    char *name; // for error message
    switch (op)
    {
    case '<':
        name = "<";
        break;
    case '>':
        name = ">";
        break;
    case '=':
        name = "=";
        break;
    case 'd':
        name = ">=";
        break;
    case 'i':
        name = "<=";
        break;
    }

    value_t *prev = exec_eval(args->car, env);
    value_t *arg = prev;

    _TYPE_LONG(name);

    args = args->cdr;
    while (args && ret->b)
    {
        arg = exec_eval(args->car, env);
        _TYPE_LONG(name);

        switch (op)
        {
        case '<':
            ret->b = prev->lng < arg->lng;
            break;
        case '>':
            ret->b = prev->lng > arg->lng;
            break;
        case '=':
            ret->b = prev->lng == arg->lng;
            break;
        case 'd':
            ret->b = prev->lng >= arg->lng;
            break;
        case 'i':
            ret->b = prev->lng <= arg->lng;
            break;
        }

        prev = arg;
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
    if (args == NULL || args->car == NULL)
        return NULL;

    if (args->cdr == NULL || args->cdr->car == NULL)
        return NULL; // error
    value_t *formals = args->car;
    value_t *body = args->cdr->car;

    if (formals->type != V_EXPRESSION || body->type != V_EXPRESSION)
        return NULL; // error for now

    value_t *ret = value_init();
    ret->type = V_PROCEDURE;

    // two lists, first is formals then body
    // formal can be other that list
    // body always list ??
    ret->proc = procedure_make(formals, body);

    return ret;
}

value_t *builtins_define(cons_t *args, env_t *env)
{
    _EXACTLY_(2, "define");

    value_t *ret = NULL;
    value_t *key = args->car;
    if (key->type != V_IDENTIFIER)
        _ERR_(ERR_T, "define", "identifier", type_to_string(key->type));

    //exec_eval(args->cdr, env); // this should replace next lines ??
    value_t *val = exec_eval(args->cdr->car, env);
    if (val == NULL)
        return NULL;

    if (args->cdr->car->type != V_EXPRESSION)
        // hashmap will free this
        args->cdr->car = NULL;


    hashmap_put(env->hm, key->str, val);

    ret = val;
    // this is bad
    // procedure, string, list dont get copied
    // maybe set flag that it hsould not be freed?
    // copying quite complicated

    return ret;
}

value_t *builtins_print(cons_t *args, env_t *env)
{
    while (args)
    {
        value_t *val = exec_eval(args->car, env);
        value_output(val, env);
        args = args->cdr;
    }
    return empty_list();
}

value_t *builtins_begin(cons_t *args, env_t *env)
{
    if (args == NULL)
        return empty_list();
    value_t *ret;
    value_t *prev;
    while (args)
    {
        value_t *arg = args->car;
        if (arg->type != V_EXPRESSION)
            _ERR_(ERR_T, "begin", "expression", type_to_string(arg->type));

        ret = exec_eval(arg, env);
        prev = arg;
        args = args->cdr;
    }
    prev->expr->val = NULL;
    return ret;
}

value_t *builtins_if(cons_t *args, env_t *env)
{
    value_t *ret;
    value_t *bool = exec_eval(args->car, env);
    if (bool == NULL)
        return NULL;
    if (bool->type != V_BOOL)
        _ERR_(ERR_T, "if", "boolean", type_to_string(bool->type));

    cons_t *tru = args->cdr;
    cons_t *fals = tru->cdr;

    value_t *val = (bool->b) ? tru->car : fals->car;

    if (val->type == V_EXPRESSION)
    {
        ret = exec_eval(val, env);
        val->expr->val = NULL;
    }
    else
    {
        ret = value_get(val, env);
        ret = value_copy(ret);
    }

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
    builtins_add_f(env->hm, "print", builtins_print);
    builtins_add_f(env->hm, "begin", builtins_begin);
    builtins_add_f(env->hm, "if", builtins_if);
}
