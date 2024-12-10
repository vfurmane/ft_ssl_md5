#include "print.h"

void print_formatted_md5_hash(
    const char *str, md5_hash_t hash, uint8_t with_quotes,
    uint8_t with_md5_prefix
) {
  if (with_md5_prefix) {
    putstr_stdout("MD5 (");
  } else {
    putstr_stdout("(");
  }
  if (with_quotes) {
    putstr_stdout("\"");
  }
  putstr_stdout(str);
  if (with_quotes) {
    putstr_stdout("\"");
  }
  putstr_stdout(")");
  if (with_md5_prefix) {
    putstr_stdout(" = ");
  } else {
    putstr_stdout("= ");
  }
  print_md5_hash(hash);
  putstr_stdout("\n");
}

void print_md5_hashed_string(const char *str, md5_hash_t hash) {
  return print_formatted_md5_hash(str, hash, 1, 1);
}

void print_md5_hashed_stdin(md5_hash_t hash) {
  return print_formatted_md5_hash("stdin", hash, 0, 0);
}
