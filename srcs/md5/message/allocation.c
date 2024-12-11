#include "debug.h"
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

void append_padding_bits_to_buffer(
    unsigned char *b, size_t origin_message_len, size_t buffer_len,
    uint8_t should_add_first_byte
) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 1

  if (b == NULL) {
    return;
  }

  ft_bzero(b + origin_message_len, buffer_len - origin_message_len);
  if (should_add_first_byte) {
    b[origin_message_len] = 1 << 7;
  }

  PRINT("adding padding bits--v%s\n", "");
#ifdef DEBUG
  for (unsigned int i = 0; i < buffer_len - origin_message_len; ++i) {
    print_8bin(((char *)(b + origin_message_len))[i]);
    write_stdout(" ", 1);
  }
  write_stdout("\n", 1);
#endif
}

void append_length_to_buffer(
    unsigned char *b, size_t buffer_len, size_t msg_bits_len
) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 1

  if (b == NULL) {
    return;
  }

  const size_t bits_per_byte = CHAR_BIT;
  const size_t length_padding_size = LENGTH_PADDING_BITS_NBR / bits_per_byte;

  for (size_t i = 0; i < length_padding_size; ++i) {
    b[buffer_len - (length_padding_size - i)] = ((char *)(&msg_bits_len))[i];
  }

  PRINT("adding length bits--v%s\n", "");
#ifdef DEBUG
  for (unsigned int i = 0; i < LENGTH_PADDING_BITS_NBR / bits_per_byte; ++i) {
    print_8bin(b[buffer_len - (bits_per_byte - i)]);
    write_stdout(" ", 1);
  }
  write_stdout("\n", 1);
#endif
}

unsigned char *pad_chunk(unsigned char *buffer, size_t i, size_t len) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 1

  const size_t chunk_size = BUFFER_BITS_NBR / CHAR_BIT;
  if (i + chunk_size <= len) {
    return buffer;
  }

  const size_t origin_message_bits_len = len * 8;
  PRINT("buffer length (in bits)=%zu\n", origin_message_bits_len);

  append_padding_bits_to_buffer(
      buffer, len > i ? len - i : 0, chunk_size, i <= len
  );
  if (i > len ||
      chunk_size - (len - i) >= (LENGTH_PADDING_BITS_NBR / CHAR_BIT) + 1) {
    append_length_to_buffer(buffer, chunk_size, origin_message_bits_len);
  }

  return buffer;
}
