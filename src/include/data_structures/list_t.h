#pragma once
#include "kernel_heap_malloc.h"
#include "ints.h"

#define LIST_ERROR_NULLPTR 1
#define LIST_ERROR_NO_MEMORY 2
#define LIST_ERROR_NOT_FOUND 3

#define DELEGATE_LIST(Type) \
struct Type##_node_t;       \
struct Type##_node_t {             \
    Type data;              \
    u32 uid;                        \
    struct Type##_node_t * next;   \
    struct Type##_node_t * prev;   \
};                          \
                            \
struct Type##_list_t {             \
    struct Type##_node_t * head;   \
    struct Type##_node_t * tail;   \
};                     \
typedef struct Type##_node_t Node; \
typedef struct Type##_list_t List; \
                            \
static u32 list_##Type##_uid_iterator = 0;\
List* list_##Type##_create()         \
{                                  \
    List *ptr = (List *) malloc((sizeof(List))); \
    if (!ptr)                      \
        return NULL;               \
    ptr->head = NULL;              \
    ptr->tail = NULL;              \
                                   \
    return ptr;                    \
}                                  \
Node* list_##Type##_create_node()    \
{                                  \
    register Node* new_node = malloc(sizeof(Node));               \
    if (!new_node)                 \
        return NULL;               \
    new_node->uid = list_##Type##_uid_iterator++;\
    return new_node;               \
}                                 \
                                  \
void list_##Type##_destroy_nodes(Node *node_p)                          \
{                           \
    if (!node_p) return;    \
                            \
    list_##Type##_destroy_nodes(node_p->next);                      \
    free(node_p);           \
}                           \
                            \
void list_##Type##_destroy(List *list_p) \
{                           \
    if (!list_p) return;    \
    list_##Type##_destroy_nodes(list_p->head);                          \
    free(list_p);           \
}                           \
                            \
int list_##Type##_add_begin(List *list_p, Type data_p)                 \
{                           \
    if (!list_p) return LIST_ERROR_NULLPTR;            \
                            \
    Node *new_node = list_##Type##_create_node();                       \
    if (!new_node) return LIST_ERROR_NO_MEMORY;                   \
                            \
    new_node->next = list_p->head; \
    new_node->prev = NULL;  \
    new_node->data = data_p;\
                            \
    list_p->head = new_node;\
                            \
    return 0;               \
}                           \
int list_##Type##_add_end(List *list_p, Type data_p)                   \
{                           \
    if (!list_p) return LIST_ERROR_NULLPTR;            \
                            \
    Node *new_node = list_##Type##_create_node();                       \
    if (!new_node) return LIST_ERROR_NO_MEMORY;                   \
                            \
    new_node->next = NULL;  \
    new_node->prev = list_p->tail; \
    new_node->data = data_p;\
                            \ /// TODO dokonczyc
                            \
    list_p->tail = new_node;\
                            \
    return 0;               \
}                           \
                            \
Node *list_##Type##_at(List *list_p, size_t n)                          \
{                           \
    if (!list_p) return NULL;      \
                            \
    register Node *ptr = list_p->head;                            \
    for (size_t i = 0; ptr && i < n; ++i)                         \
        ptr = ptr->next;    \
                            \
    return ptr;             \
}                           \
                            \
int list_##Type##_remove(List *list_p, size_t n)                        \
{                           \
    if (!list_p) return LIST_ERROR_NULLPTR;                       \
    Node *ptr = (Node *) list_##Type##_at(list_p, n);                   \
                            \
    if (!ptr) return LIST_ERROR_NOT_FOUND;                        \
                            \
    if (ptr == list_p->head)\
    {                       \
        list_p->head == ptr->next; \
        ptr->next->prev = NULL;     \
    }else if (ptr == list_p->tail)\
    {                       \
        list_p->tail == ptr->prev; \
        ptr->prev->next = NULL; \
    } else {                \
        ptr->next->prev = ptr->prev;\
        ptr->prev->next = ptr->next;\
    }\
                            \
    free(ptr);              \
                            \
    return 0;               \
} \
