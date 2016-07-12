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
void kernel_early() {
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
void kernel_main() {
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
	
	/* ===== Mouse ===== */
	/*vga_screen screen = vga_init_320_200_256();
	
	int mx = 0, my = 0;
  while (1) {
    uint8_t mouse_stat = inb(0x64);

   // if (!(mouse_stat & 0x01)){
    //   continue;
    //}

    mouse_handler();
    mx += mouse_x - 20;
    my += mouse_y - 20;
    
    if (mx < 20) mx = 20;
    if (mx > 300) mx = 300;
    if (my < 20) my = 20;
    if (my > 180) my = 180;
    
    
  //  terminal_cursorpos(mx, my);
    vga_clear_screen(&screen, COLOR_BLUE);
	  vga_fill_rect(&screen, mx, my, 3, 3, COLOR_GREEN);
  }*/
	
	char *str = (char*)malloc(10000);
	memset(str, '\0', 50);
	printf("Length: %d\n", strlen(str) + 1);
	printf("Enter a string: ");
	terminal_readstring(str);
	printf("String is: %s\n", str);
	printf("Length: %d\n", strlen(str) + 1);
	free(str);
	
	
	
  terminal_setcolor(make_color(COLOR_YELLOW, COLOR_BLUE));
	dump_heap();
  terminal_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK));
	
	printf("Type \"vga\" to enter VGA mode... ");
	
	char cmd_str[20];
	terminal_readstring(cmd_str);
	
	if (!strcmp(cmd_str, "vga")) {
	  vga_screen screen = vga_init_320_200_256();
	  
	  int offset = 15;
	  while (true) {
	    if (keyboard_getchar() == ' ') {
	      vga_fill_rect(&screen, offset, 0, 5, 55, COLOR_LIGHT_GREEN);
	      offset += 25;
	    }
	  }
	  
	} else {
	  printf("Cancelled\n");
	}
} 
