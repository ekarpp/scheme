#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "exec.h"
#include "env.h"
#include "builtins.h"

parser_t *parser_init(void)
{
    parser_t *prsr = malloc(sizeof(parser_t));
    prsr->lxr = lexer_init();
    if (prsr->lxr == NULL)
        return NULL;
    prsr->env = env_add(NULL);
    if (prsr->env == NULL)
        return NULL;
    builtins_add_all_fs(prsr->env);
    return prsr;
}

int parser_parse(parser_t *prsr, char *text)
{
    lexer_t *lxr = prsr->lxr;
    lxr->curr = text;
    lexer_get_next_token(lxr);
    value_t *val = NULL;

    while (lxr->t->type != T_END)
    {
        switch (lxr->t->type)
        {
        case '(':
            if (val)
                value_free(val);
            val = parser_expression(prsr);
            break;
        default:
            return 0;
        }
        lexer_get_next_token(lxr);
    }
    value_output(val);
    value_free(val);

    return 1;
}



value_t *parser_expression(parser_t *prsr)
{
    lexer_t *lxr = prsr->lxr;
    expr_t *expr = malloc(sizeof(expr_t));
    if (expr == NULL)
        return 0;

    expr->args = NULL;
    expr->last = NULL;
    expr->nargs = 0;

    lexer_get_next_token(lxr);

    value_t *val;
    switch(lxr->t->type)
    {
        case T_IDENTIFIER:
            expr->op = lxr->t->lexeme;
            lxr->t->lexeme = NULL;
            break;
        case '(':
            val = parser_expression(prsr);
            if (val->type != V_STRING)
                return NULL;
            expr->op = val->str;
            val->str = NULL;
            break;
    }

    lexer_get_next_token(lxr);
    while (lxr->t->type != ')')
    {
        value_t *v;
        switch (lxr->t->type)
        {
            case T_END: // unexpected EOF
                return NULL;
            case T_IDENTIFIER: case T_STRING: case T_LONG:
                v = token_to_value(lxr->t);
                break;
            case '(':
                v = parser_expression(prsr);
        }
        expr->nargs++;
        if (expr->last == NULL)
        {
            expr->last = v;
            expr->args = v;
        }
        else
        {
            expr->last->next = v;
            expr->last = v;
        }
        lexer_get_next_token(lxr);
    }
    return exec_expr(prsr->env, expr);
}

void parser_args(parser_t *prsr, expr_t *expr)
{
    lexer_t *lxr = prsr->lxr;
    while (lxr->t->type != T_END && lxr->t->type != ')')
    {
        value_t *v = malloc(sizeof(value_t));
        v->next = NULL;
        v->type = -1;
        switch (lxr->t->type)
        {
            case T_IDENTIFIER: case T_STRING:
                v->type = V_STRING;
                v->str = lxr->t->lexeme;
                break;
            case T_LONG:
                v->type = V_LONG;
                v->lng = str_to_lng(lxr->t->lexeme);
                break;
        }
        expr->nargs++;
        if (expr->last == NULL)
        {
            expr->last = v;
            expr->args = v;
        }
        else
        {
            expr->last->next = v;
            expr->last = v;
        }
        lexer_get_next_token(lxr);
    }
}

void parser_free(parser_t *prsr)
{
    lexer_free(prsr->lxr);
    env_free(prsr->env);
    free(prsr);
}
