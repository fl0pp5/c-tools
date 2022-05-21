#include <stdlib.h>

#include "box.h"


box_t *
new_box(size_t n, size_t item_size) {

    box_t *box = (box_t *) malloc(sizeof(box_t));

    if (box == NULL) {
        return NULL;
    }

    if ((box->data = malloc(n * item_size)) == NULL) {
        dealloc_box(box);

        return NULL;
    }

    box->cap        = n;
    box->len        = 0;
    box->_mem_used  = n * item_size;
    box->_item_size = item_size;

    return box;
}

int
inc_box(box_t *box) {
    void *ptr = realloc(box->data, box->_mem_used * 2);

    if (ptr == NULL) {
        return B_REALLOC_ERROR;
    }

    box->data      = ptr;
    box->cap       *= 2;
    box->_mem_used *= 2;

    return B_OK;
}

void
dealloc_box(box_t *box) {
    if (box->data != NULL) {
        free(box->data);
    }
    free(box);
}


int
box_get(box_t *box, size_t index, void *storage, void (*get_func)(box_t*, void*, void*)) {
    void *ptr;

    if (index < 0 || index >= box->len) {
        return B_INDEX_OUT_OF_RANGE;
    }

    ptr = (char *) box->data + index * box->_item_size;

    get_func(box, ptr, storage);

    return B_OK;
}

int
box_add(box_t *box, void *value, void (*add_func)(box_t*, void*, void*)) {
    void *ptr = (char *) box->data + box->len * box->_item_size;

    if (box->len < box->cap) {
        add_func(box, ptr, value);
        box->len++;
        return B_OK;
    }
    return B_CAP_OVERFLOW;
}

int
box_del(box_t *box, size_t index, size_t step, void (*del_func)(void*)) {
    void *ptr;

    if (index < 0 || index >= box->len || box->len <= 0) {
        return B_INDEX_OUT_OF_RANGE;
    }

    ptr = (char *) box->data + index * step;
    for (; index < box->len; index++, ptr += step) {
        del_func(ptr);
    }
    box->len--;

    return B_OK;
}

int
box_chg(box_t *box, size_t index, void *new_value, void (*chg_func)(box_t*, void*, void*)) {
    void *ptr;

    if (index < 0 || index >= box->len) {
        return B_INDEX_OUT_OF_RANGE;
    }

    ptr = (char *) box->data + index * box->_item_size;
    chg_func(box, ptr, new_value);

    return B_OK;
}
