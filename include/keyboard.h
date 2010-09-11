#ifndef __KEYBOARD_HEADER
#define __KEYBOARD_HEADER

// keyboard ISR handler
extern kbd_isr_2;

void store_scancode();
u_char get_raw_scancode();
void setup_keyboard();
u_char DataInKbdBuffer();
u_char kbd_get_id();
u_char kbd_set_scancode_set(u_char sc_set);
u_char kbd_curr_scancode_set();
u_char kbd_read_cmd_b_reg();
void kbd_write_cmd_b_reg(u_char bits_to_write);
u_char kbd_is_8042_buf_emp();

#endif
