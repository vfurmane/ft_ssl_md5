#include "args.h"
#include "md5/hash.h"
#include "print.h"

int main(int argc, const char **argv) {
  if (argc == 1) {
    return 1;
  }

  if (ft_strcmp(argv[1], "md5") == 0) {
    if (argc == 2) {
      const maybe_md5_hash_t ret = md5_hash_fd(STDIN_FILENO, 0);
      if (!ret.some) {
        putstr_stderr("error while reading stdin");
        return 1;
      }
      print_md5_hashed_stdin(ret.hash);
    } else {
      arg_parser_state_t arg_parser_state = INITIAL;
      for (int i = 2; i < argc; ++i) {
        if (arg_parser_state == INITIAL) {
          if (ft_strcmp(argv[i], "-s") == 0) {
            arg_parser_state = STRING;
          } else if (ft_strcmp(argv[i], "-p") == 0) {
            const maybe_md5_hash_t ret = md5_hash_fd(STDIN_FILENO, 1);
            if (!ret.some) {
              putstr_stderr("error while reading stdin");
              return 1;
            }
            print_md5_hash(ret.hash);
            putstr_stdout("\n");
            arg_parser_state = INITIAL;
          } else {
            return 1;
          }
        } else if (arg_parser_state == STRING) {
          const md5_hash_t hash = md5_hash_static_string(argv[i]);
          print_md5_hashed_string(argv[i], hash);
          arg_parser_state = INITIAL;
        } else {
          return 1;
        }
      }
    }
  }

  return 0;
}
