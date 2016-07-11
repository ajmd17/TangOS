#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif

#include <stddef.h>
#include <stdint.h>

#include <kernel/screen.h>
#include <kernel/terminal.h>
#include <kernel/keyboard.h>
#include <kernel/memory.h>
#include <kernel/vgascreen.h>
#include <kernel/sys_time.h>

#include <string.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C"
#endif
void kernel_early() {
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
	int *i_ptr = (int*)malloc(sizeof(int));
	*i_ptr = 6629;
	printf("*i_ptr = %d\n", *i_ptr);
	printf("i_ptr = %u\n", i_ptr);
	free(i_ptr);
	
	/*printf(" > ");
	unsigned char c = '\0';
	do {
	  c = keyboard_getchar();
	  putchar((int)c);
	  
	  if (c == '\n') {
	    printf(" > ");
	  }
	} while (c != '\0');*/
	
	printf("Press enter key to enter VGA mode... ");
	
	if (keyboard_getchar() == '\n') {
	  vga_screen screen = vga_init_320_200_256();
	  
	  while (true) {
	    if (keyboard_getchar() == ' ') {
	      vga_fill_rect(&screen, 150, 0, 15, 55, COLOR_LIGHT_GREEN);
	      break;
	    }
	  }
	} else {
	  printf("Cancelled\n");
	}
} 
