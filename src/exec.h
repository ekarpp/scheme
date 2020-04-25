#ifndef _EXEC_H
#define _EXEC_H

#include "env.h"
#include "cons.h"

value_t *exec_expr(cons_t *expr, env_t *env);


#endif
