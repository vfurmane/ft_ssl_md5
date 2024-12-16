#include "print.h"

void print_usage(const char *argv0) {
  putstr_stderr(argv0);
  putstr_stderr(" command [-pqr] [-s string ...] [file ...]\n\n");
  putstr_stderr(
      "  -p, echo STDIN to STDOUT and append the checksum to STDOUT\n"
  );
  putstr_stderr("  -q, quiet mode\n");
  putstr_stderr("  -r, reverse the format of the output\n");
  putstr_stderr("  -s, print the sum of the given string\n");
  putstr_stderr("\n");
}
