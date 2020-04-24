#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "env.h"
#include "cons.h"


/* cons */

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

/* helper for token_to_value */
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

/* value */

value_t *token_to_value(token_t *t)
{
    value_t *val = malloc(sizeof(value_t));
    value_t *tmp;
    switch (t->type)
    {
    case T_IDENTIFIER:
        val->type = V_IDENTIFIER;
        val->str = t->lexeme;
        t->lexeme = NULL;// leak
        break;
    case T_STRING:
        val->type = V_STRING;
        val->str = t->lexeme;
        t->lexeme = NULL;
        break;
    case T_LONG:
        val->type = V_LONG;
        val->lng = str_to_lng(t->lexeme);
        break;
    }
    return val;
}

value_t *empty_list(void)
{
    value_t *l = malloc(sizeof(value_t));
    l->type = V_LIST;
    l->cons = NULL;
    return l;
}

void cons_output(cons_t *cons, env_t *env)
{
    printf("(");
    while (cons && cons->car)
    {
        value_output(cons->car, env);
        cons = cons->cdr;
    }
    printf(")");
}

void value_output(value_t *val, env_t *env)
{
    switch (val->type)
    {
    case V_IDENTIFIER:
        value_output(env_get(env, val->str), env);
        break;
    case V_STRING:
        printf("%s", val->str);
        break;
    case V_LONG:
        printf("%lld", val->lng);
        break;
    case V_BOOL:
        printf("%s", val->b ? "#t" : "#f");
        break;
    case V_LIST:
        cons_output(val->cons, env);
        break;
    case V_PROCEDURE: case V_BUILTIN:
        break;

    }
}

void value_free(value_t *val)
{
    if (val == NULL)
        return;
    if (val->type == V_STRING && val->str)
        free(val->str);
    free(val);
}