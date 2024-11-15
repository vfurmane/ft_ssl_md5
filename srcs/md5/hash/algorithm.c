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

md5_hash_t md5_hash_round(
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

md5_hash_t md5_hash(md5_message_t message) {
#undef CURRENT_INDENT
#define CURRENT_INDENT 0
  PRINT("Hashing the buffer%s\n", "");
  md5_hash_t base_hash = {
      .a = 0x67452301, .b = 0xefcdab89, .c = 0x98badcfe, .d = 0x10325476
  };
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

    md5_hash_t hash = base_hash;
    for (size_t j = 0; j < 64; ++j) {
      hash = md5_hash_round(hash, message_chunk, j);
    }
    base_hash.a += hash.a;
    base_hash.b += hash.b;
    base_hash.c += hash.c;
    base_hash.d += hash.d;
  }

#undef CURRENT_INDENT
#define CURRENT_INDENT 1
  PRINT(
      "hash done: %#010x %#010x %#010x %#010x (endianness might be wrong here)\n", base_hash.a, base_hash.b,
      base_hash.c, base_hash.d
  );

  return base_hash;
}
