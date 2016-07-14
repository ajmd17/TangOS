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
#include <kernel/idt.h>
//#include <kernel/irq.h>

#if defined(__cplusplus)
extern "C"
#endif
void init() {
  idt_install();
  //irq_install();

	mouse_install();
	
	terminal_initialize();
  terminal_setcolor(make_color(COLOR_LIGHT_RED, COLOR_BLACK));

	printf("Kernel initialized\n");
	printf("Available memory: %d bytes\n", available_ram());
	printf("Year: %d\n", time_getyear());
	printf ("Welcome to TangOS\n\n");
	
  terminal_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK));
	
	printf("\n");
}
 
#if defined(__cplusplus)
extern "C"
#endif
void main() {
	printf("Type \"vga\" to enter VGA mode... ");
	
	char cmd_str[4];
	terminal_readstring(cmd_str, 4);
  printf("got string: %s\n", cmd_str);
  keyboard_getchar(); // wait
	
	if (!strcmp(cmd_str, "vga")) {
	  // enter graphics mode
	  vga_screen screen = vga_init_320_200_256();
	
	  int mx = 0, my = 0;
    while (true) {
      uint8_t mouse_stat = 0x00;
      do {
        mouse_stat = inb(0x64);
      } while (!(mouse_stat & 0x01));

      mouse_handler();
      mouse_handler();
      mouse_handler();
      
      mx += mouse_dx;
      my += mouse_dy;
      
      // clip in bounds
      if (mx < 0) mx = 0;
      if (mx > 318) mx = 318;
      if (my < 0) my = 0;
      if (my > 198) my = 198;
      
      vga_clear_screen(&screen, COLOR_BLACK);
   
      // some random rectangles
      vga_fill_rect(&screen, 5, 10, 50, 25, COLOR_RED);
      vga_fill_rect(&screen, 50, 80, 25, 75, COLOR_GREEN);
  
      // draw cursor
	    vga_fill_rect(&screen, mx, my, 2, 2, COLOR_MAGENTA);
	  }
	} else {
	  printf("Cancelled... Press any key to reboot\n");
	  keyboard_getchar();
	  reboot();
	}
} 
