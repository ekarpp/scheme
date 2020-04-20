#include <stdlib.h>

#include "env.h"
#include "hashmap.h"
#include "exec.h"



value_t *env_get(env_t *env, char *op)
{
    value_t *ret = NULL;
    while (env)
    {
        ret = hashmap_get(env->hm, op);
        if (ret)
            break;
        env = env->next;
    }
    return ret;
}

env_t *env_add(env_t *env)
{
    env_t *top = malloc(sizeof(env_t));
    if (top == NULL)
        return NULL;
    top->hm = hashmap_init();
    top->next = env;
    return top;
}

env_t *env_pop(env_t *env)
{
    env_t *next = env->next;
    hashmap_free(env->hm);
    free(env);
    return next;
}

void env_free(env_t *env)
{
    while (env)
    {
        hashmap_free(env->hm);
        env_t *tmp = env->next;
        free(env);
        env = tmp;
    }
}

