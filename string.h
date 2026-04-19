#pragma once
#include <stdint.h>

void* memcpy(void* dst, const void* src, uint32_t n);
int memcmp(const void* s1, const void* s2, uint32_t n);
int strcmp(const char* s1, const char* s2);
