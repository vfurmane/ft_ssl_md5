#ifndef PRINT_H
#define PRINT_H

#include "md5/hash.h"
#include "unistd.h"

size_t write_stdout(const void *buf, size_t n);
size_t putstr_stdout(const char *str);
size_t write_stdout_skip_newlines(const void *buf, size_t n);
size_t write_stderr(const void *buf, size_t n);
size_t putstr_stderr(const char *str);

void print_8bin(char nbr);

void print_32hex(unsigned int nbr);

void print_md5_hashed_string(
    const char *str, md5_hash_t hash, md5_config_t config
);
void print_md5_hashed_stdin(md5_hash_t hash, md5_config_t config);

#endif
