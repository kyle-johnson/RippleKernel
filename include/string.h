#ifndef STRING_HEADER
#define STRING_HEADER
#include <data_types.h>

void _memsetb(void *dest, u_char c, u_long n);
void _memsetw(void *dest, u_short w, u_long n);
void _memsetd(void *dest, u_long d, u_long n);
void _memcpy(void *dest, void *src, u_long n);
u_long _strlen(u_char *s);
void _strcat(u_char *dst, u_char *src);
u_char _strcmp(u_char *s1, u_char *s2);
u_char _memcmp(void *m1, void *m2, u_long n);

#endif
