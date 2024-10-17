#include "md5/message.h"
#include "stdlib.h"
#include "unistd.h"
#include "utils.h"

typedef struct md5_hash_s {
  md5_message_chunk_word_t a;
  md5_message_chunk_word_t b;
  md5_message_chunk_word_t c;
  md5_message_chunk_word_t d;
} md5_hash_t;

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

void print_md5_hash(md5_hash_t hash) {
  print_32hex(hash.a);
  print_32hex(hash.b);
  print_32hex(hash.c);
  print_32hex(hash.d);
  write_stdout("\n", 1);
}

md5_message_chunk_word_t
left_rotate(md5_message_chunk_word_t word, md5_message_chunk_word_t _bits) {
  const unsigned int word_size = 32;
  const md5_message_chunk_word_t bits = _bits % 32;

  const unsigned int word_bits = ((0x1 << bits) - 1) << (word_size - bits);
  const unsigned int rotating_chunk = (word & word_bits) >> (word_size - bits);

  return (word << bits) | rotating_chunk;
}

int main(void) {
  md5_hash_t hash = {
      .a = 0x67452301, .b = 0xefcdab89, .c = 0x98badcfe, .d = 0x10325476
  };
  md5_message_t message = allocate_message_from_string("hello, world!");

  print_md5_hash(hash);

  destroy_message(&message);

  return 0;
}
