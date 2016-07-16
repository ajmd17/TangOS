#ifndef GUI_CURSOR_HPP
#define GUI_CURSOR_HPP

#include <kernel/vgascreen.h>
#include <kernel/mouse.h>

namespace gui {
class cursor {
public:
  int x, y;

  cursor()
    : x(0), y(0),
    outside_color(COLOR_BLACK),
    inside_color(COLOR_LIGHT_RED) {
  }

  void draw(vga_screen *screen) {
    //Right side of cursor
    vga_set_pixel(screen, x, y, outside_color);
    vga_set_pixel(screen, x + 1, y + 1, outside_color);
    vga_set_pixel(screen, x + 2, y + 2, outside_color);
    vga_set_pixel(screen, x + 3, y + 3, outside_color);
    vga_set_pixel(screen, x + 4, y + 4, outside_color);
    vga_set_pixel(screen, x + 5, y + 5, outside_color);

    //Left side of cursor
    vga_set_pixel(screen, x, y + 1, outside_color);
    vga_set_pixel(screen, x, y + 2, outside_color);
    vga_set_pixel(screen, x, y + 3, outside_color);
    vga_set_pixel(screen, x, y + 4, outside_color);
    vga_set_pixel(screen, x, y + 5, outside_color);
    vga_set_pixel(screen, x, y + 6, outside_color);
    vga_set_pixel(screen, x, y + 7, outside_color);

    //Bottom of cursor
    vga_set_pixel(screen, x + 1, y + 7, outside_color);
    vga_set_pixel(screen, x + 2, y + 7, outside_color);
    vga_set_pixel(screen, x + 3, y + 6, outside_color);
    vga_set_pixel(screen, x + 4, y + 6, outside_color);

    //Inside of cursor
    vga_set_pixel(screen, x + 1, y + 2, inside_color);
    vga_set_pixel(screen, x + 1, y + 3, inside_color);
    vga_set_pixel(screen, x + 1, y + 4, inside_color);
    vga_set_pixel(screen, x + 1, y + 5, inside_color);
    vga_set_pixel(screen, x + 1, y + 6, inside_color);

    vga_set_pixel(screen, x + 2, y + 3, inside_color);
    vga_set_pixel(screen, x + 2, y + 4, inside_color);
    vga_set_pixel(screen, x + 2, y + 5, inside_color);
    vga_set_pixel(screen, x + 2, y + 6, inside_color);

    vga_set_pixel(screen, x + 3, y + 4, inside_color);
    vga_set_pixel(screen, x + 3, y + 5, inside_color);

    vga_set_pixel(screen, x + 4, y + 5, inside_color);
  }

private:
  int outside_color;
  int inside_color;
};
} // namespace gui

#endif