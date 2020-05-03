#include <stdio.h>
#include "src/interpretor.h"

int main(int argc, char **argv)
{

    interpretor_t *interp = interpretor_init();
    if (interp == NULL)
        return -1;

    if (argc > 1)
        for (int i = 1; i < argc; i++)
            interpret_file(interp, argv[i]);
    else
    {
        int ret;
        char text[255];
        do {
            printf(">> ");
            fgets(text, 255, stdin);
            ret = interpret_text(interp, text);
        } while (ret);
    }

    interpretor_free(interp);

    return 0;
}
