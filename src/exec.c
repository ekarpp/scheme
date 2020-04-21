#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "env.h"
#include "hashmap.h"

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

void value_output(value_t *val)
{
    switch (val->type)
    {
    case V_STRING:
        printf("%s\n", val->str);
        break;
    case V_LONG:
        printf("%lld\n", val->lng);
        break;
    case V_BOOL:
        printf("%s\n", val->b ? "#t" : "#f");
        break;
    case V_PROCEDURE:    case V_BUILTIN:
        break;

    }
}

long long str_to_lng(char *str)
{
    int i = 0;
    long l = 0L;
    while (str[i])
    {
        l *= 10;
        l += str[i] - '0';
        i++;
    }
    return l;
}

value_t *token_to_value(token_t *t)
{
    value_t *val = malloc(sizeof(value_t));
    switch (t->type)
    {
    case T_IDENTIFIER: case T_STRING:
        val->type = V_STRING;
        val->str = t->lexeme;
        break;
    case T_LONG:
        val->type = V_LONG;
        val->lng = str_to_lng(t->lexeme);
        break;
    }
}


cons_t *cons_init(void)
{
    cons_t *cons = malloc(sizeof(cons_t));
    if (cons == NULL)
        return NULL;
    cons->cdr = NULL;
    cons->car = NULL;
    return cons;
}

void cons_free(cons_t *cons)
{
    while (cons)
    {
        cons_t *tmp = cons->cdr;
        value_free(cons->car);
        free(cons);
        cons = tmp;
    }
}

void value_free(value_t *val)
{
    if (val->type == V_STRING && val->str)
        free(val->str);
    free(val);
}

void expr_free(expr_t *expr)
{
    cons_free(expr->args);
    free(expr);
}
