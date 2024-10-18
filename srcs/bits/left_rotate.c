#include "bits.h"

md5_message_chunk_word_t
left_rotate(md5_message_chunk_word_t word, md5_message_chunk_word_t _bits) {
  const unsigned int word_size = 32;
  const md5_message_chunk_word_t bits = _bits % word_size;

  const unsigned int word_bits = ((0x1 << bits) - 1) << (word_size - bits);
  const unsigned int rotating_chunk = (word & word_bits) >> (word_size - bits);

  return (word << bits) | rotating_chunk;
}
