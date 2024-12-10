#include "print.h"

size_t write_stdout(const void *buf, size_t n) {
  return write(STDOUT_FILENO, buf, n);
}

size_t putstr_stdout(const char *str) {
  const size_t len = ft_strlen(str);
  return write_stdout(str, len);
}

size_t write_stderr(const void *buf, size_t n) {
  return write(STDERR_FILENO, buf, n);
}
