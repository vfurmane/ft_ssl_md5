#include "md5/message.h"

md5_padded_buffer_t allocate_buffer_from_string_with_padding(const char *str) {
  const size_t len = ft_strlen(str);
  const size_t bits_len = len * 8;
  const size_t remainder_chunks_nbr = (bits_len + 1 + 64) % 512;
  const size_t full_chunks_nbr =
      (bits_len + 1 + 64) / 512 + (remainder_chunks_nbr > 0 ? 1 : 0);
  const size_t new_len = full_chunks_nbr * 512 / 8;

  unsigned char *b = malloc(new_len);
  ft_memcpy(b, str, sizeof(*b) * len);
  ft_bzero(b + len, new_len - len);
  b[len] = 128;
  b[new_len - 8] = ((char *)(&bits_len))[0];
  b[new_len - 7] = ((char *)(&bits_len))[1];
  b[new_len - 6] = ((char *)(&bits_len))[2];
  b[new_len - 5] = ((char *)(&bits_len))[3];
  b[new_len - 4] = ((char *)(&bits_len))[4];
  b[new_len - 3] = ((char *)(&bits_len))[5];
  b[new_len - 2] = ((char *)(&bits_len))[6];
  b[new_len - 1] = ((char *)(&bits_len))[7];

  md5_padded_buffer_t buffer = {.len = new_len * 8, .b = b};

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
