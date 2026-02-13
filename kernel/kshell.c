#include <kernel/kshell.h>
#include <kernel/syscall.h>

[[gnu::aligned(16)]] uint8_t kshell_thread_stack[KSHELL_STACK_SIZE];

void exit(int exit_code)
{
    syscall(SYS_EXIT, exit_code, 0, 0);
}

pid_t exec(const char* path)
{
    return syscall(SYS_EXEC, (uint32_t) path, 0, 0);
}

void kshell_thread()
{   
    exec("/bin/sh.bin");
    
    exit(0);
}

