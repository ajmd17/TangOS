#include <gui/window.h>
#include <stdlib.h>
#include <string.h>

window_t *alloc_window(tstr titlestr) {
  window_t *win = (window_t*)malloc(sizeof(window_t));
  win->id = -1; // window not yet in a workspace

  win->x = 0;
  win->y = 0;
  win->width = 50;
  win->height = 50;

  win->title = (tstr)malloc(sizeof(tchar) * WINDOW_TITLE_LENGTH);
  memset(win->title, '\0', WINDOW_TITLE_LENGTH);
  strcpy(win->title, titlestr);

  win->font = NULL;

  // Set all widgets to NULL.
  for (uint i = 0; i < MAX_WIDGETS_PER_WINDOW; i++) {
    win->widgets[i] = NULL;
  }

  win->num_widgets = 0;

  return win;
}

void free_window(window_t *window) {
  for (uint i = 0; i < MAX_WIDGETS_PER_WINDOW; i++) {
    if (window->widgets[i] != NULL) {
      free_widget(window->widgets[i]);
      window->widgets[i] = NULL;
    }
  }

  free(window->title);
  free(window);
}

bool add_widget_to_window(window_t *window, widget_t *widget) {
  if (window->num_widgets >= MAX_WIDGETS_PER_WINDOW) {
    return false;
  }

  // find a free slot in window->widgets.
  for (uint i = 0; i < MAX_WIDGETS_PER_WINDOW; i++) {
    if (window->widgets[i] == NULL) {
      set_widget_font(widget, window->font); // apply window font to widget
      window->widgets[i] = widget;
      window->num_widgets++;
      return true;
    }
  }
  // at this point, no free slots have been found.
  return false;
}

void set_window_font(window_t *window, font_sheet_t *font) {
  window->font = font;

  // apply font to all created widgets
  for (uint i = 0; i < MAX_WIDGETS_PER_WINDOW; i++) {
    if (window->widgets[i] != NULL) {
      set_widget_font(window->widgets[i], font);
    }
  }
}

void click_window(window_t *window, int mx, int my) {
  // if the mouse x and y are within the widgets's range,
  // the respective widget's callback is triggered.
  for (uint i = 0; i < MAX_WIDGETS_PER_WINDOW; i++) {
    widget_t *wid = window->widgets[i];
    if (wid != NULL) {
      uint wid_x = wid->x + WINDOW_CONTENT_PADDING_X;
      uint wid_y = wid->y + WINDOW_TITLE_BAR_HEIGHT + WINDOW_CONTENT_PADDING_Y;

      if (mx >= wid_x && mx <= wid_x + wid->width) {
        if (my >= wid_y && my <= wid_y + wid->height) {
          click_widget(wid);
        }
      }
    }
  }
}

void draw_window(window_t *window, vga_screen *screen) {
  // draw window drop shadow
  vga_fill_rect(screen, window->x + WINDOW_SHADOW_OFFSET, window->y + WINDOW_SHADOW_OFFSET, 
    window->width, window->height, COLOR_BLACK);

  // draw window
  vga_fill_rect(screen, window->x, window->y, 
    window->width, window->height, COLOR_LIGHT_GRAY);

  // draw window decoration bar
  vga_fill_rect(screen, window->x, window->y, 
    window->width, WINDOW_TITLE_BAR_HEIGHT, COLOR_LIGHT_BLUE);

  // draw title text
  font_draw_string(screen, window->font,
    window->x + 2, window->y + 2,
    window->title);

  // draw all widgets
  for (uint i = 0; i < MAX_WIDGETS_PER_WINDOW; i++) {
    if (window->widgets[i] != NULL) {
      draw_widget(window->widgets[i], screen, 
        window->x + WINDOW_CONTENT_PADDING_X, 
        window->y + WINDOW_TITLE_BAR_HEIGHT + WINDOW_CONTENT_PADDING_Y);
    }
  }
}