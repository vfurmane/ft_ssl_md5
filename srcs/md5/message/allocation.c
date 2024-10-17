#include "md5/message.h"

md5_padded_buffer_t allocate_buffer_from_string_with_padding(const char *str) {
  const size_t len = ft_strlen(str);
  const size_t remainder_chunks_nbr = (len + 1 + 64) % 512;
  const size_t full_chunks_nbr = len / 512 + (remainder_chunks_nbr > 0 ? 1 : 0);
  const size_t new_len = full_chunks_nbr * 512 / 8;

  char *b = malloc(new_len);
  ft_memcpy(b, str, sizeof(*b) * len);
  ft_bzero(b + len, new_len - len);
  b[len] = 1;
  (((size_t *)(b))[(new_len - 8) / 8]) = len;

  const md5_padded_buffer_t buffer = {.len = new_len, .b = b};

  return buffer;
}

md5_message_t allocate_message_from_string(const char *str) {
  const md5_padded_buffer_t buffer =
      allocate_buffer_from_string_with_padding(str);
  const md5_message_t message = {.chunks = buffer.len / 512, .b = buffer.b};

  return message;
}

void destroy_message(md5_message_t *message) {
  free(message->b);
}
