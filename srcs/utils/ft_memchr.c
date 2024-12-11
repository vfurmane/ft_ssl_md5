#include "utils.h"

void *ft_memchr(const void *str, int chr, size_t len) {
  size_t i;

  i = 0;
  while (i < len) {
    if (((unsigned char *)str)[i] == (unsigned char)chr)
      return (&((unsigned char *)str)[i]);
    i++;
  }
  return (NULL);
}
