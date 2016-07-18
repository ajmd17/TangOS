#ifndef FONT_H
#define FONT_H

#include <kernel/vgascreen.h>
#include <kernel/types.h>

typedef struct {
  dword char_width;
  dword char_height;
  dword sheet_width;
  dword sheet_height;
  const sbyte *sheet_data_ptr;
} font_sheet_t;

void font_draw_character(vga_screen *scr, 
    font_sheet_t *font,
    dword x, dword y, 
    char c);

void font_draw_string(vga_screen *scr, 
    font_sheet_t *font,
    dword x, dword y,
    const char *str);

#endif