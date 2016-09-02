#include <gui/workspace.h>

#include <stdlib.h>

workspace_t *alloc_workspace(void) {
  workspace_t *ws = (workspace_t*)malloc(sizeof(workspace_t));

  ws->font = NULL;

  // set all windows to NULL.
  for (uint i = 0; i < MAX_WINDOWS_PER_WORKSPACE; i++) {
    ws->windows[i] = NULL;
  }

  ws->num_windows = 0;
  return ws;
}

void free_workspace(workspace_t *workspace) {
  for (uint i = 0; i < MAX_WINDOWS_PER_WORKSPACE; i++) {
    if (workspace->windows[i] != NULL) {
      free_window(workspace->windows[i]);
      workspace->windows[i] = NULL;
    }
  }

  free(workspace);
}

bool add_window_to_workspace(workspace_t *workspace, window_t *window) {
  if (workspace->num_windows >= MAX_WINDOWS_PER_WORKSPACE) {
    return false;
  }

  // find a free slot in workspace_t->windows.
  for (uint i = 0; i < MAX_WINDOWS_PER_WORKSPACE; i++) {
    if (workspace->windows[i] == NULL) {
      window->id = i; // set window's id to it's index
      set_window_font(window, workspace->font); // apply workspace font to window
      workspace->windows[i] = window;
      workspace->num_windows++;
      return true;
    }
  }
  // at this point, no free slots have been found.
  return false;
}

bool remove_window_from_workspace(workspace_t *workspace, window_t *window) {
  if (window->id == -1) {
    // window not added to workspace!
    return false;
  }

  if (workspace->windows[window->id] != window) {
    // not the same window!
    return false;
  }

  int window_id = window->id;
  free_window(workspace->windows[window_id]);
  workspace->windows[window_id] = NULL;
  workspace->num_windows--;

  return true;
}

void set_workspace_font(workspace_t *workspace, font_sheet_t *font) {
  workspace->font = font;
  
  // apply font to all created windows
  for (uint i = 0; i < MAX_WINDOWS_PER_WORKSPACE; i++) {
    if (workspace->windows[i] != NULL) {
      set_window_font(workspace->windows[i], font);
    }
  }
}

void click_workspace(workspace_t *workspace, int mx, int my) {
  // if the mouse x and y are within the window's range,
  // the respective window is tested.
  for (uint i = 0; i < MAX_WINDOWS_PER_WORKSPACE; i++) {
    window_t *win = workspace->windows[i];
    if (win != NULL) {
      if (mx >= win->x && mx <= win->x + win->width) {
        if (my >= win->y && my <= win->y + win->height) {
          int offsetx = mx - win->x;
          int offsety = my - win->y;
          click_window(win, offsetx, offsety);
        }
      }
    }
  }
}

void draw_workspace(workspace_t *workspace, vga_screen *screen) {
  for (uint i = 0; i < MAX_WINDOWS_PER_WORKSPACE; i++) {
    if (workspace->windows[i] != NULL) {
      draw_window(workspace->windows[i], screen);
    }
  }
}