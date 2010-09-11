/*
	scanset2.c
	----------
	Scancode set 2
	by: Frank Millea, Dec. 27, 2001.
*/

u_char set2_make_normal[0x88] =
{
	0, KEY_F9, 0, KEY_F5, KEY_F3, KEY_F1, KEY_F2, KEY_F12,
	0, KEY_F10, KEY_F8, KEY_F6, KEY_F4, KEY_TAB, KEY_LQUOTE, 0,
	0, KEY_LALT, KEY_LSHIFT, 0, KEY_LCTRL, KEY_Q, KEY_1, 0,
	0, 0, KEY_Z, KEY_S, KEY_A, KEY_W, KEY_2, 0,
	0, KEY_C, KEY_X, KEY_D, KEY_E, KEY_4, KEY_3, 0,
	0, KEY_SPACE, KEY_V, KEY_F, KEY_T, KEY_R, KEY_5, 0,
	0, KEY_N, KEY_B, KEY_H, KEY_G, KEY_Y, KEY_6, 0,
	0, 0, KEY_M, KEY_J, KEY_U, KEY_7, KEY_8, 0,
	0, KEY_COMMA, KEY_K, KEY_I, KEY_O, KEY_0, KEY_9, 0,
	0, KEY_PERIOD, KEY_SLASH, KEY_L, KEY_SEMICOL, KEY_P, KEY_MINUS, 0,
	0, 0, KEY_RQUOTE, 0, KEY_LBRACK, KEY_EQUALS, 0, 0,
	KEY_CAPSLOCK, KEY_RSHIFT, KEY_ENTER, KEY_RBRACK, 0, KEY_BACKSLASH, 0, 0,
	0, 0, 0, 0, 0, 0, KEY_BKSP, 0,
	0, KEY_N_1, 0, KEY_N_4, KEY_N_7, 0, 0, 0,
	KEY_N_0, KEY_N_DOT, KEY_N_2, KEY_N_5, KEY_N_6, KEY_N_8, KEY_ESC, KEY_NUMLOCK,
	KEY_F11, KEY_N_ADD, KEY_N_3, KEY_N_SUB, KEY_N_MULT, KEY_N_9, KEY_SCRLOCK, 0,
	0, 0, 0, KEY_F7, 0, 0, 0, 0
};

u_char set2_make_extended[0x80] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, KEY_RALT, 0, 0, KEY_RCTRL, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, KEY_LWIN,
	0, 0, 0, 0, 0, 0, 0, KEY_RWIN,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, KEY_N_DIV, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, KEY_N_ENTER, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, KEY_END, 0, KEY_LEFT, KEY_HOME, 0, 0, 0,
	KEY_INS, KEY_DEL, KEY_DOWN, 0, KEY_RIGHT, KEY_UP, 0, 0,
	0, 0, KEY_PGDN, 0, KEY_PRTSCR, KEY_PGUP, 0, 0
};

u_short set2_ascii_no_numlock_shift[124] =
{
	0, '`', '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=', 0, 8, 9, 'q', 'w', 'e',
	'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	';', '\'', 0, 13, 0, 0, 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0, 0, 0, 0,
	0, ' ', 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, EXT_INS, EXT_DEL, 0, 0, EXT_LEFT,
	EXT_HOME, EXT_END, 0, EXT_UP, EXT_DOWN, EXT_PGUP, EXT_PGDN, 0, 0, EXT_RIGHT,
	0, EXT_HOME, EXT_LEFT, EXT_END, 0, '/', EXT_UP, 0/*n5*/, EXT_DOWN, EXT_INS,
	'*', EXT_PGUP, EXT_RIGHT, EXT_PGDN, EXT_DEL, '-', '+', 0, 13, 0,
	27, 0, EXT_F1, EXT_F2, EXT_F3, EXT_F4, EXT_F5, EXT_F6, EXT_F7, EXT_F8,
	EXT_F9, EXT_F10, EXT_F11, EXT_F12
};

u_short set2_ascii_shift[124] =
{
	0, '~', '!', '@', '#', '$', '%', '^', '&', '*',
	'(', ')', '_', '+', 0, 8, 9, 'Q', 'W', 'E',
	'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|',
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	':', '"', 0, 13, 0, 0, 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', 0, 0, 0, 0,
	0, ' ', 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, EXT_INS, EXT_DEL, 0, 0, EXT_LEFT,
	EXT_HOME, EXT_END, 0, EXT_UP, EXT_DOWN, EXT_PGUP, EXT_PGDN, 0, 0, EXT_RIGHT,
	0, EXT_HOME, EXT_LEFT, EXT_END, 0, '/', EXT_UP, 0/*n5*/, EXT_DOWN, EXT_INS,
	'*', EXT_PGUP, EXT_RIGHT, EXT_PGDN, EXT_DEL, '-', '+', 0, 13, 0,
	27, 0, EXT_SHIFT_F1, EXT_SHIFT_F2, EXT_SHIFT_F3, EXT_SHIFT_F4, EXT_SHIFT_F5, EXT_SHIFT_F6, EXT_SHIFT_F7, EXT_SHIFT_F8,
	EXT_SHIFT_F9, EXT_SHIFT_F10, EXT_SHIFT_F11, EXT_SHIFT_F12
};

u_short set2_ascii_numlock[124] =
{
	0, '`', '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=', 0, 8, 9, 'q', 'w', 'e',
	'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	';', '\'', 0, 13, 0, 0, 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0, 0, 0, 0,
	0, ' ', 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, EXT_INS, EXT_DEL, 0, 0, EXT_LEFT,
	EXT_HOME, EXT_END, 0, EXT_UP, EXT_DOWN, EXT_PGUP, EXT_PGDN, 0, 0, EXT_RIGHT,
	0, '7', '4', '1', 0, '/', '8', '5', '2', '0',
	'*', '9', '6', '3', '.', '-', '+', 0, 13, 0,
	27, 0, EXT_F1, EXT_F2, EXT_F3, EXT_F4, EXT_F5, EXT_F6, EXT_F7, EXT_F8,
	EXT_F9, EXT_F10, EXT_F11, EXT_F12
};

u_short set2_ascii_numlock_shift[124] =
{
	0, '~', '!', '@', '#', '$', '%', '^', '&', '*',
	'(', ')', '_', '+', 0, 8, 9, 'Q', 'W', 'E',
	'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '|',
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	':', '"', 0, 13, 0, 0, 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', 0, 0, 0, 0,
	0, ' ', 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, EXT_INS, EXT_DEL, 0, 0, EXT_LEFT,
	EXT_HOME, EXT_END, 0, EXT_UP, EXT_DOWN, EXT_PGUP, EXT_PGDN, 0, 0, EXT_RIGHT,
	0, '7', '4', '1', 0, '/', '8', '5', '2', '0',
	'*', '9', '6', '3', '.', '-', '+', 0, 13, 0,
	27, 0, EXT_SHIFT_F1, EXT_SHIFT_F2, EXT_SHIFT_F3, EXT_SHIFT_F4, EXT_SHIFT_F5, EXT_SHIFT_F6, EXT_SHIFT_F7, EXT_SHIFT_F8,
	EXT_SHIFT_F9, EXT_SHIFT_F10, EXT_SHIFT_F11, EXT_SHIFT_F12
};

u_short set2_ascii_ctrl_f[12] =
{
	EXT_CTRL_F1, EXT_CTRL_F2, EXT_CTRL_F3, EXT_CTRL_F4, EXT_CTRL_F5, EXT_CTRL_F6, EXT_CTRL_F7, EXT_CTRL_F8, EXT_CTRL_F9, EXT_CTRL_F10,
	EXT_CTRL_F11, EXT_CTRL_F12
};

u_short set2_ascii_alt_f[12] =
{
	EXT_ALT_F1, EXT_ALT_F2, EXT_ALT_F3, EXT_ALT_F4, EXT_ALT_F5, EXT_ALT_F6, EXT_ALT_F7, EXT_ALT_F8, EXT_ALT_F9, EXT_ALT_F10,
	EXT_ALT_F11, EXT_ALT_F12
};

u_short set2_ascii_alt_alpha[26] =
{
	EXT_ALT_A, EXT_ALT_B, EXT_ALT_C, EXT_ALT_D, EXT_ALT_E, EXT_ALT_F, EXT_ALT_G, EXT_ALT_H, EXT_ALT_I, EXT_ALT_J,
	EXT_ALT_K, EXT_ALT_L, EXT_ALT_M, EXT_ALT_N, EXT_ALT_O, EXT_ALT_P, EXT_ALT_Q, EXT_ALT_R, EXT_ALT_S, EXT_ALT_T,
	EXT_ALT_U, EXT_ALT_V, EXT_ALT_W, EXT_ALT_X, EXT_ALT_Y, EXT_ALT_Z
};

u_short set2_ascii_alt_num[10] =
{
	EXT_ALT_0, EXT_ALT_1, EXT_ALT_2, EXT_ALT_3, EXT_ALT_4, EXT_ALT_5, EXT_ALT_6, EXT_ALT_7, EXT_ALT_8, EXT_ALT_9
};

