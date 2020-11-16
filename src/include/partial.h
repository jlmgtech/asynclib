#include "./Generator.h"

#define PRT_PARAM(type) ((type)GeneratorYield(gen, NULL))
#define PRT_RETURN(val) GeneratorYield(gen, (void*)(val)); return
#define PRT_PARAMETERS Generator* gen
#define PRT_APPLY(p, x) apply_partial(p, (void*)(x))
typedef Generator partial_t;

void free_partial(partial_t* this);
void* apply_partial(partial_t* this, void* arg);
partial_t* make_partial(void (func)(Generator*));
