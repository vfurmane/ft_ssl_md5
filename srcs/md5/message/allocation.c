#include "md5/message.h"

size_t get_required_bytes_nbr(size_t bits_len) {
  const size_t remainder_chunks_nbr =
      (bits_len + 1 + LENGTH_PADDING_BITS_NBR) % BUFFER_BITS_NBR;
  const size_t full_chunks_nbr =
      (bits_len + 1 + LENGTH_PADDING_BITS_NBR) / BUFFER_BITS_NBR +
      (remainder_chunks_nbr > 0 ? 1 : 0);
  const size_t required_bytes_nbr = full_chunks_nbr * BUFFER_BITS_NBR / 8;

  return required_bytes_nbr;
}

void copy_original_message_into_buffer(
    unsigned char *b, const char *str, size_t str_len
) {
  ft_memcpy(b, str, sizeof(*b) * str_len);
}

void append_padding_bits_to_buffer(
    unsigned char *b, size_t origin_message_len, size_t buffer_len
) {
  if (b == NULL) {
    return;
  }

  ft_bzero(b + origin_message_len, buffer_len - origin_message_len);
  b[origin_message_len] = 1 << 7;
}

void append_length_to_buffer(
    unsigned char *b, size_t buffer_len, size_t msg_bits_len
) {
  if (b == NULL) {
    return;
  }

  const size_t bits_per_byte = CHAR_BIT;

  for (size_t i = 0; i < bits_per_byte; ++i) {
    b[buffer_len - bits_per_byte - i] = ((char *)(&msg_bits_len))[i];
  }
}

/**
 * @brief Allocates a buffer with padding for an MD5 hash computation from a
 * given string.
 *
 * This function creates a padded buffer suitable for MD5 processing by:
 * - Copying the original message into the buffer.
 * - Appending padding bits to satisfy MD5 block requirements.
 * - Adding the original message length (in bits) at the end of the buffer.
 *
 * @param str The input string to be converted into a padded buffer. Must be a
 * valid null-terminated string.
 * @return A structure of type `md5_padded_buffer_t` containing:
 *         - `.b`: A pointer to the padded buffer.
 *         - `.len`: The total length of the padded buffer in bits.
 *
 * @note The caller is responsible for freeing the memory allocated for the
 * buffer (`.b` field). You may use the `destroy_message` function to free the
 * buffer safely and cleanly.
 *
 * If memory allocation fails, the function returns a structure with
 * `.b = NULL` and `.len = 0`. Always check the `.b` field of the returned
 * structure to ensure it is not NULL before proceeding.
 */
md5_padded_buffer_t allocate_buffer_from_string_with_padding(const char *str) {
  const size_t origin_message_len = ft_strlen(str);
  const size_t origin_message_bits_len = origin_message_len * 8;
  const size_t buffer_len = get_required_bytes_nbr(origin_message_bits_len);

  unsigned char *b = malloc(buffer_len);
  if (b == NULL) {
    const md5_padded_buffer_t ret = {.len = 0, .b = NULL};
    return ret;
  }

  const size_t bits_per_byte = CHAR_BIT;

  copy_original_message_into_buffer(b, str, origin_message_len);
  append_padding_bits_to_buffer(b, origin_message_len, buffer_len);
  append_length_to_buffer(b, buffer_len, origin_message_bits_len);

  md5_padded_buffer_t buffer = {.len = buffer_len * bits_per_byte, .b = b};

  return buffer;
}

/**
 * @brief Allocates an MD5 message structure from a given string.
 *
 * This function converts a string into an `md5_message_t` structure suitable
 * for MD5 processing.
 *
 * @param str The input string to be converted into an MD5 message. Must be a
 * valid null-terminated string.
 * @return A structure of type `md5_message_t` containing:
 *         - `.b`: A pointer to the padded buffer.
 *         - `.chunks`: The number of 512-bit chunks in the padded buffer.
 *
 * @note The caller is responsible for freeing the memory allocated for the
 * buffer (`.b` field). You may use the `destroy_message` function to free the
 * buffer safely and cleanly.
 *
 * If memory allocation fails during buffer creation, the function returns a
 * structure with `.b = NULL` and `.chunks = 0`. Always check the `.b` field of
 * the returned structure to ensure it is not NULL before proceeding.
 */
md5_message_t allocate_message_from_string(const char *str) {
  const md5_padded_buffer_t buffer =
      allocate_buffer_from_string_with_padding(str);
  if (buffer.b == NULL) {
    const md5_message_t ret = {.chunks = 0, .b = NULL};
    return ret;
  }

  const md5_message_t message = {
      .chunks = buffer.len / BUFFER_BITS_NBR, .b = buffer.b
  };

  return message;
}

void destroy_message(md5_message_t *message) {
  free(message->b);
}
