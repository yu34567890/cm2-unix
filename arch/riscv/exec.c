#include <kernel/exec.h>
#include <kernel/syscall.h>
#include <lib/alloc.h>
#include <lib/kprint.h>
#include <stddef.h>

#include <arch/riscv/cm2exef.h>

//pid_t exec(const char *path, const char* argv[])
void sys_exec()
{
    walk_path_init(&current_process->exec_state.walker, (const char*) syscall_args[1]);
    current_process->exec_state.descriptor.file = NULL;
    
    block_proc();
}


void sys_exec_update(struct proc* process)
{
    struct fd* f = &process->exec_state.descriptor;
    fs_read_t* rstate = &process->exec_state.fs;

    if (f->file) {
        int8_t stat = rstate->fs->fops->read(rstate);
        
        if (stat < 0) {
            proc_resume(process, -1);
            return;
        } else if (stat == 1) {
            uint32_t program_base = (uint32_t) rstate->buffer;
            struct cm2exef_header* hdr = rstate->buffer;
            //kprintf("base: %x\n", program_base + sizeof(struct cm2exef_header));
            
            struct proc* new = proc_create(program_base + sizeof(struct cm2exef_header), program_base + hdr->initial_sp);
            new->program_base = program_base;
            new->program_size = hdr->initial_sp;
            proc_resume(process, new->pid);
            return;
        }
        return;
    }

	int8_t rt = walk_path(&process->exec_state.walker);
	if (rt < 0) {
		proc_resume(process, -1);
	} else if (rt == 1) {
        struct inode* file = process->exec_state.walker.fs_state.dir;

        void *program = malloc(file->size + 2048);
		if (!program) {
			proc_resume(process, -1); //out of memory condition
            return;
		}
        
        f->file = file;
        f->flags = 0;
        f->offset = 0; //start at the begining of the file
        //set the read state
        set_read_state(rstate, f, program, file->size);
	}
}


