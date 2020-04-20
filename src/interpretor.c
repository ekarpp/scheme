#include "interpretor.h"
#include <stdlib.h>
#include <stdio.h>

interpretor_t *interpretor_init(void)
{
    interpretor_t *interp = malloc(sizeof(interpretor_t));
    interp->prsr = parser_init();
    if (interp->prsr == NULL)
        return NULL;
    return interp;
}

int interpret(interpretor_t *interp, char *text)
{
    if (text[1] == '\0')
    {
        interpretor_free(interp);
        return 0;
    }
    else
    {
        parser_parse(interp->prsr, text);
        return 1;
    }
}

void interpretor_free(interpretor_t *interp)
{
    parser_free(interp->prsr);
    free(interp);
}
