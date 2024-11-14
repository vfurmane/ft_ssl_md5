#include "md5/hash.h"
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

void print_md5_hash(md5_hash_t hash) {
  print_32hex(hash.a);
  print_32hex(hash.b);
  print_32hex(hash.c);
  print_32hex(hash.d);
  write_stdout("\n", 1);
}

int main(void) {
  md5_message_t message = allocate_message_from_string("hello, world!");

  const md5_hash_t hash = md5_hash(message);

  print_md5_hash(hash);

  destroy_message(&message);

  return 0;
}
