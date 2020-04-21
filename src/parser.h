#ifndef PARSER
#define PARSER

#include "cons.h"
#include "lexer.h"
#include "exec.h"
#include "env.h"

typedef struct {
    lexer_t *lxr;
    env_t *env;
} parser_t;

parser_t *parser_init(void);
int parser_parse(parser_t *prsr, char *text);
value_t *parser_expression(parser_t *prsr);

void parser_free(parser_t *prsr);
#endif
