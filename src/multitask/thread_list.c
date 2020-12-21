#include "thread_list.h"
#include "kernel_heap_malloc.h"

thread_list * list_thread_create(void)
{
    thread_list *list = malloc(sizeof(thread_list));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}
u32 list_thread_push_front(thread_list *list, thread_control_block *ptr)
{
    if (!list || !ptr) return 1;

    if (list->head == NULL)
    {
        list->tail = ptr;
        list->head = ptr;
        list->head->next = NULL;
    }
    else
    {
        ptr->next = list->head;
        list->head = ptr;
    }

    ++list->size;
    return 0;
}
u32 list_thread_push_back(thread_list *list, thread_control_block *ptr)
{
    if (!list || !ptr) return 1;

    ptr->next = NULL;
    if (list->tail == NULL)
    {
        list->tail = ptr;
        list->head = ptr;
    }
    else
    {
        list->tail->next = ptr;
        list->tail = ptr;
    }
    ++list->size;
    return 0;
}
thread_control_block * list_thread_pop_front(thread_list *list)
{
    if (!list || list->size < 1)
        return NULL;

    thread_control_block *front = list->head;

    if (list->size == 1)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else {
        list->head = list->head->next;
    }

    --list->size;
    return front;
}
thread_control_block * list_thread_pop_back(thread_list *list)
{
    if (!list || list->size < 1)
        return NULL;

    thread_control_block *back = list->tail;

    if (list->size == 1)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else {
        thread_control_block *temp = list->head;

        // tcb before the last one
        while (temp->next != back)
            temp = temp->next;

        temp->next = NULL;
        list->tail = temp;
    }

    --list->size;
    return back;
}

thread_control_block * list_thread_remove(thread_list *list, thread_control_block *ptr)
{
    if (!list || !ptr)
        return NULL;

    if (ptr == list->head) return list_thread_pop_front(list);
    else if (ptr == list->tail) return list_thread_pop_back(list);

    thread_control_block *prev = NULL;
    for (thread_control_block *i = list->head; i != list->tail ; prev = i,  i = i->next)
    {
        if (i == ptr)
        {
            prev->next = prev->next->next;
            i->next = NULL;

            return i;
        }
    }
    --list->size;
    return NULL;
}