#ifndef UTILS_H
#define UTILS_H

#include "stdlib.h"
#include "stdint.h"

uint32_t ft_bswap_32(uint32_t x);

void *ft_bzero(void *str, size_t len);
void *ft_memcpy(void *dest, const void *src, size_t len);
size_t ft_strlen(const char *str);

#endif
