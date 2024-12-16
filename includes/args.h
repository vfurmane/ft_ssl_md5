#ifndef ARGS_H
#define ARGS_H

#include "utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#define ALGO_LIST                                                              \
  X(md5)                                                                       \
  X(sha256)

typedef enum arg_parser_state_e {
  STATE_STDIN,
  STATE_INITIAL,
  STATE_STRING,
  STATE_FILE,
} arg_parser_state_t;

typedef struct config_s {
  uint8_t quiet;
  uint8_t reverse;
} config_t;

config_t init_config(void);
config_t parse_args(int argc, const char **argv);

#endif
