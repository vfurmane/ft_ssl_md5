#include "utils.h"

size_t ft_min_size(size_t a, size_t b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

size_t ft_max_size(size_t a, size_t b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}
