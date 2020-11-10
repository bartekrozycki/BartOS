#pragma once

struct stack_t 
{
    struct stack_t *next;
    void * address;
};