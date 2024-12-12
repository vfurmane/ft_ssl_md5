#include "args.h"
#include "md5/hash.h"
#include "print.h"
#include "sha256/hash.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, const char **argv) {
  if (argc == 1) {
    return 1;
  }

  const uint8_t is_md5 = ft_strcmp(argv[1], "md5") == 0;
  if (is_md5 || ft_strcmp(argv[1], "sha256") == 0) {
    const config_t config = parse_args(argc, argv);

    arg_parser_state_t arg_parser_state = STATE_STDIN;
    for (int i = 2; i < argc; ++i) {
      if (arg_parser_state == STATE_STDIN ||
          arg_parser_state == STATE_INITIAL) {
        if (ft_strcmp(argv[i], "-s") == 0) {
          arg_parser_state = STATE_STRING;
        } else if (ft_strcmp(argv[i], "-p") == 0) {
          if (is_md5) {
            const maybe_md5_hash_t ret = md5_hash_fd(STDIN_FILENO, config, 1);
            if (!ret.some) {
              putstr_stderr("error while reading stdin");
              return 1;
            }
            print_md5_hash(ret.hash);
          } else {
            const maybe_sha256_hash_t ret =
                sha256_hash_fd(STDIN_FILENO, config, 1);
            if (!ret.some) {
              putstr_stderr("error while reading stdin");
              return 1;
            }
            print_sha256_hash(ret.hash);
          }
          putstr_stdout("\n");
          arg_parser_state = STATE_INITIAL;
        } else if (ft_strcmp(argv[i], "-q") != 0 &&
                   ft_strcmp(argv[i], "-r") != 0) {
          arg_parser_state = STATE_FILE;
        }
      } else if (arg_parser_state == STATE_STRING) {
        if (is_md5) {
          const md5_hash_t hash = md5_hash_static_string(argv[i]);
          print_md5_hashed_string(argv[i], hash, config);
          arg_parser_state = STATE_INITIAL;
        } else {
          const sha256_hash_t hash = sha256_hash_static_string(argv[i]);
          print_sha256_hashed_string(argv[i], hash, config);
          arg_parser_state = STATE_INITIAL;
        }
      }
      if (arg_parser_state == STATE_FILE) {
        const int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
          putstr_stderr(argv[0]);
          putstr_stderr(": ");
          putstr_stderr(argv[1]);
          putstr_stderr(": ");
          putstr_stderr(argv[i]);
          putstr_stderr(": ");
          putstr_stderr(strerror(errno));
          putstr_stderr("\n");
          continue;
        }
        if (is_md5) {
          const maybe_md5_hash_t ret = md5_hash_fd(fd, config, 0);
          close(fd);
          if (!ret.some) {
            putstr_stderr("error while reading stdin");
            return 1;
          }
          print_md5_hashed_file(ret.hash, argv[i], config);
        } else {
          const maybe_sha256_hash_t ret = sha256_hash_fd(fd, config, 0);
          close(fd);
          if (!ret.some) {
            putstr_stderr("error while reading stdin");
            return 1;
          }
          print_sha256_hashed_file(ret.hash, argv[i], config);
        }
      }
    }

    if (arg_parser_state == STATE_STDIN) {
      if (is_md5) {
        const maybe_md5_hash_t ret = md5_hash_fd(STDIN_FILENO, config, 0);
        if (!ret.some) {
          putstr_stderr("error while reading stdin");
          return 1;
        }
        print_md5_hashed_stdin(ret.hash, config);
      } else {
        const maybe_sha256_hash_t ret = sha256_hash_fd(STDIN_FILENO, config, 0);
        if (!ret.some) {
          putstr_stderr("error while reading stdin");
          return 1;
        }
        print_sha256_hashed_stdin(ret.hash, config);
      }
    }
  }

  return 0;
}
