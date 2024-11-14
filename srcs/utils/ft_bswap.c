#include "utils.h"

uint32_t ft_bswap_32(uint32_t x) {
  char *char_x = ((char *)&x);
  char tmp_buf[4] = {char_x[0], char_x[1], char_x[2], char_x[3]};

  char_x[0] = tmp_buf[3];
  char_x[1] = tmp_buf[2];
  char_x[2] = tmp_buf[1];
  char_x[3] = tmp_buf[0];

  return x;
}
