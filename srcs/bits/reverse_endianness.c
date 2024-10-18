#include "bits.h"

void *reverse_endianness(void *b, size_t len) {
  for (size_t i = 0; i < (len / 4); i += 4) {
    char *char_b = ((char *)b) + i;
    char tmp_buf[4] = {char_b[0], char_b[1], char_b[2], char_b[3]};
    char_b[0] = tmp_buf[3];
    char_b[1] = tmp_buf[2];
    char_b[2] = tmp_buf[1];
    char_b[3] = tmp_buf[0];
  }

  return b;
}
