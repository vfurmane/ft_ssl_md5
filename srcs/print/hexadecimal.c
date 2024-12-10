#include "print.h"

void print_32hex(unsigned int nbr) {
  const unsigned int buffer_size = 8;

  char buffer[buffer_size];
  const char set[] = "0123456789abcdef";

  for (unsigned int i = 0; i < buffer_size; ++i) {
    const unsigned int chunk_bits = 0xf << ((buffer_size - i - 1) * 4);
    const unsigned int chunk =
        (nbr & chunk_bits) >> ((buffer_size - i - 1) * 4);
    buffer[i] = set[chunk];
  }

  write_stdout(buffer, buffer_size);
}
