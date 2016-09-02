#ifndef APP_H
#define APP_H

#include <gui/workspace.h>

typedef struct {
  int(*init_func)(workspace_t*);
  int(*update_func)();
  int(*close_func)();
} app_t;

app_t *alloc_app(int(*init_func)(workspace_t*), int(*update_func)(), int(*close_func)());
void free_app(app_t *app);

#endif