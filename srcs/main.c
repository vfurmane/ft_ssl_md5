#include "args.h"
#include "md5/hash.h"
#include "print.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  arg_parser_state_t arg_parser_state = INITIAL;
  for (int i = 1; i < argc; ++i) {
    if (arg_parser_state == INITIAL) {
      if (ft_strcmp(argv[i], "-s") == 0) {
        arg_parser_state = STRING;
      } else {
        return 1;
      }
    } else if (arg_parser_state == STRING) {
      const md5_hash_t hash = md5_hash_static_string(argv[i]);
      print_md5_hash(hash);
    } else {
      return 1;
    }
  }

  return 0;
}
