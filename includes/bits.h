#ifndef BITS_H
#define BITS_H

#include "md5/message.h"

#include <stdlib.h>

md5_message_chunk_word_t
left_rotate(md5_message_chunk_word_t word, md5_message_chunk_word_t _bits);
void *reverse_endianness(uint32_t *b, size_t len);

#endif
