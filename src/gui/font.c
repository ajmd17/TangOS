#include <gui/font.h>
#include <kernel/image.h>
#include <stdio.h>

void font_draw_character(vga_screen *scr,
  font_sheet_t *font,
  dword x, dword y,
  char c) {

  dword index = (c - 33); // printable characters start at 33 and go to 127
  dword num_x = 11, num_y = 9;
  dword subx = (index % num_x) * font->char_width;
  dword suby = (index / num_x) * font->char_height;

  image_draw_subimage(scr,
    subx, suby,
    font->char_width, font->char_height,
    x - subx, y - suby,
    font->sheet_width, font->sheet_height,
    font->sheet_data_ptr);
}

void font_draw_string(vga_screen *scr, 
  font_sheet_t *font,
  dword x, dword y,
  const char *str) {

  int xoffset = 0;
  int yoffset = 0;
  while ((*str) != '\0') {
    if ((*str) == '\n') {
      xoffset = 0;
      yoffset += font->char_height + 1;
    } else if ((*str) == ' ') {
      xoffset += font->char_width/2 + 1;
    } else {
      font_draw_character(scr, font, x + xoffset, y + yoffset, *str);
      xoffset += font->char_width/2 + 1;
    }
    ++str;
  }
}