#include "print.h"

#include "sha256/hash.h"

void print_sha256_hash(sha256_hash_t hash) {
  print_32hex(hash.a);
  print_32hex(hash.b);
  print_32hex(hash.c);
  print_32hex(hash.d);
  print_32hex(hash.e);
  print_32hex(hash.f);
  print_32hex(hash.g);
  print_32hex(hash.h);
}
