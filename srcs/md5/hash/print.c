#include "md5/hash.h"

void print_md5_hash(md5_hash_t hash) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  buffer_bswap_32((void *)(&hash), sizeof(hash));
#endif

  print_32hex(hash.a);
  print_32hex(hash.b);
  print_32hex(hash.c);
  print_32hex(hash.d);
  write_stdout("\n", 1);
}