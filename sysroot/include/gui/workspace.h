#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <kernel/vgascreen.h>
#include <gui/window.h>
#include <gui/font.h>
#include <util/tos_types.h>

#define MAX_WINDOWS_PER_WORKSPACE 10

typedef struct {
  font_sheet_t *font;
  window_t *windows[MAX_WINDOWS_PER_WORKSPACE];
  uint num_windows;
} workspace_t;

workspace_t *alloc_workspace(void);
void free_workspace(workspace_t *workspace);

// returns true on success, false on failure.
// if added, the window* will belong to the workspace.
bool add_window_to_workspace(workspace_t *workspace, window_t *window);
// remove the window from the workspace, returns true on success.
bool remove_window_from_workspace(workspace_t *workspace, window_t *window);

// set the font for the workspace
void set_workspace_font(workspace_t *workspace, font_sheet_t *font);

// tests all objects in the workspace for click event
void click_workspace(workspace_t *workspace, int mx, int my);

// draw the workspace and all windows within it
void draw_workspace(workspace_t *workspace, vga_screen *screen);

#endif