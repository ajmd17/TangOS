#include <kernel/keyboard.h>
#include <kernel/sys_asm.h>
#include <kernel/scancodes.h>
#include <kernel/irq.h>
#include <kernel/terminal.h>

#include <stdio.h>
#include <string.h>

#define KEYBOARD_IRQ 1
#define IS_KEY_SHIFT(x) x == SCANCODE_LEFT_SHIFT || x == SCANCODE_RIGHT_SHIFT

bool keyboard_keys[256]; // extern
volatile char keyboard_lastchar;
volatile int keyboard_irq_state;
bool keyboard_shift_pressed = false;

char keycode2char(int keycode) {
  if (keycode != SCANCODE_ERROR) {
    switch (keycode) {
    case SCANCODE_1: return (!keyboard_shift_pressed) ? '1' : '!';
    case SCANCODE_2: return (!keyboard_shift_pressed) ? '2' : '@';
    case SCANCODE_3: return (!keyboard_shift_pressed) ? '3' : '#';
    case SCANCODE_4: return (!keyboard_shift_pressed) ? '4' : '$';
    case SCANCODE_5: return (!keyboard_shift_pressed) ? '5' : '%';
    case SCANCODE_6: return (!keyboard_shift_pressed) ? '6' : '^';
    case SCANCODE_7: return (!keyboard_shift_pressed) ? '7' : '&';
    case SCANCODE_8: return (!keyboard_shift_pressed) ? '8' : '*';
    case SCANCODE_9: return (!keyboard_shift_pressed) ? '9' : '(';
    case SCANCODE_0: return (!keyboard_shift_pressed) ? '0' : ')';
    case SCANCODE_DASH: return (!keyboard_shift_pressed) ? '-' : '_';
    case SCANCODE_EQUAL: return (!keyboard_shift_pressed) ? '=' : '+';
    case SCANCODE_BACKSPACE: return 8;
    case SCANCODE_TAB: return '\t';
    case SCANCODE_Q: return (!keyboard_shift_pressed) ? 'q' : 'Q';
    case SCANCODE_W: return (!keyboard_shift_pressed) ? 'w' : 'W';
    case SCANCODE_E: return (!keyboard_shift_pressed) ? 'e' : 'E';
    case SCANCODE_R: return (!keyboard_shift_pressed) ? 'r' : 'R';
    case SCANCODE_T: return (!keyboard_shift_pressed) ? 't' : 'T';
    case SCANCODE_Y: return (!keyboard_shift_pressed) ? 'y' : 'Y';
    case SCANCODE_U: return (!keyboard_shift_pressed) ? 'u' : 'U';
    case SCANCODE_I: return (!keyboard_shift_pressed) ? 'i' : 'I';
    case SCANCODE_O: return (!keyboard_shift_pressed) ? 'o' : 'O';
    case SCANCODE_P: return (!keyboard_shift_pressed) ? 'p' : 'P';
    case SCANCODE_LEFT_BRACKET: return (!keyboard_shift_pressed) ? '[' : '{';
    case SCANCODE_RIGHT_BRACKET: return (!keyboard_shift_pressed) ? ']' : '}';
    case SCANCODE_RETURN: return '\n';
    case SCANCODE_A: return (!keyboard_shift_pressed) ? 'a' : 'A';
    case SCANCODE_S: return (!keyboard_shift_pressed) ? 's' : 'S';
    case SCANCODE_D: return (!keyboard_shift_pressed) ? 'd' : 'D';
    case SCANCODE_F: return (!keyboard_shift_pressed) ? 'f' : 'F';
    case SCANCODE_G: return (!keyboard_shift_pressed) ? 'g' : 'G';
    case SCANCODE_H: return (!keyboard_shift_pressed) ? 'h' : 'H';
    case SCANCODE_J: return (!keyboard_shift_pressed) ? 'j' : 'J';
    case SCANCODE_K: return (!keyboard_shift_pressed) ? 'k' : 'K';
    case SCANCODE_L: return (!keyboard_shift_pressed) ? 'l' : 'L';
    case SCANCODE_SEMICOLON: return (!keyboard_shift_pressed) ? ';' : ':';
    case SCANCODE_QUOTE: return (!keyboard_shift_pressed) ? '\'' : '\"';
    case SCANCODE_BACK_QUOTE: return (!keyboard_shift_pressed) ? '`' : '~';
    case SCANCODE_BACKSLASH: return (!keyboard_shift_pressed) ? '\\' : '|';
    case SCANCODE_Z: return (!keyboard_shift_pressed) ? 'z' : 'Z';
    case SCANCODE_X: return (!keyboard_shift_pressed) ? 'x' : 'X';
    case SCANCODE_C: return (!keyboard_shift_pressed) ? 'c' : 'C';
    case SCANCODE_V: return (!keyboard_shift_pressed) ? 'v' : 'V';
    case SCANCODE_B: return (!keyboard_shift_pressed) ? 'b' : 'B';
    case SCANCODE_N: return (!keyboard_shift_pressed) ? 'n' : 'N';
    case SCANCODE_M: return (!keyboard_shift_pressed) ? 'm' : 'M';
    case SCANCODE_COMMA: return (!keyboard_shift_pressed) ? ',' : '<';
    case SCANCODE_PERIOD: return (!keyboard_shift_pressed) ? '.' : '>';
    case SCANCODE_FORWARDSLASH: return (!keyboard_shift_pressed) ? '/' : '?';
    case SCANCODE_SPACEBAR: return ' ';
    }
  }
  return '\0';
}

void keyboard_handler(regs_t *regs) {
  int keycode = inb(0x60);
  if (keycode & 0x80) {
    int keyreleased = keycode - 0x80;
    if (IS_KEY_SHIFT(keyreleased)) {
      keyboard_shift_pressed = false;
    } else {
      char ch = keycode2char(keyreleased);
      keyboard_keys[ch] = false;
    }
  } else if (keycode != SCANCODE_ERROR) {
    if (IS_KEY_SHIFT(keycode)) {
      keyboard_shift_pressed = true;
    } else {
      char ch = keycode2char(keycode);
      if (!keyboard_keys[ch]) {
        keyboard_lastchar = ch;
        keyboard_keys[ch] = true;
      }
      keyboard_irq_state = 1;
    }
  }
  irq_ack(KEYBOARD_IRQ);
}

void keyboard_install(void) {
  // initialize all keys to unpressed
  memset(keyboard_keys, false, 256);
  keyboard_lastchar = '\0';
  keyboard_irq_state = 0;
  irq_install_handler(KEYBOARD_IRQ, keyboard_handler);
}

char keyboard_getchar() {
  while (!keyboard_irq_state);
  keyboard_irq_state = 0;
  return keyboard_lastchar;
}