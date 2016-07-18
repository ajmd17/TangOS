#include <kernel/terminal.h>
#include <kernel/sys_asm.h>
#include <kernel/screen.h>
#include <kernel/keyboard.h>

#include <string.h>
#include <stdio.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

#define TERMINAL_STRING_BUFFER_SIZE 32
static char terminal_string_buffer[TERMINAL_STRING_BUFFER_SIZE];

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = make_color(COLOR_WHITE, COLOR_BLACK);
  terminal_buffer = TEXT_MODE_MEMORY;

  for (size_t y = 0; y < TEXT_MODE_HEIGHT; y++) {
    for (size_t x = 0; x < TEXT_MODE_WIDTH; x++) {
      const size_t index = y * TEXT_MODE_WIDTH + x;
      terminal_buffer[index] = make_vgaentry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * TEXT_MODE_WIDTH + x;
  terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(uchar c) {
  if (c == '\n') { // return
    ++terminal_row;
    terminal_column = 0;
  } else if (c == '\t') { // tab
    terminal_putchar(' ');
  } else if (c == 8) { // backspace
    --terminal_column;
    terminal_putentryat('\0', terminal_color, terminal_column, terminal_row);
  } else if (c == KEY_LEFT_ARROW) { 
    --terminal_column;  
  } else if (c == KEY_RIGHT_ARROW) { 
    ++terminal_column;  
  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == TEXT_MODE_WIDTH) {
      terminal_column = 0;
      if (++terminal_row == TEXT_MODE_HEIGHT) {
        terminal_row = 0;
      }
    }
  }

  terminal_cursorpos(terminal_row, terminal_column);
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++) {
    terminal_putchar(data[i]);
  }
}

void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}

void terminal_readstring(char *data, size_t size) {
  if (size > TERMINAL_STRING_BUFFER_SIZE - 1) {
    size = TERMINAL_STRING_BUFFER_SIZE - 1;
  }

  memset(data, '\0', size);

  char *buffer_ptr = &terminal_string_buffer[0];
  const char *buffer_start = buffer_ptr;
  char *buffer_end = buffer_ptr;
  size_t num_chars = 0;

  while (true) {
    num_chars = buffer_ptr - buffer_start;
    uchar ch = keyboard_getchar();
    if (ch == '\n') {
      terminal_putchar(ch);
      memcpy(data, terminal_string_buffer, num_chars);
      return;
    } else if (ch == 8) { // backspace
      if (buffer_ptr > buffer_start) { // don't go negative
        terminal_putchar(ch);
        *(buffer_ptr--) = '\0';
      }
    } else if (ch == KEY_LEFT_ARROW) {
      if (buffer_ptr > buffer_start) { // don't go negative
        terminal_putchar(ch);
        buffer_ptr--;
      }
    } else if (ch == KEY_RIGHT_ARROW) {
        if (num_chars < size && buffer_ptr < buffer_end) {
          terminal_putchar(ch);
          buffer_ptr++;
        }
    } else if (num_chars < size) {
      terminal_putchar(ch);
      *(buffer_ptr++) = ch;
      if (buffer_ptr > buffer_end) {
        buffer_end = buffer_ptr;      
      }
    }
  }
}

void terminal_cursorpos(int row, int col) {
  unsigned short position = (row * TEXT_MODE_WIDTH) + col;

  // cursor LOW port to vga INDEX register
  outb(0x3D4, 0x0F);
  outb(0x3D5, (unsigned char)(position & 0xFF));
  // cursor HIGH port to vga INDEX register
  outb(0x3D4, 0x0E);
  outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}
