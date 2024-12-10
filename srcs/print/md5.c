#include "print.h"

void print_md5_hashed_string(const char *str, md5_hash_t hash) {
  putstr_stdout("MD5 (\"");
  putstr_stdout(str);
  putstr_stdout("\") = ");
  print_md5_hash(hash);
  putstr_stdout("\n");
}
