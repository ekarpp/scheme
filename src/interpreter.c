#include "interpreter.h"
#include <stdlib.h>
#include <stdio.h>

interpreter_t *interpreter_init(void)
{
    interpreter_t *interp = malloc(sizeof(interpreter_t));
    interp->prsr = parser_init();
    if (interp->prsr == NULL)
        return NULL;
    return interp;
}

void interpret_file(interpreter_t *interp, char *fname)
{
    FILE *f = fopen(fname, "r");
    if (f == NULL)
    {printf("%s not found\n", fname); return;}

    fseek(f, 0, SEEK_END);
    long unsigned int len = ftell(f);
    rewind(f);

    char *txt = malloc(len + 1);
    if (len != fread(txt, 1, len, f))
        printf("ERR\n");
    fclose(f);
    txt[len] = '\0';
    interpret_text(interp, txt);
    free(txt);
}

int interpret_text(interpreter_t *interp, char *text)
{
    if (text[1] == '\0')
        return 0;
    else
    {
        parser_parse(interp->prsr, text);
        return 1;
    }
}

void interpreter_free(interpreter_t *interp)
{
    parser_free(interp->prsr);
    free(interp);
}
