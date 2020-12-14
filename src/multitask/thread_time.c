#include "threads.h"
#include "pit.h"
#include "print.h"

u32 thread_timer_last_count = 0;

void update_time_used(void) {
    u32 current_count = read_counter(); // 10E-3 seconds
    u32 elapsed = current_count - thread_timer_last_count;
    thread_timer_last_count = current_count;
//    current_thread_TCB->time_used += elapsed;
}