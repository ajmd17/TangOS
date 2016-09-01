#ifndef WIDGET_H
#define WIDGET_H

#include <util/tos_types.h>
#include <gui/font.h>
#include <kernel/screen.h>
#include <kernel/vgascreen.h>

#define WIDGET_TEXT_LENGTH 512

enum {
  WIDGET_TYPE_LABEL,
  WIDGET_TYPE_BUTTON,
  WIDGET_TYPE_FIELD,
  WIDGET_TYPE_PICTUREBOX
};

typedef struct {
  uint x;
  uint y;
  uint width;
  uint height;

  tstr text;

  font_sheet_t *font;

  uint type;

  void(*callback)();
} widget_t;

widget_t *alloc_widget(uint type, tstr text, uint x, uint y, void(*callback)());
void free_widget(widget_t *widget);

void set_widget_font(widget_t *widget, font_sheet_t *font);

// calls the widget's callback if it is not NULL
void click_widget(widget_t *widget);

// draw widget as whatever type it has been set to
void draw_widget(widget_t *widget, vga_screen *screen, 
  uint offsetx, uint offsety);

// draw the widget as a label, offsetx and offsety are the parent control's x and y.
void draw_widget_as_label(widget_t *widget, vga_screen *screen, 
  uint offsetx, uint offsety);

// draw the widget as a button, offsetx and offsety are the parent control's x and y.
void draw_widget_as_button(widget_t *widget, vga_screen *screen, 
  uint offsetx, uint offsety);

#endif