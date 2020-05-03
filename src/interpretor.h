#ifndef _INTERPRETOR_H
#define _INTERPRETOR_H

#include "parser.h"

typedef struct {
    parser_t *prsr;
} interpretor_t;


interpretor_t *interpretor_init(void);
void interpret_file(interpretor_t *interp, char *fname);
int interpret_text(interpretor_t *interp, char *text);
void interpretor_free(interpretor_t *interp);
#endif
