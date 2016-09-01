#include <gui/widget2.h>
#include <stdlib.h>
#include <string.h>

widget_t *alloc_widget(uint type, tstr text, uint x, uint y, void(*callback)()) {
  widget_t *widget = (widget_t*)malloc(sizeof(widget_t));

  widget->x = x;
  widget->y = y;
  widget->width = 40;
  widget->height = 14;
  widget->type = type;
  widget->callback = callback;

  widget->text = (tstr)malloc(sizeof(tchar) * WIDGET_TEXT_LENGTH);
  memset(widget->text, '\0', WIDGET_TEXT_LENGTH);
  strcpy(widget->text, text);

  widget->font = NULL;

  return widget;
}

void free_widget(widget_t *widget) {
  free(widget->text);
  free(widget);
}

void set_widget_font(widget_t *widget, font_sheet_t *font) {
  widget->font = font;
}

void click_widget(widget_t *widget) {
  if (widget->callback != NULL) {
    widget->callback();
  }
}

void draw_widget(widget_t *widget, vga_screen *screen, 
  uint offsetx, uint offsety) {

  switch (widget->type) {
  case WIDGET_TYPE_LABEL:
    draw_widget_as_label(widget, screen, offsetx, offsety);
    break;
  case WIDGET_TYPE_BUTTON:
    draw_widget_as_button(widget, screen, offsetx, offsety);
    break;
  }
}

void draw_widget_as_label(widget_t *widget, vga_screen *screen, 
  uint offsetx, uint offsety) {

  // draw widget text
  font_draw_string(screen, widget->font,
    widget->x + offsetx, widget->y + offsety,
    widget->text);
}

void draw_widget_as_button(widget_t *widget, vga_screen *screen, 
  uint offsetx, uint offsety) {

  int x = widget->x + offsetx;
  int y = widget->y + offsety;
  int width = widget->width;
  int height = widget->height;

  vga_fill_rect(screen,
    x, y,
    width, height,
    COLOR_LIGHT_GRAY);

  vga_fill_rect(screen,
    x, y + height,
    width + 1, 1,
    COLOR_DARK_GRAY);

  vga_fill_rect(screen,
    x + width, y,
    1, height,
    COLOR_DARK_GRAY);

  vga_fill_rect(screen,
    x, y,
    1, height,
    COLOR_WHITE);

  vga_fill_rect(screen,
    x, y,
    width, 1,
    COLOR_WHITE);
  
  font_draw_string(screen, widget->font, 
    x + 2, y + 3, widget->text);
}