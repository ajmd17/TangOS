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

#if defined(__cplusplus)
extern "C"
#endif
void init() {
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
/*	int *i_ptr = (int*)malloc(sizeof(int));
	*i_ptr = 6629;
	printf("*i_ptr = %d\n", *i_ptr);
	printf("i_ptr = %u\n", i_ptr);
	free(i_ptr);*/
	
	/*printf(" > ");
	unsigned char c = '\0';
	do {
	  c = keyboard_getchar();
	  putchar((int)c);
	  
	  if (c == '\n') {
	    printf(" > ");
	  } else if (c == '!') {
	    reboot();
	    break;
	  }
	} while (c != '\0');*/
	
	/*char *str = (char*)malloc(10000);
	memset(str, '\0', 50);
	printf("Length: %d\n", strlen(str) + 1);
	printf("Enter a string: ");
	terminal_readstring(str);
	printf("String is: %s\n", str);
	printf("Length: %d\n", strlen(str) + 1);
	free(str);
	
  terminal_setcolor(make_color(COLOR_YELLOW, COLOR_BLUE));
	dump_heap();
  terminal_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK));*/
	
	printf("Type \"vga\" to enter VGA mode... ");
	
	char cmd_str[20];
	terminal_readstring(cmd_str);
	
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
	    vga_fill_rect(&screen, mx, my, 2, 2, COLOR_WHITE);
	  }
	} else {
	  printf("Cancelled... Press any key to reboot\n");
	  char ch[2];
	  keyboard_getchar(ch);
	  reboot();
	}
} 
