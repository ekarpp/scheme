#ifndef _BUILTINS_H
#define _BUILTINS_H

#include "exec.h"
#include "env.h"

value_t *builtins_add(value_t *args);
value_t *builtins_subtract(value_t *args);
value_t *builtins_multiply(value_t *args);
value_t *builtins_divide(value_t *args);
value_t *builtins_abs(value_t *args);
value_t *builtins_eqv(value_t *args);
value_t *builtins_cmp(value_t *args, char op);
value_t *builtins_equal(value_t *args);
value_t *builtins_increasing(value_t *args);
value_t *builtins_non_increasing(value_t *args);
value_t *builtins_decreasing(value_t *args);
value_t *builtins_non_decreasing(value_t *args);
value_t *builtins_lambda(value_t *args);

void builtins_add_f(hashmap_t *hm, char *key, builtin_t f);
void builtins_add_all_fs(env_t *env);



#endif
