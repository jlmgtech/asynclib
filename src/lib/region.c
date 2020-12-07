#include <stdio.h>
#include <stdlib.h>
#include <async/region.h>

region_t* ropen(size_t size) {
    region_t* this = malloc(sizeof(region_t) + size);
    this->size = size;
    this->mem = (char*)this + sizeof(region_t);
    this->offs = 0;
    return this;
}

void* ralloc(region_t* this, size_t size) {
#ifdef _DEBUG
    if (this->offs + size >= this->size) {
        fprintf(stderr, "ERROR: exceeded region allocation bounds!\n");
        exit(1);
    }
#endif
    void* ptr = (void*)(this->mem + this->offs);
    this->offs += size;
    return ptr;
}

void rclose(region_t* this) {
    free(this);
}
