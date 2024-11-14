#include "bits.h"

void *reverse_endianness(uint32_t *b, size_t len) {
  for (size_t i = 0; i < len / sizeof(uint32_t); ++i) {
    b[i] = ft_bswap_32(b[i]);
  }

  return b;
}
