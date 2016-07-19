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
  }

private:
  int outside_color;
  int inside_color;
};
} // namespace gui

#endif
