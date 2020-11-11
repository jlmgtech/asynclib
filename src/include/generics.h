#ifndef generics_h
#define generics_h

#include "./Events.h"
#include "./Task.h"
// TODO - finish creating generic functions, like allocators and deallocators
// and other useful macros, including integrating the GC

#define destroy(x) _Generic((x), \
        Task*: TaskDestroy, \
        Events*: EventsDestroy) (x)

#endif
