#ifndef SYS_ERR_H
#define SYS_ERR_H

#include <stdio.h>
#include <stdlib.h>

void throw_error(const char *msg) {
  printf("System error: %s\n", msg);
  abort();
}

#endif
