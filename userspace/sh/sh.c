#include <stdlib.h>
#include <tty.h>

int tty0;

#define LINE_SIZE 24

char line_buffer[LINE_SIZE];

char* argv[4];

int parse_args(char* buffer, int size)
{
    int argc = 0;
    char* current_arg = buffer;
    for (int i = 0; i < size; i++) {
        if (buffer[i] == ' ') {
            buffer[i] = '\0';
            argv[argc++] = current_arg;
            current_arg = &buffer[i + 1];
        }
    }
    argv[argc++] = current_arg;

    return argc;
}


void main()
{
    tty0 = open("/dev/tty0", 0);
    
    while(1) {
        memset(line_buffer, 0, LINE_SIZE);
        write(tty0, "# ", 2);

        int count = read(tty0, line_buffer, LINE_SIZE);

        int argc = parse_args(line_buffer, count);

        if (argc == 0) {
            continue;
        }

        if (strncmp(argv[0], "exit", LINE_SIZE) == 0) {
            break;
        } else if (strncmp(argv[0], "clear", LINE_SIZE) == 0) {
            ioctl(tty0, TTY_IOCTL_CLEAR, NULL);
        } else {
            pid_t new = exec(line_buffer);
            wait(new);
        }
        
    }
    exit(0);
}


