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

  terminal_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK));

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
    gui_cursor cursor;
	
    while (true) {
      PAUSE; // wait for an interrupt
      
      cursor.x += mouse_dx;
      cursor.y += mouse_dy;
      mouse_dx = 0, mouse_dy = 0;
      // clip in bounds
      if (cursor.x < 0) cursor.x = 0;
      if (cursor.x > 318) cursor.x = 318;
      if (cursor.y < 0) cursor.y = 0;
      if (cursor.y > 198) cursor.y = 198;
      
      vga_clear_screen(&screen, COLOR_WHITE);
			
      // some random rectangles
      //vga_fill_rect(&screen, 5, 10, 50, 25, COLOR_RED);
      //vga_fill_rect(&screen, 50, 80, 25, 75, COLOR_GREEN);

      // draw cursor
      cursor.draw(&screen);

      vga_blit(&screen);
      //vga_fill_rect(&screen, mx, my, 2, 2, COLOR_MAGENTA);
    }
  } else {
    printf("Cancelled... Press any key to reboot\n");
    keyboard_getchar();
    reboot();
  }
}