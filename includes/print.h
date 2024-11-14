#ifndef PRINT_H
#define PRINT_H

#include "unistd.h"

size_t write_stdout(const void *buf, size_t n);

void print_8bin(char nbr);

#endif
