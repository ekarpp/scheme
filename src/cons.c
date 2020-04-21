#include <stdlib.h>
#include <stdio.h>

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

void value_free(value_t *val)
{
    if (val->type == V_STRING && val->str)
        free(val->str);
    free(val);
}
