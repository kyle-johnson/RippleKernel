/***************************************************************
  Most of the information for these functions came from
  Frank Millea's floppy.c file. Most of these functions he
  wrote, and I have just done a tad here 'n' there to make
  them more readable for me.
***************************************************************/
#include <data_types.h>
#include <k_defines.h>
#include <ports.h>
#include <real_time_clock.h>
#include <floppy.h>

floppy_drive_t floppy_drives[1];

unsigned char floppy_num_command_bytes[] = {0, 0, 9, 3, 2, 9, 9, 2, 1, 9, 2, 0, 9, 6, 0, 3};
unsigned char floppy_num_status_bytes[] =  {0, 0, 7, 0, 1, 7, 7, 0, 2, 7, 7, 0, 7, 7, 0, 0};
char *floppy_errors[] =
{
	"Unit check", "Not ready", "End of cylinder", "Data error",
	"Timeout", "No data", "Write protect", "No address mark",
	"CRC error", "Wrong cylinder", "Seek error", "Bad cylinder",
	"No IRQ6", "Unknown error", "Disk changed"
};

void inti_floppy()
{
	int i;
	unsigned char c, a, b;

	// these are the internal names... they can change, since it really doesn't matter what the floppy drives are called :)
	floppy_drives[0].name = "fda";
	floppy_drives[1].name = "fdb";

	outportb(0x70, 0x10);	// select floppy drive type byte in CMOS
	c = inportb(0x71);

	if(c == 0x00)
	{
		k_printf("No floppy drive(s) detected.\n");
		return;
	};

	a = c >> 4;
	b = c & 0x0F;

	floppy_drives[0].type = a;
	floppy_drives[1].type = b;

	switch(floppy_drives[0].type) {
		case 0x04:
			k_printf("Floppy drive A found(1.44 MB 3.5 Drive).\n");
			break;
		case 0x00:
			k_printf("No A floppy drive found.\n");
			break;
		case 0x05:
			k_printf("Floppy drive A found(2.88 MB 3.5 drive).\n");
			break;
		case 0x01:
			k_printf("Floppy drive A found(360 KB 5.25 Drive).\n");
			break;
		case 0x02:
			k_printf("Floppy drive A found(1.2 MB 5.25 Drive).\n");
			break;
		case 0x03:
			k_printf("Floppy drive A found(720 KB 3.5 Drive).\n");
			break;
		default:
			k_printf("Unknown A floppy drive type.\n");
			break;
	};

	switch(floppy_drives[1].type) {
		case 0x04:
			k_printf("Floppy drive B found(1.44 MB 3.5 Drive).\n");
			break;
		case 0x00:
			k_printf("No B floppy drive found.\n");
			break;
		case 0x01:
			k_printf("Floppy drive B found(360 KB 5.25 Drive).\n");
			break;
		case 0x02:
			k_printf("Floppy drive B found(1.2 MB 5.25 Drive).\n");
			break;
		case 0x03:
			k_printf("Floppy drive B found(720 KB 3.5 Drive).\n");
			break;
		case 0x05:
			k_printf("Floppy drive B found(2.88 MB 3.5 drive).\n");
			break;
		default:
			k_printf("Unknown B floppy drive type.\n");
			break;
	};

	if(floppy_drives[0].type != 1 && floppy_drives[0].type != 2) // most common
	{
		floppy_drives[0].GAP3 = 27;
	}
	else
	{
		floppy_drives[0].GAP3 = 42;
	};

	if(floppy_drives[1].type != 1 && floppy_drives[1].type != 2) // most common
	{
		floppy_drives[1].GAP3 = 27;
	}
	else
	{
		floppy_drives[1].GAP3 = 42;
	};

};

// int 0x6 is used by the FDC to tell us whether or not a command was completed
void irq6()
{
	irq6_fired = 1;
};
