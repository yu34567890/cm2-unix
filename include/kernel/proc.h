#pragma once
#include <stdint.h>

//this should be made cross platform but i dont care anymore
struct proc {
    uint32_t return_adres;
    uint32_t user_sp;
    uint32_t saved_regs[12];
    uint32_t return_value;
    enum : uint8_t { BLOCKED, READY } state;
};

#define MAX_PROCESSES 4
#define MAX_PROCESSES_MSK (MAX_PROCESSES - 1)
extern struct proc process_table[MAX_PROCESSES];

extern struct proc* current_process;

extern uint32_t kernel_sp;

int proc_enqueue(struct proc* process);
struct proc* proc_dequeue();
void proc_init();
struct proc* proc_create(uint32_t entry_point, uint32_t stack_pointer);
void proc_delete(struct proc* process); 
void proc_update();



