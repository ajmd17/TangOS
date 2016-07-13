#include <kernel/keyboard.h>
#include <kernel/sys_asm.h>
#include <kernel/scancodes.h>

#include <stdio.h>
#include <stdbool.h>

bool keyboard_shift_pressed = false;

int keyboard_getkeycode() {
  int keycode;
  while (true) {
    // wait for key
    while ((inb(0x64) & 0x01) == 0);

    keycode = inb(0x60);
    
    if (keycode & 0x80) { // release
      int keyreleased = keycode - 0x80;
      if (keyreleased == SCANCODE_LEFT_SHIFT || 
            keyreleased == SCANCODE_RIGHT_SHIFT) {
        keyboard_shift_pressed = false;
      }
      continue;
    }
    
    if (keycode != SCANCODE_ERROR) { // pressed
      if (keycode == SCANCODE_LEFT_SHIFT || 
            keycode == SCANCODE_RIGHT_SHIFT) {
        keyboard_shift_pressed = true;
        continue;
      }
      return keycode;
    } else {
      continue;
    }
    
  }
  return SCANCODE_ERROR;
}
    
unsigned char keyboard_getchar() {
  int keycode = keyboard_getkeycode();
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
