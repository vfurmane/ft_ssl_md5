#include "debug.h"
#include "md5/hash.h"

md5_message_chunk_word_t md5_hash_round_f(
    md5_hash_t hash, md5_message_chunk_word_t message_chunk_word,
    md5_message_chunk_word_t k, md5_message_chunk_word_t f
) {
  return f + hash.a + k + message_chunk_word;
}

md5_message_chunk_word_t md5_hash_round1(
    md5_hash_t hash, md5_message_chunk_t message_chunk, unsigned int i
) {
  const md5_message_chunk_word_t f = (hash.b & hash.c) | ((~hash.b) & hash.d);
  const unsigned int g = i;
  return md5_hash_round_f(hash, message_chunk[g], k_table[i], f);
}

md5_message_chunk_word_t md5_hash_round2(
    md5_hash_t hash, md5_message_chunk_t message_chunk, unsigned int i
) {
  const md5_message_chunk_word_t f = (hash.d & hash.b) | ((~hash.d) & hash.c);
  const unsigned int g = (5 * i + 1) % 16;
  return md5_hash_round_f(hash, message_chunk[g], k_table[i], f);
}

md5_message_chunk_word_t md5_hash_round3(
    md5_hash_t hash, md5_message_chunk_t message_chunk, unsigned int i
) {
  const md5_message_chunk_word_t f = hash.b ^ hash.c ^ hash.d;
  const unsigned int g = (3 * i + 5) % 16;
  return md5_hash_round_f(hash, message_chunk[g], k_table[i], f);
}

md5_message_chunk_word_t md5_hash_round4(
    md5_hash_t hash, md5_message_chunk_t message_chunk, unsigned int i
) {
  const md5_message_chunk_word_t f = hash.c ^ (hash.b | (~hash.d));
  const unsigned int g = (7 * i) % 16;
  return md5_hash_round_f(hash, message_chunk[g], k_table[i], f);
}

md5_hash_t md5_hash_chunk_round(
    md5_hash_t hash, md5_message_chunk_t message_chunk, unsigned int i
) {
  const md5_message_chunk_word_t f =
      i < 16   ? md5_hash_round1(hash, message_chunk, i)
      : i < 32 ? md5_hash_round2(hash, message_chunk, i)
      : i < 48 ? md5_hash_round3(hash, message_chunk, i)
               : md5_hash_round4(hash, message_chunk, i);
  const md5_message_chunk_word_t new_b =
      hash.b + left_rotate(f, per_round_shifts[i]);

  const md5_hash_t ret = {
      .a = hash.d,
      .d = hash.c,
      .c = hash.b,
      .b = new_b,
  };
  return ret;
}

md5_hash_t
md5_hash_chunk(md5_hash_t base_hash, md5_message_chunk_t message_chunk) {
  md5_hash_t hash = base_hash;

  for (size_t j = 0; j < 64; ++j) {
    hash = md5_hash_chunk_round(hash, message_chunk, j);
  }

  base_hash.a += hash.a;
  base_hash.b += hash.b;
  base_hash.c += hash.c;
  base_hash.d += hash.d;
  return base_hash;
}

md5_hash_t md5_hash(md5_message_t message, md5_hash_t base_hash) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 0
  PRINT("Hashing the buffer%s\n", "");
  for (size_t i = 0; i < message.chunks; ++i) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 1
    PRINT("ROUND %zu\n", i + 1);
#undef CURRENT_INDENT
#define CURRENT_INDENT 2
    PRINT(
        "current hash: %#010x %#010x %#010x %#010x\n", base_hash.a, base_hash.b,
        base_hash.c, base_hash.d
    );
    md5_message_chunk_t message_chunk = message.b + i * 16;
    base_hash = md5_hash_chunk(base_hash, message_chunk);
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

md5_hash_t md5_hash_static_string(const char *str) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 0
  PRINT("Hashing the static buffer%s\n", "");

  const size_t chunk_size = BUFFER_BITS_NBR / CHAR_BIT;
  unsigned char buffer[chunk_size];

  md5_hash_t base_hash = {
      .a = 0x67452301, .b = 0xefcdab89, .c = 0x98badcfe, .d = 0x10325476
  };

  const size_t len = ft_strlen(str);
#undef CURRENT_INDENT
#define CURRENT_INDENT 1
  PRINT("original message length: %zu\n", len);
  const size_t total_message_len = get_required_bytes_nbr(len * CHAR_BIT);
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

    pad_chunk(buffer, i, len);

    PRINT("static buffer --v%s\n", "");

#ifdef DEBUG
    for (unsigned int j = 0; j < chunk_size; ++j) {
      print_8bin(((char *)buffer)[j]);
      write_stdout(" ", 1);
    }
    write_stdout("\n", 1);
#endif

    base_hash = md5_hash_chunk(base_hash, (md5_message_chunk_t)buffer);
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
