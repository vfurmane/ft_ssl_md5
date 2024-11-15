#include "print.h"

size_t write_stdout(const void *buf, size_t n) {
  return write(STDOUT_FILENO, buf, n);
}

size_t write_stderr(const void *buf, size_t n) {
  return write(STDERR_FILENO, buf, n);
}
