#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "c_ext_ascii.h"

// I/O ports
#define KEYBOARD_DATA 0x60	// R/W (commands and data)
#define KEYBOARD_STATUS	0x64	// R (read keyboard status)
#define KEYBOARD_CONTROL 0x64	// W (write controller commands)

// repeated keys per second
#define KPS_30_0 0x00
#define KPS_26_7 0x01
#define KPS_24_0 0x02
#define KPS_21_8 0x03
#define KPS_20_0 0x04
#define KPS_18_5 0x05
#define KPS_17_1 0x06
#define KPS_16_0 0x07
#define KPS_15_0 0x08
#define KPS_13_3 0x09
#define KPS_12_0 0x0A
#define KPS_10_9 0x0B
#define KPS_10_0 0x0C
#define KPS_9_2 0x0D
#define KPS_8_5 0x0E
#define KPS_8_0 0x0F
#define KPS_7_5 0x10
#define KPS_6_7 0x11
#define KPS_6_0 0x12
#define KPS_5_5 0x13
#define KPS_5_0 0x14
#define KPS_4_6 0x15
#define KPS_4_3 0x16
#define KPS_4_0 0x17
#define KPS_3_7 0x18
#define KPS_3_3 0x19
#define KPS_3_0 0x1A
#define KPS_2_7 0x1B
#define KPS_2_5 0x1C
#define KPS_2_3 0x1D
#define KPS_2_1 0x1E
#define KPS_2_0 0x1F

// repeat delay in ms
#define RPT_DELAY_250 0x00
#define RPT_DELAY_500 0x20
#define RPT_DELAY_750 0x40
#define RPT_DELAY_1000 0x60

#define KEY_ESC 110
#define KEY_F1 112
#define KEY_F2 113
#define KEY_F3 114
#define KEY_F4 115
#define KEY_F5 116
#define KEY_F6 117
#define KEY_F7 118
#define KEY_F8 119
#define KEY_F9 120
#define KEY_F10 121
#define KEY_F11 122
#define KEY_F12 123
#define KEY_LQUOTE 1
#define KEY_1 2
#define KEY_2 3
#define KEY_3 4
#define KEY_4 5
#define KEY_5 6
#define KEY_6 7
#define KEY_7 8
#define KEY_8 9
#define KEY_9 10
#define KEY_0 11
#define KEY_MINUS 12
#define KEY_EQUALS 13
#define KEY_BKSP 15
#define KEY_TAB 16
#define KEY_Q 17
#define KEY_W 18
#define KEY_E 19
#define KEY_R 20
#define KEY_T 21
#define KEY_Y 22
#define KEY_U 23
#define KEY_I 24
#define KEY_O 25
#define KEY_P 26
#define KEY_LBRACK 27
#define KEY_RBRACK 28
#define KEY_BACKSLASH 29
#define KEY_CAPSLOCK 30
#define KEY_A 31
#define KEY_S 32
#define KEY_D 33
#define KEY_F 34
#define KEY_G 35
#define KEY_H 36
#define KEY_J 37
#define KEY_K 38
#define KEY_L 39
#define KEY_SEMICOL 40
#define KEY_RQUOTE 41
#define KEY_ENTER 43
#define KEY_LSHIFT 44
#define KEY_Z 46
#define KEY_X 47
#define KEY_C 48
#define KEY_V 49
#define KEY_B 50
#define KEY_N 51
#define KEY_M 52
#define KEY_COMMA 53
#define KEY_PERIOD 54
#define KEY_SLASH 55
#define KEY_RSHIFT 57
#define KEY_LCTRL 58
#define KEY_LWIN 59	// ??
#define KEY_LALT 60
#define KEY_SPACE 61
#define KEY_RALT 62
#define KEY_RWIN 63	// ??
#define KEY_MENU 65	// ??
#define KEY_RCTRL 64
#define KEY_PRTSCR 124
#define KEY_SCRLOCK 125
#define KEY_PAUSE 126
#define KEY_INS 75
#define KEY_HOME 80
#define KEY_PGUP 85
#define KEY_DEL 76
#define KEY_END 81
#define KEY_PGDN 86
#define KEY_UP 83
#define KEY_LEFT 79
#define KEY_DOWN 84
#define KEY_RIGHT 89
#define KEY_NUMLOCK 90
#define KEY_N_DIV 95
#define KEY_N_MULT 100
#define KEY_N_SUB 105
#define KEY_N_7 91
#define KEY_N_8 96
#define KEY_N_9 101
#define KEY_N_4 92
#define KEY_N_5 97
#define KEY_N_6 102
#define KEY_N_ADD 106
#define KEY_N_1 93
#define KEY_N_2 98
#define KEY_N_3 103
#define KEY_N_0 99
#define KEY_N_DOT 104
#define KEY_N_ENTER 108

// function prototypes
void keyboard_main();
int keyboard_init();
int identify_keyboard();
int set_keyboard_leds(u_char leds);
int set_keyboard_scancode_set(int scanset);
void keyboard_byte_out(unsigned short port, unsigned char byte);
int keyboard_byte_in();
int keyboard_command_byte(u_char byte);
int keyboard_reset();
int keyboard_decode_inbuf();
int keyboard_get_ascii(int key);

#endif
