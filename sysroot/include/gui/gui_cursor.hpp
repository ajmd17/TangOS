#ifndef GUI_CURSOR_HPP
#define GUI_CURSOR_HPP

#include <kernel/vgascreen.h>
#include <kernel/mouse.h>

class gui_cursor {
public:
  int x, y;

  gui_cursor() {
    x = 0;
    y = 0;
  }

  void draw(vga_screen *screen) {
    // draw cursor
    int mouse_outline_color = COLOR_BLACK;
    int mouse_inside_color = COLOR_LIGHT_RED;

    //Right side of cursor
    vga_set_pixel(screen, x, y, mouse_outline_color);
    vga_set_pixel(screen, x + 1, y + 1, mouse_outline_color);
    vga_set_pixel(screen, x + 2, y + 2, mouse_outline_color);
    vga_set_pixel(screen, x + 3, y + 3, mouse_outline_color);
    vga_set_pixel(screen, x + 4, y + 4, mouse_outline_color);
    vga_set_pixel(screen, x + 5, y + 5, mouse_outline_color);

    //Left side of cursor
    vga_set_pixel(screen, x, y + 1, mouse_outline_color);
    vga_set_pixel(screen, x, y + 2, mouse_outline_color);
    vga_set_pixel(screen, x, y + 3, mouse_outline_color);
    vga_set_pixel(screen, x, y + 4, mouse_outline_color);
    vga_set_pixel(screen, x, y + 5, mouse_outline_color);
    vga_set_pixel(screen, x, y + 6, mouse_outline_color);
    vga_set_pixel(screen, x, y + 7, mouse_outline_color);

    //Bottom of cursor
    vga_set_pixel(screen, x + 1, y + 7, mouse_outline_color);
    vga_set_pixel(screen, x + 2, y + 7, mouse_outline_color);
    vga_set_pixel(screen, x + 3, y + 6, mouse_outline_color);
    vga_set_pixel(screen, x + 4, y + 6, mouse_outline_color);

    //Inside of cursor
    vga_set_pixel(screen, x + 1, y + 2, mouse_inside_color);
    vga_set_pixel(screen, x + 1, y + 3, mouse_inside_color);
    vga_set_pixel(screen, x + 1, y + 4, mouse_inside_color);
    vga_set_pixel(screen, x + 1, y + 5, mouse_inside_color);
    vga_set_pixel(screen, x + 1, y + 6, mouse_inside_color);

    vga_set_pixel(screen, x + 2, y + 3, mouse_inside_color);
    vga_set_pixel(screen, x + 2, y + 4, mouse_inside_color);
    vga_set_pixel(screen, x + 2, y + 5, mouse_inside_color);
    vga_set_pixel(screen, x + 2, y + 6, mouse_inside_color);

    vga_set_pixel(screen, x + 3, y + 4, mouse_inside_color);
    vga_set_pixel(screen, x + 3, y + 5, mouse_inside_color);

    vga_set_pixel(screen, x + 4, y + 5, mouse_inside_color);
  }
};

#endif