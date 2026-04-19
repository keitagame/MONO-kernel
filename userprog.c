// userprog.c (ユーザ空間にリンクされる)
static inline int write(int fd, const char *buf, int len) {
    int ret;
    asm volatile(
        "int $0x80"
        : "=a"(ret)
        : "a"(1), "b"(fd), "c"(buf), "d"(len)
    );
    return ret;
}

static inline void exit(int code) {
    asm volatile("int $0x80" :: "a"(2), "b"(code));
}

void user_main() {
    while (1) {
        // 何もしない or 適当にループ
    }
    //const char msg[] = "Hello from user mode!\n";
    //write(1, msg, sizeof(msg)-1);
    //exit(0);
}
