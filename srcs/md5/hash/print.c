#include "md5/hash.h"

void print_md5_hash(md5_hash_t hash) {
  print_32hex(hash.a);
  print_32hex(hash.b);
  print_32hex(hash.c);
  print_32hex(hash.d);
  write_stdout("\n", 1);
}