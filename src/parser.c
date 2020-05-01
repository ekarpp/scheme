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
    cons_t *expr;

    while (lxr->t->type != T_END)
    {
        if (val)
            value_free(val);
        switch (lxr->t->type)
        {
        case '(':
            expr = parser_expression(prsr);
            val = exec_expr(expr, prsr->env);
            cons_free(expr);
            break;
        case T_LONG: case T_STRING: case T_IDENTIFIER:
            val = token_to_value(lxr->t);
            break;
        default:
            return 0;
        }
        lexer_get_next_token(lxr);
    }
    value_output(val, prsr->env);
    value_free(val);

    return 1;
}

/* parses an expression (expr) */
cons_t *parser_expression(parser_t *prsr)
{
    lexer_t *lxr = prsr->lxr;
    lexer_get_next_token(lxr);

    cons_t *expr = cons_init();
    /* parse arguments */
    cons_t *last = NULL;
    while (lxr->t->type != ')')
    {
        if (last == NULL)
            last = expr;
        else
        {
            last->cdr = cons_init();
            last = last->cdr;
        }

        switch (lxr->t->type)
        {
            case T_END: // unexpected EOF
                return NULL;
            case T_IDENTIFIER: case T_STRING: case T_LONG:
                last->car = token_to_value(lxr->t);
                break;
            case '(':
                last->car = value_init();
                last->car->type = V_LIST;
                last->car->cons = parser_expression(prsr);
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
