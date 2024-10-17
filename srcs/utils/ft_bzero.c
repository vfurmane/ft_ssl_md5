#include "utils.h"

void *ft_bzero(void *str, size_t len) {
  size_t i;

  i = 0;
  while (i < len) {
    ((char *)str)[i] = 0;
    i++;
  }
  return (str);
}
