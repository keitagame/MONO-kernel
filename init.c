// init.c
//extern int sys_write(int fd, const char* buf, int len);

void _start(void) {
    const char msg[] = "hello from /init\n";
    //sys_write(1, msg, sizeof(msg) - 1);

    for (;;)
        ;
}
