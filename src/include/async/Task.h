#ifndef Task_h
#define Task_h

typedef struct Task {
    void * function;
    void* data;
    struct Task* next;
} Task;

Task* TaskCreate(void*, void*);
void TaskCall(Task*);

#endif
