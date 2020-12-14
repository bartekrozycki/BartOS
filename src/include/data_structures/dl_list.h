#pragma once

#include "kernel_heap_malloc.h"

#define IMPLEMENT_LIST(Type) \
struct Type##_list_t;        \
struct Type##_node_t;        \
typedef struct Type##_list_t Type##_list; \
typedef struct Type##_node_t Type##_node; \
                             \
struct Type##_node_t {       \
    Type  data;              \
    Type##_node *next;              \
    Type##_node *prev;              \
};                           \
                             \
struct Type##_list_t {       \
    Type##_node *head;              \
    Type##_node *tail;              \
};                           \
                             \
                             \
void Type##_list_display(const Type##_list *ll, void (*display)( Type )) { \
    if (!ll || !(ll->head) || !(ll->tail)) return;                  \
                             \
    for (Type##_node *ptr = ll->head; ptr; ptr = ptr->next)                \
        display(ptr->data);  \
}                            \
                             \
void Type##_list_display_reverse(const Type##_list *ll, void (*display)( Type )) { \
    if (!ll || !(ll->head) || !(ll->tail)) return;                  \
                             \
    for (Type##_node *ptr = ll->tail; ptr; ptr = ptr->prev)                \
        display(ptr->data);  \
}                            \
                             \
Type##_list *Type##_list_create() { \
    Type##_list *new_ll = malloc(sizeof(Type##_list));                            \
    if (!new_ll) return NULL;\
    new_ll->head = NULL;     \
    new_ll->tail = NULL;     \
                             \
    return new_ll;           \
}                            \
                             \
int Type##_list_is_empty(const Type##_list *ll) {                          \
    if (!ll || (ll->head && !ll->tail) || (!ll->head && ll->tail))  \
        return -1;           \
    else if (ll->head && ll->tail) \
        return 0;            \
    return 1;                \
}                            \
                             \
int Type##_list_push_back(Type##_list *ll,  Type  value) {                 \
    if (!ll) return 1;       \
                             \
    Type##_node *node = (Type##_node *) malloc(sizeof(Type##_node));                     \
    if (!node) return 2;     \
                             \
    node->next = NULL;       \
    node->prev = NULL;       \
    node->data = value;      \
                             \
    switch (Type##_list_is_empty(ll)) {                             \
        case -1:             \
            free(node);      \
            return 1;        \
        case 0:              \
            node->prev = ll->tail; \
            ll->tail->next = node; \
            ll->tail = node; \
            break;           \
        case 1:              \
            ll->head = node; \
            ll->tail = node; \
            break;           \
    }                        \
                             \
    return 0;                \
}                            \
                             \
int Type##_list_push_front(Type##_list *ll,  Type  value) {                \
    if (!ll) return 1;       \
                             \
    Type##_node *node = (Type##_node *) malloc(sizeof(Type##_node));                     \
    if (!node) return 2;     \
                             \
    node->next = NULL;       \
    node->prev = NULL;       \
    node->data = value;      \
                             \
    switch (Type##_list_is_empty(ll)) {                             \
        case -1:             \
            free(node);      \
            return 1;        \
        case 0:              \
            node->next = ll->head; \
            ll->head->prev = node; \
            ll->head = node; \
            break;           \
        case 1:              \
            ll->head = node; \
            ll->tail = node; \
            break;           \
    }                        \
    return 0;                \
}                            \
                             \
Type Type##_list_pop_front(Type##_list *ll, int *err_code) {             \
    if (err_code) *err_code = 0;   \
    if (!ll) {               \
        if (err_code) *err_code = 1;                                \
        return (Type){0};            \
    }                        \
    switch (Type##_list_is_empty(ll)) {                             \
        case 1:              \
        case -1:             \
            if (err_code) *err_code = 1;                            \
            return (Type){0};        \
        case 0:              \
                             \
            if (ll->head == ll->tail) {                             \
                 Type  value = ll->head->data;                      \
                             \
                free(ll->head);    \
                             \
                ll->head = NULL;   \
                ll->tail = NULL;   \
                             \
                return value;\
            } else {         \
                 Type  value = ll->head->data;                      \
                Type##_node *temp_ptr = ll->head;                          \
                ll->head = ll->head->next;                          \
                ll->head->prev = NULL;                              \
                free(temp_ptr);    \
                return value;\
            }                \
            break;           \
    }                        \
    return (Type){0};                \
}                            \
                             \
Type  Type##_list_pop_back(Type##_list *ll, int *err_code) {              \
    if (err_code) *err_code = 0;   \
    if (!ll) {               \
        if (err_code) *err_code = 1;                                \
        return (Type){0};            \
    }                        \
    switch (Type##_list_is_empty(ll)) {                             \
        case 1:              \
        case -1:             \
            if (err_code) *err_code = 1;                            \
            return (Type){0};        \
        case 0:              \
                             \
            if (ll->head == ll->tail) {                             \
                 Type  value = ll->head->data;                      \
                             \
                free(ll->head);    \
                             \
                ll->head = NULL;   \
                ll->tail = NULL;   \
                             \
                return value;\
            } else {         \
                Type##_node *temp_ptr = ll->head;                          \
                             \
                while (temp_ptr->next != ll->tail)                  \
                    temp_ptr = temp_ptr->next;                      \
                             \
                 Type  value = temp_ptr->next->data;                \
                             \
                free(temp_ptr->next);                               \
                temp_ptr->next = NULL;                              \
                             \
                ll->tail = temp_ptr;                                \
                             \
                return value;\
            }                \
            break;           \
    }                        \
    return (Type){0};                \
}                            \
                             \
                             \
Type##_node *Type##_list_begin(Type##_list *ll) {\
    if (!ll || !ll->head || !ll->tail) return NULL;                 \
    return ll->head;         \
}                            \
                             \
Type##_node *Type##_list_end(Type##_list *ll) {  \
    if (!ll || !ll->head || !ll->tail) return NULL;                 \
    return ll->tail;         \
}                            \
                             \
 Type Type##_list_front(const Type##_list *ll, int *err_code) {           \
    if (err_code) *err_code = 0;   \
                             \
    if (!ll || !ll->head || !ll->tail) {                            \
        if (err_code) *err_code = 1;                                \
        return (Type){0};            \
    }                        \
    return ll->head->data;   \
}                            \
                             \
 Type  Type##_list_back(const Type##_list *ll, int *err_code) {            \
    if (err_code) *err_code = 0;   \
                             \
    if (!ll || !ll->head || !ll->tail) {                            \
        if (err_code) *err_code = 1;                                \
        return (Type){0};            \
    }                        \
    return ll->tail->data;   \
}                            \
                             \
int Type##_list_size(const Type##_list *ll) {                              \
    if (!ll || (ll->head && !ll->tail) || (!ll->head && ll->tail))  \
        return -1;           \
    if (ll->head == NULL)    \
        return 0;            \
    else if (ll->head == ll->tail) \
        return 1;            \
    else {                   \
        Type##_node *ptr = ll->head;\
        int counter = 1;     \
                             \
        while (ptr != ll->tail) {  \
            counter++;       \
            ptr = ptr->next; \
        }                    \
                             \
        return counter;      \
    }                        \
}                            \
                             \
 Type  Type##_list_at(const Type##_list *ll, unsigned int index, int *err_code) {  \
    if (err_code) *err_code = 0;   \
                             \
    if (!ll || (ll->head && !ll->tail) || (!ll->head && ll->tail) || index >= (unsigned) Type##_list_size(ll)) { \
        if (err_code) *err_code = 1;                                \
        return (Type){0};            \
    }                        \
    Type##_node *ptr = ll->head;    \
    if (!ptr) {              \
        if (err_code) *err_code = 1;                                \
        return (Type){0};            \
    }                        \
                             \
    for (unsigned int i = 0; i < index; i++)                        \
        ptr = ptr->next;     \
                             \
    return ptr->data;        \
}                            \
                             \
                             \
 Type##_node*  Type##_list_node_at(const Type##_list *ll, unsigned int index, int *err_code) {  \
    if (err_code) *err_code = 0;   \
                             \
    if (!ll || (ll->head && !ll->tail) || (!ll->head && ll->tail) || index >= (unsigned) Type##_list_size(ll)) { \
        if (err_code) *err_code = 1;                                \
        return (Type##_node *){0};            \
    }                        \
    Type##_node *ptr = ll->head;    \
    if (!ptr) {              \
        if (err_code) *err_code = 1;                                \
        return (Type##_node *){0};            \
    }                        \
                             \
    for (unsigned int i = 0; i < index; i++)                        \
        ptr = ptr->next;     \
                             \
    return ptr;        \
}                            \
                             \
int Type##_list_insert(Type##_list *ll, unsigned int index,  Type  value) {\
    if (!ll || (ll->head && !ll->tail) || (!ll->head && ll->tail) || index > (unsigned) Type##_list_size(ll))    \
        return 1;            \
                             \
    if (index == 0)          \
        return Type##_list_push_front(ll, value);                   \
    else if (index == (unsigned) Type##_list_size(ll))              \
        return Type##_list_push_back(ll, value);                    \
                             \
    Type##_node *ptr = ll->head;    \
    if (!ptr)                \
        return 1;            \
                             \
    Type##_node *new = (Type##_node *) calloc(1, sizeof(Type##_node));                   \
    if (!new)                \
        return 2;            \
                             \
    for (unsigned int i = 1; i < index; i++)                        \
        ptr = ptr->next;     \
                             \
    new->data = value;       \
                             \
    new->next = ptr->next;   \
    new->prev = ptr;         \
                             \
    ptr->next->prev = new;   \
    ptr->next = new;         \
                             \
                             \
    return 0;                \
}                            \
                             \
Type Type##_list_remove(Type##_list *ll, unsigned int index, int *err_code) {     \
    if (index == 0)          \
        return Type##_list_pop_front(ll, err_code);                 \
    if (index == (unsigned) (Type##_list_size(ll) - 1))             \
        return Type##_list_pop_back(ll, err_code);                  \
                             \
    if (err_code) *err_code = 0;   \
    if (!ll || (ll->head && !ll->tail) || (!ll->head && ll->tail) || index >= (unsigned) Type##_list_size(ll)) { \
        if (err_code) *err_code = 1;                                \
        return (Type){0};      \
    }                        \
                             \
    Type##_node *ptr = ll->head, *temp;   \
     Type  val;              \
    if (!ptr) {              \
        if (err_code) *err_code = 1;                                \
        return (Type){0};            \
    }                        \
                             \
    for (unsigned int i = 1; i < index; i++)                        \
        ptr = ptr->next;     \
                             \
    val = ptr->next->data;   \
    temp = ptr->next;        \
                             \
    ptr->next = ptr->next->next;   \
    ptr->next->prev = ptr;   \
    free(temp);              \
                             \
    return val;              \
}                            \
                             \
                             \
Type Type##_list_remove_node(Type##_list *ll,  Type##_node* _ptr, int *err_code) {     \
    if (_ptr == ll->head)          \
        return Type##_list_pop_front(ll, err_code);                 \
    if (_ptr == ll->tail)             \
        return Type##_list_pop_back(ll, err_code);                  \
                             \
    if (err_code) *err_code = 0;   \
    if (!ll || (ll->head && !ll->tail) || (!ll->head && ll->tail)) { \
        if (err_code) *err_code = 1;                                \
        return (Type){0};      \
    }                        \
                             \
    Type##_node *ptr = ll->head, *temp;   \
    if (!ptr) {              \
        if (err_code) *err_code = 1;                                \
        return (Type){0};            \
    }                        \
                             \
    for (; ptr && ptr->next != _ptr; ptr = ptr->next);\
                             \
    temp = ptr->next;        \
                             \
    ptr->next = ptr->next->next;   \
    ptr->next->prev = ptr;   \
    free(temp);              \
                             \
    return (Type){0};              \
}                            \
                             \
void Type##_list_clear(Type##_list *ll) { \
    int err = 1;             \
    Type##_list_pop_back(ll, &err);\
    while (!err) {           \
        Type##_list_pop_back(ll, &err);                             \
    }                        \
                             \
    return;                  \
}                            \



