#ifndef _ENV_H
#define _ENV_H

typedef struct env_t env_t;

#include "hashmap.h"
#include "cons.h"

struct env_t {
    hashmap_t *hm;
    env_t *next;
};

value_t *env_get(env_t *env, char *key);
env_t *env_add(env_t *env);
env_t *env_pop(env_t *env);
void env_free(env_t *env);

#endif
