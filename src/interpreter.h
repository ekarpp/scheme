#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "parser.h"

typedef struct {
    parser_t *prsr;
} interpreter_t;

interpreter_t *interpreter_init(void);
void interpret_file(interpreter_t *interp, char *fname);
int interpret_text(interpreter_t *interp, char *text);
void interpreter_free(interpreter_t *interp);
#endif
