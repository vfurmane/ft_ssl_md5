#include "print.h"

#include "sha256/hash.h"

void print_sha256_hash(sha256_hash_t hash) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  buffer_bswap_32((void *)(&hash), sizeof(hash));
#endif

  print_32hex(hash.a);
  print_32hex(hash.b);
  print_32hex(hash.c);
  print_32hex(hash.d);
}
