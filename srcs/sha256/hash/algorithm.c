#include "debug.h"
#include "print.h"
#include "sha256/hash.h"

sha256_message_chunk_word_t sha256_hash_round_f(
    sha256_hash_t hash, sha256_message_chunk_word_t message_chunk_word,
    sha256_message_chunk_word_t k, sha256_message_chunk_word_t f
) {
  return f + hash.a + k + message_chunk_word;
}

sha256_message_chunk_word_t sha256_hash_round1(
    sha256_hash_t hash, sha256_message_chunk_t message_chunk, unsigned int i
) {
  const sha256_message_chunk_word_t f =
      (hash.b & hash.c) | ((~hash.b) & hash.d);
  const unsigned int g = i;
  return sha256_hash_round_f(hash, message_chunk[g], sha256_k_table[i], f);
}

sha256_message_chunk_word_t sha256_hash_round2(
    sha256_hash_t hash, sha256_message_chunk_t message_chunk, unsigned int i
) {
  const sha256_message_chunk_word_t f =
      (hash.d & hash.b) | ((~hash.d) & hash.c);
  const unsigned int g = (5 * i + 1) % 16;
  return sha256_hash_round_f(hash, message_chunk[g], sha256_k_table[i], f);
}

sha256_message_chunk_word_t sha256_hash_round3(
    sha256_hash_t hash, sha256_message_chunk_t message_chunk, unsigned int i
) {
  const sha256_message_chunk_word_t f = hash.b ^ hash.c ^ hash.d;
  const unsigned int g = (3 * i + 5) % 16;
  return sha256_hash_round_f(hash, message_chunk[g], sha256_k_table[i], f);
}

/**
 * @brief Performs the fourth round of the SHA256 hashing algorithm.
 *
 * @param hash The current SHA256 hash state, represented as an `sha256_hash_t`
 * structure.
 * @param message_chunk A single 512-bit message chunk represented as an
 * `sha256_message_chunk_t`.
 * @param i The current round index. Must be less than 64.
 * @return The result of the fourth SHA256 round as an
 * `sha256_message_chunk_word_t`.
 *
 * @note The parameter `i` must satisfy `i < 64` to avoid undefined behavior, as
 * it is used to access elements in the `sha256_k_table` array, which has a size
 * of 64.
 */
sha256_message_chunk_word_t sha256_hash_round4(
    sha256_hash_t hash, sha256_message_chunk_t message_chunk, unsigned int i
) {
  if (i >= 64) {
    return 0;
  }
  const sha256_message_chunk_word_t f = hash.c ^ (hash.b | (~hash.d));
  const unsigned int g = (7 * i) % 16;
  return sha256_hash_round_f(hash, message_chunk[g], sha256_k_table[i], f);
}

sha256_hash_t sha256_hash_chunk_round(
    sha256_hash_t hash, sha256_message_chunk_t message_chunk, unsigned int i
) {
  const sha256_message_chunk_word_t f =
      i < 16   ? sha256_hash_round1(hash, message_chunk, i)
      : i < 32 ? sha256_hash_round2(hash, message_chunk, i)
      : i < 48 ? sha256_hash_round3(hash, message_chunk, i)
               : sha256_hash_round4(hash, message_chunk, i);
  const sha256_message_chunk_word_t new_b =
      hash.b + left_rotate(f, sha256_per_round_shifts[i]);

  const sha256_hash_t ret = {
      .a = hash.d,
      .d = hash.c,
      .c = hash.b,
      .b = new_b,
  };
  return ret;
}

sha256_hash_t sha256_hash_chunk(
    sha256_hash_t base_hash, sha256_message_chunk_t message_chunk
) {
  sha256_hash_t hash = base_hash;

  for (size_t j = 0; j < 64; ++j) {
    hash = sha256_hash_chunk_round(hash, message_chunk, j);
  }

  base_hash.a += hash.a;
  base_hash.b += hash.b;
  base_hash.c += hash.c;
  base_hash.d += hash.d;
  return base_hash;
}

sha256_hash_t sha256_hash_static_string(const char *str) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 0
  PRINT("Hashing the static buffer%s\n", "");

  const size_t chunk_size = BUFFER_BITS_NBR / CHAR_BIT;
  unsigned char buffer[chunk_size];

  sha256_hash_t base_hash = {
      .a = 0x67452301, .b = 0xefcdab89, .c = 0x98badcfe, .d = 0x10325476
  };

  const size_t len = ft_strlen(str);
#undef CURRENT_INDENT
#define CURRENT_INDENT 1
  PRINT("original message length: %zu\n", len);
  const size_t total_message_len = get_required_sha256_bytes_nbr(len * CHAR_BIT);
  PRINT("final message length: %zu\n", total_message_len);
  for (size_t i = 0; i < total_message_len; i += chunk_size) {
    PRINT("ROUND %zu\n", (i / chunk_size) + 1);
#undef CURRENT_INDENT
#define CURRENT_INDENT 2
    PRINT(
        "current hash: %#010x %#010x %#010x %#010x\n", base_hash.a, base_hash.b,
        base_hash.c, base_hash.d
    );
    const char *str_start = str + i;
    const size_t size = ft_max_size(ft_min_size(chunk_size, len - i), 0);
    ft_memcpy(buffer, str_start, size);

    pad_sha256_chunk(buffer, i, len);

    PRINT("static buffer --v%s\n", "");

#ifdef DEBUG
    for (unsigned int j = 0; j < chunk_size; ++j) {
      print_8bin(((char *)buffer)[j]);
      write_stdout(" ", 1);
    }
    write_stdout("\n", 1);
#endif

    base_hash = sha256_hash_chunk(base_hash, (sha256_message_chunk_t)buffer);
  }

#undef CURRENT_INDENT
#define CURRENT_INDENT 1
  PRINT(
      "hash done: %#010x %#010x %#010x %#010x (endianness might be wrong "
      "here)\n",
      base_hash.a, base_hash.b, base_hash.c, base_hash.d
  );

  return base_hash;
}

maybe_sha256_hash_t
sha256_hash_fd(int fd, config_t config, uint8_t should_print) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 0
  PRINT("Hashing the fd %d\n", fd);

  maybe_sha256_hash_t result = {.some = 0};
  const size_t chunk_size = BUFFER_BITS_NBR / CHAR_BIT;
  unsigned char buffer[chunk_size];

  size_t total_len = 0;
  size_t i = 0;
  ssize_t ret = 0;
  sha256_hash_t base_hash = {
      .a = 0x67452301, .b = 0xefcdab89, .c = 0x98badcfe, .d = 0x10325476
  };

  if (should_print && !config.quiet) {
    putstr_stdout("(\"");
  }
  do {
#undef CURRENT_INDENT
#define CURRENT_INDENT 1
    PRINT("ROUND %zu\n", (i / chunk_size) + 1);
#undef CURRENT_INDENT
#define CURRENT_INDENT 2
    PRINT(
        "current hash: %#010x %#010x %#010x %#010x\n", base_hash.a, base_hash.b,
        base_hash.c, base_hash.d
    );

    ret = read(fd, buffer, chunk_size);
    if (ret < 0) {
      PRINT("error on read, exiting%s\n", "");
      return result;
    }
    PRINT("reading %zd characters\n", ret);

    if (should_print) {
      write_stdout_skip_newlines(buffer, ret);
    }
    total_len += ret;
    pad_sha256_chunk(buffer, i, total_len);

    PRINT("static buffer --v%s\n", "");

#ifdef DEBUG
    for (unsigned int j = 0; j < chunk_size; ++j) {
      print_8bin(((char *)buffer)[j]);
      write_stdout(" ", 1);
    }
    write_stdout("\n", 1);
#endif

    base_hash = sha256_hash_chunk(base_hash, (sha256_message_chunk_t)buffer);

    i += chunk_size;
  } while ((size_t)ret >= (chunk_size - (LENGTH_PADDING_BITS_NBR / CHAR_BIT)));
  if (should_print) {
    if (config.quiet) {
      putstr_stdout("\n");
    } else {
      putstr_stdout("\")= ");
    }
  }
#undef CURRENT_INDENT
#define CURRENT_INDENT 1
  PRINT(
      "hash done: %#010x %#010x %#010x %#010x (endianness might be wrong "
      "here)\n",
      base_hash.a, base_hash.b, base_hash.c, base_hash.d
  );

  result.some = 1;
  result.hash = base_hash;
  return result;
}
