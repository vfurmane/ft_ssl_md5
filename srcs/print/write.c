#include "print.h"

size_t write_stdout(const void *buf, size_t n) {
  return write(STDOUT_FILENO, buf, n);
}

size_t putstr_stdout(const char *str) {
  const size_t len = ft_strlen(str);
  return write_stdout(str, len);
}

size_t write_stdout_skip_newlines(const void *buf, size_t n) {
  size_t len = 0;
  const char *start = buf;
  do {
    const char *end =
        ft_memchr(start, '\n', (ssize_t)n - (start - (const char *)buf));
    len += write_stdout(
        start,
        end == NULL ? (ssize_t)n - (start - (const char *)buf) : end - start
    );
    if (end == NULL) {
      break;
    }
    start = end + 1;
  } while (start - ((const char *)buf) < (ssize_t)n);

  return len;
}

size_t write_stderr(const void *buf, size_t n) {
  return write(STDERR_FILENO, buf, n);
}

size_t putstr_stderr(const char *str) {
  const size_t len = ft_strlen(str);
  return write_stderr(str, len);
}
