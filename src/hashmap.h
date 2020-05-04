#ifndef _HASHMAP_H
#define _HASHMAP_H

#include "cons.h"

typedef struct {
    char *key;
    int hash;
    value_t *value;
} hashmap_element_t;

typedef struct {
    hashmap_element_t **arr;
    int size;
    int used;
    int lim;
} hashmap_t;


int hashmap_increment(int i, int size);

int hashmap_hash(const char *string);
void hashmap_put(hashmap_t *hm, const char *key, value_t *value);
int hashmap_insert(hashmap_element_t **arr, int size, hashmap_element_t *e);
void hashmap_resize(hashmap_t *hm);
void *hashmap_get(hashmap_t *hm, char *key);
hashmap_t *hashmap_init(void);
void hashmap_free_element(hashmap_element_t *hme);
void hashmap_free(hashmap_t *hm);

#endif
