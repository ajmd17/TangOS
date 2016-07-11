#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <kernel/sys_asm.h>
#include <kernel/sys_err.h>
#include <kernel/scancodes.h>

#include <stdio.h>
#include <stdbool.h>

#define KEYBOARD_ACKNOWLEDGE 0xFA

#define MAX_SCANCODES 256

#define KEY_ERROR '\0'
#define KEY_ESC 27
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'
#define KEY_0 '0'
#define KEY_DASH '-'
#define KEY_EQUAL '='
#define KEY_BACKSPACE 8
#define KEY_TAB '\t'
#define KEY_Q 'Q'
#define KEY_W 'W'
#define KEY_E 'E'
#define KEY_R 'R'
#define KEY_T 'T'
#define KEY_Y 'Y'
#define KEY_U 'U'
#define KEY_I 'I'
#define KEY_O 'O'
#define KEY_P 'P'
#define KEY_LEFT_BRACKET '['
#define KEY_RIGHT_BRACKET ']'
#define KEY_ENTER '\n'
#define KEY_LEFT_CTRL 17
#define KEY_A 'A'
#define KEY_S 'S'
#define KEY_D 'D'
#define KEY_F 'F'
#define KEY_G 'G'
#define KEY_H 'H'
#define KEY_J 'J'
#define KEY_K 'K'
#define KEY_L 'L'
#define KEY_SEMICOLON ';'
#define KEY_SINGLE_QUOTE '\''
#define KEY_BACK_QUOTE '`'

#define KEY_BACK_SLASH '\\'

#define KEY_Z 'Z'
#define KEY_X 'X'
#define KEY_C 'C'
#define KEY_V 'V'
#define KEY_B 'B'
#define KEY_N 'N'
#define KEY_M 'M'
#define KEY_COMMA ','
#define KEY_PERIOD '.'
#define KEY_FORWARD_SLASH '/'

#define KEY_SPACE ' '

bool keyboard_shift_pressed = false;

int keyboard_getkeycode() {
  int keycode;
  while (true) {
    // wait for key
    while ((inb(0x64) & 1) == 0);

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
  if (keycode == SCANCODE_ERROR) {
    throw_error("Keycode error");
  } else {
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

#endif
