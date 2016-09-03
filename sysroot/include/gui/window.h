#ifndef WINDOW_H
#define WINDOW_H

#include <util/tos_types.h>
#include <gui/widget2.h>
#include <gui/font.h>
#include <kernel/screen.h>
#include <kernel/vgascreen.h>

#define MAX_WIDGETS_PER_WINDOW 10

#define WINDOW_TITLE_LENGTH 255

#define WINDOW_TITLE_BAR_HEIGHT 12

#define WINDOW_SHADOW_OFFSET 1

#define WINDOW_CONTENT_PADDING_X 1

#define WINDOW_CONTENT_PADDING_Y 1

typedef struct {
  int id;

  uint x;
  uint y;
  uint width;
  uint height;

  tstr title;

  font_sheet_t *font;

  widget_t *widgets[MAX_WIDGETS_PER_WINDOW];
  uint num_widgets;

  widget_t *close_button;
  widget_t *max_button;
  widget_t *min_button;

  bool close_button_clicked;
} window_t;

window_t *alloc_window(tstr titlestr);
void free_window(window_t *window);

// returns true on success, false on failure.
// if added, the window* will belong to the workspace.
bool add_widget_to_window(window_t *window, widget_t *widget);

void set_window_font(window_t *window, font_sheet_t *font);

void click_window(window_t *window, int mx, int my);

void draw_window(window_t *window, vga_screen *screen);

#endif