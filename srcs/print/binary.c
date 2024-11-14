#include "print.h"

void print_8bin(char nbr) {
  const unsigned int buffer_size = 8;

  char buffer[buffer_size];
  for (unsigned int i = 0; i < buffer_size; ++i) {
    buffer[i] = ((1 << (buffer_size - i - 1)) & nbr) == 0 ? '0' : '1';
  }

  write_stdout(buffer, buffer_size);
}
