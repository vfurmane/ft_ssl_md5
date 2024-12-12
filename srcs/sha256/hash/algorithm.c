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
  const sha256_message_chunk_word_t S1 = (right_rotate(hash.e, 6)) ^
                                         (right_rotate(hash.e, 11)) ^
                                         (right_rotate(hash.e, 25));
  const sha256_message_chunk_word_t ch =
      (hash.e & hash.f) ^ ((~hash.e) & hash.g);
  const sha256_message_chunk_word_t T1 =
      hash.h + S1 + ch + sha256_k_table[i] + message_chunk[i];

  const sha256_message_chunk_word_t S0 = (right_rotate(hash.a, 2)) ^
                                         (right_rotate(hash.a, 13)) ^
                                         (right_rotate(hash.a, 22));
  const sha256_message_chunk_word_t maj =
      (hash.a & hash.b) ^ (hash.a & hash.c) ^ (hash.b & hash.c);
  const sha256_message_chunk_word_t T2 = S0 + maj;

  const sha256_hash_t ret = {
      .h = hash.g,
      .g = hash.f,
      .f = hash.e,
      .e = hash.d + T1,
      .d = hash.c,
      .c = hash.b,
      .b = hash.a,
      .a = T1 + T2
  };
  return ret;
}

sha256_hash_t sha256_hash_chunk(
    sha256_hash_t base_hash, sha256_message_chunk_t message_chunk
) {
  sha256_hash_t hash = base_hash;
  sha256_message_chunk_word_t schedule[BUFFER_BITS_NBR / CHAR_BIT];

  ft_memcpy(schedule, message_chunk, sizeof(schedule));
#if __BYTE_ORDER == __LITTLE_ENDIAN
  buffer_bswap_32(schedule, sizeof(schedule));
#endif
  for (size_t i = 16; i < 64; ++i) {
    const sha256_message_chunk_word_t s0 =
        (right_rotate(schedule[i - 15], 7)) ^
        (right_rotate(schedule[i - 15], 18)) ^ (schedule[i - 15] >> 3);
    const sha256_message_chunk_word_t s1 = (right_rotate(schedule[i - 2], 17)) ^
                                           (right_rotate(schedule[i - 2], 19)) ^
                                           (schedule[i - 2] >> 10);
    schedule[i] = schedule[i - 16] + s0 + schedule[i - 7] + s1;
  }

  for (size_t j = 0; j < 64; ++j) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 3
    PRINT("j = %zu\n", j);
    hash = sha256_hash_chunk_round(hash, schedule, j);
    PRINT(
        "current hash: %#010x %#010x %#010x %#010x %#010x %#010x %#010x "
        "%#010x\n",
        hash.a, hash.b, hash.c, hash.d, hash.e, hash.f, hash.g, hash.h
    );
  }

  base_hash.a += hash.a;
  base_hash.b += hash.b;
  base_hash.c += hash.c;
  base_hash.d += hash.d;
  base_hash.e += hash.e;
  base_hash.f += hash.f;
  base_hash.g += hash.g;
  base_hash.h += hash.h;
  return base_hash;
}

sha256_hash_t sha256_hash_static_string(const char *str) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 0
  PRINT("Hashing the static buffer%s\n", "");

  const size_t chunk_size = BUFFER_BITS_NBR / CHAR_BIT;
  unsigned char buffer[chunk_size];

  sha256_hash_t base_hash = {
      .a = 0x6a09e667,
      .b = 0xbb67ae85,
      .c = 0x3c6ef372,
      .d = 0xa54ff53a,
      .e = 0x510e527f,
      .f = 0x9b05688c,
      .g = 0x1f83d9ab,
      .h = 0x5be0cd19,
  };

  const size_t len = ft_strlen(str);
#undef CURRENT_INDENT
#define CURRENT_INDENT 1
  PRINT("original message length: %zu\n", len);
  const size_t total_message_len =
      get_required_sha256_bytes_nbr(len * CHAR_BIT);
  PRINT("final message length: %zu\n", total_message_len);
  for (size_t i = 0; i < total_message_len; i += chunk_size) {
    PRINT("ROUND %zu\n", (i / chunk_size) + 1);
#undef CURRENT_INDENT
#define CURRENT_INDENT 2
    PRINT(
        "current hash: %#010x %#010x %#010x %#010x %#010x %#010x %#010x "
        "%#010x\n",
        base_hash.a, base_hash.b, base_hash.c, base_hash.d, base_hash.e,
        base_hash.f, base_hash.g, base_hash.h
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
      "hash done: %#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x "
      "(endianness might be wrong "
      "here)\n",
      base_hash.a, base_hash.b, base_hash.c, base_hash.d, base_hash.e,
      base_hash.f, base_hash.g, base_hash.h
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
      .a = 0x6a09e667,
      .b = 0xbb67ae85,
      .c = 0x3c6ef372,
      .d = 0xa54ff53a,
      .e = 0x510e527f,
      .f = 0x9b05688c,
      .g = 0x1f83d9ab,
      .h = 0x5be0cd19,
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
        "current hash: %#010x %#010x %#010x %#010x %#010x %#010x %#010x "
        "%#010x\n",
        base_hash.a, base_hash.b, base_hash.c, base_hash.d, base_hash.e,
        base_hash.f, base_hash.g, base_hash.h
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
      "hash done: %#010x %#010x %#010x %#010x %#010x %#010x %#010x %#010x "
      "(endianness might be wrong "
      "here)\n",
      base_hash.a, base_hash.b, base_hash.c, base_hash.d, base_hash.e,
      base_hash.f, base_hash.g, base_hash.h
  );

  result.some = 1;
  result.hash = base_hash;
  return result;
}
