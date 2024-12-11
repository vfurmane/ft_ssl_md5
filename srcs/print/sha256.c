#include "print.h"

void print_formatted_sha256_hash(
    const char *str, sha256_hash_t hash, config_t config, uint8_t with_quotes,
    uint8_t with_sha256_prefix
) {
  if (!config.reverse) {
    if (!config.quiet) {
      if (with_sha256_prefix) {
        putstr_stdout("SHA256 (");
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
      if (with_sha256_prefix) {
        putstr_stdout(" = ");
      } else {
        putstr_stdout("= ");
      }
    }
    print_sha256_hash(hash);
    putstr_stdout("\n");
  } else {
    print_sha256_hash(hash);
    if (!config.quiet && with_sha256_prefix) {
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

void print_sha256_hashed_string(
    const char *str, sha256_hash_t hash, config_t config
) {
  return print_formatted_sha256_hash(str, hash, config, 1, 1);
}

void print_sha256_hashed_stdin(sha256_hash_t hash, config_t config) {
  return print_formatted_sha256_hash("stdin", hash, config, 0, 0);
}

void print_sha256_hashed_file(
    sha256_hash_t hash, const char *file, config_t config
) {
  return print_formatted_sha256_hash(file, hash, config, 0, 1);
}
