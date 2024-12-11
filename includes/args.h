#ifndef ARGS_H
#define ARGS_H

#include "stdint.h"
#include "stdlib.h"
#include "utils.h"

typedef enum arg_parser_state_e {
  STDIN,
  INITIAL,
  STRING,
  FILE,
} arg_parser_state_t;

typedef struct md5_config_s {
  uint8_t quiet;
  uint8_t reverse;
} md5_config_t;

md5_config_t init_md5_config(void);
md5_config_t parse_md5_args(int argc, const char **argv);

#endif
