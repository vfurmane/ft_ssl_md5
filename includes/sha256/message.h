#ifndef SHA256_MESSAGE_H
#define SHA256_MESSAGE_H

#include "utils.h"

#include <limits.h>
#include <stdlib.h>

#define LENGTH_PADDING_BITS_NBR 64
#define BUFFER_BITS_NBR 512

typedef unsigned int sha256_message_chunk_word_t;
typedef sha256_message_chunk_word_t *sha256_message_chunk_t;

size_t get_required_sha256_bytes_nbr(size_t bits_len);
unsigned char *pad_sha256_chunk(unsigned char *buffer, size_t i, size_t len);

#endif
