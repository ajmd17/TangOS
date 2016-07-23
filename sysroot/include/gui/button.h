#ifndef BUTTON_H
#define BUTTON_H

#include <gui/font.h>
#include <kernel/vgascreen.h>

#include <stdbool.h>

typedef struct {
  int x, y, width, height;
  char text[24];
} button_t;

bool button_test_hit(button_t *btn, double mx, double my);
void button_draw(button_t *btn, vga_screen *scr, font_sheet_t *fnt);

#endif
