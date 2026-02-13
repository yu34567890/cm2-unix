#include <stdlib.h>
#include <vfs.h>

struct stat dirbuff[8];

void main()
{
    int fd = open("/dev/tty0", 0);
    
    int dirfd = open("/", 0);
    //int count = readdir(dirfd, dirbuff, 8);
    write(fd, "Hello world!\nthe userspace is still in development!\n", 52);
    /*
    for (int i = 0; i < count; i++) {
        char buff[] = "            \n";
        struct stat* dir = &dirbuff[i];
        strncpy(buff, dir->name, FS_INAME_LEN);
        write(fd, buff, 13);
    }*/

    
    exit(0);
}


