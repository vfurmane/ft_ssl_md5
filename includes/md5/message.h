#ifndef MD5_MESSAGE_H
#define MD5_MESSAGE_H

#include "stdlib.h"
#include "utils.h"

typedef struct md5_padded_buffer_s {
  size_t len;
  void *b;
} md5_padded_buffer_t;

typedef unsigned int md5_message_chunk_word_t;
typedef md5_message_chunk_word_t *md5_message_chunk_t;

typedef struct md5_message_t {
  unsigned int chunks;
  md5_message_chunk_t b;
} md5_message_t;

md5_padded_buffer_t reverse_buffer_endianness(md5_padded_buffer_t buffer);
md5_message_t allocate_message_from_string(const char *str);
void destroy_message(md5_message_t *message);

#endif
