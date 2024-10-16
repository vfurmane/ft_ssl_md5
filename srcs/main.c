#include "unistd.h"

ssize_t write_stdout(const void *buf, size_t n) {
  return write(STDOUT_FILENO, buf, n);
}

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

void print_md5_output(
    unsigned int a, unsigned int b, unsigned int c, unsigned int d
) {
  print_32hex(a);
  print_32hex(b);
  print_32hex(c);
  print_32hex(d);
  write_stdout("\n", 1);
}

int main(void) {
  const unsigned int nbr = 0xeeeeeeee;

  print_md5_output(nbr, 1, 2, 3);

  return 0;
}
