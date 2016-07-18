#ifndef VGASCREEN_H
#define VGASCREEN_H

#include <stdint.h>

typedef struct {
  uint16_t width;
  uint16_t height;
  uint16_t bpp;
  uint8_t *memory;
  uint8_t *buffer;
} vga_screen;

void vga_set_pixel(vga_screen *scr,
  uint16_t x, uint16_t y,
  uint16_t color);
void vga_fill_rect(vga_screen *scr,
  uint16_t origin_x, uint16_t origin_y,
  uint16_t width, uint16_t height,
  uint16_t color);
void vga_clear_screen(vga_screen *scr, uint8_t color);
void vga_blit(vga_screen *scr);

vga_screen vga_init_320_200_256();

#endif
