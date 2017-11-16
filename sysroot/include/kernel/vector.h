#ifndef VECTOR_H
#define VECTOR_H

#define VEC_START_SZ 4

typedef struct {
    void **items;
    int sz;
    int used;
    unsigned char freed;
} vector_t;

void vector_init(vector_t *vec);
unsigned char vector_freed(vector_t *vec);
void vector_resize(vector_t *vec, int sz);
void vector_add(vector_t *vec, void *item);
void vector_set(vector_t *vec, int index, void *item);
void *vector_get(vector_t *vec, int index);
void vector_remove(vector_t *vec, int index);
void vector_free(vector_t *vec);

#endif