#include <stdlib.h>
#include <stdio.h>
#include <kernel/vector.h>

void vector_init(vector_t *vec) {
    vec->sz = VEC_START_SZ;
    vec->used = 0;
    vec->items = malloc(sizeof(void *)*vec->sz);
    vec->freed = 0;
}

unsigned char vector_freed(vector_t *vec) {
    if (vec->freed == 1) {
        printf("vector with size %d does not exist!\n", vec->sz);
        return 1;
    }
    return 0;
}

void vector_resize(vector_t *vec, int sz) {
    //printf("resizing from %d to %d\n", vec->sz, sz);
    void **items = realloc(vec->items, sizeof(void *)*sz);
    if (items) {
        vec->items = items;
        vec->sz = sz;
    }
}

void vector_add(vector_t *vec, void *item) {
    if (vector_freed(vec)) return;

    if (vec->sz == vec->used) {
        vector_resize(vec, vec->sz*2);
    }
    vec->items[vec->used++] = item;
}

void vector_set(vector_t *vec, int index, void *item) {
    if (vector_freed(vec)) return;
    
    if (index >= 0 && index < vec->used) {
        vec->items[index] = item;
    }
}

void *vector_get(vector_t *vec, int index) {
    if (vector_freed(vec)) return NULL;
    
    if (index >= 0 && index < vec->used) {
        return vec->items[index];
    }
    return NULL;
}

void vector_remove(vector_t *vec, int index) {
    if (vector_freed(vec)) return;
    
    if (index < 0 || index >= vec->used) {
        return;
    }
    vec->items[index] = NULL;
    int i;
    for (i = index; i < vec->used-1; i++) {
        vec->items[i] = vec->items[i+1];
        vec->items[i+1] = NULL;
    }
    vec->used--;
    if (vec->used > 0 && vec->used == vec->sz/4) {
        vector_resize(vec, vec->sz/2);
    }
}

void vector_free(vector_t *vec) {
    if (vec->freed == 1)  {
        printf("vector of size %d has already been freed!\n", vec->sz);    
        return;
    }
    free(vec->items);
    vec->freed = 1;
}