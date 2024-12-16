#include "args.h"
#include "md5/hash.h"
#include "print.h"
#include "sha256/hash.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, const char **argv) {
  if (argc == 1) {
    print_usage(argv[0]);
    return 1;
  }

#define X(algo) const uint8_t is_##algo = ft_strcmp(argv[1], #algo) == 0;
  ALGO_LIST
#undef X
  if (0
#define X(algo) || is_##algo
      ALGO_LIST
#undef X
  ) {
    const config_t config = parse_args(argc, argv);

    arg_parser_state_t arg_parser_state = STATE_STDIN;
    for (int i = 2; i < argc; ++i) {
      if (arg_parser_state == STATE_STDIN ||
          arg_parser_state == STATE_INITIAL) {
        if (ft_strcmp(argv[i], "-s") == 0) {
          arg_parser_state = STATE_STRING;
        } else if (ft_strcmp(argv[i], "-p") == 0) {
          if (0) {
            return 1;
          }
#define X(algo)                                                                \
  else if (is_##algo) {                                                        \
    const maybe_##algo##_hash_t ret = algo##_hash_fd(STDIN_FILENO, config, 1); \
    if (!ret.some) {                                                           \
      putstr_stderr("error while reading stdin");                              \
      return 1;                                                                \
    }                                                                          \
    print_##algo##_hash(ret.hash);                                             \
  }
          ALGO_LIST
#undef X
          putstr_stdout("\n");
          arg_parser_state = STATE_INITIAL;
        } else if (ft_strcmp(argv[i], "-q") != 0 &&
                   ft_strcmp(argv[i], "-r") != 0) {
          arg_parser_state = STATE_FILE;
        }
      } else if (arg_parser_state == STATE_STRING) {
        if (0) {
          return 1;
        }
#define X(algo)                                                                \
  else if (is_##algo) {                                                        \
    const algo##_hash_t hash = algo##_hash_static_string(argv[i]);             \
    print_##algo##_hashed_string(argv[i], hash, config);                       \
    arg_parser_state = STATE_INITIAL;                                          \
  }
        ALGO_LIST
#undef X
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
        if (0) {
          return 1;
        }
#define X(algo)                                                                \
  else if (is_##algo) {                                                        \
    const maybe_##algo##_hash_t ret = algo##_hash_fd(fd, config, 0);           \
    close(fd);                                                                 \
    if (!ret.some) {                                                           \
      putstr_stderr("error while reading stdin");                              \
      return 1;                                                                \
    }                                                                          \
    print_##algo##_hashed_file(ret.hash, argv[i], config);                     \
  }
        ALGO_LIST
#undef X
      }
    }

    if (arg_parser_state == STATE_STDIN) {
      if (0) {
        return 1;
      }
#define X(algo)                                                                \
  else if (is_##algo) {                                                        \
    const maybe_##algo##_hash_t ret = algo##_hash_fd(STDIN_FILENO, config, 0); \
    if (!ret.some) {                                                           \
      putstr_stderr("error while reading stdin");                              \
      return 1;                                                                \
    }                                                                          \
    print_##algo##_hashed_stdin(ret.hash, config);                             \
  }
      ALGO_LIST
#undef X
    }
  } else {
    putstr_stderr(argv[0]);
    putstr_stderr(": ");
    putstr_stderr(argv[1]);
    putstr_stderr(": unknown command\n");
    return 1;
  }

  return 0;
}
