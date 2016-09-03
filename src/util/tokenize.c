#include <util/tokenize.h>
#include <util/logging.h>
#include <string.h>

int tokenize(char * str, char * sep, char **buf) {
  char * pch_i;
  int    argc = 0;
  pch_i = strtok(str,sep);
  if (!pch_i) { return 0; }
  while (pch_i != NULL) {
    buf[argc] = (char *)pch_i;
    ++argc;
    pch_i = strtok(NULL,sep);
  }
  buf[argc] = NULL;
  return argc;
}
