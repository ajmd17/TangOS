#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <stdbool.h>

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

int keyboard_getkeycode();
unsigned char keyboard_getchar();

#endif
