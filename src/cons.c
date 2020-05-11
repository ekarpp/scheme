#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "env.h"
#include "cons.h"
#include "exec.h"


char *type_to_string(value_type t)
{
    switch (t)
    {
    case V_BOOL:
        return "boolean";
    case V_LONG:
        return "long";
    case V_IDENTIFIER:
        return "identifier";
    case V_STRING:
        return "string";
    case V_LIST:
        return "list";
    case V_PROCEDURE:
        return "procedure";
    case V_BUILTIN:
        return "builtin function";
    case V_EXPRESSION:
        return "expression";
    }
    return "0";
}

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

cons_t *cons_copy(cons_t *cons)
{
    cons_t *ret = cons_init();
    ret->car = value_copy(cons->car);
    ret->cdr = cons_copy(cons->cdr);
    return ret;
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

/* expression */
expression_t *expression_init(void)
{
    expression_t *expr = malloc(sizeof(expression_t));
    if (expr == NULL)
        return NULL;
    expr->body = NULL;
    expr->val = NULL;
    return expr;
}

void expression_free(expression_t *expr)
{
    if (expr == NULL)
        return;
    cons_free(expr->body);
    value_free(expr->val);
    free(expr);
}

/* value */

value_t *value_init(void)
{
    value_t *val = malloc(sizeof(value_t));
    if (val == NULL)
        return NULL; // todo: handle better
    val->free = 1;
    return val;
}

value_t *token_to_value(token_t *t)
{
    value_t *val = value_init();
    value_t *tmp;
    switch (t->type)
    {
    case T_IDENTIFIER:
        val->type = V_IDENTIFIER;
        val->str = t->lexeme;
        t->lexeme = NULL;
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

value_t *value_get(value_t *val, env_t *env)
{
    char *name;
    switch (val->type)
    {
    case V_IDENTIFIER:
        name = val->str;
        while (val && val->type == V_IDENTIFIER)
            val = env_get(env, val->str);
        if (val == NULL)
            printf("\nUnbound identifier %s.\n", name);
    }
    return val;
}

value_t *value_copy(value_t *val)
{
    value_t *ret = value_init();
    ret->type = val->type;

    switch (val->type)
    {
    case V_LONG:
        ret->lng = val->lng;
        break;
    case V_BOOL:
        ret->b = val->b;
        break;
    case V_BUILTIN:
        ret->bif = val->bif;
        break;
    case V_LIST:
        ret->cons = cons_copy(val->cons);
        break;
    case V_IDENTIFIER: case V_STRING:
        ret->str = malloc(strlen(val->str) + 1);
        strcpy(ret->str, val->str);
        break;
    case V_EXPRESSION:
        ret->expr = expression_init();
        ret->expr->val = value_copy(val->expr->val);
        ret->expr->body = cons_copy(val->expr->body);
        break;
    case V_PROCEDURE:
        ret->proc = procedure_init();
        ret->proc->formals = cons_copy(val->proc->formals);
        ret->proc->expr = expression_init();
        ret->proc->expr->val = value_copy(val->proc->expr->val);
        ret->proc->expr->body = cons_copy(val->proc->expr->body);
        break;
    }

    return ret;
}

value_t *empty_list(void)
{
    value_t *l = value_init();
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
    if (val == NULL)
        return;
    switch (val->type)
    {
    case V_IDENTIFIER:
        // check here that we actually get a value
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
    case V_PROCEDURE: case V_BUILTIN: case V_EXPRESSION:
        break;

    }
}

void value_free(value_t *val)
{
    if (val == NULL || val->free == 0)
        return;
    switch (val->type)
    {
    case V_IDENTIFIER: case V_STRING:
        if (val->str)
            free(val->str);
        break;
    case V_PROCEDURE:
        if (val->proc)
            procedure_free(val->proc);
        break;
    case V_LIST:
        cons_free(val->cons);
        break;
    case V_EXPRESSION:
        expression_free(val->expr);
        break;
    }
    free(val);
}

/* procedure */
procedure_t *procedure_init(void)
{
    procedure_t *proc = malloc(sizeof(procedure_t));
    if (proc == NULL)
        return NULL; // error

    proc->formals = NULL;
    proc->expr = NULL;

    return proc;
}

// args has two lists
// first is formals ( can also be other than list, fix later)
// second is body
procedure_t *procedure_make(value_t *formals, value_t *body)
{
    procedure_t *proc = procedure_init();
    proc->formals = formals->expr->body;
    formals->expr->body = NULL;

    proc->expr = body->expr;
    body->expr = NULL;

    return proc;
}

void procedure_free(procedure_t *proc)
{
    cons_free(proc->formals);
    expression_free(proc->expr);
    free(proc);
}
