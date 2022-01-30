#include "stdlib.h"
#include "stdio.h"
#include <fcntl.h>
#include "unistd.h"
#include "string.h"
#include "stdbool.h"

#define MAX_READ 1024
#define NAME_LEN 32

void main(int argc, char *argv[]) {
    char user_buf[MAX_READ];
    char dev_name[NAME_LEN];
    int dev = 0;
    int written = 0;
    bool to_write = false;
    char write_buf[128] = "";
    if (argc > 1) {
        dev = atoi((char*)argv[1]);
        if (argc > 2) {
            strcpy(write_buf, argv[2]);
            to_write = true;
        }
    }
    sprintf(dev_name, "/dev/chardev-%d", dev);
    int fd = open(dev_name, O_RDWR);
    if (fd < 0) {
        printf("Unable to open file\n");
        exit(1);
    }
    if (to_write) {
        written = write(fd, write_buf, strlen(write_buf));
    }
    int sz = read(fd, user_buf, MAX_READ);
    user_buf[sz] = '\0';
    printf("Got:%s, Read:%d, Written:%d\n", user_buf, sz, written);
}
