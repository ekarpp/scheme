#ifndef _LEXER_H
#define _LEXER_H

#include <stddef.h>

#define TOKEN_INITIAL 16

typedef enum {
    T_END,
    T_STRING,
    T_LONG,
    T_IDENTIFIER,
    T_COM_AT
} token_type;


typedef struct {
    token_type type;
    char *lexeme;
    int size;
    int len;
} token_t;

typedef struct {
    int line_no;
    token_t *t;
    char *curr;
} lexer_t;


lexer_t *lexer_init(void);
int lexer_clean_token(lexer_t *lxr);
int lexer_add(lexer_t *lxr, int n);

int lexer_get_next_token(lexer_t *lxr);

int lexer_hashtag(lexer_t *lxr);
int lexer_string(lexer_t *lxr);
int lexer_identifier(lexer_t *lxr);
int lexer_int(lexer_t *lxr);

void lexer_free(lexer_t *lxr);

#endif
