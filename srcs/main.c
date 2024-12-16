#include "args.h"
#include "print.h"

int main(int argc, const char **argv) {
  if (argc == 1) {
    print_usage(argv[0]);
    return 1;
  }

  if (dispatch_command_args(argc, argv) < 0) {
    return 1;
  }

  return 0;
}
