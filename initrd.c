#include "string.h" // strcmp, memcmp
#include "initrd.h"
static inline uint32_t align4(uint32_t x) {
    return (x + 3) & ~3u;
}

initrd_file_t initrd_find(uint8_t* start, uint32_t size, const char* target)
{
    uint8_t* p = start;
    uint8_t* end = start + size;

    while (p + sizeof(cpio_newc_header) <= end) {
        cpio_newc_header* h = (cpio_newc_header*)p;

        if (memcmp(h->c_magic, "070701", 6) != 0) {
            break; // 不正 or 終了
        }

        uint32_t namesize = hex_to_uint(h->c_namesize, 8);
        uint32_t filesize = hex_to_uint(h->c_filesize, 8);

        p += sizeof(cpio_newc_header);
        if (p + namesize > end) break;

        char* name = (char*)p;
        p += align4(namesize);

        if (p + filesize > end) break;

        uint8_t* filedata = p;
        p += align4(filesize);

        if (strcmp(name, "TRAILER!!!") == 0) {
            break;
        }

        if (strcmp(name, target) == 0) {
            initrd_file_t f = { filedata, filesize };
            return f;
        }
    }

    initrd_file_t none = { 0, 0 };
    return none;
}
