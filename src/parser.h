#ifndef PARSER
#define PARSER

#include "lexer.h"
#include "exec.h"
#include "env.h"

typedef struct {
    lexer_t *lxr;
    env_t *env;
    int brackets;
} parser_t;




parser_t *parser_init(void);
int parser_parse(parser_t *prsr, char *text);
value_t *parser_expression(parser_t *prsr);
void parser_args(parser_t *prsr, expr_t *expr);

void parser_free(parser_t *prsr);
#endif
