#include "md5/hash.h"
#include "print.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    return 1;
  }

  const md5_hash_t hash = md5_hash_static_string(argv[1]);

  print_md5_hash(hash);

  return 0;
}
