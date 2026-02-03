#include <lib/stdlib.h>
#include <lib/hex.h>
#include <kernel/tty.h>
#include <kernel/proc.h>

#include <stdint.h>

void panic(void) {
    uint32_t pc;
    __asm__ volatile (
        "auipc %0, 0"
        : "=r" (pc)
    );
    
    freestanding_tty_puts("KERNEL PANIC:\nPC $");
    freestanding_tty_puts(u32_to_hex(pc));
    freestanding_tty_puts("\nPID: $");
    freestanding_tty_puts(u32_to_hex(current_process->pid));
    freestanding_tty_puts("\nSYSCALL: $");
    freestanding_tty_puts(u32_to_hex(current_process->syscall_operation));
    freestanding_tty_puts("\nRA: $");
    freestanding_tty_puts(u32_to_hex(current_process->return_address));
    
    while (1);
}
