#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG

#include "print.h"

#include <stdio.h>

#define PRINT_INDENT_SIZE 2
#define CURRENT_INDENT 0

#define PRINT(str, args...)                                                    \
  printf("%-*s" str, CURRENT_INDENT *PRINT_INDENT_SIZE, "", args)

#else

#define PRINT(str, ...) (void)(str)

#endif
#endif
