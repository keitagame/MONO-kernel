// initrd.h みたいなヘッダに置いておくと便利
#pragma once
#include <stdint.h>

typedef struct {
    char c_magic[6];
    char c_ino[8];
    char c_mode[8];
    char c_uid[8];
    char c_gid[8];
    char c_nlink[8];
    char c_mtime[8];
    char c_filesize[8];
    char c_devmajor[8];
    char c_devminor[8];
    char c_rdevmajor[8];
    char c_rdevminor[8];
    char c_namesize[8];
    char c_check[8];
} __attribute__((packed)) cpio_newc_header;

typedef struct {
    uint8_t* data;
    uint32_t size;
} initrd_file_t;

static inline uint32_t hex_to_uint(const char* s, int len)
{
    uint32_t v = 0;
    for (int i = 0; i < len; i++) {
        v <<= 4;
        char c = s[i];
        if (c >= '0' && c <= '9') v |= (uint32_t)(c - '0');
        else if (c >= 'A' && c <= 'F') v |= (uint32_t)(c - 'A' + 10);
        else if (c >= 'a' && c <= 'f') v |= (uint32_t)(c - 'a' + 10);
    }
    return v;
}
