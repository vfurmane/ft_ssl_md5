#include "utils.h"

void *ft_memcpy(void *dest, const void *src, size_t len) {
  int i;

  if (dest > src) {
    i = (int)len - 1;
    while (i >= 0) {
      ((char *)dest)[i] = ((char *)src)[i];
      i--;
    }
  } else {
    i = 0;
    while (i < (int)len) {
      ((char *)dest)[i] = ((char *)src)[i];
      i++;
    }
  }
  return (dest);
}
