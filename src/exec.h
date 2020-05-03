#ifndef _EXEC_H
#define _EXEC_H

#include "env.h"
#include "cons.h"

value_t *exec_expr(cons_t *expr, env_t *env);
value_t *exec_procedure(procedure_t *proc, cons_t *args, env_t *env);
void exec_eval(cons_t *arg, env_t *env);
#endif
