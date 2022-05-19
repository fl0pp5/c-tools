#ifndef BOX_H
#define BOX_H

#include <stdint.h>


enum BoxCodes {
    B_OK,
    B_CAP_OVERFLOW,
    B_INDEX_OUT_OF_RANGE,
    B_REALLOC_ERROR,
};

typedef struct {
    void    *data;
    size_t  len;
    size_t  cap;

    size_t  _mem_used;
} box_t;

box_t *
new_box(size_t n, size_t item_size);

int
inc_box(box_t *box);

void
dealloc_box(box_t *box);

int
box_get(box_t *box, size_t index, void *storage, void (*get_func)(box_t*, size_t, void*));

int
box_add(box_t *box, void *value, void (*add_func)(box_t*, void*));

int
box_del(box_t *box, size_t index, size_t step, void (*del_func)(void*));

#endif //BOX_H

