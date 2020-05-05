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

void interpret_file(interpretor_t *interp, char *fname)
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

int interpret_text(interpretor_t *interp, char *text)
{
    if (text[1] == '\0')
        return 0;
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
