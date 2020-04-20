#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "hashmap.h"

#define INITIAL_SIZE 64


inline int hashmap_increment(int i, int size)
{
    return (i + 1) % size;
}


int hashmap_hash(char *string)
{
    int sum = 0;
    int i = 0;
    while (string[i])
    {
        sum += string[i] * (i + 1);
        i++;
    }
    return sum;
}

void hashmap_put(hashmap_t *hm, char *key, void *value)
{
    const int hash = hashmap_hash(key);
    hashmap_element_t *hme = malloc(sizeof(hashmap_element_t));
    hme->value = value;
    hme->key = key;
    hme->hash = hash;
    hashmap_insert(hm->arr, hm->size, hme);
    hm->used++;
    if (hm->used >= hm->lim)
        hashmap_resize(hm);
}

#include <stdio.h>
void hashmap_insert(hashmap_element_t **arr, int size, hashmap_element_t *e)
{
    int i = e->hash % size;
    while (arr[i])
        i = hashmap_increment(i, size);
    arr[i] = e;
}


void hashmap_resize(hashmap_t *hm)
{
    const int prev = hm->size;
    hm->size *= 4;
    hashmap_element_t **next = malloc(sizeof(hashmap_element_t**) * hm->size);
    for (int i = 0; i < hm->size; i++)
        next[i] = NULL;
    for (int i = 0; i < prev; i++)
        if (hm->arr[i])
            hashmap_insert(next, hm->size, hm->arr[i]);
    free(hm->arr);
    hm->arr = next;
}

void *hashmap_get(hashmap_t *hm, char *key)
{
    const int hash = hashmap_hash(key);
    int i = hash % hm->size;
    while (1)
    {
        if (hm->arr[i])
        {
            if (hash == hm->arr[i]->hash && strcmp(key, hm->arr[i]->key) == 0)
                return hm->arr[i]->value;
            i = hashmap_increment(i, hm->size);
        }
        else
            return NULL;
    }
}

hashmap_t *hashmap_init(void)
{
    hashmap_t *hm = malloc(sizeof(hashmap_t));
    hm->size = INITIAL_SIZE;
    hm->used = 0;
    hm->lim = 3 * (INITIAL_SIZE / 4);
    hm->arr = malloc(sizeof(hashmap_element_t**) * hm->size);
    for (int i = 0; i < hm->size; i++)
        hm->arr[i] = NULL;
    return hm;
}

void hashmap_free_element(hashmap_element_t *hme)
{
    free(hme->value);
    free(hme);
}

void hashmap_free(hashmap_t *hm)
{
    for (int i = 0; i < hm->size; i++)
        if (hm->arr[i])
            hashmap_free_element(hm->arr[i]);
    free(hm->arr);
    free(hm);
}
