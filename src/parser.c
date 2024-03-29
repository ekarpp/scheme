#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "exec.h"
#include "cons.h"
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

/* parses text using prsr */
int parser_parse(parser_t *prsr, char *text)
{
    lexer_t *lxr = prsr->lxr;
    lxr->curr = text;
    lexer_get_next_token(lxr);
    value_t *val = NULL;
    expression_t *expr;

    while (lxr->t->type != T_END)
    {
        if (val)
            value_free(val);
        switch (lxr->t->type)
        {
        case '(':
            expr = parser_expression(prsr);
            if (expr == NULL)
                {val = NULL; break;}
            exec_expr(expr, prsr->env);
            val = expr->val;
            expr->val = NULL;
            expression_free(expr);
            break;
        case T_LONG: case T_STRING: case T_IDENTIFIER:
            val = token_to_value(lxr->t);
            break;
        default:
            printf("Unexpected symbol while parsing: %s.\n", lxr->t->lexeme);
            return 0;
        }
        lexer_get_next_token(lxr);
    }
    value_output(val, prsr->env);
    value_free(val);

    return 1;
}

/* parses an expression (expr) */
expression_t *parser_expression(parser_t *prsr)
{
    lexer_t *lxr = prsr->lxr;
    lexer_get_next_token(lxr);

    expression_t *expr = expression_init();
    /* parse arguments */
    cons_t *last = NULL;
    while (lxr->t->type != ')')
    {
        if (last == NULL)
        {
            expr->body = cons_init();
            last = expr->body;
        }
        else
        {
            last->cdr = cons_init();
            last = last->cdr;
        }

        switch (lxr->t->type)
        {
            case T_IDENTIFIER: case T_STRING: case T_LONG:
                last->car = token_to_value(lxr->t);
                break;
            case '(':
                last->car = value_init();
                last->car->type = V_EXPRESSION;
                last->car->expr = parser_expression(prsr);
                break;
            default:
                expression_free(expr);
                printf("Unexpected symbol while parsing: %s.\n", lxr->t->lexeme);
                return NULL;
        }
        lexer_get_next_token(lxr);
    }

    return expr;
}

void parser_free(parser_t *prsr)
{
    lexer_free(prsr->lxr);
    env_free(prsr->env);
    free(prsr);
}
