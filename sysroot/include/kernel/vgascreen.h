#ifndef VGASCREEN_H
#define VGASCREEN_H

#include <stdint.h>

typedef struct {
  uint16_t width;
  uint16_t height;
  uint16_t bpp;
  uint8_t *memory;
} vga_screen;

void vga_write_registers(uint8_t *regs);
void vga_init_colors();
void vga_add_color(uint16_t index, uint16_t red, uint16_t green, uint16_t blue);
void vga_set_pixel(vga_screen *scr, uint16_t x, uint16_t y, uint16_t color);
void vga_fill_rect(vga_screen *scr, uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t height, uint16_t color);
void vga_clear_screen(vga_screen *scr, uint8_t color);

vga_screen vga_init_320_200_256();

#endif
