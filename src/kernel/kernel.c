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

#include <img/font/img_dejavu_sans_mono.h>
#include <gui/gui.hpp>

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

  printf("Kernel initialized\n");
  printf("Available memory: %d bytes\n", available_ram());
  printf("Year: %d\n", time_getyear());
  printf("Welcome to TangOS\n\n");

  terminal_setcolor(make_color(COLOR_WHITE, COLOR_BLACK));

  printf("\n");
}

#if defined(__cplusplus)
extern "C"
#endif
void main() {
  printf("Type \"vga\" to enter VGA mode... ");

  char cmd_str[128];
  terminal_readstring(cmd_str, 128);

  if (!strcmp(cmd_str, "vga")) {
    // enter graphics mode
    vga_screen screen = vga_init_320_200_256();
    gui::cursor cur;

    while (true) {

      cur.x += mouse_dx;
      cur.y += mouse_dy;
      mouse_dx = 0, mouse_dy = 0;
      // clip in bounds
      if (cur.x < 0) cur.x = 0;
      else if (cur.x > 318) cur.x = 318;

      if (cur.y < 0) cur.y = 0;
      else if (cur.y > 198) cur.y = 198;

      vga_clear_screen(&screen, COLOR_DARK_GRAY);

      image_draw(&screen,
        15, 15,
        img_error_small_width, img_error_small_height,
        img_error_small_data);

      image_draw(&screen,
        35, 15,
        img_info_small_width, img_info_small_height,
        img_info_small_data);

      image_draw(&screen,
        55, 15,
        img_trash_small_width, img_trash_small_height,
        img_trash_small_data);

      image_draw(&screen,
        75, 15,
        img_pencil_small_width, img_pencil_small_height,
        img_pencil_small_data);

      image_draw(&screen,
        95, 15,
        img_folder_small_width, img_folder_small_height,
        img_folder_small_data);

      image_draw(&screen,
        115, 15,
        img_poop_small_width, img_poop_small_height,
        img_poop_small_data);

      image_draw(&screen,
        135, 15,
        img_smile_small_width, img_smile_small_height,
        img_smile_small_data);

      { // warning messagebox
        int msg_x = 45,  msg_y = 45,
            msg_w = 100, msg_h = 28;

        vga_fill_rect(&screen, msg_x + 3, msg_y + 3, msg_w, msg_h, COLOR_BLACK);
        vga_fill_rect(&screen, msg_x,     msg_y,     msg_w, msg_h, COLOR_RED);

        image_draw(&screen,
          msg_x + 2, msg_y + (msg_h/2 - img_warning_small_height/2),
          img_warning_small_width, img_warning_small_height,
          img_warning_small_data);

        image_draw(&screen,
          msg_x + 2, msg_y,
          img_dejavu_sans_mono_width, img_dejavu_sans_mono_height,
          img_dejavu_sans_mono_data);

        /*image_draw_subimage(&screen,
          3, 3,
          8, 8,
          35, 20, 
          img_error_small_width, img_error_small_height,
          img_error_small_data);*/
      }

      // draw cursor
      cur.draw(&screen);

      vga_blit(&screen);

      PAUSE; // wait for an interrupt
    }
  } else {
    printf("Cancelled... Press any key to reboot\n");
    keyboard_getchar();
    reboot();
  }
}
