#include <gui/button.h>
#include <kernel/screen.h>

bool button_test_hit(button_t *btn, double mx, double my) {
  if (mx >= btn->x && mx <= btn->x + btn->width) {
    if (my >= btn->y && my <= btn->y + btn->height) {
       return true;
    }
  }
  return false;
}

void button_draw(button_t *btn, vga_screen *scr, font_sheet_t *fnt) {
  int x = btn->x;
  int y = btn->y;
  int width = btn->width;
  int height = btn->height;

  vga_fill_rect(scr,
    x, y,
    width, height,
    COLOR_LIGHT_GRAY);

  vga_fill_rect(scr,
    x, y + height,
    width + 1, 1,
    COLOR_DARK_GRAY);

  vga_fill_rect(scr,
    x + width, y,
    1, height,
    COLOR_DARK_GRAY);

  vga_fill_rect(scr,
    x, y,
    1, height,
    COLOR_WHITE);

  vga_fill_rect(scr,
    x, y,
    width, 1,
    COLOR_WHITE);
  
  font_draw_string(scr, fnt, x + 6, y + 2, btn->text);
}
