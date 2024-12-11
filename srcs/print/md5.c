#include "print.h"

void print_formatted_md5_hash(
    const char *str, md5_hash_t hash, md5_config_t config, uint8_t with_quotes,
    uint8_t with_md5_prefix
) {
  if (!config.reverse) {
    if (!config.quiet) {
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
    }
    print_md5_hash(hash);
    putstr_stdout("\n");
  } else {
    print_md5_hash(hash);
    if (with_md5_prefix) {
      putstr_stdout(" ");
      if (with_quotes) {
        putstr_stdout("\"");
      }
      putstr_stdout(str);
      if (with_quotes) {
        putstr_stdout("\"");
      }
    }
    putstr_stdout("\n");
  }
}

void print_md5_hashed_string(
    const char *str, md5_hash_t hash, md5_config_t config
) {
  return print_formatted_md5_hash(str, hash, config, 1, 1);
}

void print_md5_hashed_stdin(md5_hash_t hash, md5_config_t config) {
  return print_formatted_md5_hash("stdin", hash, config, 0, 0);
}

void print_md5_hashed_file(
    md5_hash_t hash, const char *file, md5_config_t config
) {
  return print_formatted_md5_hash(file, hash, config, 0, 1);
}
