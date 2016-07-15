#include <kernel/vgascreen.h>
#include <kernel/screen.h>
#include <kernel/sys_asm.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VGA_ADDRESS 0xA0000
#define VGA_AC_INDEX 0x3C0
#define VGA_AC_WRITE 0x3C0
#define VGA_AC_READ 0x3C1
#define VGA_MISC_WRITE 0x3C2
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define VGA_DAC_READ_INDEX 0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA 0x3C9
#define VGA_MISC_READ 0x3CC
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF
#define VGA_CRTC_INDEX 0x3D4      /* 0x3B4 */
#define VGA_CRTC_DATA 0x3D5      /* 0x3B5 */
#define VGA_INSTAT_READ 0x3DA
#define VGA_NUM_SEQ_REGS 5
#define VGA_NUM_CRTC_REGS 25
#define VGA_NUM_GC_REGS 9
#define VGA_NUM_AC_REGS 21
#define VGA_NUM_REGS (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

/**
* CREATE THE REGISTER ARRAY TAKEN FROM http://wiki.osdev.org/VGA_Hardware
*/
uint8_t mode_320_200_256[] = {
  /* MISC
   *
   * 0x63 => 01100011
   * 7 6 5 4 3 2 1 0
   * 1 1 0 0 0 1 1 0
   * VSP HSP - - CS CS ERAM IOS
   * 7,6 - 480 lines
   * 5,4 - free
   * 3,2 - 28,322 MHZ Clock
   * 1 - Enable Ram
   * 0 - Map 0x3d4 to 0x3b4
   */
  0x63,
  /* SEQ
   *
   * index 0x00 - Reset
   * 0x03 = 11
   * Bits 1,0 Synchronous reset
    */
  0x03,
  /*
   * index 0x01
   * Clocking mode register
   * 8/9 Dot Clocks
   */
  0x01,
  /*
   * Map Mask Register, 0x02
   * 0x0F = 1111
   * Enable all 4 Maps Bits 0-3
   * chain 4 mode
   */
  0x0F,
  /*
   * map select register, 0x03
   * no character map enabled
   */
  0x00,
  /*
   * memory mode register 0x04
   * enables ch4,odd/even,extended memory
   */
  0x0E,
  /* CRTC */
  0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
  0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
  0xFF,
  /* GC */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
  0xFF,
  /* AC */
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x41, 0x00, 0x0F, 0x00, 0x00
};

void vga_write_registers(uint8_t *regs) {
  uint16_t i;

  /* write MISCELLANEOUS reg */
  outb(VGA_MISC_WRITE, *regs);
  regs++;
   
  /* write SEQUENCER regs */
  for (i = 0; i < VGA_NUM_SEQ_REGS; i++) {
    outb(VGA_SEQ_INDEX, i);
    outb(VGA_SEQ_DATA, *regs);
    regs++;
  }
   
  /* unlock CRTC registers */
  outb(VGA_CRTC_INDEX, 0x03);
  outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
  outb(VGA_CRTC_INDEX, 0x11);
  outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
  
  /* make sure they remain unlocked */
  regs[0x03] |= 0x80;
  regs[0x11] &= ~0x80;
  
  /* write CRTC regs */
  for (i = 0; i < VGA_NUM_CRTC_REGS; i++) {
    outb(VGA_CRTC_INDEX, i);
    outb(VGA_CRTC_DATA, *regs);
    regs++;
  }
  
  /* write GRAPHICS CONTROLLER regs */
  for (i = 0; i < VGA_NUM_GC_REGS; i++) {
    outb(VGA_GC_INDEX, i);
    outb(VGA_GC_DATA, *regs);
    regs++;
  }
  
  /* write ATTRIBUTE CONTROLLER regs */
  for (i = 0; i < VGA_NUM_AC_REGS; i++) {
    (void)inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, i);
    outb(VGA_AC_WRITE, *regs);
    regs++;
  }
   
  /* lock 16-color palette and unblank display */
  (void)inb(VGA_INSTAT_READ);
  outb(VGA_AC_INDEX, 0x20);
}

void vga_init_colors() {
  vga_add_color(COLOR_BLACK, 0, 0, 0);
  vga_add_color(COLOR_BLUE, 0, 0, 170);
  vga_add_color(COLOR_GREEN, 0, 170, 0);
  vga_add_color(COLOR_CYAN, 0, 170, 170);
  vga_add_color(COLOR_RED, 170, 0, 0);
  vga_add_color(COLOR_MAGENTA, 170, 0, 170);
  vga_add_color(COLOR_BROWN, 170, 85, 0);
  vga_add_color(COLOR_LIGHT_GREY, 170, 170, 170);
  vga_add_color(COLOR_DARK_GREY, 85, 85, 85);
  vga_add_color(COLOR_LIGHT_BLUE, 85, 85, 255);
  vga_add_color(COLOR_LIGHT_GREEN, 85, 255, 85);
  vga_add_color(COLOR_LIGHT_CYAN, 85, 255, 255);
  vga_add_color(COLOR_LIGHT_RED, 255, 85, 85);
  vga_add_color(COLOR_LIGHT_MAGENTA, 255, 85, 255);
  vga_add_color(COLOR_YELLOW, 255, 255, 85);
  vga_add_color(COLOR_WHITE, 255, 255, 255);
}

void vga_add_color(uint16_t index, uint16_t red, uint16_t green, uint16_t blue) {
   outb(VGA_DAC_WRITE_INDEX, index);
   outb(VGA_DAC_DATA, red);
   outb(VGA_DAC_DATA, green);
   outb(VGA_DAC_DATA, blue);
}

void vga_set_pixel(vga_screen *scr, uint16_t x, uint16_t y, uint16_t color) {
  if (x <= scr->width && y <= scr->height) {
    scr->buffer[scr->width * y + x] = color;
  }
}

void vga_fill_rect(vga_screen *scr, uint16_t origin_x, uint16_t origin_y, uint16_t width, uint16_t height, uint16_t color) {
  uint16_t x, y;
  for (y = origin_y; y < (origin_y + height); y++) {
    for (x = origin_x; x < (origin_x + width); x++) {
      vga_set_pixel(scr, x, y, color);
    }
  }
}

void vga_clear_screen(vga_screen *scr, uint8_t color) {
  vga_fill_rect(scr, 0, 0, scr->width, scr->height, color);
}

void vga_blit(vga_screen *scr) {
  // copy buffer to vga memory
  memcpy(scr->memory, scr->buffer, sizeof(uint8_t) * scr->width * scr->height);
}

vga_screen vga_init_320_200_256() {
  vga_screen screen;
  screen.width = 320;
  screen.height = 200;
  screen.bpp = 256;
  screen.memory = (uint8_t*)VGA_ADDRESS;
  screen.buffer = (uint8_t*)calloc(sizeof(uint8_t) * screen.width * screen.height);

  if (screen.buffer == NULL) {
    printf("Could not create screen buffer - not enough memory");
    STOP;
  }

  //enables the mode 13 state
  vga_write_registers(mode_320_200_256);
  vga_init_colors();
  
  vga_clear_screen(&screen, COLOR_BLACK);
  
  return screen;
}
