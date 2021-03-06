#ifndef events_h
#define events_h
#include <async/Task.h>
#include <pthread.h>

typedef struct Events {
    size_t count;
    Task* first;
    Task* last;
    pthread_mutex_t lock;
} Events;

Events* EventsCreate();
void EventsFinalize(void*);
void EventsRun(Events*);
void EventsPush(Events*, void (*)(void*), void*);
Task* EventsUnshift(Events*);

#endif
