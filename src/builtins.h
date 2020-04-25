#ifndef _BUILTINS_H
#define _BUILTINS_H

#include "cons.h"
#include "env.h"

value_t *builtins_add(cons_t *args, env_t *env);
value_t *builtins_subtract(cons_t *args, env_t *env);
value_t *builtins_multiply(cons_t *args, env_t *env);
value_t *builtins_divide(cons_t *args, env_t *env);
value_t *builtins_abs(cons_t *args, env_t *env);

value_t *builtins_eqv(cons_t *args, env_t *env);

value_t *builtins_cmp(cons_t *args, char op, env_t *env);
value_t *builtins_equal(cons_t *args, env_t *env);
value_t *builtins_increasing(cons_t *args, env_t *env);
value_t *builtins_non_increasing(cons_t *args, env_t *env);
value_t *builtins_decreasing(cons_t *args, env_t *env);
value_t *builtins_non_decreasing(cons_t *args, env_t *env);

value_t *builtins_define(cons_t *args, env_t *env);
value_t *builtins_lambda(cons_t *args, env_t *env);

void builtins_add_f(hashmap_t *hm, char *key, builtin_t f);
void builtins_add_all_fs(env_t *env);



#endif
