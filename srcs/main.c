#include "md5/hash.h"
#include "print.h"

int main(void) {
  md5_message_t message = allocate_message_from_string("hello, world!");
  if (message.b == NULL) {
    const char *error_message = "cannot allocate buffer";
    write_stderr(error_message, ft_strlen(error_message));
    return 1;
  }

  const md5_hash_t hash = md5_hash(message);

  print_md5_hash(hash);

  destroy_message(&message);

  return 0;
}
