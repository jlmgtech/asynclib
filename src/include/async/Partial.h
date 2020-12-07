#include <async/Generator.h>

#define PRT_PARAM(type) ((type)GeneratorYield(gen, NULL))
#define PRT_RETURN(val) GeneratorYield(gen, (void*)(val)); return
#define PRT_PARAMETERS Generator* gen
#define PRT_APPLY(p, x) PartialApply(p, (void*)(x))
typedef Generator partial_t;

void* PartialApply(partial_t* this, void* arg);
partial_t* PartialCreate(void (func)(Generator*));
