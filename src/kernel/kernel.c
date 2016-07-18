#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <kernel/screen.h>
#include <kernel/terminal.h>
#include <kernel/keyboard.h>
#include <kernel/mouse.h>
#include <kernel/memory.h>
#include <kernel/vgascreen.h>
#include <kernel/sys_time.h>
#include <kernel/power.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/irq.h>
#include <kernel/isrs.h>
#include <kernel/image.h>

#include <img/img_error_small.h>
#include <img/img_warn_small.h>
#include <img/img_info_small.h>
#include <img/img_trash_small.h>
#include <img/img_pencil_small.h>
#include <img/img_folder_small.h>
#include <img/img_poop_small.h>
#include <img/img_smile_small.h>

#include <img/cursor/img_cursor_black.h>
#include <img/cursor/img_cursor_black.h>

#include <img/font/img_consolas.h>
#include <img/font/img_dejavu_sans_mono.h>

#include <gui/gui.hpp>
#include <gui/font.h>

vga_screen *screen_ptr = NULL;
int smiley_x = 135, smiley_y = 15;
gui::cursor cur;

void left_click() {
  if (screen_ptr != NULL) {
    smiley_x = cur.x - 8;
    smiley_y = cur.y - 8;
  }
}

#if defined(__cplusplus)
extern "C"
#endif
void init() {
  gdt_install();
  idt_install();
  isrs_install();
  irq_install();

  mouse_install();
  keyboard_install();

  terminal_initialize();
  terminal_setcolor(make_color(COLOR_LIGHT_RED, COLOR_BLACK));
  printf("Welcome to TangOS\n");
  terminal_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));

  printf("\n");
}

#if defined(__cplusplus)
extern "C"
#endif
void main() {
  mouse_bind_event(mouse_left_released_event, left_click);

  printf("Type \"vga\" to enter VGA mode... ");

  char command[4];
  terminal_readstring(command, 4);

  if (!strcmp(command, "vga")) {
    // enter graphics mode
    vga_screen screen = vga_init_320_200_256();
    screen_ptr = &screen;

    font_sheet_t dejavu_sans_mono;
    dejavu_sans_mono.char_width = 11;
    dejavu_sans_mono.char_height = 11;
    dejavu_sans_mono.sheet_width = img_dejavu_sans_mono_width;
    dejavu_sans_mono.sheet_height = img_dejavu_sans_mono_height;
    dejavu_sans_mono.sheet_data_ptr = &img_dejavu_sans_mono_data[0];

    while (true) {
      cur.x += mouse_dx;
      cur.y += mouse_dy;
      mouse_dx = 0, mouse_dy = 0;
      // clip in bounds
      if (cur.x < 0) cur.x = 0;
      else if (cur.x > 318) cur.x = 318;

      if (cur.y < 0) cur.y = 0;
      else if (cur.y > 198) cur.y = 198;

      vga_clear_screen(screen_ptr, COLOR_DARK_GRAY);

      image_draw(screen_ptr,
        15, 15,
        img_error_small_width, img_error_small_height,
        img_error_small_data);

      image_draw(screen_ptr,
        35, 15,
        img_info_small_width, img_info_small_height,
        img_info_small_data);

      image_draw(screen_ptr,
        55, 15,
        img_trash_small_width, img_trash_small_height,
        img_trash_small_data);

      image_draw(screen_ptr,
        75, 15,
        img_pencil_small_width, img_pencil_small_height,
        img_pencil_small_data);

      image_draw(screen_ptr,
        95, 15,
        img_folder_small_width, img_folder_small_height,
        img_folder_small_data);

      image_draw(screen_ptr,
        115, 15,
        img_warning_small_width, img_warning_small_height,
        img_warning_small_data);

      { // warning messagebox
        int msg_x = 45, msg_y = 45,
          msg_w = 210, msg_h = 45;

        vga_fill_rect(screen_ptr, msg_x + 3, msg_y + 3, msg_w, msg_h, COLOR_BLACK);
        vga_fill_rect(screen_ptr, msg_x, msg_y, msg_w, msg_h, COLOR_LIGHT_BLUE);

        /*image_draw(&screen,
          msg_x + 2, msg_y + (msg_h / 2 - img_warning_small_height / 2),
          img_warning_small_width, img_warning_small_height,
          img_warning_small_data);*/

        image_draw(screen_ptr,
          msg_x + 2, msg_y + (msg_h / 2 - img_warning_small_height / 2),
          img_poop_small_width, img_poop_small_height,
          img_poop_small_data);

        font_draw_string(screen_ptr, &dejavu_sans_mono,
          msg_x + 8 + img_warning_small_width, msg_y + 12,
          "It's dangerous to go alone,\ntake this!"); //Welcome to TangOS!\nBeware of crappy bugs ;)
      }

      image_draw(screen_ptr,
        smiley_x, smiley_y,
        img_smile_small_width, img_smile_small_height,
        img_smile_small_data);

      // cursor
      image_draw(screen_ptr,
        cur.x, cur.y,
        img_cursor_black_width, img_cursor_black_height,
        img_cursor_black_data);

      vga_blit(screen_ptr);

      PAUSE; // wait for an interrupt
    }
  } else {
    printf("Cancelled... Press any key to reboot\n");
    keyboard_getchar();
    reboot();
  }
}
