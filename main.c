#include <stdio.h>
#include "src/interpretor.h"

int main(void)
{
    char text[255];
    interpretor_t *interp = interpretor_init();
    if (interp == NULL)
        return -1;
    int ret;
    do {
        printf(">> ");
        fgets(text, 255, stdin);
        ret = interpret(interp, text);
    } while (ret);

    return 0;
}
