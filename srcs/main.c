#include "md5/hash.h"
#include "print.h"

int main(void) {
  // "hello, world!", with md5 padding
  uint8_t buffer[BUFFER_BITS_NBR / CHAR_BIT] = {104, 101, 108, 108, 111, 44, 32,
                                                119, 111, 114, 108, 100, 33};
  md5_message_t message = {.chunks = 1, .b = (uint32_t *)pad_chunk(buffer, 13)};
  md5_hash_t base_hash = {
      .a = 0x67452301, .b = 0xefcdab89, .c = 0x98badcfe, .d = 0x10325476
  };
  const md5_hash_t hash = md5_hash(message, base_hash);

  print_md5_hash(hash);

  return 0;
}
