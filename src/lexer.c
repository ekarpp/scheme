#include <stdlib.h>

#include "lexer.h"

lexer_t *lexer_init(void)
{
    lexer_t *lxr = malloc(sizeof(lexer_t));
    lxr->line_no = 1;
    lxr->t = malloc(sizeof(token_t));
    lxr->t->type = T_END;
    lxr->t->len = TOKEN_INITIAL;
    lxr->t->lexeme = malloc(lxr->t->len);
    lxr->t->lexeme[0] = '\0';
    lxr->t->size = 0;

    return lxr;
}

void lexer_free(lexer_t *lxr)
{
    free(lxr->t->lexeme);
    free(lxr->t);
    free(lxr);
    return;
}

int lexer_clean_token(lexer_t *lxr)
{
    if (lxr->t->lexeme != NULL)
        free(lxr->t->lexeme);
    lxr->t->len = TOKEN_INITIAL;
    lxr->t->lexeme = malloc(lxr->t->len);
    if (lxr->t->lexeme == NULL)
        return 0;
    lxr->t->lexeme[0] = '\0';
    lxr->t->size = 0;
    return 1;
}

int lexer_resize(lexer_t *lxr)
{
    lxr->t->len <<= 1;
    char* newptr = realloc(lxr->t->lexeme, lxr->t->len);
    if (newptr == NULL)
        return 0;
    lxr->t->lexeme = newptr;
    return 1;
}

/* add n characters to curr token */
int lexer_add(lexer_t *lxr, int n)
{
    int reti = 1;
    if (lxr->t->size + n >= lxr->t->len)
        reti = lexer_resize(lxr);
    for (int i = 0; i < n; i++)
        lxr->t->lexeme[lxr->t->size + i] = lxr->curr[i];
    lxr->curr += n;
    lxr->t->size += n;
    lxr->t->lexeme[lxr->t->size] = '\0';
    return reti;
}

int lexer_add_char(lexer_t *lxr, char c)
{
    int reti = 0;
    if (lxr->t->size + 1 > lxr->t->len)
        reti = lexer_resize(lxr);
    lxr->curr += 2;
    lxr->t->lexeme[lxr->t->size] = c;
    lxr->t->size++;
    lxr->t->lexeme[lxr->t->size] = '\0';
    return reti;
}

int lexer_get_next_token(lexer_t *lxr)
{
    lexer_clean_token(lxr);
    // skip whitespace
    while (*lxr->curr && *lxr->curr <= ' ')
    {
        if (*lxr->curr == '\n')
            lxr->line_no++;
        lxr->curr++;
    }

    // end of string
    if (*lxr->curr == '\0')
    {
        free(lxr->t->lexeme);
        lxr->t->lexeme = NULL;
        lxr->t->type = T_END;
        return 1;
    }

    switch (*lxr->curr)
    {
        case '(': case ')': case '\'':
        case '`': case '.':
            lxr->t->type = *lxr->curr;
            return lexer_add(lxr, 1);
        case '+': case '-':
            lxr->t->type = T_IDENTIFIER;
            return lexer_add(lxr, 1);
        case ',':
            lxr->t->type = ',';
            if (*(lxr->curr + 1) != '@')
                return lexer_add(lxr, 1);
            lxr->t->type = T_COM_AT;
            return lexer_add(lxr, 2);
        case ';':
            while (*lxr->curr && *lxr->curr != '\n' && *lxr->curr != '\r')
                lxr->curr++;
            return lexer_get_next_token(lxr);
        case '#':
            return lexer_hashtag(lxr);
        case '"':
            lxr->t->type = T_STRING;
            return lexer_string(lxr);
        case '!': case '$': case '%': case '&': case '*':
        case '/': case ':': case '<': case '=': case '>':
        case '?': case '^': case '_': case '~':
            lxr->t->type = T_IDENTIFIER;
            return lexer_identifier(lxr);
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            lxr->t->type = T_LONG;
            return lexer_int(lxr);
        default:
            if ((*lxr->curr >= 'A' && *lxr->curr <= 'Z')
               || (*lxr->curr >= 'a' && *lxr->curr <= 'z'))
                lxr->t->type = T_IDENTIFIER;
                return lexer_identifier(lxr);
    }

    return 0;
}

int lexer_hashtag(lexer_t *lxr)
{
    lxr->curr++;
    switch(*lxr->curr)
    {
        case '|':
            BACK:
            do
                lxr->curr++;
            while(*lxr->curr != '|');
            if (*(lxr->curr + 1) != '#')
                goto BACK;
            lxr->curr += 2;
            return lexer_get_next_token(lxr);
        case 't':

        case 'f':
            break;
    }
}

int lexer_string(lexer_t *lxr)
{
    *lxr->curr++;
    int reti = 1;
    while (*lxr->curr != '"')
    {
        if (reti == 0 || *lxr->curr == '\0')
            return 0;
        switch(*lxr->curr)
        {
            case '\\':
            {
                switch (*(lxr->curr + 1))
                {
                    case 'a': reti = lexer_add_char(lxr, '\a'); break;
                    case 'b': reti = lexer_add_char(lxr, '\b'); break;
                    case 't': reti = lexer_add_char(lxr, '\t'); break;
                    case 'n': reti = lexer_add_char(lxr, '\n'); break;
                    case 'r': reti = lexer_add_char(lxr, '\r'); break;
                    case '"':
                    case '\\': reti = lexer_add(lxr, 2); break;
                    default: return 0;
                }
                break;
            }
            default:
                if (lexer_add(lxr, 1) == 0)
                    return 0;
        }
    }
    *lxr->curr++;
    return reti;
}

int lexer_identifier(lexer_t *lxr)
{
    if (lexer_add(lxr, 1) == 0)
        return 0;
    switch (*lxr->curr)
    {
        case '!': case '$': case '%': case '&': case '*':
        case '/': case ':': case '<': case '=': case '>':
        case '?': case '^': case '_': case '~':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return lexer_identifier(lxr);
        default:
            if ((*lxr->curr >= 'A' && *lxr->curr <= 'Z')
               || (*lxr->curr >= 'a' && *lxr->curr <= 'z'))
                return lexer_identifier(lxr);
            else
                return 1;

    }
}

int lexer_int(lexer_t *lxr)
{
    if (lexer_add(lxr, 1) == 0)
        return 0;
    switch (*lxr->curr)
    {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return lexer_int(lxr);
        default:
            return 1;
    }
}
