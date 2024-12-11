#include "args.h"

md5_config_t init_md5_config(void) {
  md5_config_t config = {.quiet = 0};

  return config;
}

md5_config_t parse_md5_args(int argc, const char **argv) {
  md5_config_t config = init_md5_config();

  for (size_t i = 2; (ssize_t)i < argc; ++i) {
    if (ft_strcmp(argv[i], "-q") == 0) {
      config.quiet = 1;
    }
  }

  return config;
}
