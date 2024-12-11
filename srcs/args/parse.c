#include "args.h"

config_t init_config(void) {
  config_t config = {.quiet = 0, .reverse = 0};

  return config;
}

config_t parse_args(int argc, const char **argv) {
  config_t config = init_config();

  for (size_t i = 2; (ssize_t)i < argc; ++i) {
    if (ft_strcmp(argv[i], "-q") == 0) {
      config.quiet = 1;
    } else if (ft_strcmp(argv[i], "-r") == 0) {
      config.reverse = 1;
    }
  }

  return config;
}
