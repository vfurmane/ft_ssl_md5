#ifndef UTILS_H
#define UTILS_H

#include "stdint.h"
#include "stdlib.h"

uint32_t ft_bswap_32(uint32_t x);

void *ft_bzero(void *str, size_t len);
void *ft_memchr(const void *str, int chr, size_t len);
void *ft_memcpy(void *dest, const void *src, size_t len);
size_t ft_min_size(size_t a, size_t b);
size_t ft_max_size(size_t a, size_t b);
int ft_strcmp(const char *str1, const char *str2);
size_t ft_strlen(const char *str);

#endif
