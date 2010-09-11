#include "k_printf.h"
#include "putchar.h"

void k_printf(char *message)
{
	while(*message!=0)
	{
		putc(*message);
		*message++;
	};
};

// from Frank's cottontail OS
void ntostr(u_long n, u_char base, u_char *s)
{
	u_long i, j;
	u_char temp[20], digit;

	i = 0;
	do
	{
		j = n;
		n /= base;	// divide num by base.
		n *= base;	// multiply num by b to isolate the digit.
		digit = j - n;
		// change the numerical digit to printable ASCII value.
		if(digit <= 9)
		{
			temp[i] = digit + '0';
		};
		else
		{
			temp[i] = digit - 0x0A + 'A';
		};
		n /= base;	// divide num by b again to get to the next digit.
		i++;
	} while(n != 0);
	temp[i] = '\0';	// add a terminator

	// now flip the string
	j = _strlen(temp) - 1;
	for(i=0; i<_strlen(temp); i++)
	{
		s[i] = temp[j];
		j--;	// decrement j while incrementing i
	};
	s[i] = '\0';	// terminate the string
};

// Cottontail OS again
int kgetchar(int x, int y)
{
	u_char *text = (u_char *)0xB8000;
	return text[(x + (y * 80)) * 2];
};
