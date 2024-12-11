#ifndef MD5_MESSAGE_H
#define MD5_MESSAGE_H

#include "stdlib.h"
#include "utils.h"

#include <limits.h>

#define LENGTH_PADDING_BITS_NBR 64
#define BUFFER_BITS_NBR 512

typedef unsigned int md5_message_chunk_word_t;
typedef md5_message_chunk_word_t *md5_message_chunk_t;

size_t get_required_bytes_nbr(size_t bits_len);
unsigned char *pad_chunk(unsigned char *buffer, size_t i, size_t len);

#endif
