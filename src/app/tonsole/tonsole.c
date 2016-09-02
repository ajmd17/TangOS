#include <app/tonsole/tonsole.h>

// a welcome messagebox
static window_t *welcome_msg = NULL;
// the workspace this app is in
static workspace_t *workspace = NULL;

static void ok_clicked() {
  // close the welcome message
  if (welcome_msg != NULL) {
    if (!remove_window_from_workspace(workspace, welcome_msg)) {
      free_window(welcome_msg);
      welcome_msg = NULL;
    }
  }
}

int tonsole_init(workspace_t *ws) {
  workspace = ws;

  welcome_msg = alloc_window("Welcome");
  welcome_msg->x = 50;
  welcome_msg->y = 100;
  welcome_msg->width = 150;

  { // create OK button
    widget_t *ok_btn = alloc_widget(WIDGET_TYPE_BUTTON, 
      "Ok", 
      welcome_msg->width - 25, 
      welcome_msg->height - 30, ok_clicked);
    ok_btn->width = 20;

    add_widget_to_window(welcome_msg, ok_btn);
  }

  { // create 'welcome' label
    widget_t *welcome_label = alloc_widget(WIDGET_TYPE_LABEL, 
      "Welcome to tangOS!\nWatch out for nugs!", 
      4, 4, NULL);
    
    add_widget_to_window(welcome_msg, welcome_label);
  }

  add_window_to_workspace(workspace, welcome_msg);

  return 0;
}

int tonsole_update() {
  return 0;
}

int tonsole_close() {
  return 0;
}