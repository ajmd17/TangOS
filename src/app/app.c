#include <app/app.h>
#include <stdlib.h>

app_t *alloc_app(int(*init_func)(workspace_t*), int(*update_func)(), int(*close_func)()) {
  app_t *app = (app_t*)malloc(sizeof(app_t));
  app->init_func = init_func;
  app->update_func = update_func;
  app->close_func = close_func;
  return app;
}

void free_app(app_t *app) {
  free(app);
}