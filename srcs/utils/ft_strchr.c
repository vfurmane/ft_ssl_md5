#include "utils.h"

const char *ft_strchr(const char *str, int chr) {
  int i;

  i = 0;
  while (str[i]) {
    if (str[i] == (unsigned char)chr)
      return ((char *)&str[i]);
    i++;
  }
  if ((unsigned char)chr == '\0')
    return ((const char *)&str[i]);
  return (NULL);
}
