// this the second "version" of this file. The original may be found in k_printf.old
#include <k_printf.h>
#include <data_types.h>
#include <putchar.h>
#include <string.h>

// this is somewhat "biased" toward Frank's "kprintf" function because
// it's the source for a printf that I read to figure out how to make one
void k_printf(char *message, ...)
{
	unsigned long length_of_message, i, tmp, *parm = (void *)((u_long)&message + 4);
	unsigned char is_percent_sign=0, total_message[200];

	total_message[0] = '\0';
	length_of_message = _strlen(message);

	for(i = 0; i < length_of_message; i++)
	{
		if(message[i] == '%')
		{
			if(is_percent_sign == 0)
			{
				is_percent_sign = 1;
			}
			else // print a percent sign
			{
				tmp = _strlen(total_message);
				total_message[tmp] = message[i];
				total_message[tmp +1] = '\0';
				is_percent_sign = 0;
			};
		}
		else if(is_percent_sign != 1)
		{
			tmp = _strlen(total_message);
			total_message[tmp] = message[i];
			total_message[tmp +1] = '\0';
		}
		else // is_percent_sign == 1
		{
			switch(message[i])
			{
				case 's': // IMHO, this is the most common, so it comes first for speed
					_strcat(&total_message[_strlen(total_message)], (void *)*parm);
					break;
				case 'd': // to display a number in decimal(base 10)
					ntostr(*parm, 10, &total_message[_strlen(total_message)]);
					break;
				case 'c': // to display just one char
					tmp = _strlen(total_message);
					total_message[tmp] = (unsigned char)*parm;
					total_message[tmp + 1] = '\0';
					break;
				case 'x': // to display a number in hex(bas 16)
					ntostr(*parm, 16, &total_message[_strlen(total_message)]);
					break;
				case 'o': // to display a number in octal
					ntostr(*parm, 8, &total_message[_strlen(total_message)]);
					break;
			};
			(unsigned long)parm += 4;
			is_percent_sign = 0;
		};
	};
	i = 0;
	while(total_message[i] != '\0')
	{
		putc(total_message[i]);
		i++;
	};
};

// from Frank's cottontail OS
void ntostr(unsigned long n, unsigned char base, unsigned char *s)
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
		}
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
