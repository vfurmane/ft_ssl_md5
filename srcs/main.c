#include "args.h"
#include "md5/hash.h"
#include "print.h"

int main(int argc, const char **argv) {
  if (argc == 1) {
    return 1;
  }

  if (ft_strcmp(argv[1], "md5") == 0) {
    const md5_config_t config = parse_md5_args(argc, argv);

    arg_parser_state_t arg_parser_state = STDIN;
    for (int i = 2; i < argc; ++i) {
      if (arg_parser_state == STDIN || arg_parser_state == INITIAL) {
        if (ft_strcmp(argv[i], "-s") == 0) {
          arg_parser_state = STRING;
        } else if (ft_strcmp(argv[i], "-p") == 0) {
          const maybe_md5_hash_t ret = md5_hash_fd(STDIN_FILENO, config, 1);
          if (!ret.some) {
            putstr_stderr("error while reading stdin");
            return 1;
          }
          print_md5_hash(ret.hash);
          putstr_stdout("\n");
          arg_parser_state = INITIAL;
        } else if (ft_strcmp(argv[i], "-q") != 0) {
          return 1;
        }
      } else if (arg_parser_state == STRING) {
        const md5_hash_t hash = md5_hash_static_string(argv[i]);
        print_md5_hashed_string(argv[i], hash, config);
        arg_parser_state = INITIAL;
      } else {
        return 1;
      }
    }

    if (arg_parser_state == STDIN) {
      const maybe_md5_hash_t ret = md5_hash_fd(STDIN_FILENO, config, 0);
      if (!ret.some) {
        putstr_stderr("error while reading stdin");
        return 1;
      }
      print_md5_hashed_stdin(ret.hash, config);
    }
  }

  return 0;
}
