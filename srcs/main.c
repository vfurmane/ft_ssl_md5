#include "md5/hash.h"
#include "print.h"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  const md5_hash_t hash = md5_hash_static_string("hello, world!");

  print_md5_hash(hash);

  return 0;
}
